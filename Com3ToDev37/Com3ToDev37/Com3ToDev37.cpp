/* Com3ToDev37.cpp : This file contains the 'main' function. Program execution begins and ends there.
1. get and transform the ComPort
2. Dev ID list with path 
3. Com ports list with path 
4. connect DevID and ComPort 
5. Return the Device ID for the searched Com Port
*/

#include <iostream>
#include <string> 
#include "DeviceFinder.h"
#include <comutil.h>


using namespace std;

int main(int argc, char *argv[])
{
//	argv[0][0] = '0';
//	argv[0][1] = '6';




	cout << "____________________________________________________________" << endl;
	//cout << argc << " - " << *argv << endl;
	//--------------------------------------------------------------------------------------
	// 1. get and transform the ComPort
	//--------------------------------------------------------------------------------------

	if (argc == 0|| argc >= 2){
		// cancel if we have less or more than one member
		cout << "fail in argument, only one Member aloud!\n";
	}

	// transform the char input in INT
	string sCom;
	sCom = sCom + argv[0][0] + argv[0][1];		// transform char in String
	int qCom = stoi(sCom);						// transform string in Int // question Com (erfragt)
	cout << "search the Video Device ID for COM " << qCom << " ..." << endl;

	


	cout << "------------------------------------------------------------" << endl;
	//--------------------------------------------------------------------------------------
	// 2. Dev ID list with path 
	//--------------------------------------------------------------------------------------
	cout << "search first Device IDs" << endl << endl;
	
	DeviceEnumerator de;			// def ennumerator funktions
	std::map<int, Device> devices = de.getVideoDevicesMap();	// find all Video Devices
	findDev stufffind;				// Definition of header funktions
	std::string PNPdev;				// Path value
	Device gotDevID[10];			// var to save all values of the Devices
	string Identus;					// to save a spezifik part of the path
	for (int i = 0; i < 10; i++) {	// reset ID off all values
		gotDevID[i].id = 666;		// an value to break if we have no Video Device
	}
	int count = 0;					// counter = enummerate the array
	// Print information about the devices
	for (auto const &device : devices) {
		Identus = "";											// reset the identus
		PNPdev = device.second.devicePath;						// get the Path									
		Identus = stufffind.findINN(PNPdev, "_4007&", 15, 6);	// get the Ident from the Path
		gotDevID[count].Ident = Identus;						// save the Dev Name and define the space, because get overwriten later
		gotDevID[count].deviceName = device.second.deviceName;	// get Device Name
		gotDevID[count].devicePath = device.second.devicePath;	// get Device Path
		gotDevID[count].id = device.first;						// get the Device ID
		
		for (int i = 0; i <= Identus.length(); i++) {			// change all Kapital Letter to smal latte, for better comparing (later)
			gotDevID[count].Ident[i] = tolower(Identus[i]);		// save the Dev Name
		}
		cout << "DevNo: " << gotDevID[count].id << " DevName: " << gotDevID[count].deviceName << " Ident: " << gotDevID[count].Ident << endl;
		count++;
	}
	cout << "------------------------------------------------------------" << endl;
	//--------------------------------------------------------------------------------------
	// 3. Com ports list with path 
	//--------------------------------------------------------------------------------------
	cout << "search COM Ports..." << endl << endl;
	
	IEnumWbemClassObject* pEnumeratorn = stufffind.FindDevice();	// list all Ports in enumerator
	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;

	std::string identify;	// declare Iden var
	Device gotDevCom[256];	// var to save all values of the Com Ports
	count = 0;				// reset counter
	string found = "COM";	// to search in all the Ports, that with COM
	string comNum = "";		// to internal save the Com Port value
	int zaehler = 0;
	while (pEnumeratorn)	// repeat for all Port devices
	{	
		zaehler++;
		HRESULT hr = pEnumeratorn->Next(WBEM_INFINITE, 1,&pclsObj, &uReturn); //get next Port Device
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

			const char *isNum = comNum.c_str();						// need to translate to char, for checking the Com is a number and not the part of a name

			if (isdigit(*isNum)) {									// check the com number is a number and noch any letter
				gotDevCom[count].ComPort = stoi(comNum);			// save the COM Port

				VariantClear(&vtProp);									// reset the Prop
				hr = pclsObj->Get(L"PNPDeviceID", 0, &vtProp, 0, 0);	// choose an other Prop Object to get the Path
				PNPdev = _bstr_t(vtProp.bstrVal, false);				// get the Path value

				gotDevCom[count].devicePath = PNPdev;					// save the Path into the array
				identify = stufffind.findINN(PNPdev, "_4007&", 15, 6);	// get the Ident from the Path
				gotDevCom[count].Ident = identify;						// make space for the Path in the var

				for (int i = 0; i <= identify.length(); i++) {			// change all Kapital Letter to smal latte, for better comparing (later)
					gotDevCom[count].Ident[i] = tolower(identify[i]);	// save char for char
				}
				cout << " Name: " << gotDevCom[count].deviceName << " COM: " << gotDevCom[count].ComPort << " Ident : " << gotDevCom[count].Ident << endl;
				VariantClear(&vtProp);									// reset the Prop
				count++;
			}			
		}
		pclsObj->Release();
	}
	pEnumeratorn->Release();
	cout << "------------------------------------------------------------" << endl;
	//--------------------------------------------------------------------------------------
	// 4. connect DevID and ComPort 
	//--------------------------------------------------------------------------------------
	int ReturnDevID = 666;								// create the Return value 666 is the fail code
	// remind: -qCom- is the Int value; -sCom- is the String value

	for (int i = 0; i <= 255; i++) {					// repate for all posible ports
		if (gotDevCom[i].ComPort == qCom) {		// if we have a Port = input value
			for (int j = 0; j <= 10; j++) {				// repeat for all posible devices
				if (gotDevCom[i].Ident == gotDevID[j].Ident && gotDevID[j].id != 666) {
														// if Port and Dev Ident simular & DevID existing
					ReturnDevID = gotDevID[j].id;		// write Dev ID to return
					break;								//-down	we got what we want
				}
			}
			break;										//-down	we got what we want
		}
	}
	//--------------------------------------------------------------------------------------
	// 5. Return the Device ID for the searched Com Port
	//--------------------------------------------------------------------------------------
	if (ReturnDevID == 666) {
		cout << "No Viedeo Device ID for Com Port "<< sCom <<" found!"<<endl;
	}
	else {
		cout << "The Video Device ID for the COM Port: " << qCom << " is: " << ReturnDevID<<endl;
	}

	cout << "thank you for choosing Com3toDev37" << endl<<"by AL 24.08.21"<<endl;
	cout << "____________________________________________________________" << endl;
	return ReturnDevID;
	//--------------------------------------------------------------------------------------
}

