#include <Arduino.h>

/**
 * @Hardwares: M5AtomS3
 * @Platform Version: Arduino M5Stack Board Manager v2.0.9
 * @Dependent Library:
 * M5GFX: https://github.com/m5stack/M5GFX
 * M5Unified: https://github.com/m5stack/M5Unified
 * M5AtomS3: https://github.com/m5stack/M5AtomS3
 */

#include "M5AtomS3.h"
#include <SensirionI2CScd4x.h>

SensirionI2CScd4x scd4x;
int button_step = 0;

void setup() {

    uint16_t error;
    char errorMessage[256];
    scd4x.begin(Wire);
    Wire.begin(2,1);
    auto cfg = M5.config();
    AtomS3.begin(cfg);

        // stop potentially previously started measurement
    error = scd4x.stopPeriodicMeasurement();
    if (error) {
        Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

    // Start Measurement
    error = scd4x.startPeriodicMeasurement();
    if (error) {
        Serial.print("Error trying to execute startPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }
    
    AtomS3.Display.setTextColor(GREEN);
    //AtomS3.Display.setTextDatum(middle_center);
    AtomS3.Display.setFont(&fonts::Orbitron_Light_24);
    AtomS3.Display.setTextSize(1);
    AtomS3.Display.drawString("Click!", AtomS3.Display.width() / 2,
                              AtomS3.Display.height() / 2);
    Serial.println("Click BtnA to Test");
}

void loop() {

    AtomS3.update();

uint16_t error;
    char errorMessage[256];

    delay(100);

    // Read Measurement
    uint16_t co2      = 0;
    float co2pro      = 0.0f;
    float temperature = 0.0f;
    float humidity    = 0.0f;
    bool isDataReady  = false;
    error             = scd4x.getDataReadyFlag(isDataReady);
    if (error) {
        M5.Lcd.print("Error trying to execute readMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
        return;
    }
    if (!isDataReady) {
        return;
    }
    error = scd4x.readMeasurement(co2, temperature, humidity);
    if (error) {
        M5.Lcd.print("Error trying to execute readMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else if (co2 == 0) {
        M5.Lcd.println("Invalid sample detected, skipping.");
    } else {
        AtomS3.Display.clear();
        co2pro = co2*1.0e-4;
        AtomS3.Display.drawString("C: ", 5, 5);
        AtomS3.Display.drawString(String(co2pro), 40, 5);

        Serial.print("Co2:");
        Serial.print(co2);
        Serial.print(";Temp:");
        Serial.print(temperature);
        Serial.print(";RH:");
        Serial.println(humidity);
        AtomS3.Display.drawString("T: ", 5, 54);
        AtomS3.Display.drawString(String(temperature), 40, 54);
        AtomS3.Display.drawString("H: ", 5, 100);
        AtomS3.Display.drawString(String(humidity), 40, 100);

    }
}
