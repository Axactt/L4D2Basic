#ifndef EXTRA_H
#define EXTRA_H

namespace extra 
{

	struct Choices
	{
		bool showTeamMates{ false };
		bool snapLines{ false };
		bool box2D{ false };
		bool status2D{ false };
		bool statusText{ true };
		bool statusTextTeam{ false };
		bool box3D{ true };
		bool headLineEsp{ false };
		bool rcsCrossHair{ true };
		bool statusTextEntity{ true };
		bool velocityEsp{ true };
	};
	inline Choices g_choices{};

	//inline std::string get_entity_name
}



#endif // !CHOICES_H

