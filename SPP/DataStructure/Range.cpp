#include <map>
#include "Range.h"
#include "../Calculate/Constant.h"

using namespace std;
using namespace std::chrono;

Range rangeLast;

void Range::ErrorDetect()
{
	using enum SignalType;
	for (auto it = this->sateDataMap.begin(); it != this->sateDataMap.end(); )
	{
		if (it->second.obsMap.size() > 1)
		{
			SignalType fr1 = it->first.naviSys == NavigationSys::GPS ? L1CA : B1I;
			SignalType fr2 = it->first.naviSys == NavigationSys::GPS ? L2PY : B3I;
			it->second.LinerCombination(fr1, fr2);
			if (rangeLast.sateDataMap.contains(it->first) && (this->gpsTime - rangeLast.gpsTime).count() < 1.1e9)
			{
				if (abs(it->second.mwComb - rangeLast.sateDataMap[it->first].mwCombAve) < 1.5 && abs(it->second.gfComb - rangeLast.sateDataMap[it->first].gfComb) < 0.05)
				{
					//此时表明该历元观测值可用
					auto i = rangeLast.sateDataMap[it->first].epochIndex;
					it->second.epochIndex = i + 1;
					it->second.mwCombAve = (i - 1) * rangeLast.sateDataMap[it->first].mwCombAve / ((double)i) + it->second.mwComb / ((double)i);
					it->second.isError = false;
				}
				else
				{
					it->second.epochIndex = 1; //此时表明该历元观测值有粗差或者周跳
				}
			}
			else
			{
				it->second.epochIndex = 1; //此时表明观测值有中断
			}
			++it;
		}
		else
		{
			it = this->sateDataMap.erase(it);  //没有双频观测值,删除该卫星
		}
	}
	rangeLast = *this;
}

void Range::GetSateMotionState(std::map<Satellite, BaseEphemeris*>& ephems)
{
	using enum SignalType;
	for (auto& [sate, sateData] : sateDataMap)
	{
		if (sateData.isError == false && ephems.contains(sate))
		{
			double pse = (sate.naviSys == NavigationSys::GPS) ? sateData.obsMap[L1CA].pseudorange : sateData.obsMap[B1I].pseudorange;
			auto tr = gpsTime.Gps() - duration_cast<nanoseconds>(duration<double>(pse / LIGHT_SPEED));
			auto time_tr = GPSTime(tr);
			ephems[sate]->Initialize(time_tr);
			sateData.clockError = ephems[sate]->CalClockError();
			tr -= duration_cast<nanoseconds>(duration<double>(sateData.clockError));
			time_tr = GPSTime(tr);
			ephems[sate]->Initialize(time_tr);
			auto timeLim = ephems[sate]->TimeAvailableLim();
			if ((time_tr.Gps() - ephems[sate]->toe_gpstime.Gps() - timeLim).count() < 0)
			{
				sateData.satePos = ephems[sate]->CalSatePos();
				sateData.clockError = ephems[sate]->CalClockError();
				sateData.sateVel = ephems[sate]->CalSateVelocity();
				sateData.clockVel = ephems[sate]->CalClockVelocity();
			}
			else
			{
				sateData.isError = true;
			}
		}
		else
		{
			sateData.isError = true;
		}
	}
}

std::ostream& operator<<(std::ostream& cout, const Range& range)
{
	for (auto& [sate, sateData] : range.sateDataMap)
	{
		if (sateData.isError == false && sateData.isLowHeightAngle == false)
		{
			char name = '\0';
			if (sate.naviSys == NavigationSys::GPS)
			{
				name = 'G';
			}
			if (sate.naviSys == NavigationSys::BDS)
			{
				name = 'C';
			}
			cout << name << std::format("{:02} X={:13.3f} Y={:13.3f} Z={:13.3f} Clk={:15.6E} Vx={:13.4f} Vy={:13.4f} Vz={:13.4f} Clkd={:14.5E} PIF={:15.4f} Trop={:8.3f} E={:8.3f}deg\n", sate.PRN, sateData.satePos_Rz(1), sateData.satePos_Rz(2), sateData.satePos_Rz(3),
				sateData.clockError, sateData.sateVel(1), sateData.sateVel(2), sateData.sateVel(3), sateData.clockVel, sateData.ifComb, sateData.trop, sateData.heightAngle);
		}
	}
	return cout;
}
