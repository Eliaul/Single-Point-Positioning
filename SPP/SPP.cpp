#include "SPP.hpp"

using namespace std;
using namespace std::chrono;

//!!!��ʹ��C++20��׼����!!!

int main()
{
//#pragma region "OEM7����"
//	WebSPP<CoordinateSystem::WGS84>("47.114.134.129", 7190);
//#pragma endregion
	
#pragma region "OEM7�ļ���"
	string filePath;
	cout << "�����ļ�·��:\n";
	cin >> filePath;
	cout << "���ڼ���...\n";
	auto s = high_resolution_clock::now();
	FileSPP<CoordinateSystem::WGS84>(filePath, "oem7.log");
	auto e = high_resolution_clock::now();
	cout << "�������!��ʱ:" << duration_cast<duration<double>>(e - s) << '\n';
	system("pause");
#pragma endregion

	return 0;
}