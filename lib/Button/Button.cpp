#include "Button.h"

Button::Button(uint8_t pin, bool pullupEnabled /*=true*/)
{
    uint32_t currentTime = millis();
    bool currentState = digitalRead(pin); // Button::RELEASED;

    pinMode(pin, pullupEnabled ? INPUT_PULLUP : INPUT);

    this->pin = pin;
    this->pullupEnabled = pullupEnabled;

    this->state = currentState;
    this->lastState = currentState;

    this->pinChangeTime = currentTime;
    this->prevPinValue = digitalRead(pin);
}

bool Button::readInput(void)
{
    bool debouncedState = this->state;

    // Debouncing Start
    uint32_t currentTime  = millis();
    bool pinValue = digitalRead(this->pin);
    if (pinValue != this->prevPinValue) {
        this->pinChangeTime = currentTime;
    }
    if (currentTime - this->pinChangeTime > this->pinDebounceTime) {
        debouncedState = this->pullupEnabled ? !pinValue : pinValue;
    }
    this->prevPinValue = pinValue;
    // Debouncing End

    this->lastState = this->state;
    this->state = debouncedState;



    if (this->stateChanged() && this->isReleased()) {
        this->releaseStartTime = currentTime;
    }

    if (this->stateChanged() && this->isPressed()) {
        this->pressStartTime = currentTime;
    }

    if (this->isPressed()) {
        pressedDuration = currentTime - pressStartTime;
    }
    if (this->isReleased()) {
        releasedDuration = currentTime - releaseStartTime;
    }

    
    // if (waitForEvent == "LCD" && pressedDuration > 1000 && this->isPressed()) {
    //     Serial.println("LONG PRESS");
    //     waitForEvent = "N";
    // } else if (waitForEvent == "LCD" && this->isReleased() && this->wasPressed()) {
    //     waitForEvent = "ClickDouble";
    // } else if (waitForEvent == "N" && this->isReleased()) {
    //     waitForEvent = "LCD";
    // } else if (waitForEvent == "CD" && this->isReleased() && releasedDuration > 300) {
    //     Serial.println("CLICK");
    //     waitForEvent = "LCD";
    // } else if (waitForEvent == "CD" && this->isReleased() && this->wasPressed()) {
    //     Serial.println("DOUBLE CLICK");
    //     waitForEvent = "LCD";
    // }


    this->event = EVENT_NONE;
    if (waitForEvent == "LC" && pressedDuration > 1200 && this->isPressed()) {
        this->event = EVENT_LONG_PRESS;
        waitForEvent = "N";
    } else if (waitForEvent == "LC" && this->isReleased() && this->wasPressed()) {
        this->event = EVENT_CLICK;
    } else if (waitForEvent == "N" && this->isReleased()) {
        waitForEvent = "LC";
    }

    return this->state;
}

bool Button::isPressed(void)
{
    return this->state == Button::PRESSED;
}

bool Button::wasPressed(void)
{
    return this->lastState == Button::PRESSED;
}

bool Button::wasReleased(void)
{
    return this->lastState == Button::RELEASED;
}

bool Button::getState() {
    return this->state;
}

bool Button::stateChanged() {
    return this->state != this->lastState;
}

bool Button::isReleased() {
    return this->state == Button::RELEASED;
}

uint8_t Button::getEvent() {
    return this->event;
}