#ifndef EXTRA_H
#define EXTRA_H

namespace extra 
{

	struct Choices
	{
		bool showMenu{ false };
		//xbool showTeamMates{ false };
		bool snapLines{ false };
		bool box2D{ false };
		//xbool status2D{ false };
		bool statusText{ false };
		bool statusTextTeam{ false };
		bool box3D{ false };
		bool headLineEsp{ false };
		bool rcsCrossHair{ false };
		bool statusTextEntity{ false };
		bool velocityEsp{ false };
	};
	//!inline variable with multiple file includes to be included anywhere
	inline Choices g_choices{}; 

	//One button for each of above menu items to enable disable
	//! we are initializing each memu-button identifier with  particular key_state so it can be used elsewher in code
	struct Buttons
	{
		DWORD showMenuBtn{ VK_INSERT };
		//xbool showTeamMates{ false };
		DWORD snapLinesBtn{ VK_F1 };
		DWORD box2DBtn{ VK_F2 };
		//xbool status2D{ false };
		DWORD statusTextBtn{VK_F3 };
		DWORD statusTextTeamBtn{ VK_F4 };
		DWORD box3DBtn{ VK_F5};
		DWORD headLineEspBtn{VK_F6 };
		DWORD rcsCrossHairBtn{ VK_F7 };
		DWORD statusTextEntityBtn{ VK_F8 };
		DWORD velocityEspBtn{ VK_F9};
	};
	//Another inline variable to be copied 
	inline Buttons g_Buttons{};
}



#endif // !CHOICES_H

