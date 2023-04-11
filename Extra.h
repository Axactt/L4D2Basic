#ifndef EXTRA_H
#define EXTRA_H

namespace extra 
{

	struct Choices
	{
		bool showTeamMates{ false };
		bool snapLines{ true };
		bool box2D{ false };
		bool status2D{ false };
		bool statusText{ true };
		bool statusTextTeam{ false };
		bool statusTextEntity{ true };
		bool box3D{ false };
		bool velEsp{false };
		bool headLineEsp{ false };
		bool rcsCrossHair{ true };

	};
	inline Choices g_choices{};

	//inline std::string get_entity_name
}



#endif // !CHOICES_H

