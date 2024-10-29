#include "Arduino.h"

class Keypad {  // The class
public:         // Access specifier
  Keypad(byte cols, byte rows, byte maxNumberOfKeysPressed, byte *rowPins, byte *colPins)
    : _cols(cols), _rows(rows), _maxNumberOfKeysPressed(maxNumberOfKeysPressed), _rowPins(rowPins), _colPins(colPins) {
  }
  void initKeypad();
  char *getPressedKeys();

private:
  const byte _cols;
  const byte _rows;
  const byte _maxNumberOfKeysPressed;
  const byte *_rowPins;
  const byte *_colPins;
  byte _keypadState[4][3];
  char *_pressedButtons = "\0\0\0\0\0\0\0\0\0\0";

  void deleteKeyFromList(char *list, char key);
  void addKeyInList(char *list, char key);
  char _keys[4][3] = {
    { 'D', 'M', 'H' },
    { '7', '8', 'S' },
    { '4', '5', '6' },
    { '1', '2', '3' },
  };
};
