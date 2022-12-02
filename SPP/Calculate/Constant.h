#pragma once

#include <corecrt_math_defines.h>

#define DEG2RAD(x) ((x) * (M_PI / 180))
#define RAD2DEG(x) ((x) * (180 / M_PI))

constexpr double LIGHT_SPEED = 2.99792458e8;
constexpr double H0 = 0; //Ã×
constexpr double T0 = 288.16; //¿ª¶ûÎÄ
constexpr double P0 = 1013.25; //mbar
constexpr double RH0 = 0.5; //%