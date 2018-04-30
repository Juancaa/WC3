
#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <Tlhelp32.h>
#include <fstream>


using namespace std;

// Debug Priviledges.
void EnableDebugPriv( ){
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;
	OpenProcessToken( GetCurrentProcess( ), TOKEN_ADJUST_PRIVILEGES |TOKEN_QUERY, &hToken );
	LookupPrivilegeValue( NULL, SE_DEBUG_NAME, &sedebugnameValue );
	tkp.PrivilegeCount = 1;tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges( hToken, false, &tkp, sizeof( tkp ), NULL, NULL );
	CloseHandle( hToken );
 }

int main(){
    cout <<"WC3 CDKEY GRABBER\n" << endl;
    EnableDebugPriv();

	TCHAR War3Name[32] = TEXT("Warcraft III");
	HWND hWar3 = FindWindow(War3Name, NULL);

	if(!hWar3){
		cout << "WC3 NOT FOUND." << endl;
		system("pause");
		return 1;
	}

	DWORD pid;
	GetWindowThreadProcessId( hWar3, &pid );
	HANDLE hOpen = OpenProcess( PROCESS_ALL_ACCESS, false, pid );
	if( !hOpen ){
		cout << "Can't open Warcraft III process." << endl;system( "pause" );
		return 1;
	}

	// Finding the cd-key's address.
	DWORD Address = 112;
	DWORD Address2 = 112;
	DWORD Buffer = 0;
	DWORD Buffer2 = 0;

	DWORD WINAPI GetLastError(void);
	SIZE_T BytesRead = 0;
	while (true){
		ReadProcessMemory(hOpen, (LPCVOID)Address, &Buffer, 4, &BytesRead);
		if(Buffer == 1766204479){
			Address += 48;// ROC Key
			break;
		} else {
			Address += 65536;
		}
	}

	while (true){
		ReadProcessMemory(hOpen, (LPCVOID)Address2, &Buffer2, 4, &BytesRead);
		if(Buffer2 == 1766204479){
			Address2 += 80;// TFT Key
			break;
		} else {
			Address2 += 65536;
		}
	}

	// Couting out key.
	char Key1[27];
	Key1[26] = 0;
	char Key2[27];
	Key2[26] = 0;

	for( unsigned int i = 0; i < 26; i++ ){
		ReadProcessMemory( hOpen, (LPVOID)( Address + i ), &Buffer, 1, &BytesRead );
		Key1[i] = Buffer;
		ReadProcessMemory( hOpen, (LPVOID)( Address2 + i ), &Buffer2, 1, &BytesRead );
		Key2[i] = Buffer2;
	}

	//CloseHandle( hOpen );
	cout << "WAR3 Key: ";
	cout << Key1 << endl << endl;
	cout << "TFT Key:  ";
	cout << Key2 << endl << endl;
	cout << "www.d3scene.com";

	system("pause");

}
