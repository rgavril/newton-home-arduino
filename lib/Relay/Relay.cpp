#include "Relay.h"

Relay::Relay(uint8_t pin, bool invertedLogic)
{
    this->pin = pin;
    this->invertedLogic = invertedLogic;

    pinMode(pin, OUTPUT);
    this->setState(Relay::OFF);
}

void Relay::setState(bool state)
{
    this->state = state;
    
    bool logicalState = this->invertedLogic ? !state : state;
    digitalWrite(this->pin, logicalState);
}

bool Relay::getState(void)
{
    return this->state;
}

void Relay::toggleState()
{
    this->setState(this->isOn() ? Relay::OFF : Relay::ON);
}

bool Relay::isOn() {
    return this->state == Relay::ON;
}

bool Relay::isOff() {
    return this->state == Relay::OFF;
}

void Relay::recordState() {
    this->recordedState = this->state;
}

bool Relay::stateChanged() {
    return this->recordedState != this->state;
}