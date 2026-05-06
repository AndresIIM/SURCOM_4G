#include "durafet.h"
#include <math.h>

float getDurafetTemp(float VTherm) {
    const float c0 = 340.9819863, c1 = -9.10257e-5;
    const float c2 = -95.08806667, c3 = 0.965370274;

    float RTherm = 10000.0 / ((3.3 / VTherm) - 1.0);
    float logR = log10(RTherm);

    return c0 + c1 * RTherm + c2 * logR + c3 * pow(logR, 3);
}

float calcularPH(float Eint_mV, float E0int25_mV, float temp_C) {
    const float R = 8.3145, F = 96487.0;
    float tempK = temp_C + 273.15;
    float S_T = (R * tempK / F) * log(10.0) * 1000.0;

    return (Eint_mV - E0int25_mV) / S_T;
}

float calcularE0int25(float calEint_mV, float calpH, float calTemp_C) {
    const float R = 8.3145, F = 96487.0;
    float tempK = calTemp_C + 273.15;
    float S_T = (R * tempK / F) * log(10.0) * 1000.0;

    return calEint_mV - S_T * calpH;
}