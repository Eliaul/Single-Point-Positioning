#pragma once

#include <fstream>
#include <sstream>
#include <map>
#include <tuple>
#include "../Calculate/BIN2DEC.hpp"
#include "../DataStructure/GPSEphemeris.h"
#include "../DataStructure/BDSEphemeris.h"
#include "../DataStructure/Range.h"
#include "OEM7Msg.hpp"

constexpr auto POLYCRC32 = 0xEDB88320u;

template<class T>
concept StreamType = std::same_as<T, std::ifstream> || std::same_as<T, std::stringstream>;

template<StreamType T>
class OEM7Decoder
{
public:
	~OEM7Decoder();
	T stream;

	std::map<Satellite, BaseEphemeris*> ephemsMap;
	OEM7Msg<Range> rangeMsg;
	bool isAvailable;

	OEM7Decoder() :isAvailable(false) {}
	explicit OEM7Decoder(T& stream) :stream(std::move(stream)), isAvailable(false) {}
	void Decode();
	void Close();

private:
	std::pair<Satellite, GPSEphemeris> GPSEphemerisDecode(unsigned char* msg) const;
	std::pair<Satellite, BDSEphemeris> BDSEphemerisDecode(unsigned char* msg) const;
	std::tuple<NavigationSys, double, SignalType, bool> GetSateMap(unsigned char navisys, unsigned char signalType) const;

	Range RangeDecode(unsigned char* msg) const;
};


inline unsigned int crc32(const unsigned char* buff, int len) {
	int i, j;
	unsigned int crc = 0;
	for (i = 0; i < len; i++)
	{
		crc ^= buff[i];
		for (j = 0; j < 8; j++)
		{
			if (crc & 1) crc = (crc >> 1) ^ POLYCRC32;
			else crc >>= 1;
		}
	}
	return crc;
}

template<StreamType T>
OEM7Decoder<T>::~OEM7Decoder()
{
	for (auto it = ephemsMap.begin(); it != ephemsMap.end();)
	{
		delete it->second;
		it->second = nullptr;
		it = ephemsMap.erase(it);
	}
	ephemsMap.clear();
}

template<StreamType T>
inline void OEM7Decoder<T>::Decode()
{
	isAvailable = false;
	while (stream.eof() == false)
	{
		unsigned char sync = 0;
		stream.read((char*)(&sync), 1);
		if (sync == 0xAA)
		{
			stream.read((char*)(&sync), 1);
			if (sync == 0x44)
			{
				stream.read((char*)(&sync), 1);
				if (sync == 0x12)
				{
					int msgLen = 0;
					unsigned int crcCheck = 0;
					stream.seekg(static_cast<int>(stream.tellg()) + 5);
					stream.read(reinterpret_cast<char*>(&msgLen), 2);
					std::unique_ptr<unsigned char> msg(new unsigned char[msgLen + 28]);
					stream.seekg(static_cast<int>(stream.tellg()) - 10);
					stream.read(reinterpret_cast<char*>(msg.get()), msgLen + 28);
					stream.read(reinterpret_cast<char*>(&crcCheck), 4);
					if (crcCheck != crc32(msg.get(), msgLen + 28))
					{
						//CRC校验不通过
						continue;
					}
					GPSTime msgTime = GPSTime(GetField<unsigned short>(msg.get() + 14), (double)GetField<unsigned long>(msg.get() + 16) / 1000);
					switch (auto msgType = (msgID)GetField<unsigned short>(msg.get() + 4); msgType)
					{
					case msgID::GPSEPHM:
					{
						const auto& [sate, ephem] = GPSEphemerisDecode(msg.get());
						if (ephemsMap.contains(sate))
							*ephemsMap[sate] = ephem;
						else
							ephemsMap[sate] = new GPSEphemeris(ephem);
						break;
					}
					case msgID::BESTPOS:
						break;
					case msgID::RANGE:
					{
						rangeMsg.msgHead = OEM7MsgHead(msgType, msgLen, msgTime);
						rangeMsg.msgData = RangeDecode(msg.get());
						rangeMsg.msgData.gpsTime = msgTime;
						isAvailable = true;
						return;
					}
					case msgID::BDSEPHEM:
					{
						const auto& [sate, ephem] = BDSEphemerisDecode(msg.get());
						if (ephemsMap.contains(sate))
							*ephemsMap[sate] = ephem;
						else
							ephemsMap[sate] = new BDSEphemeris(ephem);
						break;
					}
					case msgID::OTHER:
						break;
					default:
						break;
					}
					return;
				}
			}
		}
	}
	return;
}

template<>
inline void OEM7Decoder<std::ifstream>::Close()
{
	stream.close();
}

template<>
void OEM7Decoder<std::stringstream>::Close()
{

}

template<StreamType T>
inline std::pair<Satellite, GPSEphemeris> OEM7Decoder<T>::GPSEphemerisDecode(unsigned char* msg) const
{
	GPSEphemeris gpsEphem;
	Satellite sate(static_cast<unsigned short>(GetField<unsigned long>(msg + 28)), NavigationSys::GPS);
	auto msgData = msg + 28;
	gpsEphem.PRN = GetField<unsigned long>(msgData);
	gpsEphem.health = GetField<unsigned long>(msgData + 12);
	auto week = GetField<unsigned long>(msgData + 24);
	auto toe = GetField<double>(msgData + 32);
	gpsEphem.toe_gpstime = GPSTime(gps_time<nanoseconds>(std::chrono::weeks(week) + duration_cast<nanoseconds>(std::chrono::duration<double>(toe))));
	gpsEphem.toe_sec = toe;
	gpsEphem.A = GetField<double>(msgData + 40);
	gpsEphem.Delta_N = GetField<double>(msgData + 48);
	gpsEphem.M_0 = GetField<double>(msgData + 56);
	gpsEphem.ecc = GetField<double>(msgData + 64);
	gpsEphem.omega = GetField<double>(msgData + 72);
	gpsEphem.cuc = GetField<double>(msgData + 80);
	gpsEphem.cus = GetField<double>(msgData + 88);
	gpsEphem.crc = GetField<double>(msgData + 96);
	gpsEphem.crs = GetField<double>(msgData + 104);
	gpsEphem.cic = GetField<double>(msgData + 112);
	gpsEphem.cis = GetField<double>(msgData + 120);
	gpsEphem.I_0 = GetField<double>(msgData + 128);
	gpsEphem.I_dot = GetField<double>(msgData + 136);
	gpsEphem.omega_0 = GetField<double>(msgData + 144);
	gpsEphem.omega_dot = GetField<double>(msgData + 152);
	gpsEphem.toc_gpstime = GPSTime(gps_time<nanoseconds>(std::chrono::weeks(week) + duration_cast<nanoseconds>(std::chrono::duration<double>(GetField<double>(msgData + 164)))));
	gpsEphem.a_f0 = GetField<double>(msgData + 180);
	gpsEphem.a_f1 = GetField<double>(msgData + 188);
	gpsEphem.a_f2 = GetField<double>(msgData + 196);
	return std::pair<Satellite, GPSEphemeris>(sate, gpsEphem);
}

template<StreamType T>
inline std::pair<Satellite, BDSEphemeris> OEM7Decoder<T>::BDSEphemerisDecode(unsigned char* msg) const
{
	BDSEphemeris bdsEphem;
	Satellite sate(static_cast<unsigned short>(GetField<unsigned long>(msg + 28)), NavigationSys::BDS);
	auto msgData = msg + 28;
	bdsEphem.PRN = GetField<unsigned long>(msgData);
	auto week = GetField<unsigned long>(msgData + 4);
	bdsEphem.health = GetField<unsigned long>(msgData + 16);
	auto toc = GetField<unsigned long>(msgData + 40);
	bdsEphem.toc_gpstime = GPSTime(gps_time<nanoseconds>(std::chrono::weeks(week + 1356) + (toc + 14) * 1s));
	bdsEphem.a_f0 = GetField<double>(msgData + 44);
	bdsEphem.a_f1 = GetField<double>(msgData + 52);
	bdsEphem.a_f2 = GetField<double>(msgData + 60);
	auto toe = GetField<unsigned long>(msgData + 72);
	bdsEphem.toe_sec = GetField<unsigned long>(msgData + 72);
	bdsEphem.toe_gpstime = GPSTime(gps_time<nanoseconds>(std::chrono::weeks(week + 1356) + (toe + 14) * 1s));
	bdsEphem.A = GetField<double>(msgData + 76);
	bdsEphem.A *= bdsEphem.A;
	bdsEphem.ecc = GetField<double>(msgData + 84);
	bdsEphem.omega = GetField<double>(msgData + 92);
	bdsEphem.Delta_N = GetField<double>(msgData + 100);
	bdsEphem.M_0 = GetField<double>(msgData + 108);
	bdsEphem.omega_0 = GetField<double>(msgData + 116);
	bdsEphem.omega_dot = GetField<double>(msgData + 124);
	bdsEphem.I_0 = GetField<double>(msgData + 132);
	bdsEphem.I_dot = GetField<double>(msgData + 140);
	bdsEphem.cuc = GetField<double>(msgData + 148);
	bdsEphem.cus = GetField<double>(msgData + 156);
	bdsEphem.crc = GetField<double>(msgData + 164);
	bdsEphem.crs = GetField<double>(msgData + 172);
	bdsEphem.cic = GetField<double>(msgData + 180);
	bdsEphem.cis = GetField<double>(msgData + 188);
	bdsEphem.tgd1 = GetField<double>(msgData + 20);
	bdsEphem.tgd2 = GetField<double>(msgData + 28);
	return std::pair<Satellite, BDSEphemeris>(sate, bdsEphem);
}

template<StreamType T>
inline Range OEM7Decoder<T>::RangeDecode(unsigned char* msg) const
{
	using enum NavigationSys;
	Range range;
	unsigned long offset = 0;
	range.obsNum = GetField<unsigned long>(msg + 28);
	auto msgData = msg + 28;
	for (unsigned long i = 0; offset <= (range.obsNum - 1) * 44; i++)
	{
		unsigned long trackingStatus = GetField<unsigned long>(msgData + offset + 44);
		auto navisys = BitCut<unsigned char, unsigned long, 3>(trackingStatus, 16);
		auto signalType = BitCut<unsigned char, unsigned long, 5>(trackingStatus, 21);
		if (auto [sys, frequency, signalType, isTrue] = GetSateMap(navisys, signalType); isTrue)
		{
			Satellite sate(GetField<unsigned short>(msgData + offset + 4), sys);
			OBS obs(GetField<double>(msgData + offset + 8), GetField<float>(msgData + offset + 16), -GetField<double>(msgData + offset + 20), GetField<float>(msgData + offset + 28), -GetField<float>(msgData + offset + 32), GetField<float>(msgData + offset + 36), trackingStatus, frequency);
			range.sateDataMap[sate].obsMap[signalType] = obs;
		}
		offset += 44;
	}
	return range;
}

template<StreamType T>
std::tuple<NavigationSys, double, SignalType, bool> OEM7Decoder<T>::GetSateMap(unsigned char navisys, unsigned char signalType) const
{
	using enum NavigationSys;
	switch ((NavigationSys)navisys)
	{
	case GPS:
	{
		using enum SignalType;
		switch (signalType)
		{
		case 0:
			return std::make_tuple(GPS, 1575.42e6, L1CA, true);
		case 9:
			return std::make_tuple(GPS, 1227.60e6, L2PY, true);
		}
	}
	case BDS:
	{
		using enum SignalType;
		switch (signalType)
		{
		case 0:
			return std::make_tuple(BDS, 1561.098e6, B1I, true);
		case 2:
			return std::make_tuple(BDS, 1268.52e6, B3I, true);
		case 4:
			return std::make_tuple(BDS, 1561.098e6, B1I, true);
		case 6:
			return std::make_tuple(BDS, 1268.52e6, B3I, true);
		}
	}
	}
	return std::make_tuple(NavigationSys::Other, 0, SignalType::Other, false);
}