#pragma once

#pragma warning(disable:26495)

#include <map>
#include <iostream>
#include "../TimeSystem/GPSTime.h"
#include "Satellite.h"
#include "SatelliteData.h"
#include "BaseEphemeris.h"

class Range
{
public:
	GPSTime gpsTime;
	unsigned long obsNum;
	/// <summary>
	/// ���������������ֵ�
	/// </summary>
	std::map<Satellite, SatelliteData> sateDataMap;

	Range() = default;
	void ErrorDetect();
	/// <summary>
	/// �����������ǵ�λ��,�ٶ�,�Ӳ������
	/// </summary>
	/// <param name="ephems">�����ֵ�</param>
	void GetSateMotionState(std::map<Satellite, BaseEphemeris*>& ephems);

};

std::ostream& operator<<(std::ostream& cout, const Range& range);