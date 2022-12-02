#pragma once

#include "VecBase.hpp"
#include "Matrix.hpp"
#include <initializer_list>

class Vec3d;

class Vec : public VecBase<Vec>
{
    friend class Vec3d;
    friend class Matrix;

public:
    Vec() :VecBase<Vec>() {}
    explicit Vec(int length) :VecBase<Vec>(length) { }
	Vec(std::initializer_list<double> list);
    Vec(const Vec& vec) : VecBase<Vec>(vec) {}
	Vec(Vec&& vec) noexcept :VecBase<Vec>(std::move(vec)) {}
	Vec CloneSize() const { return Vec(row); }
    operator Matrix() const&;
    operator Matrix() &&;
    explicit operator Vec3d() const;

    using MatrixVecBase<Vec>::operator=;

    Vec& Resize(int length);
};

inline Matrix::operator Vec() const&
{
    if (this->col != 1)
    {
        throw "无法转换";
    }
    Vec res(this->row);
    for (int i = 0; i < this->row; i++)
    {
        res(i + 1) = (*this)(i + 1, 1);
    }
    return res;
}

inline Matrix::operator Vec()&&
{
    if (this->col != 1)
    {
        throw "无法转换";
    }
    Vec res;
    res.row = this->row;
    res.col = 1;
    res.realRow = this->realRow;
    res.realCol = 1;
    res.elements = this->elements;
    this->elements = nullptr;
    return res;
}

inline Vec Matrix::GetRow(int row) const
{
    assert(row > 0);
    assert(row < this->row + 1);
    Vec res(this->col);
    for (int i = 0; i < this->col; i++)
    {
        res.elements[i][0] = this->elements[row - 1][i];
    }
    return res;
}

inline Vec Matrix::GetCol(int col) const
{
    assert(col > 0);
    assert(col < this->col + 1);
    Vec res(this->row);
    for (int i = 0; i < this->row; i++)
    {
        res.elements[i][0] = this->elements[i][col - 1];
    }
    return res;
}

inline Vec Matrix::operator*(const Vec& vec) const
{
    if (this->col != vec.Length())
    {
        throw "无法相乘";
    }
    Vec res(this->row);
    for (int i = 0; i < res.Length(); i++)
    {
        double num = 0;
        for (int j = 0; j < this->col; j++)
        {
            num += this->elements[i][j] * vec(j + 1);
        }
        res(i + 1) = num;
    }
    return res;
}

inline Vec::Vec(std::initializer_list<double> list)
	:VecBase<Vec>(static_cast<int>(list.size()))
{
	for (int i = 0; i < this->row; i++)
	{
		this->elements[i][0] = *(list.begin() + i);
	}
}

inline Vec::operator Matrix() const&
{
    Matrix res(this->row, 1);
    for (int i = 0; i < this->row; i++)
    {
        res(i + 1, 1) = (*this)(i + 1);
    }
    return res;
}

inline Vec::operator Matrix()&&
{
    Matrix res;
    res.row = this->row;
    res.col = 1;
    res.realRow = this->row;
    res.realCol = 1;
    res.elements = this->elements;
    this->elements = nullptr;
    return res;
}

inline Vec& Vec::Resize(int length)
{
    assert(length > 0);
    if (length <= this->row)
    {
        this->row = length;
        for (int i = this->row; i < this->realRow; i++)
        {
            delete[] elements[i];
            elements[i] = nullptr;
        }
        this->realRow = row;
        return *this;
    }
    for (int i = 0; i < this->realRow; i++)
    {
        delete[] this->elements[i];
        elements[i] = nullptr;
    }
    delete this->elements;
    elements = nullptr;
    this->row = length;
    this->realRow = length;
    this->elements = new double* [row];
    for (int i = 0; i < row; i++)
    {
        this->elements[i] = new double[1];
    }
    return *this;
}
