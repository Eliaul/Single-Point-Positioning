#include "Satellite.h"

bool Satellite::operator<(const Satellite& s) const
{
	return naviSys == s.naviSys ? PRN < s.PRN : naviSys < s.naviSys;
}

bool Satellite::operator==(const Satellite& s) const
{
	return (naviSys == s.naviSys && PRN == s.PRN);
}