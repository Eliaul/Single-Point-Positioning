#pragma once

#include "Vec.hpp"
#include <typeinfo>
#include <iostream>

class Vec3d: public VecBase<Vec3d>
{
public:
	Vec3d() :VecBase<Vec3d>(3) {}
	Vec3d(double x, double y, double z) :VecBase<Vec3d>(3) { elements[0][0] = x; elements[1][0] = y; elements[2][0] = z; }
	Vec3d(const Vec3d& vec) : VecBase<Vec3d>(vec) {}
	Vec3d(Vec3d&& vec) noexcept :VecBase<Vec3d>(std::move(vec)) { }
	Vec3d(std::initializer_list<double> list);
	Vec3d CloneSize() const { return Vec3d(); }
	operator Vec() const&;
	operator Vec() &&;

	using MatrixVecBase<Vec3d>::operator=;

	static Vec3d CrossProduct(const Vec3d& v1, const Vec3d& v2);
	static Vec3d Zeros() { return VecBase::Zeros(3); }
	static Vec3d Constant(double num) { return VecBase::Constant(3, num); }
};

inline Vec::operator Vec3d() const
{
	if (this->row != 3)
	{
		throw "无法转换";
	}
	return Vec3d(this->elements[0][0], this->elements[1][0], this->elements[2][0]);
}

inline Vec3d::Vec3d(std::initializer_list<double> list) : Vec3d()
{
	if (list.size() != 3)
		throw "长度不为3!";
	for (int i = 0; i < 3; i++)
	{
		elements[i][0] = *(list.begin() + i);
	}
}

inline Vec3d::operator Vec() const&
{
	Vec res(3);
	for (int i = 0; i < 3; i++)
	{
		res(i + 1) = this->elements[i][0];
	}
	return res;
}

inline Vec3d::operator Vec() &&
{
	Vec res;
	res.row = 3;
	res.col = 1;
	res.realRow = 3;
	res.realCol = 1;
	res.elements = this->elements;
	this->elements = nullptr;
	return res;
}

inline Vec3d Vec3d::CrossProduct(const Vec3d& v1, const Vec3d& v2)
{
	double x1 = v1.elements[0][0];
	double y1 = v1.elements[1][0];
	double z1 = v1.elements[2][0];
	double x2 = v2.elements[0][0];
	double y2 = v2.elements[1][0];
	double z2 = v2.elements[2][0];
	return Vec3d{ -y2 * z1 + y1 * z2 , x2 * z1 - x1 * z2 ,-x2 * y1 + x1 * y2 };
}