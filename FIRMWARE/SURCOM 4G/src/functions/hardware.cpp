#include "functions/hardware.h"
#include "functions/config.h"
#include <Wire.h>
#include <SD.h>

// Archivo de inicialización de hardware. Separa la lógica de configuración
// de los sensores y buses de comunicación en un solo lugar.

void initI2C() {
  // Inicializa el bus I2C para dispositivos como el RTC y los sensores I2C.
  Wire.begin();
  Wire.setClock(100000);
}

void initSD() {
  // Configura la tarjeta SD y crea el archivo de datos si no existe.
  pinMode(SD_CS_PIN, OUTPUT);

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Error SD");
    return;
  }

  if (!SD.exists("/data.csv")) {
    File f = SD.open("/data.csv", FILE_WRITE);
    if (f) {
      f.println("Time,pH,Temp,Cond,O2,Volts,Current,Power");
      f.close();
    }
  }
}

void initRTC() {
  // Inicializa el reloj de tiempo real.
  if (!rtc.begin()) {
    Serial.println("Error RTC");
  }
}

void initADS() {
  // Inicializa el ADC ADS1220 con los parámetros básicos de medición.
  if (!ads1220.init()) {
    Serial.println("Error ADS1220");
    return;
  }

  ads1220.setCompareChannels(ADS1220_MUX_0_1);
  ads1220.setGain(ADS1220_GAIN_1);
  ads1220.setDataRate(ADS1220_DR_LVL_6);
  ads1220.enableTemperatureSensor(true);
  ads1220.start();
}

void initModbus() {
  // Configura RS485 y el objeto Modbus.
  pinMode(RS485_EN_PIN, OUTPUT);
  digitalWrite(RS485_EN_PIN, LOW);

  SERIAL_MODBUS.begin(9600, SERIAL_8N1, MODBUS_RX_PIN, MODBUS_TX_PIN);
  node.begin(1, SERIAL_MODBUS); // ID esclavo 1
}