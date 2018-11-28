#include "cmd.h"

void cmd_CardNotify(String cardData) {
    Serial3.println((String)"@CARD_NOTIFY " + cardData);
    // Serial.println((String)"@CARD_NOTIFY " + cardData);
}

/**
 * @brief Notify ESP8266, trough serial, about the current state of a relay.
 * This function should be called whenever a relay changes it's state. 
 * It can also be requested by EPS8266 by sending a @RELAY_GET command.
 *
 * @RELAY_NOTIFY <relayId:INT> <relayState:BOOL>
 * 
 * @param relayId The id of the relay that will send a state notification
 */
void cmd_RelayNotify(uint8_t relayId) {
    bool state = relay[relayId].getState();
    Serial3.println((String)"@RELAY_NOTIFY " + relayId + " " + state);
    // Serial.println((String)"@RELAY_NOTIFY " + relayId + " " + state);
}

/**
 * @brief Notify ESP8266, trough serial, when a sensor changed it's state.
 * This function should be called whenever a sensor changed it's state.
 * It can also be requested by ESP8266 by sending a @SENSOR_GET command.
 * 
 * @SENSOR_NOTIFY <sensorId:INT> <sensorState:BOOL>
 * 
 * @param sensorId The id of the sensor that will send a state notification
 */
void cmd_SensorNotify(uint8_t sensorId) {
    bool state = sensor[sensorId].getState();
    Serial3.println((String)"@SENSOR_NOTIFY " + sensorId + " " + state);
    // Serial.println((String)"@SENSOR_NOTIFY " + sensorId + " " + state);
}

/**
 * @brief Handler of the @RELAY_SET command that is issued by ESP8266 when it
 * wants to set the state of a relay.
 * 
 * @RELAY_SET <relayId:INT> <relayState:BOOL>
 */
void cmd_onRelaySet(void) {
    char *arg1 = espSerial.next(); if (arg1 == NULL) return;
    char *arg2 = espSerial.next(); if (arg2 == NULL) return;

    uint8_t relayId = atoi(arg1); 
    if (relayId >= MAX_RELAYS) { return; }

    bool state = atoi(arg2);
    relay[relayId].setState(state);
}

void cmd_onRelaysGet(void) {
    cmd_RelaysNotify();
}
void cmd_RelaysNotify(void) {
    uint32_t relayStates = 0;

    for(int relayId = 0; relayId < MAX_RELAYS; relayId++) {
        relayStates ^= (-relay[relayId].getState() ^ relayStates) & (1UL << relayId);
    }

    Serial3.println((String)"@RELAYS_NOTIFY " + relayStates);
    // Serial.println((String)"@RELAYS_NOTIFY " + relayStates);
}

void cmd_onSensorsGet(void) {
    cmd_SensorsNotify();
}
void cmd_SensorsNotify(void) {
    uint32_t sensorStates = 0;

    for(int sensorId = 0; sensorId < MAX_RELAYS; sensorId++) {
        sensorStates ^= (-sensor[sensorId].getState() ^ sensorStates) & (1UL << sensorId);
    }

    Serial3.println((String)"@SENSORS_NOTIFY " + sensorStates);
    // Serial.println((String)"@SENSORS_NOTIFY " + sensorStates);
}

void cmd_ButtonNotify(uint8_t buttonId) {
    //Serial.println((String)"@BUTTON_NOTIFY " + buttonId + " " + button[buttonId].getEvent());
    Serial3.println((String)"@BUTTON_NOTIFY " + buttonId + " " + button[buttonId].getEvent());
}