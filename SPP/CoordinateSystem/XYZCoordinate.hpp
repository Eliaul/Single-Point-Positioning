#pragma once

#include "../Calculate/NewtonIteration.hpp"
#include "BLHCoordinate.hpp"
#include "../LinerAlgebra/Vec3d.hpp"
#include "../Calculate/Constant.h"
#include <format>

class XYZCoordinate
{
	friend std::ostream& operator<<(std::ostream& cout, const XYZCoordinate& xyz);

public:
	XYZCoordinate() :x(0), y(0), z(0) {};
	XYZCoordinate(double x, double y, double z) noexcept;
	template<CoordinateSystem T>
	static XYZCoordinate FromBLH(const BLHCoordinate<T>& blh);
	template<CoordinateSystem T>
	static BLHCoordinate<T> ToBLH(const XYZCoordinate& xyz);
	static XYZCoordinate FromVec(const Vec3d& vec);
	static Vec3d ToVec(const XYZCoordinate& xyz);
	static double Distance(const XYZCoordinate& p1, const XYZCoordinate& p2);
	XYZCoordinate operator+(const XYZCoordinate& xyz) const;
	XYZCoordinate operator-(const XYZCoordinate& xyz) const;
	XYZCoordinate& operator+=(const XYZCoordinate& xyz);
	double& operator()(int i);
    const double& operator()(int i) const;
	bool operator==(const XYZCoordinate& xyz) const;

private:
	double x;
	double y;
	double z;
};

inline std::ostream& operator<<(std::ostream& cout, const XYZCoordinate& xyz)
{
	cout << "(" << xyz.x << "m," << xyz.y << "m," << xyz.z << "m)";
	return cout;
}

inline XYZCoordinate::XYZCoordinate(double x, double y, double z) noexcept
{
	this->x = x;
	this->y = y;
	this->z = z;
}

inline bool XYZCoordinate::operator==(const XYZCoordinate& xyz) const
{
	return abs(x - xyz.x) < 50 * DBL_EPSILON && abs(y - xyz.y) < 50 * DBL_EPSILON && abs(z - xyz.z) < 50 * DBL_EPSILON;
}

template<CoordinateSystem T>
inline BLHCoordinate<T> XYZCoordinate::ToBLH(const XYZCoordinate& xyz)
{
	if (xyz != XYZCoordinate(0, 0, 0))
	{
		BLHCoordinate<T> blh;
		blh.l = (atan2(xyz.y, xyz.x));
		double R = sqrt(xyz.x * xyz.x + xyz.y * xyz.y + xyz.z * xyz.z);
		double phi = atan(xyz.z / sqrt(xyz.x * xyz.x + xyz.y * xyz.y));
		auto func = [=](const double& x) {
			double W = sqrt(1 - Elliposid<T>::eccentricity_1 * Elliposid<T>::eccentricity_1 * sin(x) * sin(x));
			double equation = x - atan(tan(phi) * (1 + Elliposid<T>::semi_major * pow(Elliposid<T>::eccentricity_1, 2) * sin(x) / (xyz(3) * W)));
			double Dequation = 1 - (Elliposid<T>::semi_major * pow(Elliposid<T>::eccentricity_1, 2) * xyz(3) * cos(x) * tan(phi)) / (W * (xyz(3) * xyz(3) / pow(cos(phi), 2)
				+ pow(Elliposid<T>::eccentricity_1, 2) * sin(x) * (2 * Elliposid<T>::semi_major * xyz(3) * W * pow(tan(phi), 2) - sin(x) * (xyz(3) * xyz(3) + (-pow(Elliposid<T>::eccentricity_1, 2) * pow(Elliposid<T>::semi_major, 2) + xyz(3) * xyz(3)) * pow(tan(phi), 2)))));
			return equation / Dequation;
		};
		blh.b = NewtonIteration(phi, 1e-12, 10, func);
		double W = sqrt(1 - Elliposid<T>::eccentricity_1 * Elliposid<T>::eccentricity_1 * sin(blh.b) * sin(blh.b));
		blh.h = R * cos(phi) / cos(blh.b) - Elliposid<T>::semi_major / W;
		blh.l = RAD2DEG(blh.l);
		blh.b = RAD2DEG(blh.b);
		return blh;
	}
	return BLHCoordinate<T>(0, 0, -Elliposid<T>::semi_major);
}

inline XYZCoordinate XYZCoordinate::FromVec(const Vec3d& vec)
{
	return XYZCoordinate(vec(1), vec(2), vec(3));
}

inline Vec3d XYZCoordinate::ToVec(const XYZCoordinate& xyz)
{
	return Vec3d{ xyz.x, xyz.y, xyz.z };
}

inline double XYZCoordinate::Distance(const XYZCoordinate& p1, const XYZCoordinate& p2)
{
	return sqrt(pow(p1(1) - p2(1), 2) + pow(p1(2) - p2(2), 2) + pow(p1(3) - p2(3), 2));
}

template<CoordinateSystem T>
inline XYZCoordinate XYZCoordinate::FromBLH(const BLHCoordinate<T>& blh)
{
	double h = blh.h;
	double b = DEG2RAD(blh.b);
	double l = DEG2RAD(blh.l);
	double N = Elliposid<T>::semi_major / sqrt(1 - pow(Elliposid<T>::eccentricity_1, 2) * sin(b) * sin(b));
	XYZCoordinate xyz;
	xyz.x = (N + h) * cos(b) * cos(l);
	xyz.y = (N + h) * cos(b) * sin(l);
	xyz.z = (N * (1 - pow(Elliposid<T>::eccentricity_1, 2)) + h) * sin(b);
	return xyz;
}


inline XYZCoordinate XYZCoordinate::operator+(const XYZCoordinate& xyz) const
{
	return XYZCoordinate(this->x + xyz.x, this->y + xyz.y, this->z + xyz.z);
}

inline XYZCoordinate XYZCoordinate::operator-(const XYZCoordinate& xyz) const
{
	return XYZCoordinate(this->x - xyz.x, this->y - xyz.y, this->z - xyz.z);
}

inline XYZCoordinate& XYZCoordinate::operator+=(const XYZCoordinate& xyz)
{
	*this = XYZCoordinate(this->x + xyz.x, this->y + xyz.y, this->z + xyz.z);
	return *this;
}

inline double& XYZCoordinate::operator()(int i)
{
	switch (i)
	{
	case 1:
		return x;
	case 2:
		return y;
	case 3:
		return z;
	default:
		throw "索引无效";
	}
}

inline const double& XYZCoordinate::operator()(int i) const
{
	switch (i)
	{
	case 1:
		return x;
	case 2:
		return y;
	case 3:
		return z;
	default:
		throw "索引无效";
	}
}