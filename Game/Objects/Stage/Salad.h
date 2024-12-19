#pragma once
#include<functional>
#include"../../../Game/Objects/Stage/Components/BehaviorComponents/Component_PlayerBehavior.h"

struct Salad
{
	std::function<bool(Component_PlayerBehavior*)> effect_0;
	std::function<bool(Component_PlayerBehavior*)> effect_1;
	std::function<bool(Component_PlayerBehavior*)> effect_2;
};

