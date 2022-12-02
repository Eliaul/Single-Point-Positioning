#pragma once

#include <assert.h>
#include <format>

template<class Derived, typename T>
concept myReference = std::same_as<T, Derived> || std::same_as<T, Derived&> || std::same_as<T, Derived&&> || std::same_as<T, const Derived&> || std::same_as<T, const Derived&&>;

template<class Derived>
class MatrixVecBase
{
    template<typename Derived> requires std::is_base_of_v<MatrixVecBase<Derived>, Derived>
    friend std::ostream& operator<<(std::ostream& cout, const Derived& derived);
	friend struct std::formatter<MatrixVecBase<Derived>>;

protected:
	int row;
	int col;
	int realRow;
	int realCol;
	double** elements;
	MatrixVecBase() : row(0), col(0), realRow(0), realCol(0), elements(nullptr) {}
	MatrixVecBase(int row, int col);
	MatrixVecBase(MatrixVecBase&& m) noexcept;
	MatrixVecBase(const MatrixVecBase& m);
	Derived& GetDerived() { return static_cast<Derived&>(*this); }
	Derived const& GetDerived() const { return static_cast<Derived const&>(*this); }

public:
	~MatrixVecBase();
	Derived operator+(const Derived& derived) const&;
	Derived operator+(const Derived& derived) &&;
	Derived& operator+=(const Derived& derived);
	Derived operator-() const&;
	Derived operator-() &&;
	Derived operator-(const Derived& derived) const&;
	Derived operator-(const Derived& derived) &&;
	Derived& operator-=(const Derived& derived);
	Derived operator*(double num) const&;
	Derived operator*(double num) &&;
	Derived& operator*=(double num);
	Derived operator /(double num) const&;
	Derived operator /(double num) &&;
	Derived& operator /=(double num);
	Derived& operator=(const Derived& derived);
    Derived& operator=(Derived&& derived);
};

template<typename Derived> requires std::is_base_of_v<MatrixVecBase<Derived>, Derived>
inline std::ostream& operator<<(std::ostream& out, const Derived& derived)
{
    for (int i = 0; i < derived.row; i++)
    {
        for (int j = 0; j < derived.col; j++)
        {
            out << std::format("{:12.4f}\t", derived.elements[i][j]);
        }
		out << std::format("{}", '\n');
    }
    return out;
}

template<class Derived>
inline Derived MatrixVecBase<Derived>::operator-() const&
{
	Derived res = this->GetDerived().CloneSize();
	for (int i = 0; i < res.row; i++)
	{
		for (int j = 0; j < res.col; j++)
		{
			res.elements[i][j] = -this->elements[i][j];
		}
	}
	return res;
}

template<class Derived>
inline Derived MatrixVecBase<Derived>::operator-() &&
{
	Derived res = std::move(this->GetDerived());
	for (int i = 0; i < res.row; i++)
	{
		for (int j = 0; j < res.col; j++)
		{
			res.elements[i][j] = -res.elements[i][j];
		}
	}
	return res;
}

template<class Derived>
inline Derived operator*(double num, const Derived& derived)
{
	return derived * num;
}

template<class Derived>
inline MatrixVecBase<Derived>::~MatrixVecBase()
{
	if (this->elements != nullptr)
	{
		for (int i = 0; i < this->realRow; i++)
		{
			delete[] elements[i];
			elements[i] = nullptr;
		}
		delete elements;
		elements = nullptr;
	}
}

template<class Derived>
inline MatrixVecBase<Derived>::MatrixVecBase(int row, int col)
	:row(row), col(col),  realRow(row), realCol(col), elements(new double*[row])
{
	for (int i = 0; i < row; i++)
	{
		elements[i] = new double[col];
	}
}

template<class Derived>
inline Derived MatrixVecBase<Derived>::operator+(const Derived& derived) const&
{
	if (this->row != derived.row || this->col != derived.col)
	{
		throw "两个矩阵大小不同";
	}
	Derived res = this->GetDerived().CloneSize();
	for (int i = 0; i < res.row; i++)
	{
		for (int j = 0; j < res.col; j++)
		{
			res.elements[i][j] = this->elements[i][j] + derived.elements[i][j];
		}
	}
	return res;
}

template<class Derived>
inline Derived MatrixVecBase<Derived>::operator+(const Derived& derived) &&
{
	if (this->row != derived.row || this->col != derived.col)
	{
		throw "两个矩阵大小不同";
	}
	Derived res = std::move(this->GetDerived());
	for (int i = 0; i < res.row; i++)
	{
		for (int j = 0; j < res.col; j++)
		{
			res.elements[i][j] += derived.elements[i][j];
		}
	}
	return res;
}

template<class Derived>
inline Derived& MatrixVecBase<Derived>::operator+=(const Derived& derived)
{
	if (this->row != derived.row || this->col != derived.col)
	{
		throw "两个矩阵大小不同";
	}
	for (int i = 0; i < this->row; i++)
	{
		for (int j = 0; j < this->col; j++)
		{
			this->elements[i][j] += derived.elements[i][j];
		}
	}
	return this->GetDerived();
}

template<class Derived>
inline Derived MatrixVecBase<Derived>::operator-(const Derived& derived) const&
{
	if (this->row != derived.row || this->col != derived.col)
	{
		throw "两个矩阵大小不同";
	}
	Derived res = this->GetDerived().CloneSize();
	for (int i = 0; i < res.row; i++)
	{
		for (int j = 0; j < res.col; j++)
		{
			res.elements[i][j] = this->elements[i][j] - derived.elements[i][j];
		}
	}
	return res;
}

template<class Derived>
inline Derived MatrixVecBase<Derived>::operator-(const Derived& derived) &&
{
	if (this->row != derived.row || this->col != derived.col)
	{
		throw "两个矩阵大小不同";
	}
	Derived res = std::move(this->GetDerived());
	for (int i = 0; i < res.row; i++)
	{
		for (int j = 0; j < res.col; j++)
		{
			res.elements[i][j] -=  derived.elements[i][j];
		}
	}
	return res;
}

template<class Derived>
inline MatrixVecBase<Derived>::MatrixVecBase(MatrixVecBase<Derived>&& m) noexcept
	:row(m.row), col(m.col), realRow(m.realRow), realCol(m.realCol), elements(m.elements)
{
	m.elements = nullptr;
}

template<class Derived>
inline MatrixVecBase<Derived>::MatrixVecBase(const MatrixVecBase& m)
	:row(m.row), col(m.col), realRow(m.realRow), realCol(m.realCol), elements(new double*[m.row])
{
	for (int i = 0; i < row; i++)
	{
		elements[i] = new double[col];
		for (int j = 0; j < col; j++)
		{
			elements[i][j] = m.elements[i][j];
		}
	}
}

template<class Derived>
inline Derived& MatrixVecBase<Derived>::operator-=(const Derived& derived)
{
	if (this->row != derived.row || this->col != derived.col)
	{
		throw "两个矩阵大小不同";
	}
	for (int i = 0; i < this->row; i++)
	{
		for (int j = 0; j < this->col; j++)
		{
			this->elements[i][j] -= derived.elements[i][j];
		}
	}
	return this->GetDerived();
}

template<class Derived>
inline Derived MatrixVecBase<Derived>::operator*(double num) const&
{
	Derived res = this->GetDerived().CloneSize();
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			res.elements[i][j] = this->elements[i][j] * num;
		}
	}
	return res;
}

template<class Derived>
inline Derived MatrixVecBase<Derived>::operator*(double num) &&
{
	Derived res = std::move(this->GetDerived());
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			res.elements[i][j] *= num;
		}
	}
	return res;
}

template<class Derived>
inline Derived& MatrixVecBase<Derived>::operator*=(double num)
{
	for (int i = 0; i < this->row; i++)
	{
		for (int j = 0; j < this->col; j++)
		{
			this->elements[i][j] *= num;
		}
	}
	return this->GetDerived();
}

template<class Derived>
inline Derived MatrixVecBase<Derived>::operator/(double num) const&
{
	return (*this) * (1.0 / num);
}

template<class Derived>
inline Derived MatrixVecBase<Derived>::operator/(double num) &&
{
	return std::move((*this)) * (1.0 / num);
}

template<class Derived>
inline Derived& MatrixVecBase<Derived>::operator/=(double num)
{
	for (int i = 0; i < this->row; i++)
	{
		for (int j = 0; j < this->col; j++)
		{
			this->elements[i][j] /= num;
		}
	}
	return this->GetDerived();
}

template<class Derived>
inline Derived& MatrixVecBase<Derived>::operator=(const Derived& derived)
{
	if (this->col == derived.col && this->row == derived.row && this->elements != nullptr)
	{
		for (int i = 0; i < this->row; i++)
		{
			for (int j = 0; j < this->col; j++)
			{
				this->elements[i][j] = derived.elements[i][j];
			}
		}
		return this->GetDerived();
	}
	if (this->elements != nullptr)
	{
		for (int i = 0; i < this->realRow; i++)
		{
			delete[] elements[i];
			elements[i] = nullptr;
		}
		delete elements;
		elements = nullptr;
	}
	this->row = derived.row;
	this->col = derived.col;
	this->realRow = row;
	this->realCol = col;
	this->elements = new double* [row];
	for (int i = 0; i < this->row; i++)
	{
		this->elements[i] = new double[col];
		for (int j = 0; j < this->col; j++)
		{
			this->elements[i][j] = derived.elements[i][j];
		}
	}
	return this->GetDerived();
}

template<class Derived>
inline Derived& MatrixVecBase<Derived>::operator=(Derived&& derived)
{
    if (this->elements != nullptr)
    {
        for (int i = 0; i < this->realRow; i++)
        {
            delete[] elements[i];
            elements[i] = nullptr;
        }
        delete elements;
        elements = nullptr;
    }
    this->row = derived.row;
    this->col = derived.col;
    this->realRow = derived.realRow;
    this->realCol = derived.realCol;
    this->elements = derived.elements;
    derived.elements = nullptr;
    return this->GetDerived();
}
