#include "PersistentData.h"
#include "EEPROM.h"

void PersistentData::updateDataAddress(void) {
    this->dataAddress += sizeof(Storable);

    // If there is no enough space to store the data at the current calculated
    // address we'll move back to the begining of the EEPROM, right after where
    // the dataAddress value is stored.
    if (this->dataAddress + sizeof(Storable) > EEPROM.length()) {
        this->dataAddress = sizeof(uint8_t);
    }

    Serial.println("PersistentData::updateDataAddress() is performing a EEPROM write.");
    EEPROM.put(0, this->dataAddress);
}

void PersistentData::save(void)
{
    static uint8_t numberOfSaves = 0;
    static uint32_t lastSaveTime = 0;

    // Perform the write only if data actually changed
    if (! this->dataChangedFlag) {
        return;
    }

    // Prevent writing more the MAX_NUMBER_OF_WRITES on the same address
    if (++numberOfSaves > this->maxWrites) {
        this->updateDataAddress();
        numberOfSaves = 0;
    }

    // Perform the actual write
    if (millis() - lastSaveTime > 1000) {
        Serial.println("PersistentData::save() is performing a EEPROM write.");
        EEPROM.put(this->dataAddress, this->data);
        lastSaveTime = millis();
    } else {
        Serial.println("PersistentData::save() is performing a EEPROM writes to fast !!");
    }

    // Reset the chagned flag
    this->dataChangedFlag = false;
}

void PersistentData::load(void)
{
    EEPROM.get(0, this->dataAddress);
    EEPROM.get(this->dataAddress, this->data);
}

void PersistentData::setRelayState(uint8_t relayId, bool state)
{
    uint16_t relayStatesOld = data.relayStates;

    // Set the bit at position 'relayId' to value 'state'
    data.relayStates ^= (-state ^ data.relayStates) & (1UL << relayId);

    if (data.relayStates != relayStatesOld) {
        this->dataChangedFlag = true;
    }
}

bool PersistentData::getRelayState(uint8_t relayId)
{
    // Return the value of the bit from position 'relayId'
    return data.relayStates & 1 << relayId;
}