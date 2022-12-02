#include "SatelliteData.h"
#include "../Calculate/Constant.h"
#include "BaseEphemeris.h"
#include "../LinerAlgebra/Matrix3d.hpp"

void SatelliteData::PosEarthRotateCorr(BaseEphemeris* ephem, double signalSpreadTime)
{
	Matrix3d R_z = Matrix3d::Identity();
	R_z(1, 1) = cos(ephem->OMAGE_E() * signalSpreadTime);
	R_z(1, 2) = sin(ephem->OMAGE_E() * signalSpreadTime);
	R_z(2, 1) = -R_z(1, 2);
	R_z(2, 2) = R_z(1, 1);
	satePos_Rz = XYZCoordinate::FromVec(R_z * XYZCoordinate::ToVec(satePos));
}

void SatelliteData::VelEarthRotateCorr(BaseEphemeris* ephem, double signalSpreadTime)
{
	Matrix3d R_z = Matrix3d::Identity();
	R_z(1, 1) = cos(ephem->OMAGE_E() * signalSpreadTime);
	R_z(1, 2) = sin(ephem->OMAGE_E() * signalSpreadTime);
	R_z(2, 1) = -R_z(1, 2);
	R_z(2, 2) = R_z(1, 1);
	sateVel = R_z * sateVel;
}

SatelliteData::SatelliteData(SignalType signalType, const OBS& obs)
	:mwComb(0), gfComb(0), ifComb(0), epochIndex(1), isError(true), isLowHeightAngle(true), mwCombAve(0), satePos(0, 0, 0), sateVel(0,0,0), clockError(0), clockVel(0), heightAngle(0), trop(0)
{
	this->obsMap[signalType] = obs;
}

void SatelliteData::LinerCombination(SignalType signal1, SignalType signal2)
{
	auto carrierPhase1 = obsMap[signal1].carrierPhase;
	auto carrierPhase2 = obsMap[signal2].carrierPhase;
	auto frequency1 = obsMap[signal1].frequency;
	auto frequency2 = obsMap[signal2].frequency;
	auto pseudorange1 = obsMap[signal1].pseudorange;
	auto pseudorange2 = obsMap[signal2].pseudorange;
	mwComb = LIGHT_SPEED * (carrierPhase1 - carrierPhase2) / (frequency1 - frequency2)
		- (pseudorange1 * frequency1 + pseudorange2 * frequency2) / (frequency1 + frequency2);
	mwCombAve = mwComb;
	gfComb = LIGHT_SPEED * (carrierPhase1 / frequency1 - carrierPhase2 / frequency2);
	ifComb = (frequency1 * frequency1 * pseudorange1 - frequency2 * frequency2 * pseudorange2) / (pow(frequency1, 2) - pow(frequency2, 2));
}

void SatelliteData::IFCombination(double bdsTgd1)
{
	using enum SignalType;
	auto frequency1 = obsMap[B1I].frequency;
	auto frequency2 = obsMap[B3I].frequency;
	auto pseudorange1 = obsMap[B1I].pseudorange;
	auto pseudorange2 = obsMap[B3I].pseudorange;
	double k = pow(frequency1 / frequency2, 2);
	double pseudorange = (pseudorange2 - k * pseudorange1) / (1 - k) + LIGHT_SPEED * k * bdsTgd1 / (1 - k);
	ifComb = pseudorange;
}

