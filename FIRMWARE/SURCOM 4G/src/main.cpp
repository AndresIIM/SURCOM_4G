#include <Arduino.h>
#include "functions/config.h"
#include "functions/hardware.h"
#include "functions/task.h"

// ---------------------------------------------------------------------------
// Este es el archivo principal de la aplicación.
// PlatformIO compila este archivo junto con los otros .cpp y crea el firmware.
// Aquí se declara el hardware global y se levanta el programa.
// ---------------------------------------------------------------------------

// Objetos globales: declarados aquí y referenciados desde otros módulos.
Adafruit_INA219 ina219;
RTC_DS3231 rtc;
ADS1220_WE ads1220(ADS1220_CS_PIN, ADS1220_DRDY_PIN);
ModbusMaster node;

uint32_t sensorReadIntervalMs = DEFAULT_SENSOR_INTERVAL_MS;
uint32_t dataSendIntervalMs = DEFAULT_SEND_INTERVAL_MS;
bool debugMode = DEFAULT_DEBUG_MODE;

// RTOS
QueueHandle_t sensorQueue;
SemaphoreHandle_t sdSemaphore;
SemaphoreHandle_t rs485Semaphore;

void setup() {
  Serial.begin(115200);

  initI2C();
  initSD();
  initRTC();
  initADS();
  initModbus();

  // Usamos una cola de longitud 1 para mantener siempre el último dato leído.
  sensorQueue = xQueueCreate(1, sizeof(SensorData));
  sdSemaphore = xSemaphoreCreateMutex();
  rs485Semaphore = xSemaphoreCreateMutex();

  xTaskCreate(taskSensor, "Sensor", 4096, NULL, 3, NULL);
  xTaskCreate(taskSD, "SD", 4096, NULL, 2, NULL);
  xTaskCreate(taskWatchdog, "WD", 2048, NULL, 4, NULL);
}

void loop() {
  vTaskDelay(pdMS_TO_TICKS(10000));
}