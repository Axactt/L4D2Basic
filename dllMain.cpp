#include"Includes.h"
#include"dxStuff.h"
#include"drawLogic.h"
#include"Methods.h"
#include<thread> //! Include to create a thread function from Lambda for which we willw ait upon till completion

//!New global variable made for viewProjMatrix[16] as inside player class it was overriding other offsets
//!This was causing weird bug of boomer sticker appearing
float g_viewProjMatrix[16]{};

DWORD WINAPI myThreadProc(HMODULE hInstDLL)
{
	AllocConsole(); // To allocate console for logging
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);

	LocalPlayer* localPlayerBaseAddress = LocalPlayer::getLocalPlayerPtr();
	EntityListInstance* entityListBaseAddress = EntityListInstance::getEntityListInstancePtr();

	std::cout << "LocalPlayer Base-Address:\t" << std::hex << (ptrdiff_t)localPlayerBaseAddress << '\n';
	std::cout << "EntityList Base-Address:\t" << std::hex << (ptrdiff_t)entityListBaseAddress << '\n';
	std::cout << "Closest enemy address is:\t" << std::hex << (ptrdiff_t)entityListBaseAddress->GetClosestEnemy() << '\n';
	std::cout << "The view angles pointer editable is: " << localPlayerBaseAddress->getViewAnglesPtr() << '\n';


	std::cout << " CEngineTraceClient base address: " << std::hex << (ptrdiff_t)CEngineTraceClient::instance();
	std::cout << " Trace-ray game function: " << std::hex << CEngineTraceClient::lpOrigTraceRayAddress;

	//!To Hook endScene() function of Direct3d9 where various loops execute
	DirectXStuff::dxstfInstance()->getEndSceneHooked();

	while (!GetAsyncKeyState(VK_END) & 1)
	{

		localPlayerBaseAddress->updateMatrixButtons(); // This has to causing the bug
		/*
		if (GetAsyncKeyState( VK_F1 ) & 1)
		{
			for (int i{ 0 }; i < 16; ++i)
			{
				if (i % 4 == 0)
					std::cout << '\n';
				std::cout <<g_viewProjMatrix[i] << '\t';
			}
			std::cout << '\n' << '\n';
		}*/

		Sleep(1);
	}

	//unload of Dll and reource deallocation code.
	//xg_HnP.unhook<7>((char*)DirectXStuff::dxstfInstance()->lpOriginalFuncAddress);

	//unload of Dll and reource deallocation code.
	//! std::thread include to convert unHook code as a thread function
	//! We will wait upon this thread function till it finishes . so that The Game does not crash when Dll is uninjected
	std::thread myWaitThreadFunction { g_HnP.unHook,(char*)DirectXStuff::dxstfInstance()->lpOriginalFuncAddress,7 };

	//! Std::thread ::join() to brute force wait upon execution of thread unHook function
	myWaitThreadFunction.join();

	Sleep(2000);

	if (f)
		fclose( f );
	FreeConsole();
	FreeLibraryAndExitThread( hInstDLL, 0 );//to call DLL_PROCESS_DETACH and resource deallocation
	//xSleep(3000);
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