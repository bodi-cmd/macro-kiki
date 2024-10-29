#include "Keypad.h"

void Keypad::initKeypad() {
  for (int i = 0; i < _cols; i++) {
    pinMode(_colPins[i], OUTPUT);
  }
  for (int i = 0; i < _rows; i++) {
    pinMode(_rowPins[i], INPUT_PULLUP);
  }
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      _keypadState[i][j] = 0;
    }
  }
  for (int i = 0; i < _maxNumberOfKeysPressed + 1; i++) {
    _pressedButtons[i] = 0;
  }
}

void Keypad::deleteKeyFromList(char* list, char key) {
  int found = -1;
  for (int i = 0; i < _maxNumberOfKeysPressed; i++) {
    if (list[i] == key) {
      found = i;
    }
  }
  if (found > -1) {
    for (int i = found; i < _maxNumberOfKeysPressed - 1; i++) {
      list[i] = list[i + 1];
    }
    list[_maxNumberOfKeysPressed - 1] = 0;
  }
}

void Keypad::addKeyInList(char* list, char key) {
  for (int i = 0; i < _maxNumberOfKeysPressed; i++) {
    if (list[i] == 0) {
      list[i] = key;
      return;
    }
  }
}

char* Keypad::getPressedKeys() {
  for (int j = 0; j < _cols; j++) {
    for (int k = 0; k < _cols; k++) {
      if (k == j) {
        pinMode(_colPins[k], OUTPUT);
        digitalWrite(_colPins[k], LOW);
      } else {
        pinMode(_colPins[k], INPUT);
      }
    }
    for (int i = 0; i < _rows; i++) {
      int buttonSum = 0;
      for(int k = 0; k < 20; k++){
        buttonSum += digitalRead(_rowPins[i]);
      }
      uint8_t buttonNewState = buttonSum == 0;
      if (buttonNewState != _keypadState[i][j]) {
        if (buttonNewState != 0) {
          addKeyInList(_pressedButtons, _keys[i][j]);
        } else {
          deleteKeyFromList(_pressedButtons, _keys[i][j]);
        }
        _keypadState[i][j] = buttonNewState;
      }
    }
  }
  return _pressedButtons;
}