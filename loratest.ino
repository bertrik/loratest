/**
 *  Simple test application, sends a ASCII sentence using an RFM95 module.
 *  
 *  The sentence is sent with parameters that allow it to be received using
 *  the LoRa plugin in sdrangelove.
 */

#include <SPI.h>
#include <RH_RF95.h>

// Singleton instance of the radio driver
static RH_RF95 rf95;

void setup()
{
    Serial.begin(9600);
    Serial.println("LoRa test application");
    if (!rf95.init()) {
        Serial.println("RF95 init failed!");
    }

    rf95.setFrequency(869.850);
    const RH_RF95::ModemConfig cfg = {
        // Register 0x1D:
        // BW         CR      1=implicit
        (0 << 4) | (2 << 1) | (1 << 0),
        // Register 0x1E:
        // SF
        (8 << 4),
        // Register 0x26:
        // bit3 = LowDataRateOptimization
        (1 << 3)
    };
    rf95.setModemRegisters(&cfg);
    rf95.setPreambleLength(8);
    rf95.setTxPower(0);
}

void loop()
{
    // send the LoRa message
    char data[] = "Hello World!";
    unsigned long txstart = millis();
    Serial.print("Sending: ");
    Serial.println(data);
    rf95.send((uint8_t *)data, sizeof(data));
    rf95.waitPacketSent();

    // wait some time
    unsigned long waittime = 10L * (millis() - txstart);
    delay(waittime);
}

