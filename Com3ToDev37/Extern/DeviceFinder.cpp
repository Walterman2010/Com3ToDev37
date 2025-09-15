#include "DeviceFinder.h"

std::map<int, Device> DeviceEnumerator::getVideoDevicesMap() {
	return getDevicesMap(CLSID_VideoInputDeviceCategory);
}

std::map<int, Device> DeviceEnumerator::getAudioDevicesMap() {
	return getDevicesMap(CLSID_AudioInputDeviceCategory);
}

// Returns a map of id and devices that can be used
std::map<int, Device> DeviceEnumerator::getDevicesMap(const GUID deviceClass)
{
	std::map<int, Device> deviceMap;

	HRESULT hr = CoInitialize(nullptr);
	if (FAILED(hr)) {
		return deviceMap; // Empty deviceMap as an error
	}

	// Create the System Device Enumerator
	ICreateDevEnum *pDevEnum;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));

	// If succeeded, create an enumerator for the category
	IEnumMoniker *pEnum = NULL;
	if (SUCCEEDED(hr)) {
		hr = pDevEnum->CreateClassEnumerator(deviceClass, &pEnum, 0);
		if (hr == S_FALSE) {
			hr = VFW_E_NOT_FOUND;
		}
		pDevEnum->Release();
	}

	// Now we check if the enumerator creation succeeded
	int deviceId = -1;
	if (SUCCEEDED(hr)) {
		// Fill the map with id and friendly device name
		IMoniker *pMoniker = NULL;
		while (pEnum->Next(1, &pMoniker, NULL) == S_OK) {

			IPropertyBag *pPropBag;
			HRESULT hr = pMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag));
			if (FAILED(hr)) {
				pMoniker->Release();
				continue;
			}

			// Create variant to hold data
			VARIANT var;
			VariantInit(&var);

			std::string deviceName;
			std::string devicePath;

			// Read FriendlyName or Description
			hr = pPropBag->Read(L"Description", &var, 0); // Read description
			if (FAILED(hr)) {
				// If description fails, try with the friendly name
				hr = pPropBag->Read(L"FriendlyName", &var, 0);
			}
			// If still fails, continue with next device
			if (FAILED(hr)) {
				VariantClear(&var);
				continue;
			}
			// Convert to string
			else {
				deviceName = ConvertBSTRToMBS(var.bstrVal);
			}

			VariantClear(&var); // We clean the variable in order to read the next value

								// We try to read the DevicePath
			hr = pPropBag->Read(L"DevicePath", &var, 0);//
			if (FAILED(hr)) {
				VariantClear(&var);
				continue; // If it fails we continue with next device
			}
			else {
				devicePath = ConvertBSTRToMBS(var.bstrVal);
			}

			// We populate the map
			deviceId++;
			Device currentDevice;
			currentDevice.id = deviceId;
			currentDevice.deviceName = deviceName;
			currentDevice.devicePath = devicePath;
			deviceMap[deviceId] = currentDevice;

		}
		pEnum->Release();
	}
	CoUninitialize();
	return deviceMap;
}

/*
This two methods were taken from
https://stackoverflow.com/questions/6284524/bstr-to-stdstring-stdwstring-and-vice-versa
*/

std::string DeviceEnumerator::ConvertBSTRToMBS(BSTR bstr)
{
	int wslen = ::SysStringLen(bstr);
	return ConvertWCSToMBS((wchar_t*)bstr, wslen);
}

std::string DeviceEnumerator::ConvertWCSToMBS(const wchar_t* pstr, long wslen)
{
	int len = ::WideCharToMultiByte(CP_ACP, 0, pstr, wslen, NULL, 0, NULL, NULL);

	std::string dblstr(len, '\0');
	len = ::WideCharToMultiByte(CP_ACP, 0 /* no flags */,
		pstr, wslen /* not necessary NULL-terminated */,
		&dblstr[0], len,
		NULL, NULL /* no default char */);

	return dblstr;
}

//Suche nach ComPorts
/*
weitere infos:
	https://www.dmtf.org/standards/cim/cim_schema_v2540 -> doku
	https://docs.microsoft.com/en-us/windows/win32/cimwin32prov/cim-logicaldevice
	Original code:
	https://docs.microsoft.com/de-de/windows/win32/wmisdk/example--getting-wmi-data-from-the-local-computer?redirectedfrom=MSDN
	entnommen an 24.11.20
*/

std::string findDev::findINN(std::string target, std::string Ident, int lengthOFcop, int offset ) {
	/*
	this Funktion find specific Charater i a String and give Back connected values, as String
		Target[string] - the string where we search for the value
		Ident[string] - this will trigger the funktion, and start to work direkt after the Identstring,
						the fkt will search only one time in the Taget string, so the Ident have to be
						chousen that it can not repeat
		lengthOFcop[int] - the number of character given back
		offset[int] - optional - Number of character between Ident and returnvalue (searchValue)
						in case the Ident have Character which are incompatible for strings
	*/
	std::string searchValue = "";
	int SameCount;
	SameCount = 0;

	for (int i = 0; i <= target.length(); i++)	// loop so long like the target
	{
		if (target[i] == Ident[SameCount])		// in case the target have the same character like
		{										// the Ident, a Counter increase, otherwise they 
			SameCount++;						// the counter will be reseted
		}
		else
		{
			SameCount = 0;
		}										// in case the counter have the same value like the 
		if (SameCount >= Ident.length())		// length of the Ident, The Ident is in total in the 
		{										// target, the Funktion start to Copy
			for (int k = i + 1 + offset; k <= lengthOFcop + i; k++)
			{									// The fkt start to copy after the Identvalue, or the
				searchValue += target[k];		// offsent, and copy the "legth of copy"
			}
			break;								// after coppying, it will be finished
		}
	}
	return searchValue;
}

IEnumWbemClassObject* findDev::FindDevice(){//std::string Query, std::string findCap) {

	HRESULT hres;	//errorhandling

	// 1. Step: Initialize the Com Port
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
	{
		//return "fail Step 1";                  // Program has failed.
	}

	// 2. Step: set security level of general Com 
	hres = CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
	// ...(0,COM authentication,Authentication services, Reserved, Default authentication, Default Impersonation, Authentication info, Additional capabilities, Reserved)

	if (FAILED(hres))
	{
		CoUninitialize();
		//return IUnknown;                    // Program has failed.
	}
	// 3. Step: Obtain the initial locator to WMI
	IWbemLocator *pLoc = NULL;

	hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&pLoc);

	if (FAILED(hres))
	{
		CoUninitialize();
		//return "fail Step 3";                 // Program has failed.
	}
	// 4. Step: Connect to WMI through the IWbemLocator::ConnectServer method
	IWbemServices *pSvc = NULL;

	// Connect to the root\cimv2 namespace with
	// the current user and obtain pointer pSvc
	// to make IWbemServices calls.
	hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSvc);
	//...(Object path of WMI namespace,User name. NULL = current user;User password. NULL = current;Locale. NULL indicates current;Security flags.,Authority (for example, Kerberos),Context object, pointer to IWbemServices proxy)

	if (FAILED(hres))
	{
		pLoc->Release();
		CoUninitialize();
		//return "fail Step 4";                // Program has failed.
	}
	//cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;

	// 5. Step: Not needed, because we search local

	// 6. Step: Use the IWbemServices pointer to make requests of WMI
	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_PnPEntity "), //CIM_LogicalDevice __________________hier anpassen

		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres))
	{
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		//return "fail Step 6";               // Program has failed.
	}
	
	return pEnumerator;
}

bool findDev::CheckCom(int Com, std::string DeviceName) {
	// these funktion check if the Comport the right machine, to prevent a Bad connection
	findDev stufffind;
	IEnumWbemClassObject* pEnumeratorn = stufffind.FindDevice();	// list all Ports in enumerator
	IWbemClassObject* pclsObj = NULL;
	ULONG uReturn = 0;
	std::string PNPdev;				// Path value
	int length;
	std::string identify;	// declare Iden var
	Device gotDevCom[256];	// var to save all values of the Com Ports
	int count = 0;				// reset counter
	std::string found = "COM";	// to search in all the Ports, that with COM
	std::string comNum = "";		// to internal save the Com Port value
	int zaehler = 0;
	while (pEnumeratorn)	// repeat for all Port devices
	{
		zaehler++;
		HRESULT hr = pEnumeratorn->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn); //get next Port Device
		if (0 == uReturn)											// break if there are no diveses anymore
		{
			break;
		}

		VARIANT vtProp;												// declarate information Var

		hr = pclsObj->Get(L"Caption", 0, &vtProp, 0, 0);			// Get the value of the Name property
		// cout << zaehler << endl;

		if (vtProp.bstrVal || NULL) {								// check there is some value inside
			PNPdev = _bstr_t(vtProp.bstrVal, false);				// get the Port Device informations
		}


		if (strstr(PNPdev.c_str(), found.c_str())) {				// we are only interresstet in Ports with Port number (look for Com)

			gotDevCom[count].deviceName = PNPdev;					// save the Port name / Device Name
			comNum = stufffind.findINN(PNPdev, "COM", 2);			// find the value of the comport, and save it as string

			const char* isNum = comNum.c_str();						// need to translate to char, for checking the Com is a number and not the part of a name

			if (isdigit(*isNum)) {									// check the com number is a number and noch any letter
				gotDevCom[count].ComPort = stoi(comNum);			// save the COM Port

				VariantClear(&vtProp);								// reset the Prop

				length = DeviceName.length();						// get length of input string
																	// searsch inputstring in Device Name
				PNPdev = stufffind.findINN(gotDevCom[count].deviceName, DeviceName, 0, -length);

				if ((Com== gotDevCom[count].ComPort)&&(PNPdev == DeviceName)) {	 // check String and Com Port
					
					pclsObj->Release();
					pEnumeratorn->Release();
					return true;									// string is part of the port
				}
				count++;
			}
		}
		pclsObj->Release();
	}
	pEnumeratorn->Release();
	return false;													// string is not part of the port
}