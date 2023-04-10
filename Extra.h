#ifndef EXTRA_H
#define EXTRA_H

namespace extra 
{

	struct Choices
	{
		bool showTeamMates{ true };
		bool snapLines{ false };
		bool box2D{ false };
		bool status2D{ false };
		bool statusText{ true };
		bool box3D{ true };
		bool velEsp{ true };
		bool headLineEsp{ false };
		bool rcsCrossHair{ true };
	};
	inline Choices g_choices{};
}



#endif // !CHOICES_H

