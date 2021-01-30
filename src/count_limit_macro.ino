#include <Keyboard.h>

#define COUNT_LIMIT {limit}
#define KEY_NUMBERS {key_numbers}
uint8_t listKeyCodes[KEY_NUMBERS] = \{{keys | unescaped}};
unsigned long listKeyIntervals[KEY_NUMBERS] = \{{intervals | unescaped}};


#define MY_PIN_SWITCH 15
#define MY_PIN_LED 4

uint8_t offValue;
bool stateCurrent = false;
bool statePrevious = false;
uint8_t keyIndex = 0;
uint8_t keyState = 0;
uint8_t count = 0;
unsigned long keyIntervalBase;

void execMacro() \{
  uint8_t keyCode = listKeyCodes[keyIndex];
  unsigned long interval = listKeyIntervals[keyIndex];
  unsigned long now = millis();

  switch (keyState) \{
    case 0:
      Keyboard.press(keyCode);
      keyIntervalBase = millis();
      keyState = 1;
      break;
    case 1:
      if (now - keyIntervalBase >= 500) \{
        Keyboard.release(keyCode);
        keyIntervalBase = millis();
        keyState = 2;
      }
      break;
    case 2:
      if (now - keyIntervalBase >= interval) \{
        keyState = 0;
        keyIndex = (keyIndex + 1) % KEY_NUMBERS;
        if (keyIndex == 0) \{
          count += 1;
        }
      }
      break;
  }
}

bool checkBreaker() \{
  return count >= COUNT_LIMIT;
}

void toggleOn() \{
  stateCurrent = true;
  if (stateCurrent == statePrevious) return;
  keyState = 0;
  keyIndex = 0;
  count = 0;
}

void doOn() \{
  toggleOn();
  if (checkBreaker()) \{
    toggleOff();
    offValue = offValue == HIGH ? LOW : HIGH;
    return;
  }
  execMacro();
}

void toggleOff() \{
  stateCurrent = false;
  if (stateCurrent == statePrevious) return;
  Keyboard.releaseAll();
}

void doOff() \{
  toggleOff();
}

void manageLED() \{
  if (stateCurrent == statePrevious) return;
  statePrevious = stateCurrent;
  uint8_t value = stateCurrent ? HIGH : LOW;
  digitalWrite(MY_PIN_LED, value);
}

void setup() \{
  Keyboard.begin();
  pinMode(MY_PIN_LED, OUTPUT);
  offValue = digitalRead(MY_PIN_SWITCH);
}

void loop() \{
  if (digitalRead(MY_PIN_SWITCH) == offValue) \{
    doOff();
  } else \{
    doOn();
  }
  manageLED();
}
