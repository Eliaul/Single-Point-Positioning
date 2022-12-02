#pragma once

#include "OEM7MsgHead.h"
#include "../DataStructure/BaseEphemeris.h"
#include "../DataStructure/Range.h"
#include "../Calculate/Constant.h"

template<class Type>
concept DataType = std::same_as<BaseEphemeris*, Type> || std::same_as<Range, Type>;

template<DataType Type>
class OEM7Msg
{
public:
	OEM7MsgHead msgHead;
	Type msgData;
};

template<>
class OEM7Msg<BaseEphemeris*>
{
public:
	OEM7MsgHead msgHead;
	BaseEphemeris* msgData;

	~OEM7Msg<BaseEphemeris*>();
};

inline OEM7Msg<BaseEphemeris*>::~OEM7Msg()
{
	delete msgData;
	msgData = nullptr;
}