#pragma once

#include <cmath>

/// <summary>
/// 必须重载小括号运算符,如Lambda表达式
/// </summary>
template<typename T>
concept Functor = requires (T f, double x) { f(x); };

template<Functor _Functor, Functor _DFunctor>
inline double NewtonIteration(double x0, double tolerance, int maxItNum, _Functor func, _DFunctor dfunc)
{
	double x_back = x0;
	double x = x0 - func(x0) / dfunc(x0);
	int count = 0;
	while (abs(x - x_back) > tolerance)
	{
		x_back = x;
		count++;
		x -= func(x) / dfunc(x);
		if (count > maxItNum)
			break;
	}
	return x;
}

template<Functor _Functor>
inline double NewtonIteration(double x0, double tolerance, int maxItNum, const _Functor& func)
{
	double x_back = x0;
	double x = x0 - func(x0);
	int count = 0;
	while (abs(x - x_back) > tolerance)
	{
		x_back = x;
		count++;
		x -= func(x);
		if (count > maxItNum)
			break;
	}
	return x;
}