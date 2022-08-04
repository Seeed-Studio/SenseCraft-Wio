
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "Arduino.h"
#include "Seeed_Arduino_ooFreeRTOS.h"
#include "utils.h"

// create a buttion class use ooFreeRTOS task

using namespace cpp_freertos;

class ButtonThread : public Thread
{
public:
    ButtonThread(Message &q);

protected:
    virtual void Run();

private:
    uint8_t buttonState;           // the current reading from the input pin
    uint8_t lastButtonState = LOW; // the previous reading from the input pin

    // the following variables are unsigned longs because the time, measured in
    // milliseconds, will quickly become a bigger number than can be stored in an int.
    unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
    unsigned long debounceDelay = 50;   // the debounce time; increase if the output flickers

    Message &Mail;

    const uint8_t btn[8] = {WIO_KEY_A, WIO_KEY_B, WIO_KEY_C, WIO_5S_UP, WIO_5S_DOWN, WIO_5S_LEFT, WIO_5S_RIGHT, WIO_5S_PRESS};
};

#endif // __BUTTON_H__