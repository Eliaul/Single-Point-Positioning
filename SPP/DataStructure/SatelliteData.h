#pragma once

#include "../CoordinateSystem/XYZCoordinate.hpp"
#include "Satellite.h"
#include <map>

struct OBS
{
	double pseudorange;
	float psrSigma;
	double carrierPhase;
	float adrSigma;
	float dopp;
	float carrierToNoiseDensityRatio;
	unsigned long trackingStatus;
	double frequency; //��λHz

	OBS() = default;
	OBS(double pseudorange, float psrSigma, double carrierPhase, float adrSigma, float dopp, float carrierToNoiseDensityRatio, unsigned long trackingStatus, const double& frequency)
		:pseudorange(pseudorange), psrSigma(psrSigma), carrierPhase(carrierPhase), adrSigma(adrSigma), dopp(dopp), carrierToNoiseDensityRatio(carrierToNoiseDensityRatio), trackingStatus(trackingStatus), frequency(frequency) {};
};

class BaseEphemeris;

class SatelliteData
{
public:
	XYZCoordinate satePos;
	XYZCoordinate satePos_Rz;
	Vec3d sateVel;
	double clockError;
	double clockVel;
	double mwComb;
	double gfComb;
	double ifComb;
	double mwCombAve;
	int epochIndex;
	/// <summary>
	/// �����������Ƿ����
	/// </summary>
	bool isError;
	/// <summary>
	/// �Ƿ�Ϊ�͸߶Ƚ�
	/// </summary>
	bool isLowHeightAngle;
	double trop;
	double heightAngle; //�߶Ƚ�(deg)
	/// <summary>
	/// ���ǹ۲�ֵ�ֵ�
	/// </summary>
	std::map<SignalType, OBS> obsMap;

	SatelliteData() :mwComb(0),sateVel(0,0,0), gfComb(0), ifComb(0), epochIndex(1), mwCombAve(0), isError(true), isLowHeightAngle(true), satePos(0, 0, 0), clockError(0), clockVel(0), trop(0), heightAngle(0) {}	
	SatelliteData(SignalType signalType, const OBS& obsData);
	void PosEarthRotateCorr(BaseEphemeris* ephem, double signalSpreadTime);
	void VelEarthRotateCorr(BaseEphemeris* ephem, double signalSpreadTime);
	void LinerCombination(SignalType signal1, SignalType signal2);
	void IFCombination(double bdstgd1); 
};