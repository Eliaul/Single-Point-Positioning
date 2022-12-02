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
	/// �볤��(��)
	/// </summary>
	double A;
	/// <summary>
	/// ƽ�����ٶȵĸ���ֵ
	/// </summary>
	double Delta_N;
	/// <summary>
	/// ƽ�����
	/// </summary>
	double M_0;
	/// <summary>
	/// ƫ����
	/// </summary>
	double ecc;
	/// <summary>
	/// ���ص�Ǿ�
	/// </summary>
	double omega;
	/// <summary>
	/// ������ǵ����ҵ��͸���������(rad)
	/// </summary>
	double cuc;
	/// <summary>
	/// ������ǵ����ҵ��͸���������(rad)
	/// </summary>
	double cus;
	/// <summary>
	/// ���������ľ�������ҵ��͸���������(m)
	/// </summary>
	double crc;
	/// <summary>
	/// ���������ľ�������ҵ��͸���������(m)
	/// </summary>
	double crs;
	/// <summary>
	/// �����ǵ����ҵ��͸���������(rad)
	/// </summary>
	double cic;
	/// <summary>
	/// �����ǵ����ҵ��͸���������(rad)
	/// </summary>
	double cis;
	/// <summary>
	/// toeʱ�̵Ĺ�����(rad)
	/// </summary>
	double I_0;
	/// <summary>
	/// �����ǵı仯��(rad/s)
	/// </summary>
	double I_dot;
	/// <summary>
	/// ������ྭ
	/// </summary>
	double omega_0;
	/// <summary>
	/// ������ྭ�ı仯��
	/// </summary>
	double omega_dot;
	/// <summary>
	/// �����Ӳ�(s)
	/// </summary>
	double a_f0;
	/// <summary>
	/// ������Ư��(s/s)
	/// </summary>
	double a_f1;
	/// <summary>
	/// ������Ư���ٶ�(s/s/s)
	/// </summary>
	double a_f2;

	virtual ~BaseEphemeris() {};
	[[nodiscard]] virtual XYZCoordinate CalSatePos() const;
	[[nodiscard]] virtual double CalClockError() const;
	[[nodiscard]] virtual double CalClockVelocity() const;
	[[nodiscard]] virtual Vec3d CalSateVelocity() const;
	/// <summary>
	/// ��ʼ������
	/// </summary>
	/// <param name="time_tr">�źŷ���ʱ��</param>
	virtual void Initialize(const GPSTime& time_tr) const;
	virtual const double GM() const noexcept = 0; //����GM
	virtual const double OMAGE_E() const noexcept = 0; //������ת���ٶ�
	virtual const double F() const noexcept = 0;
	/// <summary>
	/// ������Ч��
	/// </summary>
	/// <returns>����</returns>
	virtual const nanoseconds TimeAvailableLim() const noexcept = 0;

protected:
	mutable double t_toc;
	mutable double t_k;
	mutable double E_k;
	mutable double Phi_k;
	mutable double v_k;
	mutable double Omega_k;
};
