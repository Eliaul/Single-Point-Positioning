#pragma once

enum class CoordinateSystem : char
{
	WGS84, CGCS2000
};

template<CoordinateSystem T>
struct Elliposid;

template<>
struct Elliposid<CoordinateSystem::CGCS2000>
{
	constexpr static double semi_minor     = 6356752.3141403558;				//�̰���
	constexpr static double semi_major     = 6378137;							//������
	constexpr static double oblateness     = 1.0 / 298.2572221010042;			//����
	constexpr static double eccentricity_1 = 0.081819191042811;				    //��һƫ����
	constexpr static double eccentricity_2 = 0.0820944381519236;			    //�ڶ�ƫ����
};

template<>
struct Elliposid<CoordinateSystem::WGS84>
{
	constexpr static double semi_minor     = 6356752.3142451795;				//�̰���
	constexpr static double semi_major     = 6378137;							//������
	constexpr static double oblateness     = 1.0 / 298.2572235629972;		 	//����
	constexpr static double eccentricity_1 = 0.0818191908425524;		     	//��һƫ����
	constexpr static double eccentricity_2 = 0.0820944379496565;			    //�ڶ�ƫ����
};