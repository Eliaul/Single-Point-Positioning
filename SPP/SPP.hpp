#pragma once

#include "LinerAlgebra/Vec.hpp"
#include "TimeSystem/CommomTime.h"
#include "TimeSystem/GPSTime.h"
#include "CoordinateSystem/Ellipsoid.hpp"
#include "OEM7File/OEM7Decoder.hpp"
#include "DataStructure/PositionResult.hpp"
#include "Format/FormatOutput.hpp"
#include "Sockets.h"

template<CoordinateSystem T>
inline void FileSPP(std::string filePath, std::string outFileName)
{
	using namespace std;
	ifstream ifs(filePath, ios::in | ios::binary);
	OEM7Decoder<ifstream> oem7(ifs);
	ofstream ofs(outFileName, ios::out | ios::trunc);
	//ofs << "TF week sec X Y Z Vx Vy Vz B L H GPSClk BDSClk VClk PDOP Sigma VSigma GPSNum BDSNum\n";
	while (!oem7.stream.eof())
	{
		oem7.Decode();
		if (!oem7.isAvailable)
		{
			continue;
		}
		oem7.rangeMsg.msgData.ErrorDetect();
		SPPResult<T> res;
		res.SPPCalculate(oem7.rangeMsg.msgData, oem7.ephemsMap);
		res.SPVCalculate(oem7.rangeMsg.msgData, oem7.ephemsMap);
		ofs << oem7.rangeMsg.msgData;
		ofs << res;
	}
	oem7.Close();
	ofs.close();
}

template<CoordinateSystem T>
inline void WebSPP(const char* ip, int port)
{
	using namespace std;
	using namespace std::chrono;
	unsigned char* buff = new unsigned char[20480];
	auto now = time_point_cast<seconds>(system_clock::now());
	std::ios::sync_with_stdio(false);
	std::cout.tie(0);
	SOCKET server;
	bool isOpen = OpenSocket(server, ip, port);
	OEM7Decoder<stringstream> oem7;
	ofstream ofs(format("SPP{0:%F}_{0:%H}-{0:%M}-{0:%S}.txt", now), ios::out | ios::app);
	if (isOpen)
	{
		while (true)
		{
			int len = recv(server, reinterpret_cast<char*>(buff), 20480, 0);
			if (len > 0)
			{
				stringstream s;
				for (int i = 0; i < len; i++)
				{
					s << buff[i];
				}
				oem7.stream = std::move(s);
				while (!oem7.stream.eof())
				{
					oem7.Decode();
					if (!oem7.isAvailable)
					{
						continue;
					}
					oem7.rangeMsg.msgData.ErrorDetect();
					SPPResult<T> res;
					res.SPPCalculate(oem7.rangeMsg.msgData, oem7.ephemsMap);
					res.SPVCalculate(oem7.rangeMsg.msgData, oem7.ephemsMap);
					if (res.isAvailable)
					{
						ofs << res;
						cout << res << '\n';
					}
					else
					{
						cout << "false\n";
					}
				}
			}
		}
	}
}