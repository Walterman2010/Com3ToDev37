
// Aufrufen externer Programme
// links:
// https://www.codeproject.com/Tips/333559/CreateProcess-and-wait-for-result
// https://stackoverflow.com/questions/57036329/how-to-run-an-exe-with-multiple-arguments-with-createprocessw-in-c


// How To, mit den auskommentieren wir das obere Prog ent kommentiert und das untere Kommentiert
/*

// diese Programm muss nach den Kompelieren in "test.exe" umbenannt werden und in den Pfad:
// C:\\Users\\ecrhwincc\\source\\repos\\testProject\\Debug\\ verschoben werden, bzw der Pfad 
// im folgeprogramm muss angepasst werden
// dieses Programm erhält von volgeprogramm Argumente und zeigt beispielhaft wie diese 
// gehandhabt werden können


#include <iostream>
#include <string> 
using namespace std;

int main(int argc, char *argv[]) { 
	// argc anzahl der Argumente
	// argv char Array mit argumenten. Wenn eins aus mehreren Ziffern besteht, ist das array mehrdimensional

	if (argc > 0) {
		// zeigt das handhaben mehrdimensionaler Char Array
		std::cout << "jojojo: " << argv[0][0] <<" "<< argv[0][1] << std::endl;
	}
	int tow = 0;
	if (argc == 2) { // wenn genau 2 argumente übergeben wurden
		// einfaches umwandelt eines Char in Int, eine stelle (0-9)
		tow = *argv[1] - 48;
	}

	// handhaben von zahlen Argumenten, mit mehreren stellen
	string ein;
	ein = ein + argv[0][0] + argv[0][1];	// umwandeln char in String
	int inputa = stoi(ein);					// umwandeln string in Int

	// bisn Tüdlölü
	std::cout << "inputa: " << inputa << std::endl;
	std::cout << "1 + 2: " << inputa + tow  << std::endl;

	//entsprechende Return Codes generieren
	if (inputa == 23) {
		return 42;
	}
	return 3;
}


/*/

// Programm 2: dieses Programm ruft das test.exe Programm auf, übergibt Argumente
// wartet bis die exe ausgeführt wurde und erhält return codes 

#include <windows.h> 
#include <iostream>
#include <string>	// für Möglichkeit 1

using namespace std;

int main(void)
{
	// Procedure Stuff
	PROCESS_INFORMATION pif;		//Gives info on the thread and..
									//..process for the new process
	STARTUPINFO si;					//Defines how to start the program
	ZeroMemory(&si, sizeof(si));	//Zero the STARTUPINFO struct
	si.cb = sizeof(si);				//Must set size of structure



	int dazu = 23;
	cout << "Zu welchen COM Port suchen Sie die Device ID?" << endl;
	cin >> dazu;
	
		// erste Möglichkeit 
		std::wstring arg = L"";
		arg = arg + std::to_wstring(dazu);

		CONST wchar_t* input = arg.c_str();		// Argumente die an test Programm übergeben werden

		BOOL bRet = CreateProcess(
			//TEXT("C:\\Users\\ecrhwincc\\source\\repos\\Com3ToDev37\\x64\\Release\\Com3ToDev37.exe"), //Path to executable file, need to be a full path 
			TEXT("C:\\Users\\User\\Documents\\ArbeitArbeit_sprach_der_Ork\\ComeToDev\\com3todev37\\Com3ToDev37\\x64\\Release\\Com3ToDev37.exe"),
			(LPWSTR)input,   //Command string - not needed here
			NULL,   //Process handle not inherited
			NULL,   //Thread handle not inherited
			FALSE,  //No inheritance of handles
			0,      //No special flags
			NULL,   //Same environment block as this prog
			NULL,   //Current directory - no separate path
			&si,    //Pointer to STARTUPINFO
			&pif);   //Pointer to PROCESS_INFORMATION

		if (bRet == FALSE)
		{
			// Abbruch wenn es nicht funktioniert hat
			return 1;
		}


		// Funktion wartet bis die Exe beendet wurde
		WaitForSingleObject(pif.hProcess, INFINITE);


		// return Values von Process bekommen
		int result;				// bekommen hat funktioniert wenn = 1 obwohl so ganz stimmt das auch nicht
								// man bekommt auch eins wenn nicht auf die exe gewartet wurde
		DWORD exitCode;			// var in dem der Return Code der Exe abe
		result = GetExitCodeProcess(pif.hProcess, &exitCode);//&exitCode

		std::cout << "die entsprechende Device ID ist: " << exitCode << std::endl;


		CloseHandle(pif.hProcess);   //Close handle to process
		CloseHandle(pif.hThread);    //Close handle to thread
	
	Sleep(2000);

	return 0;
}

