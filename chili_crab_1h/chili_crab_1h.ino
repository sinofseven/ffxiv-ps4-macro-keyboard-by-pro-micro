#include <Keyboard.h>

#define BREAKER_MAX_TIME 3540000
#define KEY_NUMBERS 5
uint8_t listKeyCodes[KEY_NUMBERS] = {KEY_INSERT, KEY_INSERT, KEY_INSERT, '1', '2'};
unsigned long listKeyIntervals[KEY_NUMBERS] = {500, 500, 1000, 35000, 35000};


#define MY_PIN_SWITCH 15
#define MY_PIN_LED 4

uint8_t offValue;
bool stateCurrent = false;
bool statePrevious = false;
unsigned long breakerTimeBase;

uint8_t keyIndex = 0;
uint8_t keyState = 0;
unsigned long keyIntervalBase;

void execMacro() {
  uint8_t keyCode = listKeyCodes[keyIndex];
  unsigned long interval = listKeyIntervals[keyIndex];
  unsigned long now = millis();

  switch (keyState) {
    case 0:
      Keyboard.press(keyCode);
      keyIntervalBase = millis();
      keyState = 1;
      break;
    case 1:
      if (now - keyIntervalBase >= 500) {
        Keyboard.release(keyCode);
        keyIntervalBase = millis();
        keyState = 2;
      }
      break;
    case 2:
      if (now - keyIntervalBase >= interval) {
        keyState = 0;
        keyIndex = (keyIndex + 1) % KEY_NUMBERS; 
      }
      break;
  }
}

bool checkBreaker() {
  unsigned long now = millis();
  return now - breakerTimeBase >= BREAKER_MAX_TIME;
}

void toggleOn() {
  stateCurrent = true;
  if (stateCurrent == statePrevious) return;
  breakerTimeBase = millis();
  keyState = 0;
  keyIndex = 0;
}

void doOn() {
  toggleOn();
  if (checkBreaker()) {
    toggleOff();
    offValue = offValue == HIGH ? LOW : HIGH;
    return;
  }
  execMacro();
}

void toggleOff() {
  stateCurrent = false;
}

void doOff() {
  toggleOff();
}

void manageLED() {
  if (stateCurrent == statePrevious) return;
  statePrevious = stateCurrent;
  uint8_t value = stateCurrent ? HIGH : LOW;
  digitalWrite(MY_PIN_LED, value);
}

void setup() {
  Keyboard.begin();
  pinMode(MY_PIN_LED, OUTPUT);
  offValue = digitalRead(MY_PIN_SWITCH);
}

void loop() {
  if (digitalRead(MY_PIN_SWITCH) == offValue) {
    doOff();
  } else {
    doOn();
  }
  manageLED();
}
