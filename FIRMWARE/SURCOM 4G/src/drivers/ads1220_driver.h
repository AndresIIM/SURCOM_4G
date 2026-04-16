#pragma once
#include <ADS1220_WE.h>

class ADS1220_Driver {
public:
    ADS1220_Driver(int cs, int drdy);

    bool begin();
    float readElectrode_mV();
    float readThermistor_mV();

private:
    ADS1220_WE ads;
};