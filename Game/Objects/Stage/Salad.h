#pragma once
#include<functional>
#include"../../../Game/Objects/Stage/Components/BehaviorComponents/Component_PlayerBehavior.h"

struct Salad
{
	std::function<void(Component_PlayerBehavior* pb)> effect_0;
	std::function<void(Component_PlayerBehavior* pb)> effect_1;
	std::function<void(Component_PlayerBehavior* pb)> effect_2;
};

