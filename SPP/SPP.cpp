#include "SPP.hpp"

using namespace std;
using namespace std::chrono;

//!!!请使用C++20标准编译!!!

int main()
{
//#pragma region "OEM7网络"
//	WebSPP<CoordinateSystem::WGS84>("47.114.134.129", 7190);
//#pragma endregion
	
#pragma region "OEM7文件流"
	string filePath;
	cout << "输入文件路径:\n";
	cin >> filePath;
	cout << "正在计算...\n";
	auto s = high_resolution_clock::now();
	FileSPP<CoordinateSystem::WGS84>(filePath, "oem7.log");
	auto e = high_resolution_clock::now();
	cout << "计算完成!用时:" << duration_cast<duration<double>>(e - s) << '\n';
	system("pause");
#pragma endregion

	return 0;
}