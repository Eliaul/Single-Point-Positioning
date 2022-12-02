//#include "Ellipsoid.h"

///// <summary>
///// 长半轴单位为米
///// </summary>
///// <param name="coorsys"></param>
//Ellipsoid::Ellipsoid(CoordinateSystem coorsys) noexcept
//{
//	switch (coorsys)
//	{
//	case CoordinateSystem::WGS84:
//		semi_minor = 6356752.3142451795;
//		semi_major = 6378137;
//		oblateness = 1.0 / 298.2572235629972;
//		eccentricity_1 = 0.0818191908425524;
//		eccentricity_2 = 0.0820944379496565;
//		break;
//	case CoordinateSystem::CGCS2000:
//		semi_minor = 6356752.3141403558;
//		semi_major = 6378137;
//		oblateness = 1.0 / 298.2572221010042;
//		eccentricity_1 = 0.081819191042811;
//		eccentricity_2 = 0.0820944381519236;
//		break;
//	default:
//		semi_minor = 6356752.3142451795;
//		semi_major = 6378137;
//		oblateness = 1.0 / 298.2572235629972;
//		eccentricity_1 = 0.0818191908425524;
//		eccentricity_2 = 0.0820944379496565;
//		break;
//	}
//}