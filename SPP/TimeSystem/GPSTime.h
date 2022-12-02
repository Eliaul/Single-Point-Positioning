#pragma once

#include "CommomTime.h"

using std::chrono::gps_time;

class GPSTime
{
	friend std::ostream& operator<<(std::ostream& cout, const GPSTime& gpsTime);

public:
	explicit GPSTime(gps_time<nanoseconds> gps);
	GPSTime(const int& week, const double& sec);
	GPSTime() : gps(), gpsWeek(), gpsSecOfWeek() {}
	std::chrono::weeks GpsWeek() const;
	nanoseconds GpsSecOfWeek() const;
	gps_time<nanoseconds> Gps() const;
	nanoseconds operator-(const GPSTime& g) const;
	static GPSTime FromCommTime(const CommonTime& comm);
	static CommonTime ToCommTime(const GPSTime& gps);

private:
	gps_time<nanoseconds> gps;
	std::chrono::weeks gpsWeek;
	nanoseconds gpsSecOfWeek;
};

std::ostream& operator<<(std::ostream& cout, const GPSTime& gpsTime);