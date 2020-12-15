#include <Keyboard.h>
#define KEYS_NUMBER 4


uint8_t keys[KEYS_NUMBER] = {KEY_INSERT, KEY_INSERT, '1', '1'};
uint32_t nextIntervals[KEYS_NUMBER] = {500, 500, 35000, 39000};

uint8_t pinSwitch = 15;
uint8_t pinLED = 4;

uint8_t ledPrevious = LOW;
uint8_t ledCurrent = LOW;

uint8_t keyIndex;
uint8_t initValue;
unsigned long timeBreakerBase;
unsigned long timeMeasureIntervalBase;

#define STATE_RANGE 3 // 0: unpress, 1: pressed, 2: wait
uint8_t state;

// unsigned long timeBreakerMax = 840000; // 1000 * 60 * 14
unsigned long timeBreakerMax = 1000 * 3;

void execMacro() {
  uint8_t keyCode = keys[keyIndex];
  uint32_t interval = nextIntervals[keyIndex];
  unsigned long now = millis();
  switch (state) {
    case 0:
      Keyboard.press(keyCode);
      timeMeasureIntervalBase = now;
      state = 1;
      break;
    case 1:
      if (now - timeMeasureIntervalBase >= 500) {
        Keyboard.release(keyCode);
        timeMeasureIntervalBase = now;
        state = 2;
      }
      break;
    case 2:
      if (now - timeMeasureIntervalBase >= interval) {
        state = 0;
        keyIndex = (keyIndex + 1) % KEYS_NUMBER;
      }      
      break;
  }
}

void toggleOn() {
  ledCurrent = HIGH;
  if (ledCurrent == ledPrevious) return;
  timeBreakerBase = millis();
  keyIndex = 0;
  state = 0;
}

void toggleOff() {
  ledCurrent = LOW;
}

void execOn() {
  toggleOn();
  if (checkBreaker() ) {
    execBreaker();
    toggleOff();
    return;
  }
}

void execOff() {
  toggleOff();
}

void manageLight() {
  if (ledCurrent == ledPrevious) return;
  ledPrevious = ledCurrent;
  digitalWrite(pinLED, ledCurrent);
}

bool checkButton() {
  return digitalRead(pinSwitch) == initValue;
}

bool checkBreaker() {
  unsigned long now = millis();
  return now - timeBreakerBase > timeBreakerMax;
}

void execBreaker() {
  initValue = initValue == HIGH ? LOW : HIGH;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(15, OUTPUT);
  initValue = digitalRead(pinSwitch);
  
}

void loop() {
  if (checkButton()) {
    execOff();
  } else {
    execOn();
  }
  manageLight();
}
