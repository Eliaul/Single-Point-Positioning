#include "BDSEphemeris.h"

#include "../Calculate/Constant.h"
#include "../LinerAlgebra/Matrix3d.hpp"

XYZCoordinate BDSEphemeris::CalSatePos() const
{
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
	double sin_Omega_k = sin(Omega_k);
	double cos_Omega_k = cos(Omega_k);
	double x_k = x_k1 * cos_Omega_k - y_k1 * cos_i_k * sin_Omega_k;
	double y_k = x_k1 * sin_Omega_k + y_k1 * cos_i_k * cos_Omega_k;
	double z_k = y_k1 * sin_i_k;
	if (!(PRN < 6 || PRN > 58))
	{
		return XYZCoordinate(x_k, y_k, z_k);
	}
	Omega_k = omega_0 + omega_dot * t_k - this->OMAGE_E() * toe_sec;
	sin_Omega_k = sin(Omega_k);
	cos_Omega_k = cos(Omega_k);
	double x_gk = x_k1 * cos_Omega_k - y_k1 * cos_i_k * sin_Omega_k;
	double y_gk = x_k1 * sin_Omega_k + y_k1 * cos_i_k * cos_Omega_k;
	double z_gk = y_k1 * sin_i_k;
	Vec3d xyz_gk = { x_gk, y_gk, z_gk };
	Matrix3d R_x = Matrix3d::Identity();
	R_x(2, 2) = cos(DEG2RAD(-5));
	R_x(2, 3) = sin(DEG2RAD(-5));
	R_x(3, 2) = -R_x(2, 3);
	R_x(3, 3) = R_x(2, 2);
	Matrix3d R_z = Matrix3d::Identity();
	R_z(1, 1) = cos(OMAGE_E() * t_k);
	R_z(1, 2) = sin(OMAGE_E() * t_k);
	R_z(2, 1) = -R_z(1, 2);
	R_z(2, 2) = R_z(1, 1);
	return XYZCoordinate::FromVec(R_z * R_x * xyz_gk);
}