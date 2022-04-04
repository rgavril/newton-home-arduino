#include <Arduino.h>

#include <Relay.h>
#include <Button.h>
#include <PersistentData.h>
#include <SerialCommand.h>
#include <CardReader.h>

#include "globals.h"
#include "cmd.h"

PersistentData  persistentData;
SerialCommand   espSerial;
CardReader      cardReader(Serial2, 2);

Relay relay[MAX_RELAYS] = {
    Relay(A0),  // Releu 0 - Balcon
    Relay(A1),  // Releu 1 - Dining Table
    Relay(A2),  // Releu 2 - Dormitor
    Relay(A3),  // Releu 3 - Hol Dormitor
    Relay(A4),  // Releu 4 - Birou / Office
    Relay(A5),  // Releu 5 - Living Hue Lights
    Relay(A6),  // Releu 6 - Spoturi Dining/Living
    Relay(A7),  // Releu 7 - Dressing
    Relay(A8),  // Releu 8 - Intrare
    Relay(A9),  // Releu 9 - Ventilator Baie Dormitor
    Relay(A10), // Releu 10 - Baie Dormitor
    Relay(A11), // Releu 11 - Ventilator Baie Oaspeti
    Relay(A12), // Releu 12 - Baie Oaspeti
    Relay(A13), // Releu 13 -
    Relay(A14), // Releu 14 - Senzor Fum
    Relay(A15)  // Releu 15 - Electroyala Usa Intrare
};

Button button[MAX_BUTTONS] = {
    Button(28), // Buton 0 - Baie Oaspeti S1
    Button(24), // Buton 1 - Dormitor Hol
    Button(4),  // Buton 2 - Intrare
    Button(9),  // Buton 3 - Perete Dining S4
    Button(30), // Buton 4 - Baie Oaspeti S3
    Button(10), // Buton 5 - Balcon
    Button(26), // Buton 6 - Baie Dormitor S1
    Button(27), // Buton 7 - Birou S1
    Button(7),  // Buton 8 - Dressing
    // Button(13), // Buton 2 - Intrare
    Button(4), // Buton 2 - Intrare
    Button(9), // Buton 3 - Perete Dining S4
    Button(30),  // Buton 4 - Baie Oaspeti S3
    Button(10),  // Buton 5 - Balcon
    Button(26),  // Buton 6 - Baie Dormitor S1
    Button(27),  // Buton 7 - Birou
    Button(7), // Buton 8 - Dressing
    Button(11), // Buton 9 - Perete Dining S2
    Button(25), // Buton 10 - Baie Dormitor S2
    Button(29), // Buton 11 - Baie Oaspeti S2
    Button(6),  // Buton 12 - Perete Dining S3
    Button(8),  // Buton 13 - Perete Dining S1
    Button(12), // Buton 14 - Dormitor S2 / Pat
    Button(5),  // Buton 15 - Dormitor S1
    Button(23)  // Buton 16 - Birou S2
};

Button sensor[MAX_SENSORS] = {
    Button(31), // Sensor 0 - Usa Intrare
    Button(32), // Sensor 1 - Geam Living
    Button(33), // Sensor 2 - Fum
    Button(34)  // Sensor 3 - Apa
};


/**
 * @brief Toggles a relay with id relayId when the button with id buttonId
 * was clicked, meaning that it changed state from released to pressed.
 * 
 * @param buttonId The id of the button that will be checked for clicks
 * @param relayId  The id of the relay that will be toggled
 */
void toggleRelayOnClick(int buttonId, int relayId) {

    if (button[buttonId].getEvent() == Button::EVENT_CLICK) {
        relay[relayId].toggleState();
    }
}

void toggleRelayOnLongPress(int buttonId, int relayId) {
    if (button[buttonId].getEvent() == Button::EVENT_LONG_PRESS) {
        relay[relayId].toggleState();
    }
}

void automaticallyCloseDoor() {
    static uint32_t doorOpenedTime = 0;
    static bool doorIsOpen = false;

    uint32_t currentTime = millis();

    if (relay[RELAY_ENTRANCE_DOOR].isOn() && !doorIsOpen) {
        doorIsOpen = true;
        doorOpenedTime = currentTime;
    }

    if (currentTime - doorOpenedTime > 3000 && doorIsOpen) {
        relay[RELAY_ENTRANCE_DOOR].setState(false);
        doorIsOpen = false;
    }
}

/**
 * @brief Called when a sketch starts. Run once, after each powerup or reset
 * of the Arduino board.
 * 
 */
void setup() {
    // Setup serial
    Serial.begin(115200);
    Serial3.begin(115200);
    Serial.println("\nNewton Relay Control Started");

    // FIXME: Use PIN 3 as GND for MAX485 (Fix this by changing the cable in the panel)
    pinMode(3, OUTPUT); digitalWrite(3, LOW);

    // Load relay states from EEPROM
    persistentData.load();
    for (int i=0; i<MAX_RELAYS; i++) {
        if (i != RELAY_ENTRANCE_DOOR) {
            relay[i].setState(persistentData.getRelayState(i));
        }
    }
    
    // Notify ESP8266 of the current sensor states
    for (int i=0; i<MAX_SENSORS; i++) {
        cmd_SensorNotify(i);
    }

    // Notify ESP8266 of the current relay states
    for (int i=0; i<MAX_RELAYS; i++) {
        cmd_RelayNotify(i);
    }

    // Add serial commands
    espSerial.addCommand("@RELAY_SET"  , cmd_onRelaySet);
    espSerial.addCommand("@RELAYS_GET" , cmd_onRelaysGet);
    espSerial.addCommand("@SENSORS_GET", cmd_onSensorsGet);
}

void loop() {
    // Read button inputs
    for (int i=0; i<MAX_BUTTONS; i++) button[i].readInput();

    // Read the current state of the sensors
    for (int i=0; i<MAX_SENSORS; i++) sensor[i].readInput();

    // Record current state of the relays
    for (int i=0; i<MAX_RELAYS; i++) relay[i].recordState();

    // Perform command received from EPS8266 trough serial
    espSerial.readSerial();

    // Read the card if it was scanned
    cardReader.readCard();
    if (cardReader.cardWasScanned()) {
        Serial.println((String)"Card Scanned : " + cardReader.getCardData());
        if (
            1 == 0
            || cardReader.getCardData() == "c5:9:41:e2:6f"  // Razvan
            || cardReader.getCardData() == "85:5:45:e2:27"  // Andreea
        ) {
            cardReader.notifySuccess();
            relay[RELAY_ENTRANCE_DOOR].setState(true);
        } else {
            cardReader.notifyFailure();
        }
    }

    // Perform lightswitch actions
    toggleRelayOnClick(BUTTON_OFFICE_1          , RELAY_OFFICE_LIGHT);
    toggleRelayOnClick(BUTTON_BEDROOM_HALLWAY   , RELAY_BEDROOM_HALLWAY_LIGHT);
    toggleRelayOnClick(BUTTON_BEDROOM_BATHROOM_1, RELAY_BEDROOM_BATHROOM_LIGHT);
    toggleRelayOnClick(BUTTON_BEDROOM_BATHROOM_2, RELAY_BEDROOM_BATHROOM_FAN);
    toggleRelayOnClick(BUTTON_DINING_BALCONY    , RELAY_BALCONY_LIGHT);
    toggleRelayOnClick(BUTTON_DINING_WALL_1     , RELAY_ENTRANCE_LIGHT);
    toggleRelayOnClick(BUTTON_DINING_WALL_2     , RELAY_DINING_SPOT_LIGHT);
    toggleRelayOnClick(BUTTON_DINING_WALL_4     , RELAY_DINING_TABLE_LIGHT);
    toggleRelayOnClick(BUTTON_LIVING_BATHROOM_2 , RELAY_LIVING_BATHROOM_LIGHT);
    toggleRelayOnClick(BUTTON_LIVING_BATHROOM_3 , RELAY_LIVING_BATHROOM_FAN);
    toggleRelayOnClick(BUTTON_DRESSING_WALL     , RELAY_DRESSING_LIGHT);
    toggleRelayOnClick(BUTTON_ENTRANCE          , RELAY_ENTRANCE_LIGHT);

    toggleRelayOnLongPress(BUTTON_BEDROOM_1         , RELAY_BEDROOM_LIGHT);
    toggleRelayOnLongPress(BUTTON_DINING_WALL_3     , RELAY_LIVING_LIGHT);
    toggleRelayOnLongPress(BUTTON_LIVING_BATHROOM_1 , RELAY_LIVING_LIGHT);

    // Make sure the door relay will not stay open for to long
    automaticallyCloseDoor();

    // Notify ESP8266 when a relay changed it's state
    for (int i=0; i<MAX_RELAYS; i++) {
        if (relay[i].stateChanged()) {
            cmd_RelayNotify(i);
        }
    }

    // Notify ESP8266 when a sensor changed it's state
    for (int i=0; i<MAX_SENSORS; i++) {
        if (sensor[i].stateChanged()) {
            cmd_SensorNotify(i);
        }
    }

    // Notify ESP8266 when a button generated a event
    for (int i=0; i<MAX_BUTTONS; i++) {
        if (button[i].getEvent() != Button::EVENT_NONE) {
            cmd_ButtonNotify(i);
        }
    }

    // Store relay states in eeprom
    for (int i=0; i<MAX_RELAYS; i++) {
        if (i == RELAY_ENTRANCE_DOOR) continue;
        // Store relay state in EEPROM
        persistentData.setRelayState(i, relay[i].getState());
    }

    persistentData.save(); //<-- Avoid this until production to save EEPROM
}