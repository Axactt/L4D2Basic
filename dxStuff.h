#pragma once
#include"Windows.h"
#include"drawLogic.h"
#include"UTILITIES/HookTemplate.h"
#include"Player.h"
#include"Methods.h"
#include"Extra.h"
#include<string>
#include<sstream>

class DirectXStuff
{
private:
	DirectXStuff() = default;
public:

	using aliasEndScene = HRESULT( __stdcall* )(IDirect3DDevice9*);
	static inline aliasEndScene EndScenePtr{  };

	using entityNameFuncAlias = char* (__thiscall*)(void* pECX);
	static inline entityNameFuncAlias entityNameFuncPtr = (entityNameFuncAlias) (SigFunctor{}("client.dll", "\x56\x8B\xF1\x8B\x06\x8B\x90\x00\x00\x00\x00\xFF\xD2\x50\xE8\x00\x00\x00\x00\x83\xC4\x00\x84\xC0\x74\x00\x8B\xB6", "xxxxxxx????xxxx????xx?xxx?xx").GetAddress());

	ptrdiff_t lpOriginalFuncAddress{};
	// interface pointer to IDirect3DDevice9
	IDirect3DDevice9* pDevice{};

	static DirectXStuff* dxstfInstance()
	{
		static DirectXStuff dxstuff;
		return &dxstuff;
	}

	// get the size of GameWindow
	static  Vector2 getWindowSize()
	{
		//get HWND of Game window
		HWND gameWindow = FindWindowA( NULL, "Left 4 Dead 2 - Direct3D 9" );
		float gameWindowWidth{};
		float gameWindowHeight{};
		RECT rect{};
		if (GetWindowRect( gameWindow, &rect ))
		{
			gameWindowWidth = rect.right - rect.left;
			gameWindowHeight = rect.bottom - rect.top;
			//adjustment
			gameWindowWidth -= 5;
			gameWindowHeight -= 29;
		}
		return { gameWindowWidth,gameWindowHeight };

	}
	// A separate variable for window-size created
	// apart from global variable to be used inside class
	static inline Vector2 windowSize{ getWindowSize() };


	// Create our hook function having prototype as Endscene
	// FIX THIS LATER== TOO MUCH STATIC
	//hookEndScene member function has to be made static for being called by hook inside its own class
	static HRESULT __stdcall hookEndScene( IDirect3DDevice9* pDevice )
	{
		//?Trace-Ray called here to have same Thread Local storage as game thread calling trace-ray function
		CEngineTraceClient::instance()->traceRayHook();

		//drawing text stuff
		//xThis has to be fixed : Not working OK
		if (extra::g_choices.statusText)
		{
			DrawTextEsp( pDevice, "First copied Esp by ReVirus. More original stuff to follow!", windowSize.m_x / 2, windowSize.m_y - 20, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}

		//drawing menu with controls

		int menuOffsetX = windowSize.m_x / 2;
		int menuOffsetY = 50;
		D3DCOLOR enabled = D3DCOLOR_ARGB( 255, 0, 255, 0 );
		D3DCOLOR disabled = D3DCOLOR_ARGB( 255, 255, 0, 0 );

		if (!extra::g_choices.showMenu) //if menu key is disabled only this will show
		{
			DrawTextEsp( pDevice,"Show Menu (INS)", menuOffsetX, menuOffsetY, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}
		else
		{
			//Menu items to show when menu key is enabled
			DrawTextEsp( pDevice, "Show Menu (INS)", menuOffsetX, menuOffsetY + 0 * 12, extra::g_choices.showMenu ? enabled : disabled );
			DrawTextEsp( pDevice, "Show snapLines (F1)", menuOffsetX, menuOffsetY +1 * 12, extra::g_choices.snapLines ? enabled : disabled );
			DrawTextEsp( pDevice, "Show box2D (F2)", menuOffsetX, menuOffsetY + 2 * 12, extra::g_choices.box2D ? enabled : disabled );
			DrawTextEsp( pDevice, "Show statusText (F3)", menuOffsetX, menuOffsetY + 3 * 12, extra::g_choices.statusText ? enabled : disabled );
			DrawTextEsp( pDevice, "Show statusTextTeam (F4)", menuOffsetX, menuOffsetY + 4 * 12, extra::g_choices.statusTextTeam ? enabled : disabled );
			DrawTextEsp( pDevice, "Show box3D (F5)", menuOffsetX, menuOffsetY + 5 * 12, extra::g_choices.box3D ? enabled : disabled );
			DrawTextEsp( pDevice, "Show headlineEsp (F6)", menuOffsetX, menuOffsetY + 6 * 12, extra::g_choices.headLineEsp ? enabled : disabled );
			DrawTextEsp( pDevice, "Show rcsCrossHair (F7)", menuOffsetX, menuOffsetY + 7 * 12, extra::g_choices.rcsCrossHair ? enabled : disabled );
			DrawTextEsp( pDevice, "Show statusTextEntity(F8)", menuOffsetX, menuOffsetY + 8 * 12, extra::g_choices.statusTextEntity ? enabled : disabled );
			DrawTextEsp( pDevice, "Show velocityEsp(F9)", menuOffsetX, menuOffsetY + 9 * 12, extra::g_choices.velocityEsp ? enabled : disabled );
			DrawTextEsp( pDevice, "Hide Menu (INS)", menuOffsetX, menuOffsetY + 10 * 12, D3DCOLOR_ARGB(255,255,255,255 ));
		}



		if (extra::g_choices.rcsCrossHair)
		{
			//DrawLine( pDevice, src, dst,  width,  antialias, D3DCOLOR color );
			// all drawing stuff goes here
			// Drawing custom Crosshair for check

			Vector2 crossHair2d{};
			crossHair2d.m_x = g_windowSize.m_x / 2;
			crossHair2d.m_y = g_windowSize.m_y / 2;

			Vector2 l{};
			Vector2 r{};
			Vector2 t{};
			Vector2 b{};

			l = r = t = b = crossHair2d; // point to representing crosshair
			l.m_x -= 4;
			r.m_x += 4;
			b.m_y += 4;
			t.m_y -= 4;

			DrawLine( pDevice, l, r, 2, false, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			DrawLine( pDevice, t, b, 2, false, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			//Line3D( pDevice, l, r, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
			//Line3D( pDevice, t, b, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
		}

		//Drawing snapLines to various entities

		//EntityListInstance* entityListAddress = EntityListInstance::getEntityListInstancePtr();
		//LocalPlayer* localPLayerBaseAddress = LocalPlayer::getLocalPlayerPtr();
		//Vector2 entityPoshead2D{};
		//Vector3 entityPoshead3D{};
		//Vector2 entityBottomPos2D{};
		//Vector3 entityBottomPos3D{};

		for (int id{ 0 }; id < 900; ++id)
		{
			LocalPlayer* entity = EntityListInstance::getEntityListInstancePtr()->GetOtherEntity( id );
			if (extra::g_choices.statusTextTeam)
			{
				if (id == 2 || id == 3 || id == 4)
				{
					Vector3 entityLeg3D = entity->vecOrigin;
					Vector2 entityLeg2D{};
					if (W2S( entityLeg3D, entityLeg2D ))
					{
						std::stringstream str1{};
						std::stringstream str2{};
						str1 << entity->health; //! setting stringstream buffer object using insertion(<<) operator
						std::string t1 = "Hp: " + str1.str(); // using .str() function to retrieve the results of stringstream buffer and appending to existing string object
						//!calling get_entity name vtable function for any entity to read name.
						std::string charName2string{ LocalPlayer::getLocalPlayerPtr()->get_entity_name( entity ) }; // creating/converting the returned char* back to string for further opeartion with sstream class
						str2 << charName2string;
						std::string t2 = " Name: " + str2.str();
						char* healthValue = (char*) t1.c_str();
						char* nameType = (char*) t2.c_str();

						DrawTextEsp( pDevice, nameType, entityLeg2D.m_x, entityLeg2D.m_y, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
						DrawTextEsp( pDevice, healthValue, entityLeg2D.m_x, entityLeg2D.m_y + 12, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
					}
				}
			}

			if ((!entity) || (id == 0) || (id == 1) || (entity->iTeamNum != 3) || (entity->isDormant))
				continue;

			Vector3 entityPoshead3D = entity->vecOrigin + entity->m_vecViewOffset;


			if (entityPoshead3D.m_x != 0.0 && entityPoshead3D.m_y != 0.0f && entityPoshead3D.m_z != 0.0f)

			{

				Vector2 dest{ g_windowSize.m_x / 2, g_windowSize.m_y };

				Vector3 entityBottomPos3D = entity->vecOrigin;
				Vector2 entityPoshead2D{};
				if (LocalPlayer::getLocalPlayerPtr()->worldToScreen( entityPoshead3D, entityPoshead2D ))
				{
					//? Line3D( pDevice, entityPoshead2D.m_x, entityPoshead2D.m_y, 1.0f, dest.m_x, dest.m_y, 1.0f, D3DCOLOR_ARGB( 255, 255, 0, 0 ) ); // if use z as 0.0f model become black(not working well)
					

					if (extra::g_choices.snapLines)
					{
						DrawLine( pDevice, entityPoshead2D, dest, 2, false, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
					}
					Vector2 entityBottomPos2D{};
					if (LocalPlayer::getLocalPlayerPtr()->worldToScreen( entityBottomPos3D, entityBottomPos2D ))
					{

						//Drawing a velocity vector of entity
						if (extra::g_choices.velocityEsp)
						{
							Vector3 velocityVec3D = entityBottomPos3D + (entity->mVecVelocity)*0.25f;
							Vector2 velocityVec2D{};
							if (W2S( velocityVec3D, velocityVec2D ))
							{
								DrawLine( pDevice, entityBottomPos2D, velocityVec2D, 2, false, D3DCOLOR_ARGB( 255, 255, 0, 0 ));
								
								//todoDrawFillRect(pDevice, velocityVec2D.m_x-2, velocityVec2D.m_y-2,)
							}
						}




						//If world2screen valid for entity bottom position or vecOrigin draw 2d and 3d esp boxes
						if (extra::g_choices.box2D)
						{
							DrawEspBox2D( pDevice, entityBottomPos2D, entityPoshead2D, 2, false, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
							//DrawEspBox2D( pDevice, entityBottomPos2D, entityPoshead2D, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
						}

						//DrwaEspBox3d placed here as w2s shold br true for entityBottomPos3D
						if (extra::g_choices.box3D)
						{
							DrawEspBox3D( pDevice, entityPoshead3D, entityBottomPos3D, entity->viewAngles, 25, 2, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );


						}
						if (extra::g_choices.headLineEsp)
						{
							Vector3 entityAngles{ entity->viewAngles };
							Vector3 endPoint3D{ entity->TransFormVector        ( entityPoshead3D,entityAngles,60 ) };// 60 is distance to which velocity vector has to be drwan
							Vector2 endPoint2D{};
							 W2S( entityPoshead3D, entityPoshead2D );
							if (W2S( endPoint3D, endPoint2D ))
							{
								DrawLine( pDevice, entityPoshead2D, endPoint2D, 2, false, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
							}
						}
						if (extra::g_choices.statusTextEntity)
						{

							std::stringstream str1{};
							std::stringstream str2{};

							//! setting stringstream buffer object using insertion(<<) operator
							str1 << entity->infected_name_index; 

							//!using .str() function to retrieve the results of stringstream buffer and appending to existing string object
							std::string t1 = "Index: " + str1.str(); 

							//!calling get_entity name vtable function for any entity to read name.
							//!  creating/converting the returned char* back to string for further opeartion with sstream class
							std::string charName2string{ LocalPlayer::getLocalPlayerPtr()->get_entity_name( entity ) }; 

							//! setting stringstream buffer object using insertion(<<) operator
							str2 << charName2string;
							std::string t2 = " Name: " + str2.str();

						//!Converting the string object back to C-style char string using c_str()
						//! before using in DrawTextEsp function to draw 
							char* someValue = (char*) t1.c_str();
							char* nameType = (char*) t2.c_str();

							DrawTextEsp( pDevice, nameType, entityBottomPos2D.m_x, entityBottomPos2D.m_y, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );
							DrawTextEsp( pDevice, someValue, entityBottomPos2D.m_x, entityBottomPos2D.m_y + 12, D3DCOLOR_ARGB( 255, 255, 255, 255 ) );

						}


					}

				}
			}

		}
		return dxstfInstance()->EndScenePtr( pDevice );
	}

	void* FindEndScene()
	
	{
		//get HWND of Game window
		HWND gameWindow = FindWindowA( NULL, "Left 4 Dead 2 - Direct3D 9" );
		if (!gameWindow)
		{
			std::cout << "Game window HWND find failed. Error: " << GetLastError() << '\n';
		}
		//index for endscene func in VTABLe
		constexpr int endsceneIndex{ 42 };
		//index for Present function in Vtable. same for all D3D9.dll
		constexpr int presentIndex{ 17 };
		// function to create a pointer to IDirect3D9 interface 
		//methods of IDirect3D9 interface is used to create MS Direct3d objects and set-up environ
		//This interface includes method for enumerating and retrieving capabilities of device
		//IDirect3D9 interface allows creation of IDirect3DDevice9 objects
		IDirect3D9* pD3D{ nullptr };
		pD3D = { Direct3DCreate9( D3D_SDK_VERSION ) };

		if (!pD3D)
		{
			std::cout << " IDirect3d9 interface pointer not returned.\n";

		}
		/*else
			std::cout << " IDirect3D9* pD3D-->\t" << std::hex << pD3D << '\n';*/

		if (pD3D)
		{
			//Now initialize value for D3DPRESENT_PARAMETRS structure that is used to create Direct3D device
			D3DPRESENT_PARAMETERS d3dpp{};
			d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
			d3dpp.BackBufferCount = 1;
			d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
			d3dpp.hDeviceWindow = gameWindow;
			d3dpp.Windowed = TRUE;
			d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
			d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

			//now use IDirect3D9* interface with createdevice() function
			//this will return address of IDirect3Device9* interface in form of pDevice pointer
			//Various implementation methods can be used from IDirect3DDevice9* interface
			//returns fully working device interface, set to the required display mode
			// and allocated with appropriate back-buffers
			IDirect3DDevice9* pDevice{};
			pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, gameWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pDevice );
			if (pDevice)
			{
				// These two lines get the address of vTable functions present in IDirect3DDevice9 interface
				// first read value of VTable from class pointer pDevice (interface class pointer)
			   // Then add the desired index to read the desired function pointer
				// Then read the value of function address from that found in step(3)
				// cast it to void*
				void* presentAddress = (void*) *(std::intptr_t*) (*(std::intptr_t*) pDevice + presentIndex * sizeof( std::intptr_t ));
				void* endSceneAddress = (void*) *(std::intptr_t*) (*(std::intptr_t*) pDevice + endsceneIndex * sizeof( std::intptr_t ));

				std::cout << " IDirect3DDevice9* pDevice --->\t" << std::hex << pDevice << '\n';
				std::cout << " Present() addrs --->\t" << std::hex << presentAddress << '\n';
				std::cout << " EndScene() addrs --->\t" << std::hex << endSceneAddress << '\n';

				//pDevice->Release(); // release the IDirect3DDevice9* interface pDevice created to prevent memory leaks
				return endSceneAddress;

			}
			//pD3D->Release(); // release the IDirect3D9* interface object pD3D created to prevent memory leaks
		}
		return nullptr;
	}

	void getEndSceneHooked()
	{
		HWND gameWindow = FindWindowA( NULL, "Left 4 Dead 2 - Direct3D 9" );
		if (!gameWindow)
		{
			std::cout << "Game window HWND find failed. Error: " << GetLastError() << '\n';
		}
		else
		{
			//If game-window is found we will find the EndScene address
			//getWindowSize( gameWindow );

			//type-cast it to a EndScene func pointer type-alias to EndScenePtr variable
			EndScenePtr = (aliasEndScene) FindEndScene();

		}
		//This will assign the real-endscene function address to EndSceneptr functionptr variable
		//Now we will use the EndSceneptr holding value of real endscene function 
		// to create a Trampoline-Hook to our hookEndScene

		//src original EndScene function address saved here in a member variable
		lpOriginalFuncAddress = (ptrdiff_t) EndScenePtr;
		ptrdiff_t lpFinalHookaddrs = (ptrdiff_t) &DirectXStuff::hookEndScene;
		//7 is the length of stolen bytes
		if (EndScenePtr) //NULL-POINTER CHECK TO AVOID HOOK TO NULLPTR
			EndScenePtr = (aliasEndScene) g_HnP.trampHook<7>( (char*) lpOriginalFuncAddress, (char*) lpFinalHookaddrs );
	}

};
// Make a separate global variable at last to deal with transfer of value to player.h
//Cannot inlude "dxstuff.h" in "player.h" as it will lead to cyclic redundancy includes
//So a global variable defined here to transfer window-size value
//In "Player.h" extern declaration given 
Vector2 g_windowSize{ DirectXStuff::dxstfInstance()->getWindowSize() };

//inline Vector2 g_windowSize{ DirectXStuff::dxstfInstance()->getWindowSize() };

//USage is in dllmain like call DirectXStuff::dxstfInstance()->getEndSceneHooked()