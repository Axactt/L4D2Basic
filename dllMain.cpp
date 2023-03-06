#include"Includes.h"

 
void setThirdPerson(bool bThirdPerson )
{
	CCSInput_base* pCCSInputBase = g_memEdit.get_THIS_frm_ptr2ObjBaseAdrs<CCSInput_base>( engineBase + pCCSInput_base );

	if (GetAsyncKeyState( VK_INSERT ) & 1)
	{
		
		bThirdPerson = !bThirdPerson;
		if (bThirdPerson)
		{
			DWORD oProc;
			VirtualProtect((LPVOID)pCCSInputBase, 0xFC, PAGE_EXECUTE_READWRITE, &oProc );
			pCCSInputBase->m_fCameraInThirdPerson = true;
			VirtualProtect( (LPVOID) pCCSInputBase, 0xFC, oProc, &oProc );
		}
		else
		{
			DWORD oProc;
			VirtualProtect( (LPVOID) pCCSInputBase, 0xFC, PAGE_EXECUTE_READWRITE, &oProc );
			pCCSInputBase->m_fCameraInThirdPerson = false;
			VirtualProtect( (LPVOID) pCCSInputBase, 0xFC, oProc, &oProc );
		}
	}


}



DWORD WINAPI myThreadProc( HMODULE hInstDLL )
{
	AllocConsole(); // To allocate console for logging
	FILE* f;
	freopen_s( &f, "CONOUT$", "w", stdout );
	LocalPlayer* localPlayerBaseAddress = LocalPlayer::getLocalPlayerPtr();
	EntityListInstance* entityListBaseAddress = EntityListInstance::getEntityListInstancePtr();
	CCSInput_base* pCCSInputBase = g_memEdit.get_THIS_frm_ptr2ObjBaseAdrs<CCSInput_base>( clientBase + pCCSInput_base );
	std::cout<<"LocalPlayer Base-Address:\t" << std::hex << (ptrdiff_t) localPlayerBaseAddress<<'\n';
	std::cout <<"EntityList Base-Address:\t" << std::hex << (ptrdiff_t) entityListBaseAddress<<'\n';
	std::cout << "Closest enemy address:\t " << std::hex << (ptrdiff_t) entityListBaseAddress->GetClosestEnemy()<<'\n';
	std::cout << "View angle address:\t" << localPlayerBaseAddress->getViewAnglesPtr() << '\n';
	std::cout << "This pointer of  CCInput_base:\t" << std::hex << (ptrdiff_t) pCCSInputBase << '\n';
	
	//std::cout << "Toggle Insert-key to set third-person- mode On/off.\n";
	bool bThirdPerson = false;

	while (!GetAsyncKeyState( VK_END ) & 1)
	{
		//MAIN GAME LOOP WHICH RUNS WITH EVERY FRAME
		//localPlayerBaseAddress->aimAt( entityListBaseAddress->targetEntityVec() );
		//std::cout << "Offset_Pointer to m_fCameraInThirdPerson:\t" <<  (g_memEdit.get_THIS_frm_ptr2ObjBaseAdrs<CCSInput_base>( clientBase + pCCSInput_base )->m_fCameraInThirdPerson) << '\n';
		if (GetAsyncKeyState( VK_INSERT ) & 1)
		{

			bThirdPerson = !bThirdPerson;
			if (bThirdPerson)
			{	
				pCCSInputBase->m_fCameraInThirdPerson = true;
			}
			else
			{
				pCCSInputBase->m_fCameraInThirdPerson = false;
			}
		}
		std::cout << "Current state of m_fCameraInThirdPerson: " << pCCSInputBase->m_fCameraInThirdPerson << '\n';
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