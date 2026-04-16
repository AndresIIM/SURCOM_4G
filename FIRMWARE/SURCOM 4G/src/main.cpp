#include <Arduino.h>

#include "drivers/ads1220_driver.h"
#include "sensors/durafet.h"
#include "storage/sd_logger.h"
#include "config/pins.h"

ADS1220_Driver adc(ADS_CS, ADS_DRDY);

float E0int25 = 0;

void setup() {
    Serial.begin(115200);

    if (!adc.begin()) {
        Serial.println("ADS1220 ERROR");
        while(1);
    }

    if (!sdInit()) {
        Serial.println("SD ERROR");
    }

    E0int25 = calcularE0int25(76.40, 8.04, 26.8);

    Serial.println("Sistema listo");
}

void loop() {
    float e = adc.readElectrode_mV();
    float vt = adc.readThermistor_mV();

    float temp = getDurafetTemp(vt / 1000.0);
    float ph = calcularPH(e, E0int25, temp);

    String line = String(ph,3) + "," + String(temp,2);

    Serial.println(line);
    sdAppend(line);

    delay(5000);
}