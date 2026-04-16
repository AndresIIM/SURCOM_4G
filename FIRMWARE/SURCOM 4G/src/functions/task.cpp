#include "functions/task.h"
#include "functions/config.h"
#include <SD.h>

static void ensureDirectory(const char *path) {
  if (!SD.exists(path)) {
    if (!SD.mkdir(path) && debugMode) {
      Serial.printf("ERROR: no se pudo crear directorio %s\n", path);
    }
  }
}

static void buildDailyFilePath(const DateTime &now, char *outPath, size_t size) {
  char pathYear[32];
  char pathMonth[48];

  sprintf(pathYear, "/surcom/%04u", now.year());
  sprintf(pathMonth, "%s/%02u", pathYear, now.month());

  ensureDirectory("/surcom");
  ensureDirectory(pathYear);
  ensureDirectory(pathMonth);

  sprintf(outPath, "%s/%04u%02u%02u_SURCOM.txt", pathMonth, now.year(), now.month(), now.day());
}

static void buildFrame(const SensorData &data, const DateTime &now, char *outFrame, size_t size) {
  sprintf(outFrame,
          "$DURAFET,%04u%02u%02u,%02u%02u%02u,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f",
          now.year(), now.month(), now.day(),
          now.hour(), now.minute(), now.second(),
          data.voltage,
          data.current,
          data.temp,
          data.cond,
          data.o2,
          data.saturation);
}

// Este archivo define las tareas que ejecuta FreeRTOS.
// Cada tarea corre en paralelo y realiza una responsabilidad clara.

void taskSensor(void *parameter) {
  SensorData data;

  while (true) {
    vTaskDelay(pdMS_TO_TICKS(sensorReadIntervalMs));

    // Lee la hora del RTC.
    DateTime now = rtc.now();
    data.timestamp = now.unixtime();

    // Lee datos de la fuente de corriente/voltaje INA219.
    data.voltage = ina219.getBusVoltage_V();
    data.current = ina219.getCurrent_mA();
    data.power   = ina219.getPower_mW();

    // Lee el ADC ADS1220 en milivoltios.
    float rawPHmV = ads1220.getVoltage_mV();
    data.ph = rawPHmV * 0.0005;

    data.temp = ads1220.getTemperature();

    // Lectura Modbus con exclusión mutua RS485.
    if (xSemaphoreTake(rs485Semaphore, pdMS_TO_TICKS(100))) {

      digitalWrite(RS485_EN_PIN, HIGH); // Habilita TX en el transceptor.

      uint8_t result = node.readHoldingRegisters(0x00, 4);

      if (result == node.ku8MBSuccess) {
        data.cond = node.getResponseBuffer(0) / 10.0;
        data.o2   = node.getResponseBuffer(1) / 10.0;
      }

      digitalWrite(RS485_EN_PIN, LOW); // Vuelve a RX.

      xSemaphoreGive(rs485Semaphore);
    }

    data.saturation = 0.0; // Valor placeholder, ajustar según sensor RS485 si se dispone.
    xQueueOverwrite(sensorQueue, &data);
  }
}

void taskSD(void *parameter) {
  // Tarea que escribe los datos en la SD en un archivo diario.
  SensorData data;
  char filePath[128];
  char frame[192];

  while (true) {
    vTaskDelay(pdMS_TO_TICKS(dataSendIntervalMs));

    if (xQueueReceive(sensorQueue, &data, 0) == pdTRUE) {
      DateTime now = rtc.now();
      buildDailyFilePath(now, filePath, sizeof(filePath));
      buildFrame(data, now, frame, sizeof(frame));

      Serial.println(frame);
      if (debugMode) {
        Serial.println("DEBUG: Enviando trama al log y SD");
      }

      if (xSemaphoreTake(sdSemaphore, portMAX_DELAY)) {
        File file = SD.open(filePath, FILE_APPEND);
        if (file) {
          file.println(frame);
          file.close();
        } else if (debugMode) {
          Serial.printf("ERROR: no se pudo abrir %s\n", filePath);
        }
        xSemaphoreGive(sdSemaphore);
      }
    } else if (debugMode) {
      Serial.println("DEBUG: no hay datos nuevos para escribir en SD");
    }
  }
}

void taskWatchdog(void *parameter) {
  // Tarea que vigila el voltaje de la batería y reinicia el ESP32 si es necesario.
  while (true) {
    vTaskDelay(pdMS_TO_TICKS(60000));

    if (ina219.getBusVoltage_V() < 11.0) {
      Serial.println("LOW BATTERY");
      ESP.restart();
    }
  }
}