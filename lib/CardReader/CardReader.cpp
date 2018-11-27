#include "CardReader.h"

CardReader::CardReader(HardwareSerial &serial, uint8_t pin_de_re) : 
    pin_de_re(pin_de_re), 
    serial(serial) 
{
    pinMode(this->pin_de_re, OUTPUT);
    digitalWrite(this->pin_de_re, LOW);
    this->serial.begin(38400);
};

void CardReader::readCard(void) {
    this->cardData = "";

    digitalWrite(this->pin_de_re, LOW);
    delay(1);

    if (serial.available()) {
        int byteNumber           = 0;
        bool startHeaderOk       = true;
        bool startHeaderReceived = false;
        uint32_t startTime       = millis();

        while (startHeaderOk && millis() - startTime < 1000) {
            uint8_t byte;

            // READ BYTE
            if (serial.available()) {
                 byte = serial.read();
            } else {
                delay(10);  continue;
            }


            // CHECK START HEADER
            if (byteNumber == 0 && byte != 0xDD) { startHeaderOk = false; }
            if (byteNumber == 1 && byte != 0x42) { startHeaderOk = false; }
            if (byteNumber == 2 && byte != 0x00) { startHeaderOk = false; }
            if (byteNumber == 3 && byte != 0x00) { startHeaderOk = false; }
            if (byteNumber == 4 && byte != 0x01) { startHeaderOk = false; }
            if (byteNumber == 5 && byte != 0x05) { startHeaderOk = false; }
            if (byteNumber == 6 && startHeaderOk) {
                startHeaderReceived = true;
            }
            if (!startHeaderReceived) {
                byteNumber++;
                continue;
            }
            if (!startHeaderOk) {
                Serial.println("CARDREADER: Received Malformed Header");
                break;
            }
            
            // CHECK END HEADER
            if (byte == 0x9B) { 
                break; 
            }

            // READ CARD
            char strHexByte[2];
            sprintf(strHexByte, "%x", byte);
            if (this->cardData == "") {
                this->cardData = strHexByte; 
            } else {
                this->cardData = this->cardData + ":" + strHexByte;
            }
        }
    }
}

bool CardReader::cardWasScanned(void) {
    return this->cardData != "";
}

String CardReader::getCardData(void) {
    return this->cardData;
}

void CardReader::notifySuccess(void) {
    uint8_t sequence[] = {0xDD, 0x42, 0x0, 0x0, 0x1, 0x9E};

    digitalWrite(this->pin_de_re, HIGH);
    delay(1);

    for (uint8_t i=0; i < sizeof(sequence); i++) {
        this->serial.write(sequence[i]);
    }
    this->serial.flush();
}

void CardReader::notifyFailure(void) {
    uint8_t sequence[] = {0xDD, 0x42, 0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x5, 0xA, 0xA, 0x5, 0xA, 0xA, 0x9C};

    digitalWrite(this->pin_de_re, HIGH);
    delay(1);

    for (uint8_t i=0; i < sizeof(sequence); i++) {
        this->serial.write(sequence[i]);
    }
    this->serial.flush();
}