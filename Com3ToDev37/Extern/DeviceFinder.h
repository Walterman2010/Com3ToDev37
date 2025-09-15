
// some Part are got by:
// see also: https://github.com/studiosi/OpenCVDeviceEnumerator

#pragma once

#include <Windows.h>
#include <dshow.h>

#pragma comment(lib, "strmiids")

#include <map>
#include <string>


#define _WIN32_DCOM
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

struct Device
{
	int iComPort;			//
	std::string devicePath; //
	std::string deviceName; // This can be used to show the devices to the user
	std::string Ident;		//
	uint32_t serialNo;		//
	
	int ComPort = 666;		//
	int id;					// This can be used to open the device in OpenCV
};

class findDev {
public:
	// extract a spezific string from an other
	std::string findINN(std::string target, std::string Ident, int lengthOFcop, int offset = 0);

	IEnumWbemClassObject* FindDevice();

	bool CheckCom(int Com, std::string DeviceName);
};

class DeviceEnumerator {
	// Part by Studiosi -> https://github.com/studiosi/OpenCVDeviceEnumerator
public:
	DeviceEnumerator() = default;
	std::map<int, Device> getDevicesMap(const GUID deviceClass);
	std::map<int, Device> getVideoDevicesMap();
	std::map<int, Device> getAudioDevicesMap();

private:
	std::string ConvertBSTRToMBS(BSTR bstr);
	std::string ConvertWCSToMBS(const wchar_t* pstr, long wslen);
};
