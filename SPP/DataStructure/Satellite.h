#pragma once

enum class NavigationSys : char
{
	GPS = 0,
	BDS = 4,
	GLONASS = 1,
	QZSS = 5,
	SBAS = 2,
	Galileo = 3,
	Other = 7,
	NavIC = 6
};

enum class SignalType : char
{
	L1CA,
	L2P,
	L2PY,
	L5Q,
	L1CP,
	L2CM,
	B1I,
	B2I,
	B3I,
	Other
};

class Satellite
{
public:
	unsigned short PRN;
	NavigationSys naviSys;
	Satellite(unsigned short PRN, NavigationSys naviSys) :PRN(PRN), naviSys(naviSys) {};
	Satellite() = default;
	bool operator<(const Satellite& s) const;
	bool operator==(const Satellite& s) const;
};
