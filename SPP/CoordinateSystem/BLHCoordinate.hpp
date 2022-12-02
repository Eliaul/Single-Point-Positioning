#pragma once

#define _USE_MATH_DEFINES
#include <iostream>
#include <string>
#include "Ellipsoid.hpp"

class XYZCoordinate;

template<CoordinateSystem T>
class BLHCoordinate
{
	friend class XYZCoordinate;
	template<CoordinateSystem TT>
	friend std::ostream& operator<<(std::ostream& cout, const BLHCoordinate<TT>& blh);

public:
	BLHCoordinate() :b(0), l(0), h(0) {}
	BLHCoordinate(double x, double y, double z) noexcept;
	double& operator()(int i);
    const double& operator()(int i) const;

private:
	double b;
	double l;
	double h;
};

template<CoordinateSystem T>
inline std::ostream& operator<<(std::ostream& cout, const BLHCoordinate<T>& blh)
{
	cout << "(" << blh.b << "°," << blh.l << "°," << blh.h << "m)";
	return cout;
}

template<CoordinateSystem T>
inline double& BLHCoordinate<T>::operator()(int i)
{
	switch (i)
	{
	case 1:
		return this->b;
	case 2:
		return this->l;
	case 3:
		return this->h;
	default:
		throw "索引无效";
	}
}

template<CoordinateSystem T>
inline const double& BLHCoordinate<T>::operator()(int i) const
{
	switch (i)
	{
	case 1:
		return this->b;
	case 2:
		return this->l;
	case 3:
		return this->h;
	default:
		throw "索引无效";
	}
}

template<CoordinateSystem T>
inline BLHCoordinate<T>::BLHCoordinate(double b, double l, double h) noexcept
{
	this->b = b;
	this->l = l;
	this->h = h;
}
