#ifndef CONFIG_H
#define CONFIG_H

// Archivo de configuración compartida.
// Aquí se definen pines, tipos de datos y variables globales usadas por varios módulos.

#include <Arduino.h>
#include <Adafruit_INA219.h>
#include <RTClib.h>
#include <ADS1220_WE.h>
#include <ModbusMaster.h>

// Pines de hardware utilizados por el firmware.
#define SD_CS_PIN 15
#define RS485_EN_PIN 16
#define MODBUS_TX_PIN 17
#define MODBUS_RX_PIN 18
#define ADS1220_CS_PIN 5
#define ADS1220_DRDY_PIN 4

#define SERIAL_MODBUS Serial1

#define DEFAULT_SENSOR_INTERVAL_MS 1000
#define DEFAULT_SEND_INTERVAL_MS 1000
#define DEFAULT_DEBUG_MODE false

// Estructura para los datos leídos por la tarea de sensores.
struct SensorData {
  long timestamp;
  float ph;
  float temp;
  float cond;
  float o2;
  float saturation;
  float voltage;
  float current;
  float power;
};

// Declaraciones externas de variables globales.
// Se instancian en main.cpp y se usan desde otros archivos.
extern Adafruit_INA219 ina219;
extern RTC_DS3231 rtc;
extern ADS1220_WE ads1220;
extern ModbusMaster node;

extern uint32_t sensorReadIntervalMs;
extern uint32_t dataSendIntervalMs;
extern bool debugMode;

// Declaraciones externas de objetos FreeRTOS.
extern QueueHandle_t sensorQueue;
extern SemaphoreHandle_t sdSemaphore;
extern SemaphoreHandle_t rs485Semaphore;

#endif