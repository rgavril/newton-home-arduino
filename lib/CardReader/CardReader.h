#ifndef __CARD_READER_H__
#define __CARD_READER_H__

#include "Arduino.h"

// Not all pins on the Mega and Mega 2560 support change interrupts, so only the following can be used for RX: 
// 10, 11, 12, 13, 14, 15, 50, 51, 52, 53, A8 (62), A9 (63), A10 (64), A11 (65), A12 (66), A13 (67), A14 (68), A15 (69).
class CardReader {
    private:
        uint8_t pin_de_re;       // Pin used by 485 converter to switch between transmit and receive
        HardwareSerial &serial;  // Serial port connected to 485 converter  
        String cardData;         // Data from the last card read
    
    public:
        /**
         * @brief Construct a new Card Reader object
         * 
         * @param serial     Serial port connected to 485 converter
         * @param pin_de_re  Pin connected to DE/RE on 485 converter
         */
        CardReader(HardwareSerial &serial, uint8_t pin_de_re);

        /**
         * @brief Tries to read a card from the card reader. This method will overwrite
         * the data from pervious card read.
         */
        void readCard(void);

        /**
         * @brief Returns true if the previous call to readCard was successfull.
         * 
         * @return true 
         * @return false 
         */
        bool cardWasScanned(void);

        /**
         * @brief Get the Card Data object. The data will be returned as in a string
         * as a list of hex values separated by double colon.
         * 
         * @return String 
         */
        String getCardData(void);

        /**
         * @brief Send a notification to the card reader that will make it
         * show a green color that represents scuccess.
         * 
         */
        void notifySuccess(void);

        /**
         * @brief Sends a notification to the card reader that will make it
         * show a red pulsating color that represents failure.
         * 
         */
        void notifyFailure(void);
};

#endif