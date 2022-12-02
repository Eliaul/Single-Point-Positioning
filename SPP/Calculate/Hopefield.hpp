#pragma once

#include <cmath>
#include "Constant.h"

[[nodiscard]]
inline double Hopefield(double angle, double H)
{
	if (H > 10000 || H < -10000)
	{
		return 0;
	}
	double RH = RH0 * exp(-0.0006396 * (H - H0));
	double P = P0 * pow(1 - 0.0000226 * (H - H0), 5.225);
	double T = T0 - 0.0065 * (H - H0);
	double e = RH * exp(-37.2465 + 0.213166 * T - 0.000256908 * T * T);
	double h_d = 40136 + 148.72 * (T0 - 273.16);
	double K_w = 155.2e-7 * 4810 * e * (11000 - H) / (T * T);
	double K_d = 155.2e-7 * P * (h_d - H) / T;
	return K_d / sin(DEG2RAD(sqrt(angle * angle + 6.25))) + K_w / sin(DEG2RAD(sqrt(angle * angle + 2.25)));
}