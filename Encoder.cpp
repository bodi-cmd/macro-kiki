#include "Encoder.h"

void Encoder::initEncoder() {
  pinMode(_pinL, INPUT_PULLUP);
  pinMode(_pinR, INPUT_PULLUP);
  pinMode(_buttonPin, INPUT_PULLUP);
  lastState = digitalRead(_pinL);
  lastBtnState = digitalRead(_buttonPin);
}
char Encoder::getEncoderEvent() {
  byte newState = digitalRead(_pinL);
  char result = 0;
  if (newState != lastState && !newState) {
    result = digitalRead(_pinR) == newState ? '+' : '-';
  }
  lastState = newState;

  byte newBtnState = digitalRead(_buttonPin);
  if (newBtnState != lastBtnState && !newBtnState) {
    result = 'B';
  }
  lastBtnState = newBtnState;
  return result;
}