#pragma once

#include "BaseEphemeris.h"

using namespace std::literals::chrono_literals;

class GPSEphemeris :
	public BaseEphemeris
{
public:
	virtual const double GM() const noexcept override { return 3.986005e14; };
	virtual const double OMAGE_E() const noexcept override { return 7.2921151467e-5; };
	virtual const double F() const noexcept override { return -4.44280763339e-10; };
	virtual const std::chrono::nanoseconds TimeAvailableLim() const noexcept override { return std::chrono::nanoseconds(2 * 1h + 10 * 1min); };
};
