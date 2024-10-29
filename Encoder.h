#include "Arduino.h"

class Encoder
{
public:
    Encoder(byte pinL, byte pinR, byte buttonPin)
        : _pinL(pinL), _pinR(pinR), _buttonPin(buttonPin) {}
    void initEncoder();
    char getEncoderEvent();

private:
    const byte _pinL;
    const byte _pinR;
    const byte _buttonPin;
    byte lastState;
    byte lastBtnState;
};