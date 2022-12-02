#pragma once

#include "BaseEphemeris.h"

using namespace std::literals::chrono_literals;

class BDSEphemeris :
	public BaseEphemeris
{
public:
	double tgd1;
	double tgd2;

	virtual XYZCoordinate CalSatePos() const override;
	virtual const double GM() const noexcept override { return 3.986004418e14; };
	virtual const double OMAGE_E() const noexcept override { return 7.2921150e-5; };
	virtual const double F() const noexcept override { return -4.44280763339e-10; };
	virtual const std::chrono::nanoseconds TimeAvailableLim() const noexcept override { return std::chrono::nanoseconds(1 * 1h + 10 * 1min); };
};
