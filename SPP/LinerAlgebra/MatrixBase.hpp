#pragma once
#include "MatrixVecBase.hpp"
#include <random>
#include <tuple>

template<class Derived>
class MatrixBase: public MatrixVecBase<Derived>
{
protected:
	MatrixBase() :MatrixVecBase<Derived>() {}
	MatrixBase(int row, int col) :MatrixVecBase<Derived>(row, col) {}
	MatrixBase(MatrixBase&& m) noexcept :MatrixVecBase<Derived>(std::move(m)) {}
	MatrixBase(const MatrixBase& m) : MatrixVecBase<Derived>(m) {}

public:
	static Derived Zeros(int row, int col);
	static Derived Identity(int rank);
	static Derived Random(int row, int col);
    static Derived Diagonal(std::initializer_list<double> nums);
    template<typename T> requires myReference<Derived, T>
	static Derived SwapRow(T&& derived, int row1, int row2);
    template<typename T> requires myReference<Derived, T>
    static Derived SwapCol(T&& derived, int col1, int col2);
    template<typename T> requires myReference<Derived, T>
	static Derived RowMultiplyNum(T&& derived, int row, double num);
    template<typename T> requires myReference<Derived, T>
	static Derived Row1MultiplyNumAddRow2(T&& derived, int row1, int row2, double num);
	static Derived Transpose(const Derived& derived);
    template<typename T> requires myReference<Derived, T>
	static Derived Inverse(T&& derived);
    template<typename T> requires myReference<Derived, T>
    static double Det(T&& derived);
    template<typename T> requires myReference<Derived, T>
    static int Rank(T&& derived);
    static double Trace(const Derived& derived);
	
	using MatrixVecBase<Derived>::operator*;
	Derived operator*(const Derived& derived) const;
	using MatrixVecBase<Derived>::operator/;
    template<typename T> requires myReference<Derived, T>
	Derived operator/(T&& derived) const;
	double& operator()(int rowIndex, int colIndex);
	const double& operator()(int rowIndex, int colIndex) const;

    int Row() const noexcept { return this->row; }
    int Col() const noexcept { return this->col; }
	std::tuple<int, int> Size() const noexcept { return std::make_tuple(this->row, this->col); }
	Derived& ResetValue(double value);
	Derived& SwapRow(int row1, int row2);
	Derived& SwapCol(int col1, int col2);
	Derived& RowMultiplyNum(int row, double num);
	Derived& Row1MultiplyNumAddRow2(int row1, int row2, double num);
	Derived Transpose() const;
	Derived Inverse() const&;
	Derived Inverse() &&;
    double Det() const&;
	double Det() &&;
    int Rank() const&;
	int Rank() &&;
    double Trace() const;
};

template<class Derived>
inline double MatrixBase<Derived>::Trace(const Derived& derived)
{
    if (derived.row != derived.col)
    {
        throw "该矩阵不是方阵";
    }
    double res = 0;
    for (int i = 0; i < derived.row; i++)
    {
        res += derived.elements[i][i];
    }
    return res;
}

template<class Derived>
inline Derived MatrixBase<Derived>::operator*(const Derived& derived) const
{
	if (this->col != derived.row)
	{
		throw "两个矩阵无法相乘";
	}
	Derived res = Zeros(this->row, derived.col);
	for (int i = 0; i < res.row; i++)
	{
		for (int k = 0; k < this->col; k++)
		{
			for (int j = 0; j < res.col; j++)
			{
				res.elements[i][j] += this->elements[i][k] * derived.elements[k][j];
			}
		}
	}
	return res;
}

template<class Derived> template<typename T> requires myReference<Derived, T>
inline Derived MatrixBase<Derived>::operator/(T&& derived) const
{
	return this->GetDerived() * Derived::Inverse(std::forward<T>(derived));
}

template<class Derived>
inline double& MatrixBase<Derived>::operator()(int rowIndex, int colIndex)
{
	assert(rowIndex > 0);
	assert(rowIndex < this->row + 1);
	assert(colIndex > 0);
	assert(colIndex < this->col + 1);
	return *(this->elements[rowIndex - 1] + colIndex - 1);
}

template<class Derived>
inline const double& MatrixBase<Derived>::operator()(int rowIndex, int colIndex) const
{
	assert(rowIndex > 0);
	assert(rowIndex < this->row + 1);
	assert(colIndex > 0);
	assert(colIndex < this->col + 1);
	return this->elements[rowIndex - 1][colIndex - 1];
}

template<class Derived>
inline Derived& MatrixBase<Derived>::ResetValue(double value)
{
	for (int i = 0; i < this->row; i++)
	{
		for (int j = 0; j < this->col; j++)
		{
			this->elements[i][j] = value;
		}
	}
	return this->GetDerived();
}

template<class Derived>
inline Derived& MatrixBase<Derived>::SwapRow(int row1, int row2)
{
    if (row1 == row2)
    {
        return this->GetDerived();
    }
    double* temp = this->elements[row1 - 1];
    this->elements[row1 - 1] = this->elements[row2 - 1];
    this->elements[row2 - 1] = temp;
	return this->GetDerived();
}

template<class Derived>
inline Derived& MatrixBase<Derived>::SwapCol(int col1, int col2)
{
	if (col1 == col2)
	{
		return this->GetDerived();
	}
	for (int i = 0; i < this->row; i++)
	{
		double temp = this->elements[i][col1 - 1];
		this->elements[i][col1 - 1] = this->elements[i][col2 - 1];
		this->elements[i][col2 - 1] = temp;
	}
	return this->GetDerived();
}

template<class Derived>
inline Derived& MatrixBase<Derived>::RowMultiplyNum(int row, double num)
{
	for (int i = 0; i < this->col; i++)
	{
		this->elements[row - 1][i] *= num;
	}
	return this->GetDerived();
}

template<class Derived>
inline Derived& MatrixBase<Derived>::Row1MultiplyNumAddRow2(int row1, int row2, double num)
{
	for (int i = 0; i < this->col; i++)
	{
		this->elements[row2 - 1][i] += this->elements[row1 - 1][i] * num;
	}
	return this->GetDerived();
}

template<class Derived>
inline Derived MatrixBase<Derived>::Transpose() const
{
	return Derived::Transpose(this->GetDerived());
}

template<class Derived>
inline Derived MatrixBase<Derived>::Inverse() const&
{
	return Derived::Inverse(this->GetDerived());
}

template<class Derived>
inline Derived MatrixBase<Derived>::Inverse() &&
{
	return Derived::Inverse(std::move(this->GetDerived()));
}

template<class Derived>
inline double MatrixBase<Derived>::Det() const&
{
    return Derived::Det(this->GetDerived());
}

template<class Derived>
inline double MatrixBase<Derived>::Det() &&
{
	return Derived::Det(std::move(this->GetDerived()));
}

template<class Derived>
inline int MatrixBase<Derived>::Rank() const&
{
    return Derived::Rank(this->GetDerived());
}

template<class Derived>
inline int MatrixBase<Derived>::Rank() &&
{
	return Derived::Rank(std::move(this->GetDerived()));
}

template<class Derived>
inline double MatrixBase<Derived>::Trace() const
{
    return Derived::Trace(this->GetDerived());
}

template<class Derived>
Derived MatrixBase<Derived>::Zeros(int row, int col)
{
	Derived res{};
	res.row = row;
	res.col = col;
	res.realRow = row;
	res.realCol = col;
	if (res.elements == nullptr)
	{
		res.elements = new double* [row];
		for (int i = 0; i < row; i++)
		{
			res.elements[i] = new double[col];
		}
	}
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			res.elements[i][j] = 0;
		}
	}
	return res;
}

template<class Derived>
Derived MatrixBase<Derived>::Identity(int rank)
{
	Derived res{};
	res.row = rank;
	res.col = rank;
	res.realRow = rank;
	res.realCol = rank;
	if (res.elements == nullptr)
	{
		res.elements = new double* [rank];
		for (int i = 0; i < rank; i++)
		{
			res.elements[i] = new double[rank];
		}
	}
	for (int i = 0; i < rank; i++)
	{
		for (int j = 0; j < rank; j++)
		{
			res.elements[i][j] = (i != j) ? 0 : 1;
		}
	}
	return res;
}

template<class Derived>
Derived MatrixBase<Derived>::Random(int row, int col)
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0.0, 1.0);
	Derived res{};
	res.row = row;
	res.col = col;
	res.realRow = row;
	res.realCol = col;
	if (res.elements == nullptr)
	{
		res.elements = new double* [row];
		for (int i = 0; i < row; i++)
		{
			res.elements[i] = new double[col];
		}
	}
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			res.elements[i][j] = dist(mt);
		}
	}
	return res;
}

template<class Derived>
inline Derived MatrixBase<Derived>::Diagonal(std::initializer_list<double> nums)
{
    Derived res(static_cast<int>(nums.size()), static_cast<int>(nums.size()));
    for (int i = 0; i < res.row; i++)
    {
        for (int j = 0; j < res.col; j++)
        {
            res.elements[i][j] = (i != j) ? 0 : *(nums.begin() + i);
        }
    }
    return res;
}

template<class Derived> template<typename T> requires myReference<Derived, T>
inline Derived MatrixBase<Derived>::SwapRow(T&& derived, int row1, int row2)
{
	Derived res = std::forward<T>(derived);
    if (row1 == row2)
    {
        return res;
    }
    double* temp = res.elements[row1 - 1];
    res.elements[row1 - 1] = res.elements[row2 - 1];
    res.elements[row2 - 1] = temp;
	return res;
}

template<class Derived> template<typename T> requires myReference<Derived, T>
inline Derived MatrixBase<Derived>::SwapCol(T&& derived, int col1, int col2)
{
    Derived res = std::forward<T>(derived);
    if (col1 == col2)
    {
        return res;
    }
    for (int i = 0; i < res.row; i++)
    {
        double temp = res.elements[i][col1 - 1];
        res.elements[i][col1 - 1] = res.elements[i][col2 - 1];
        res.elements[i][col2 - 1] = temp;
    }
    return res;
}

template<class Derived> template<typename T> requires myReference<Derived, T>
inline Derived MatrixBase<Derived>::RowMultiplyNum(T&& derived, int row, double num)
{
	Derived res = std::forward<T>(derived);
	for (int i = 0; i < res.col; i++)
	{
		res.elements[row - 1][i] *= num;
	}
	return res;
}

template<class Derived> template<typename T> requires myReference<Derived, T>
inline Derived MatrixBase<Derived>::Row1MultiplyNumAddRow2(T&& derived, int row1, int row2, double num)
{
	Derived res = std::forward(derived);
	for (int i = 0; i < res.col; i++)
	{
		res.elements[row2 - 1][i] += derived.elements[row1 - 1][i] * num;
	}
	return res;
}

template<class Derived>
inline Derived MatrixBase<Derived>::Transpose(const Derived& derived)
{
	Derived res{};
	res.row = derived.col;
	res.col = derived.row;
	res.realRow = derived.col;
	res.realCol = derived.row;
	res.elements = new double* [res.row];
	for (int i = 0; i < res.row; i++)
	{
		res.elements[i] = new double[res.col];
		for (int j = 0; j < res.col; j++)
		{
			res.elements[i][j] = derived.elements[j][i];
		}
	}
	return res;
}

template<class Derived> template<typename T> requires myReference<Derived, T>
inline Derived MatrixBase<Derived>::Inverse(T&& matrix)
{
	if (matrix.row != matrix.col)
	{
		throw "该矩阵不是方阵";
	}

	Derived unitMatrix = Identity(matrix.row);
	Derived derived = std::forward<T>(matrix);

	for (int col = 0; col < derived.col; col++)
	{
		for (int row = col; row < derived.row; row++)
		{
			if (std::abs(derived.elements[row][col]) > 10 * DBL_EPSILON) //不等于0
			{
				derived.SwapRow(row + 1, col + 1);	//将第col+1列不为0的元素移到第col+1行第col+1列
				unitMatrix.SwapRow(row + 1, col + 1);	//单位阵做同样的操作
				break;
			}
			if (row == derived.row - 1)
			{
				throw "该矩阵不可逆";
			}
		}

		unitMatrix.RowMultiplyNum(col + 1, 1.0 / derived.elements[col][col]);
		derived.RowMultiplyNum(col + 1, 1.0 / derived.elements[col][col]);

		for (int i = 0; i < col; i++)
		{
			unitMatrix.Row1MultiplyNumAddRow2(col + 1, i + 1, -derived.elements[i][col]);
			derived.Row1MultiplyNumAddRow2(col + 1, i + 1, -derived.elements[i][col]);
		}

		for (int i = col + 1; i < derived.row; i++)
		{
			unitMatrix.Row1MultiplyNumAddRow2(col + 1, i + 1, -derived.elements[i][col]);
			derived.Row1MultiplyNumAddRow2(col + 1, i + 1, -derived.elements[i][col]);
		}
	}

	return unitMatrix;
}

template<class Derived> template<typename T> requires myReference<Derived, T>
inline double MatrixBase<Derived>::Det(T&& matrix)
{
    if (matrix.row != matrix.col)
    {
        throw "该矩阵不是方阵";
    }

    Derived derived = std::forward<T>(matrix);
    int exchangeCount = 0;

    for (int col = 0; col < derived.col; col++)
    {
        if (std::abs(derived.elements[col][col]) < 10 * DBL_EPSILON)
        {
            for (int row = col + 1; row < derived.row; row++)
            {
                if (std::abs(derived.elements[row][col]) > 10 * DBL_EPSILON) //不等于0
                {
                    derived.SwapRow(row + 1, col + 1);	//将第col+1列不为0的元素移到第col+1行第col+1列
                    exchangeCount++;
                    break;
                }
                if (row == derived.row - 1)
                {
                    return 0;
                }
            }
        }

        for (int i = col + 1; i < derived.row; i++)
        {
            derived.Row1MultiplyNumAddRow2(col + 1, i + 1, -derived.elements[i][col] / derived.elements[col][col]);
        }
    }

    double det = 1;
    for (int i = 0; i < derived.row; i++)
    {
        det *= derived.elements[i][i];
    }
    det = (exchangeCount % 2 == 0) ? det : -det;
    return det;
}

template<class Derived> template<typename T> requires myReference<Derived, T>
inline int MatrixBase<Derived>::Rank(T&& matrix)
{
    Derived derived = std::forward<T>(matrix);
    int rank = 0;

    for (int col = 0; col < derived.col; col++)
    {
        for (int row = col; row < derived.row; row++)
        {
            if (std::abs(derived.elements[row][col]) > 10 * DBL_EPSILON) //不等于0
            {
                derived.SwapRow(row + 1, col + 1);	//将第col+1列不为0的元素移到第col+1行第col+1列
                rank++;
                break;
            }
        }

        for (int i = col + 1; i < derived.row; i++)
        {
            if (std::abs(derived.elements[col][col]) < 10 * DBL_EPSILON)
            {
                continue;
            }
            derived.Row1MultiplyNumAddRow2(col + 1, i + 1, -derived.elements[i][col] / derived.elements[col][col]);
        }
    }

    return rank;
}
