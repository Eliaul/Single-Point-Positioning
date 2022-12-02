#pragma once

#pragma warning(disable:26495)

#include "../TimeSystem/GPSTime.h"
#include "../CoordinateSystem/XYZCoordinate.hpp"

class BaseEphemeris
{
public:
	GPSTime toe_gpstime;
	GPSTime toc_gpstime;
	unsigned long PRN;
	unsigned long health;
	double toe_sec;
	/// <summary>
	/// 半长轴(米)
	/// </summary>
	double A;
	/// <summary>
	/// 平均角速度的改正值
	/// </summary>
	double Delta_N;
	/// <summary>
	/// 平近点角
	/// </summary>
	double M_0;
	/// <summary>
	/// 偏心率
	/// </summary>
	double ecc;
	/// <summary>
	/// 近地点角距
	/// </summary>
	double omega;
	/// <summary>
	/// 升交距角的余弦调和改正项的振幅(rad)
	/// </summary>
	double cuc;
	/// <summary>
	/// 升交距角的正弦调和改正项的振幅(rad)
	/// </summary>
	double cus;
	/// <summary>
	/// 卫星至地心距离的余弦调和改正项的振幅(m)
	/// </summary>
	double crc;
	/// <summary>
	/// 卫星至地心距离的正弦调和改正项的振幅(m)
	/// </summary>
	double crs;
	/// <summary>
	/// 轨道倾角的余弦调和改正项的振幅(rad)
	/// </summary>
	double cic;
	/// <summary>
	/// 轨道倾角的正弦调和改正项的振幅(rad)
	/// </summary>
	double cis;
	/// <summary>
	/// toe时刻的轨道倾角(rad)
	/// </summary>
	double I_0;
	/// <summary>
	/// 轨道倾角的变化率(rad/s)
	/// </summary>
	double I_dot;
	/// <summary>
	/// 升交点赤经
	/// </summary>
	double omega_0;
	/// <summary>
	/// 升交点赤经的变化率
	/// </summary>
	double omega_dot;
	/// <summary>
	/// 卫星钟差(s)
	/// </summary>
	double a_f0;
	/// <summary>
	/// 卫星钟漂移(s/s)
	/// </summary>
	double a_f1;
	/// <summary>
	/// 卫星钟漂移速度(s/s/s)
	/// </summary>
	double a_f2;

	virtual ~BaseEphemeris() {};
	[[nodiscard]] virtual XYZCoordinate CalSatePos() const;
	[[nodiscard]] virtual double CalClockError() const;
	[[nodiscard]] virtual double CalClockVelocity() const;
	[[nodiscard]] virtual Vec3d CalSateVelocity() const;
	/// <summary>
	/// 初始化计算
	/// </summary>
	/// <param name="time_tr">信号发射时刻</param>
	virtual void Initialize(const GPSTime& time_tr) const;
	virtual const double GM() const noexcept = 0; //常数GM
	virtual const double OMAGE_E() const noexcept = 0; //地球自转角速度
	virtual const double F() const noexcept = 0;
	/// <summary>
	/// 星历有效期
	/// </summary>
	/// <returns>纳秒</returns>
	virtual const nanoseconds TimeAvailableLim() const noexcept = 0;

protected:
	mutable double t_toc;
	mutable double t_k;
	mutable double E_k;
	mutable double Phi_k;
	mutable double v_k;
	mutable double Omega_k;
};
