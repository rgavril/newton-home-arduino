#ifndef _PERSISTENT_DATA_h
#define _PERSISTENT_DATA_h

#include "Arduino.h"

class PersistentData {
    private:
        typedef struct Storable {         
            uint16_t relayStates;
        } Storable;

        Storable data;                    // Data structure that is stored in the eeprom
        bool dataChangedFlag = false;     // You have to set it to true whenever you change data
        uint8_t dataAddress;              // eeprom address where the data is currenly saved
        const uint8_t maxWrites = 250;    // number of consecutive eeprom writes on dataAddress 

        // update the dataAddress by rotating it to the right
        void updateDataAddress(void);

    public:
        // save the data into eeprom. This method will also do a check and
        // avoid writing if nothing changed since last write.
        void save();

        // Load the data from eeprom
        void load();

        // Updates the state of a the relayId in the storable data stucture
        void setRelayState(uint8_t relayId, bool state);

        // Read the state of a relay from the storable data structure
        bool getRelayState(uint8_t relayId);
};

#endif