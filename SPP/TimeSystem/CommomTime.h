#pragma once

#include <iostream>
#include <chrono>

using std::chrono::nanoseconds;
using std::chrono::hh_mm_ss;
using std::chrono::year_month_day;

class CommonTime
{
	friend std::ostream& operator<<(std::ostream& cout, const CommonTime& ct);

public:
	CommonTime(const int& year, const int& month, const int& day, const int& hour, const int& minute, const double& second);
	CommonTime(const year_month_day& ymd, const hh_mm_ss<nanoseconds>& hms);
	year_month_day Year_month_day() const;
	hh_mm_ss<nanoseconds> Hh_mm_ss() const;

private:
	year_month_day ymd;
	hh_mm_ss<nanoseconds> hms;
};

std::ostream& operator<<(std::ostream& cout, const CommonTime& ct);