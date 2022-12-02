#include "BaseEphemeris.h"
#include "../LinerAlgebra/Matrix3d.hpp"

using namespace std::chrono;

XYZCoordinate BaseEphemeris::CalSatePos() const
{
	v_k = atan2(sqrt(1 - ecc * ecc) * sin(E_k) / (1 - ecc * cos(E_k)), (cos(E_k) - ecc) / (1 - ecc * cos(E_k)));
	Phi_k = v_k + omega;
	double sin_2Phi_k = sin(2 * Phi_k);
	double cos_2Phi_k = cos(2 * Phi_k);
	double delta_u_k = cus * sin_2Phi_k + cuc * cos_2Phi_k;
	double delta_r_k = crs * sin_2Phi_k + crc * cos_2Phi_k;
	double delta_i_k = cis * sin_2Phi_k + cic * cos_2Phi_k;
	double u_k = Phi_k + delta_u_k;
	double r_k = A * (1 - ecc * cos(E_k)) + delta_r_k;
	double i_k = I_0 + delta_i_k + I_dot * t_k;
	double sin_i_k = sin(i_k);
	double cos_i_k = cos(i_k);
	double x_k1 = r_k * cos(u_k);
	double y_k1 = r_k * sin(u_k);
	Omega_k = omega_0 + (omega_dot - this->OMAGE_E()) * t_k - this->OMAGE_E() * toe_sec;
	double sin_Omega_k = sin(Omega_k);
	double cos_Omega_k = cos(Omega_k);
	double x_k = x_k1 * cos_Omega_k - y_k1 * cos_i_k * sin_Omega_k;
	double y_k = x_k1 * sin_Omega_k + y_k1 * cos_i_k * cos_Omega_k;
	double z_k = y_k1 * sin_i_k;
	return XYZCoordinate(x_k, y_k, z_k);
}

double BaseEphemeris::CalClockError() const
{
	return a_f0 + a_f1 * t_toc + a_f2 * t_toc * t_toc + F() * ecc * sqrt(A) * sin(E_k);
}

double BaseEphemeris::CalClockVelocity() const
{
	double E_k_dot = (sqrt(this->GM() / pow(A, 3)) + Delta_N) / (1 - ecc * cos(E_k));
	double delta_tr1 = F() * ecc * sqrt(A) * cos(E_k) * E_k_dot;
	return a_f1 + 2 * a_f2 * t_toc + delta_tr1;
}

Vec3d BaseEphemeris::CalSateVelocity() const
{
	double E_k_dot = (sqrt(this->GM() / pow(A, 3)) + Delta_N) / (1 - ecc * cos(E_k));
	double cos_2Phi_k = cos(2 * Phi_k);
	double sin_2Phi_k = sin(2 * Phi_k);
	double delta_u_k = cus * sin_2Phi_k + cuc * cos_2Phi_k;
	double delta_i_k = cis * sin_2Phi_k + cic * cos_2Phi_k;
	double delta_r_k = crs * sin_2Phi_k + crc * cos_2Phi_k;
	double i_k = I_0 + delta_i_k + I_dot * t_k;
	double u_k = Phi_k + delta_u_k;
	double cos_Omega_k = cos(Omega_k);
	double sin_Omega_k = sin(Omega_k);
	double sin_i_k = sin(i_k);
	double cos_i_k = cos(i_k);
	double r_k = A * (1 - ecc * cos(E_k)) + delta_r_k;
	double Phi_k_dot = sqrt((1 + ecc) / (1 - ecc)) * pow(cos(v_k / 2) / cos(E_k / 2), 2) * E_k_dot;
	double u_k_dot = 2 * (cus * cos_2Phi_k - cuc * sin_2Phi_k) * Phi_k_dot + Phi_k_dot;
	double r_k_dot = A * ecc * sin(E_k) * E_k_dot + 2 * (crs * cos_2Phi_k - crc * sin_2Phi_k) * Phi_k_dot;
	double I_k_dot = I_dot + 2 * (cis * cos_2Phi_k - cic * sin_2Phi_k) * Phi_k_dot;
	double Omega_k_dot = omega_dot - OMAGE_E();
	double x_k1 = r_k * cos(u_k);
	double y_k1 = r_k * sin(u_k);
	Matrix R_dot
	{
		{cos_Omega_k, -sin_Omega_k * cos_i_k, -x_k1 * sin_Omega_k - y_k1 * cos_Omega_k * cos_i_k, y_k1 * sin_Omega_k * sin_i_k},
		{sin_Omega_k, cos_Omega_k * cos_i_k, x_k1 * cos_Omega_k - y_k1 * sin_Omega_k * cos_i_k, -y_k1 * cos_Omega_k * sin_i_k},
		{0, sin_i_k, 0, y_k1 * cos_i_k}
	};
	Vec v
	{
		r_k_dot * cos(u_k) - r_k * u_k_dot * sin(u_k),
		r_k_dot * sin(u_k) + r_k * u_k_dot * cos(u_k),
		Omega_k_dot,
		I_k_dot
	};
	return (Vec3d)(R_dot * v);
}

void BaseEphemeris::Initialize(const GPSTime& time_tr) const
{
	t_toc = duration<double>(time_tr - toc_gpstime).count();
	t_k = duration<double>(time_tr - toe_gpstime).count();
	double n = sqrt(this->GM() / pow(A, 3)) + Delta_N;
	double M_k = M_0 + n * t_k;
	auto keplerFunc = [=](const double& x) { return (x - ecc * sin(x) - M_k) / (1 - ecc * cos(x)); };
	E_k = NewtonIteration(0, 1e-15, 15, keplerFunc);
	v_k = atan2(sqrt(1 - ecc * ecc) * sin(E_k) / (1 - ecc * cos(E_k)), (cos(E_k) - ecc) / (1 - ecc * cos(E_k)));
	Phi_k = v_k + omega;
	Omega_k = omega_0 + (omega_dot - this->OMAGE_E()) * t_k - this->OMAGE_E() * toe_sec;
}
