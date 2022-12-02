#pragma once

#include "../TimeSystem/GPSTime.h"

enum class msgID
{
	GPSEPHM = 7,
	BESTPOS = 42,
	RANGE = 43,
	BDSEPHEM = 1696,
	OTHER = -1,
};

struct OEM7MsgHead
{
	msgID msgType;
	unsigned short msgLen;
	GPSTime msgTime;

	OEM7MsgHead() :msgType(msgID::OTHER), msgLen(0), msgTime() {}
	OEM7MsgHead(msgID msgType, unsigned short msgLen, GPSTime msgTime) :msgType(msgType), msgLen(msgLen), msgTime(msgTime) {}
};
