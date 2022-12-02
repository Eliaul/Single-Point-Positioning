#pragma once

#include <numeric>
#include "../TimeSystem/GPSTime.h"
#include "../CoordinateSystem/XYZCoordinate.hpp"
#include "Range.h"
#include "BaseEphemeris.h"
#include "../Calculate/CalHeightAngle.hpp"
#include "../Calculate/Hopefield.hpp"
#include "../CoordinateSystem/Ellipsoid.hpp"

template<CoordinateSystem T>
class SPPResult
{
public:
	GPSTime gpsTime;
	XYZCoordinate posXYZ;
	BLHCoordinate<T> posBLH;
	Vec velXYZ;

	double PDOP;
	double posSigma;
	double velSigma;
	double ClkDot;
	/// <summary>
	/// 钟差字典
	/// </summary>
	std::map<NavigationSys, double> Clk;
	/// <summary>
	/// 卫星数字典
	/// </summary>
	std::map<NavigationSys, int> sateNum;
	bool isAvailable;

	SPPResult() :ClkDot(0), velSigma(0), posXYZ(0, 0, 0), posBLH(0, 0, -Elliposid<T>::semi_major), velXYZ{0, 0, 0}, isAvailable(false), PDOP(999.999), posSigma(999.999), sateNum({std::pair(NavigationSys::GPS, 0), std::pair(NavigationSys::BDS, 0)}), Clk({std::pair(NavigationSys::GPS, 0), std::pair(NavigationSys::BDS, 0)}) {};
	void SPPCalculate(Range& range, std::map<Satellite, BaseEphemeris*>& ephems);
	void SPVCalculate(Range& range, std::map<Satellite, BaseEphemeris*>& ephems);

private:
	/// <summary>
	/// 构建B矩阵
	/// </summary>
	/// <param name="map">系数字典</param>
	/// <param name="keyArray">键集合</param>
	/// <returns>系数矩阵</returns>
	std::pair<Matrix, Vec> BuildMatrix(std::multimap<NavigationSys, std::vector<double>>& map, const std::vector<NavigationSys>& keyArray) const;
	/// <summary>
	/// 获取键集合
	/// </summary>
	/// <param name="map">系数字典</param>
	/// <returns>键集合</returns>
	std::vector<NavigationSys> GetKeyArray(std::multimap<NavigationSys, std::vector<double>>& map) const;
};

template<CoordinateSystem T>
inline std::ostream& operator<<(std::ostream& cout, const SPPResult<T>& spp)
{
	cout << std::format("{} ", spp.isAvailable) << spp.gpsTime << std::format(" X:{:12.4f} Y:{:12.4f} Z:{:12.4f} Vx:{:8.4f} Vy:{:8.4f} Vz:{:8.4f} B:{:12.8f} L:{:12.8f} H:{:8.3f} GPSClk:{:8.3f} BDSClk:{:8.3f} VClk:{:8.3f} PDOP:{:8.3f} Sigma:{:8.3f} VSigma:{:8.3f} GPSSats:{:3} BDSSats:{:3}\n", spp.posXYZ(1), spp.posXYZ(2), spp.posXYZ(3), spp.velXYZ(1), spp.velXYZ(2), spp.velXYZ(3), spp.posBLH(1),
		spp.posBLH(2), spp.posBLH(3), spp.Clk.at(NavigationSys::GPS), spp.Clk.at(NavigationSys::BDS), spp.ClkDot, spp.PDOP, spp.posSigma, spp.velSigma, spp.sateNum.at(NavigationSys::GPS), spp.sateNum.at(NavigationSys::BDS));
	//if (spp.isAvailable)
	//{
	//	cout << std::format("{} ", spp.isAvailable) << spp.gpsTime << std::format(" {:12.4f} {:12.4f} {:12.4f} {:8.4f} {:8.4f} {:8.4f} {:12.8f} {:12.8f} {:8.3f} {:8.3f} {:8.3f} {:8.3f} {:8.3f} {:8.3f} {:8.3f} {:3} {:3}\n", spp.posXYZ(1), spp.posXYZ(2), spp.posXYZ(3), spp.velXYZ(1), spp.velXYZ(2), spp.velXYZ(3), spp.posBLH(1),
	//		spp.posBLH(2), spp.posBLH(3), spp.Clk.at(NavigationSys::GPS), spp.Clk.at(NavigationSys::BDS), spp.ClkDot, spp.PDOP, spp.posSigma, spp.velSigma, spp.sateNum.at(NavigationSys::GPS), spp.sateNum.at(NavigationSys::BDS));
	//}
	//else
	//{
	//	cout << "false\n";
	//}
	return cout;
}

template<CoordinateSystem T>
inline std::pair<Matrix, Vec> SPPResult<T>::BuildMatrix(std::multimap<NavigationSys, std::vector<double>>& map, const std::vector<NavigationSys>& keyArray) const
{
	std::vector<size_t> rowCount(keyArray.size());
	size_t matrixRow = 0;
	for (size_t i = 0; i < keyArray.size(); i++)
	{
		rowCount[i] = map.count(keyArray[i]);
		matrixRow += rowCount[i];
	}
	Matrix BMatrix = Matrix::Zeros(static_cast<int>(matrixRow), static_cast<int>(3 + keyArray.size()));
	Vec WVec(static_cast<int>(BMatrix.Row()));
	size_t rowStart = 1;
	for (size_t i = 0; i < keyArray.size(); i++)
	{
		auto range = map.equal_range(keyArray[i]);
		size_t row = rowStart;
		for (auto& it = range.first; it != range.second; ++it)
		{
			BMatrix(static_cast<int>(row), 1) = it->second[0];
			BMatrix(static_cast<int>(row), 2) = it->second[1];
			BMatrix(static_cast<int>(row), 3) = it->second[2];
			WVec(static_cast<int>(row)) = it->second[3];
			BMatrix(static_cast<int>(row), static_cast<int>(i) + 4) = 1;
			++row;
		}
		rowStart += rowCount[i];
	}
	return std::pair(std::move(BMatrix), std::move(WVec));
}

template<CoordinateSystem T>
inline std::vector<NavigationSys> SPPResult<T>::GetKeyArray(std::multimap<NavigationSys, std::vector<double>>& map) const
{
	std::vector<NavigationSys> keyArray;
	for (auto it = map.begin(); it != map.end(); it = map.upper_bound(it->first))
	{
		keyArray.emplace_back(it->first);
	}
	return keyArray;
}

template<CoordinateSystem T>
inline void SPPResult<T>::SPPCalculate(Range& range, std::map<Satellite, BaseEphemeris*>& ephems)
{
	using enum NavigationSys;
	this->gpsTime = range.gpsTime;
	int count = 0;
	range.GetSateMotionState(ephems);
	std::multimap<NavigationSys, std::vector<double>> matrixDataMap;
	while (true)
	{
		for (auto& [NavigationSys, Num] : sateNum)
			Num = 0;
		for (auto& [sate, sateData] : range.sateDataMap)
		{
			if (sateData.isError || ephems[sate]->health == 1)
				continue;
			double rho = XYZCoordinate::Distance(posXYZ, sateData.satePos);
			double deltaT = rho / LIGHT_SPEED;
			sateData.PosEarthRotateCorr(ephems[sate], deltaT);
			rho = XYZCoordinate::Distance(posXYZ, sateData.satePos_Rz);
			sateData.heightAngle = CalHeightAngle(posXYZ, posBLH, sateData.satePos_Rz);
			sateData.isLowHeightAngle = (sateData.heightAngle < 15);
			if (sateData.isLowHeightAngle)
				continue;
			sateData.trop = Hopefield(sateData.heightAngle, posBLH(3));
			double delta_t = Clk[sate.naviSys];
			sateNum[sate.naviSys] ++;
			if (sate.naviSys == BDS)
				sateData.IFCombination(dynamic_cast<BDSEphemeris*>(ephems[sate])->tgd1);
			std::vector<double> temp(4);
			temp[3] = sateData.ifComb - (rho + delta_t - LIGHT_SPEED * sateData.clockError + sateData.trop);
			for (int i = 0; i < 3; i++)
				temp[i] = (posXYZ(i + 1) - sateData.satePos_Rz(i + 1)) / rho;
			//加入系数字典
			matrixDataMap.emplace(sate.naviSys, temp);
		}
		//对卫星数字典的所有值求和
		int avaSateNum = std::accumulate(sateNum.begin(), sateNum.end(), 0, [](int a, const std::pair<const NavigationSys, int>& b) { return a + b.second; });
		if (avaSateNum < 6)
			break;
		auto keyArray = GetKeyArray(matrixDataMap);
		const auto& [B, w] = BuildMatrix(matrixDataMap, keyArray);
		Matrix P = Matrix::Identity(avaSateNum);
		Matrix Qxx = Matrix::Inverse(Matrix::Transpose(B) * P * B);
		Vec x = Qxx * Matrix::Transpose(B) * P * w;
		posXYZ += XYZCoordinate(x(1), x(2), x(3));
		posBLH = XYZCoordinate::ToBLH<T>(posXYZ);
		for (int i = 0; i < keyArray.size(); i++)
			Clk[keyArray[i]] += x(4 + i);
		count++;
		matrixDataMap.clear();
		if (x.Norm() < 1e-6)
		{
			isAvailable = true;
			Vec v = B * x - w;
			posSigma = sqrt((Matrix::Transpose((Matrix)v) * P * v)(1) / (avaSateNum - 5));
			PDOP = sqrt(Qxx(1, 1) + Qxx(2, 2) + Qxx(3, 3));
			break;
		}
		if (count == 10)
			break;
	}
}

template<CoordinateSystem T>
inline void SPPResult<T>::SPVCalculate(Range& range, std::map<Satellite, BaseEphemeris*>& ephems)
{
	using enum SignalType;
	if (isAvailable == false)
	{
		return;
	}
	int avaSateNum = std::accumulate(sateNum.begin(), sateNum.end(), 0, [](int a, const std::pair<const NavigationSys, int>& b) {return a + b.second; });
	Matrix Bmatrix(avaSateNum, 4);
	Vec wvector(avaSateNum);
	int index = 0;
	for (auto& [sate, sateData] : range.sateDataMap)
	{
		if (sateData.isError == false && sateData.isLowHeightAngle == false)
		{
			index++;
			double rho = XYZCoordinate::Distance(posXYZ, sateData.satePos_Rz);
			sateData.VelEarthRotateCorr(ephems[sate], rho / LIGHT_SPEED);
			Bmatrix(index, 1) = (posXYZ(1) - sateData.satePos_Rz(1)) / rho;
			Bmatrix(index, 2) = (posXYZ(2) - sateData.satePos_Rz(2)) / rho;
			Bmatrix(index, 3) = (posXYZ(3) - sateData.satePos_Rz(3)) / rho;
			Bmatrix(index, 4) = 1;
			double rho_dot = ((sateData.satePos_Rz(1) - posXYZ(1)) * sateData.sateVel(1) + (sateData.satePos_Rz(2) - posXYZ(2)) * sateData.sateVel(2) + (sateData.satePos_Rz(3) - posXYZ(3)) * sateData.sateVel(3)) / rho;
			wvector(index) = sate.naviSys == NavigationSys::GPS ? sateData.obsMap[L1CA].dopp * LIGHT_SPEED / sateData.obsMap[L1CA].frequency - (rho_dot - LIGHT_SPEED * sateData.clockVel) : sateData.obsMap[B1I].dopp * LIGHT_SPEED / sateData.obsMap[B1I].frequency - (rho_dot - LIGHT_SPEED * sateData.clockVel);
		}
	}
	Matrix P = Matrix::Identity(avaSateNum);
	Matrix x = Matrix::Inverse(Matrix::Transpose(Bmatrix) * P * Bmatrix) * Matrix::Transpose(Bmatrix) * P * wvector;
	velXYZ = { x(1,1),x(2,1),x(3,1) };
	ClkDot = x(4, 1) / LIGHT_SPEED;
	Matrix v = Bmatrix * x - wvector;
	velSigma = sqrt((Matrix::Transpose(v) * P * v)(1, 1) / (avaSateNum - 4));
}


