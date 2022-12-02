#pragma once
#include "Matrix.hpp"
#include "Vec3d.hpp"

class Matrix3d : public MatrixBase<Matrix3d>
{
public:
	Matrix3d() :MatrixBase<Matrix3d>(3, 3) {}
	Matrix3d(const Matrix3d& matrix) : MatrixBase<Matrix3d>(matrix) {}
	Matrix3d(Matrix3d&& matrix) noexcept :MatrixBase<Matrix3d>(std::move(matrix)) {}
	Matrix3d(const Vec3d& v1, const Vec3d& v2, const Vec3d& v3);
	Matrix3d(std::initializer_list<std::initializer_list<double>> list);
	using MatrixBase<Matrix3d>::operator*;
	Vec3d operator*(const Vec3d& vec) const;
	Matrix3d CloneSize() const { return Matrix3d(); }
	
	operator Matrix() const&;
	operator Matrix() &&;

	using MatrixVecBase<Matrix3d>::operator=;

	static Matrix3d Zeros() { return MatrixBase::Zeros(3, 3); }
	static Matrix3d Identity() { return MatrixBase::Identity(3); }
	static Matrix3d Random() { return MatrixBase::Random(3, 3); }
};

inline Matrix3d::Matrix3d(const Vec3d& v1, const Vec3d& v2, const Vec3d& v3)
	:MatrixBase<Matrix3d>(3, 3)
{
	for (int j = 0; j < 3; j++)
	{
		this->elements[0][j] = v1(j + 1);
	}
	for (int j = 0; j < 3; j++)
	{
		this->elements[1][j] = v2(j + 1);
	}
	for (int j = 0; j < 3; j++)
	{
		this->elements[2][j] = v3(j + 1);
	}
}

inline Matrix3d::Matrix3d(std::initializer_list<std::initializer_list<double>> list)
	: Matrix3d()
{
	if (list.size() != 3 || list.begin()->size() != 3)
	{
		throw "长度不为9";
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			elements[i][j] = *((list.begin() + i)->begin() + j);
		}
	}
}

inline Vec3d Matrix3d::operator*(const Vec3d& vec) const
{
	Vec3d res;
	for (int i = 0; i < 3; i++)
	{
		double num = 0;
		for (int j = 0; j < 3; j++)
		{
			num += this->elements[i][j] * vec(j + 1);
		}
		res(i + 1) = num;
	}
	return res;
}

inline Matrix3d::operator Matrix() const&
{
	Matrix res(3, 3);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			res(i + 1, j + 1) = this->elements[i][j];
		}
	}
	return res;
}

inline Matrix3d::operator Matrix()&&
{
	Matrix res;
	res.row = 3;
	res.col = 3;
	res.realRow = 3;
	res.realCol = 3;
	res.elements = this->elements;
	this->elements = nullptr;
	return res;
}

