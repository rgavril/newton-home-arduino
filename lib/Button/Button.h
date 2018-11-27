#ifndef _BUTTON_h
#define _BUTTON_h

#include "Arduino.h"


class Button
{
    private:
        uint8_t pin;                    // arduino pin connected to the button
        bool pullupEnabled;             // internal pullup resitor enabled
        bool state;                     // current state of the button
        bool lastState;                 // previous state of the button

        uint32_t pinChangeTime;         // last time the pin changed it's value (for debouncing)
        bool prevPinValue;              // value of the pin on the last reading (for debouncing)
        uint32_t pinDebounceTime = 60;  // time the pin needs to keep a value before being considered a stable value

        uint32_t pressStartTime = 0;
        uint32_t releaseStartTime = 0;
        uint32_t pressedDuration = 0;
        uint32_t releasedDuration = 0;
        String waitForEvent = "LC";
        uint8_t event = EVENT_NONE;

    public:
        static const bool PRESSED = true;
        static const bool RELEASED = false;

        static const enum {
            EVENT_NONE,
            EVENT_CLICK,
            EVENT_DOUBLE_CLICK,
            EVENT_LONG_PRESS
        } Event;

        /**
         * @brief Construct a new Button object
         * 
         * @param pin             the arduino pin the button is connected to
         * @param pullupEnabled   true to enable the AVR internal pullup resistor (default true)
         */
        Button(uint8_t pin, bool pullupEnabled=true);

        /**
         * @brief Reads the current debounced button state, true if pressed,
         * false if released. Best practice would be to call it at begining
         * of the loop() function.
         * 
         * @return true 
         * @return false 
         */
        bool readInput(void);

        /**
         * @brief Returns true if the button state was pressed at the last call 
         * to readInput().
         * Does not cause the pin to be read again.
         * 
         * @return true 
         * @return false 
         */
        bool isPressed();

        bool isReleased();

        /**
         * @brief Returns true if the button state at the last call to readInput()
         * was released, and this was a change since the previous read.
         * 
         * @return true 
         * @return false 
         */
        bool wasReleased();

        bool wasPressed();

        /**
         * @brief Return the current state of the button as it was read at the
         * last time readInput() was called.
         * 
         */
        bool getState();
        
        /**
         * @brief Returns true if the current state changed the previous time readInput()
         * was called.
         * 
         * @return true 
         * @return false 
         */
        bool stateChanged();

        uint8_t getEvent();
};

#endif
