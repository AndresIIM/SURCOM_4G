#pragma once

float getDurafetTemp(float vTherm);
float calcularPH(float Eint_mV, float E0int25_mV, float temp_C);
float calcularE0int25(float calEint_mV, float calpH, float calTemp_C);