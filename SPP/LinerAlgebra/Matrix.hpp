#pragma once

#include <iostream>
#include <format>
#include "MatrixBase.hpp"

class Vec;
class Matrix3d;

class Matrix : public MatrixBase<Matrix>
{
    friend class Vec;
    friend class Matrix3d;
    friend struct std::formatter<Matrix>;

public:
    Matrix() :MatrixBase<Matrix>() { }
    Matrix(int row, int col) :MatrixBase<Matrix>(row, col) { }
	Matrix(std::initializer_list<std::initializer_list<double>> list);
    Matrix(const Matrix& matrix) : MatrixBase<Matrix>(matrix) { }
    Matrix(Matrix&& matrix) noexcept :MatrixBase<Matrix>(std::move(matrix)) { }
	Matrix CloneSize() const { return Matrix(row, col); }

    explicit operator Vec() const&;
    explicit operator Vec() &&;

    using MatrixVecBase<Matrix>::operator=;
    using MatrixBase<Matrix>::operator*;
    Vec operator*(const Vec& vec) const;

    Matrix& Resize(int row, int col);
    Matrix& DeleteRow(int row);
    Matrix& DeleteCol(int col);
    Matrix& DeleteCol(int startCol, int length);
    Vec GetRow(int row) const;
    Vec GetCol(int col) const;
};

#pragma region "自定义Format"
template<>
struct std::formatter<Matrix> : std::formatter<double>
{
    auto format(const Matrix& matrix, std::format_context& ctx)
    {
        for (int i = 0; i < matrix.row; i++)
        {
            for (int j = 0; j < matrix.col; j++)
            {
                std::formatter<double>::format(matrix.elements[i][j], ctx);
                std::format_to(ctx.out(), " ");
            }
            std::format_to(ctx.out(), "\n");
        }
        return ctx.out();
    }
};
#pragma endregion

inline Matrix::Matrix(std::initializer_list<std::initializer_list<double>> list)
	:MatrixBase<Matrix>(static_cast<int>(list.size()), static_cast<int>(list.begin()->size()))
{
	for (int i = 0; i < this->row; i++)
	{
		for (int j = 0; j < this->col; j++)
		{
			this->elements[i][j] = *((list.begin() + i)->begin() + j);
		}
	}
}

inline Matrix& Matrix::Resize(int row, int col)
{
    assert(row > 0);
    assert(col > 0);
    if (row <= this->row && col <= this->col)
    {
        this->row = row;
        this->col = col;
        for (int i = this->row; i < this->realRow; i++)
        {
            delete[] elements[i];
            elements[i] = nullptr;
        }
        this->realRow = row;
        this->realCol = col;
        return *this;
    }
    for (int i = 0; i < this->realRow; i++)
    {
        delete[] this->elements[i];
        elements[i] = nullptr;
    }
    delete this->elements;
    elements = nullptr;
    this->row = row;
    this->col = col;
    this->realRow = row;
    this->realCol = col;
    this->elements = new double* [row];
    for (int i = 0; i < row; i++)
    {
        this->elements[i] = new double[col];
    }
    return *this;
}

inline Matrix& Matrix::DeleteRow(int row)
{
    if (row < 0 || row > this->row)
    {
        throw "索引超出界限";
    }
    double* temp = this->elements[row - 1];
    for (int i = row - 1; i < this->row - 1; i++)
    {
        this->elements[i] = this->elements[i + 1];
    }
    this->elements[this->row - 1] = temp;
    delete[] elements[this->row - 1];
    elements[this->row - 1] = nullptr;
    temp = nullptr;
    this->row--;
    this->realRow--;
    return *this;
}

inline Matrix& Matrix::DeleteCol(int col)
{
    if (col < 0 || col > this->col)
    {
        throw "索引超出界限";
    }
    for (int i = 0; i < this->row; i++)
    {
        for (int j = col - 1; j < this->col - 1; j++)
        {
            this->elements[i][j] = this->elements[i][j + 1];
        }
    }
    this->col--;
    return *this;
}

inline Matrix& Matrix::DeleteCol(int startCol, int length)
{
    if (startCol < 0 || startCol > this->col)
    {
        throw "索引超出界限";
    }
    for (int i = 0; i < this->row; i++)
    {
        for (int j = startCol - 1; j < this->col - length; j++)
        {
            this->elements[i][j] = this->elements[i][j + length];
        }
    }
    this->col -= length;
    return *this;
}