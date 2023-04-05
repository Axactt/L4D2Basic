#include"Includes.h"
#include"dxStuff.h"
#include"drawLogic.h"
#include"Methods.h"
DWORD WINAPI myThreadProc( HMODULE hInstDLL )
{
	AllocConsole(); // To allocate console for logging
	FILE* f;
	freopen_s( &f, "CONOUT$", "w", stdout );
	LocalPlayer* localPlayerBaseAddress = LocalPlayer::getLocalPlayerPtr();
	EntityListInstance* entityListBaseAddress = EntityListInstance::getEntityListInstancePtr();
	std::cout << "LocalPlayer Base-Address:\t" << std::hex << (ptrdiff_t) localPlayerBaseAddress << '\n';
	std::cout << "EntityList Base-Address:\t" << std::hex << (ptrdiff_t) entityListBaseAddress << '\n';
	std::cout << "Closest enemy address is:\t" << std::hex << (ptrdiff_t) entityListBaseAddress->GetClosestEnemy() << '\n';
	std::cout << "The view angles pointer editable is: " << localPlayerBaseAddress->getViewAnglesPtr() << '\n';
	//std::cout << " The localplayer head bone position is: " << localPlayerBaseAddress->GetBonePosition( 14 ).m_x << ' ' << localPlayerBaseAddress->GetBonePosition( 14 ).m_y << ' ' << localPlayerBaseAddress->GetBonePosition(14).m_z<<'\n';

	std::cout << " CEngineTraceClient base address: " << std::hex << (ptrdiff_t) CEngineTraceClient::instance();
	std::cout << " Trace-ray game function: " << std::hex << CEngineTraceClient::lpOrigTraceRayAddress;

	// const auto dxstuff = DirectXStuff::dxstfInstance();
	DirectXStuff::dxstfInstance()->getEndSceneHooked();

	while (!GetAsyncKeyState( VK_END ) & 1)
	{

		localPlayerBaseAddress->updateMatrix();
		if (GetAsyncKeyState( VK_F1 ) & 1)
		{
			for (int i{ 0 }; i < 16; ++i)
			{
				if (i % 4 == 0)
					std::cout << '\n';
				std::cout << localPlayerBaseAddress->viewProjMatrix[i] << '\t';
			}
			std::cout << '\n' << '\n';
		}
		//localPlayerBaseAddress->aimAt( entityListBaseAddress->targetEntityVec() );  */
		Sleep( 1 );
	}
	//unload of Dll and reource deallocation code.
	g_HnP.unhook<7>( (char*) DirectXStuff::dxstfInstance()->lpOriginalFuncAddress );
	if (f)
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