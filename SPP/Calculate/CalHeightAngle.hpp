#pragma once

#include "../CoordinateSystem/XYZCoordinate.hpp"


template<CoordinateSystem T>
[[nodiscard]]
inline double CalHeightAngle(const XYZCoordinate& stationXYZPos, const BLHCoordinate<T>& stationBLHPos, const XYZCoordinate& sateXYZPos)
{
	using enum CoordinateSystem;
	if (stationXYZPos == XYZCoordinate(0, 0, 0))
	{
		return 90;
	}
	BLHCoordinate<T> pointBLHPos(stationBLHPos(1), stationBLHPos(2), stationBLHPos(3) + 100000);
	Vec3d v1 = XYZCoordinate::ToVec(XYZCoordinate::FromBLH(pointBLHPos) - stationXYZPos);
	Vec3d v2 = XYZCoordinate::ToVec(sateXYZPos - stationXYZPos);
	double theta = acos(Vec3d::Dot(v1, v2) / (v1.Norm() * v2.Norm()));
	return 90 - RAD2DEG(theta);
}