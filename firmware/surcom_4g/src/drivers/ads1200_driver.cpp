#include "ads1220_driver.h"

ADS1220_Driver::ADS1220_Driver(int cs, int drdy)
    : ads(cs, drdy) {}

bool ADS1220_Driver::begin() {
    if (!ads.init()) return false;
    ads.setDataRate(ADS1220_DR_LVL_1);
    ads.bypassPGA(true);
    return true;
}

float ADS1220_Driver::readElectrode_mV() {
    ads.setCompareChannels(ADS1220_MUX_0_1);
    ads.getRawData();
    return ads.getVoltage_mV();
}

float ADS1220_Driver::readThermistor_mV() {
    ads.setCompareChannels(ADS1220_MUX_2_AVSS);
    ads.getRawData();
    return ads.getVoltage_mV();
}