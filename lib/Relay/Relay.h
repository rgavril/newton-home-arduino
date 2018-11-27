#ifndef _RELAY_h
#define _RELAY_h

#include "Arduino.h"

class Relay
{
  private:
    uint8_t pin;        // arduino pin connected to the relay
    bool invertedLogic; // if true will send LOW signal to turn relay on
    bool state;         // current state of the relay
    bool recordedState; // previous state of the relay

  public:
    static const bool ON = true;
    static const bool OFF = false;

    // Consturctor
    //
    // Required parameters:
    // pin             the arduino pin the button is connected to
    //
    // Optional parameters:
    // invertedLogic   true to send low logic signal to turn on (default false)
    Relay(uint8_t pin, bool invertedLogic = false);

    // Sets the state of the relay. True to turn the relay on on or false 
    // to turn it off.
    void setState(bool state);

    // Returns the relay status, true for ON, false for OFF
    bool getState(void);

    // Toggles the relay by reveting it's current state.
    void toggleState();

    // Returns true if relay is ON, false if relay is OFF
    bool isOn();

    // Returns true if the relay is OFF, false if relay is ON
    bool isOff();


    void recordState();
    bool stateChanged();
};

#endif
