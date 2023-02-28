#include"Includes.h"


void printAddress1()
{
	// address of get_local_player function
	class SigScanner sigScan{"client.dll" };
	byte bMask[]{ "\xE8\x00\x00\x00\x00\x8B\xF0\x83\xC4\x00\x85\xF6\x75\x00\x5E\x32\xC0\x5B\x8B\xE5" };
	char szMask[]{ "x????xxxx?xxx?xxxxxx" };
	std::cout<<"address found for getLocalPlayer() function: "<<std::hex<<sigScan.FindPattern(bMask, szMask).GetAddress()<<'\n';

}
void printAddress2()
{
	// Address of pCClientEntityList i.e ptr to CClientEntityLIst
	class SigScanner sigScan
	{
		"client.dll"
	};
	byte bMask[]{ "\x8B\x0D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x85\xC0\x74\x00\x8B\x90" };
	char szMask[]{ "xx????x????xxx?xx" };
	std::cout << "address  for location containing ptr to CClinetEntitylist class: " << std::hex << sigScan.FindPattern( bMask, szMask ).add(2).GetAddress() << '\n';
	std::cout << "Offset-address::ptr to CClinetEntitylist class: " << std::hex << sigScan.FindPattern( bMask, szMask ).GetOffsetAddress32(2)<<'\n';
}

DWORD WINAPI myThreadProc( HMODULE hInstDLL )
{
	AllocConsole(); // To allocate console for logging
	FILE* f;
	freopen_s( &f, "CONOUT$", "w", stdout );
	printAddress1();
	printAddress2();
	while (!GetAsyncKeyState( VK_END ) & 1)
	{
		//MAIN GAME LOOP WHICH RUNS WITH EVERY FRAME

		Sleep( 1 );

	}
	//unload of Dll and reource deallocation code.
	fclose( f );
	FreeConsole();
	FreeLibraryAndExitThread( hInstDLL, 0 );//to call DLL_PROCESS_DETACH and resource deallocation
	return 0;
}

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:

		::DisableThreadLibraryCalls( hinstDLL );
		CreateThread( nullptr, 0, LPTHREAD_START_ROUTINE( myThreadProc ), hinstDLL, 0, nullptr );
		break;

		case DLL_PROCESS_DETACH:
		//perform any necesary clean-up
		break;

	}
	return TRUE; //Succesful DLL_PROCESS_ATTACH

}