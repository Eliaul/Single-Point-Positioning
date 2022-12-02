#include "GPSTime.h"
#include <format>

using namespace std::chrono;

GPSTime::GPSTime(gps_time<nanoseconds> gps)
	:gps(gps)
{
	this->gpsWeek = floor<weeks>(gps.time_since_epoch());
	this->gpsSecOfWeek = gps.time_since_epoch() - gpsWeek;
}

GPSTime::GPSTime(const int& week, const double& sec)
	:gpsWeek(week), gpsSecOfWeek(duration_cast<nanoseconds>(duration<double>(sec)))
{
	this->gps = gps_time<nanoseconds>(gpsWeek + gpsSecOfWeek);
}

weeks GPSTime::GpsWeek() const
{
	return this->gpsWeek;
}

nanoseconds GPSTime::GpsSecOfWeek() const
{
	return this->gpsSecOfWeek;
}

gps_time<nanoseconds> GPSTime::Gps() const
{
	return this->gps;
}

nanoseconds GPSTime::operator-(const GPSTime& g) const
{
	return this->gps - g.gps;
}

GPSTime GPSTime::FromCommTime(const CommonTime& comm)
{
	sys_time<nanoseconds> sys = sys_days(comm.Year_month_day()) + comm.Hh_mm_ss().to_duration();
	return GPSTime(clock_cast<gps_clock, system_clock, nanoseconds>(sys));
}

CommonTime GPSTime::ToCommTime(const GPSTime& gps)
{
	sys_time<nanoseconds> sys = clock_cast<system_clock, gps_clock, nanoseconds>(gps.gps);
	auto sysDay = floor<days>(sys);
	year_month_day ymd(sysDay);
	hh_mm_ss<nanoseconds> hms(sys - sysDay);
	return CommonTime(ymd, hms);
}

std::ostream& operator<<(std::ostream& cout, const GPSTime& gpsTime)
{
	cout << std::format("{0:4} {1:10.3f}", gpsTime.gpsWeek.count(), std::chrono::duration_cast<duration<double>>(gpsTime.gpsSecOfWeek).count());
	return cout;
}