#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include "Arduino.h"
#include <SerialCommand.h>
#include <Relay.h>
#include <Button.h>
#include <CardReader.h>

enum {
    RELAY_BALCONY_LIGHT,          // Releu 0 - Balcon
    RELAY_DINING_TABLE_LIGHT,     // Releu 1 - Dining Table
    RELAY_BEDROOM_LIGHT,          // Releu 2 - Dormitor
    RELAY_BEDROOM_HALLWAY_LIGHT,  // Releu 3 - Hol Dormitor
    RELAY_OFFICE_LIGHT,           // Releu 4 - Birou / Office
    RELAY_LIVING_LIGHT,           // Releu 5 - Living Hue Lights
    RELAY_DINING_SPOT_LIGHT,      // Releu 6 - Spoturi Dining/Living
    RELAY_DRESSING_LIGHT,         // Releu 7 - Dressing
    RELAY_ENTRANCE_LIGHT,         // Releu 8 - Intrare
    RELAY_BEDROOM_BATHROOM_FAN,   // Releu 9 - Ventilator Baie Dormitor
    RELAY_BEDROOM_BATHROOM_LIGHT, // Releu 10 - Baie Dormitor
    RELAY_LIVING_BATHROOM_FAN,    // Releu 11 - Ventilator Baie Oaspeti
    RELAY_LIVING_BATHROOM_LIGHT,  // Releu 12 - Baie Oaspeti
    RELAY_UNUSED_1,               // Relay 13 - N E U T I L I Z A T
    RELAY_UNUSED_2,               // Relay 14 - N E U T I L I Z A T
    RELAY_ENTRANCE_DOOR,          // Relay 15 - Usa Intrare

    MAX_RELAYS
};

enum {
    BUTTON_LIVING_BATHROOM_1,  // Buton 0 : Baie Oaspeti #S1
    BUTTON_BEDROOM_HALLWAY,    // Buton 1 : Hol Dormitor
    BUTTON_ENTRANCE,           // Buton 2 : Intrare
    BUTTON_DINING_WALL_4,      // Buton 3 : Perete Dining S4
    BUTTON_LIVING_BATHROOM_3,  // Buton 4 : Baile Oaspeti S3
    BUTTON_DINING_BALCONY,     // Buton 5 : Balcon
    BUTTON_BEDROOM_BATHROOM_1, // Buton 6 : Baie Dormitor S1
    BUTTON_OFFICE,             // Buton 7 : Birou
    BUTTON_DRESSING_WALL,      // Buton 8 : Dressing
    BUTTON_DINING_WALL_2,      // Buton 9 : Perete Dining S2
    BUTTON_BEDROOM_BATHROOM_2, // Buton 10: Baie Dormitor S2
    BUTTON_LIVING_BATHROOM_2,  // Buton 11: Baie Oaspeti S2
    BUTTON_DINING_WALL_3,      // Buton 12: Perete Dining S3
    BUTTON_DINING_WALL_1,      // Buton 13: Perete Dining S1
    BUTTON_BEDROOM,            // Buton 14: Dormitor

    MAX_BUTTONS
};

enum {
    SENSOR_ENTRY_DOOR,    // Sensor 0 - Usa Intrare
    SENSOR_LIVING_WINDOW, // Sensor 1 - Geam Living

    MAX_SENSORS
};

extern SerialCommand  espSerial;
extern CardReader     cardReader;
extern Relay          relay[MAX_RELAYS];
extern Button         button[MAX_BUTTONS];
extern Button         sensor[MAX_SENSORS];

#endif