#ifndef __COMMANDS_H__
#define __COMMANDS_H__

#include "globals.h"

void cmd_CardNotify    (String cardData);
void cmd_RelaysNotify  (void);
void cmd_SensorsNotify (void);
void cmd_RelayNotify   (uint8_t relayId);
void cmd_SensorNotify  (uint8_t sensorId);
void cmd_ButtonNotify  (uint8_t buttonId);


void cmd_onRelaySet   (void);
void cmd_onRelayGet   (void);
void cmd_onSensorGet  (void);
void cmd_onSensorsGet (void);
void cmd_onRelaysGet  (void);

#endif