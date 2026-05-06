#include "sd_logger.h"
#include <SPI.h>
#include <SD.h>
#include "config/pins.h"

bool sdReady = false;

bool sdInit() {
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
    sdReady = SD.begin(SD_CS);
    return sdReady;
}

String filename() {
    return "/data.csv";  // simplificamos ahora
}

bool sdAppend(const String& line) {
    if (!sdReady && !sdInit()) return false;

    File f = SD.open(filename(), FILE_APPEND);
    if (!f) return false;

    f.println(line);
    f.close();
    return true;
}