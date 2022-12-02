#include "CommomTime.h"

using namespace std::chrono;

CommonTime::CommonTime(const int& year, const int& month, const int& day, const int& hour, const int& minute, const double& second)
{
	using _day = std::chrono::day;
	using _year = std::chrono::year;
	using _month = std::chrono::month;
	this->ymd = year_month_day(_year(year), _month(month), _day(day));
	this->hms = hh_mm_ss(duration_cast<nanoseconds>(hour * 1h + minute * 1min + second * 1s + 0 * 1ms + 0 * 1us + 0 * 1ns));
}

CommonTime::CommonTime(const year_month_day& ymd, const hh_mm_ss<nanoseconds>& hms)
	:ymd(ymd), hms(hms)
{
}

year_month_day CommonTime::Year_month_day() const
{
	return ymd;
}

hh_mm_ss<nanoseconds> CommonTime::Hh_mm_ss() const
{
	return hms;
}

std::ostream& operator<<(std::ostream& cout, const CommonTime& ct)
{
	cout << ct.ymd << " " << ct.hms;
	return cout;
}