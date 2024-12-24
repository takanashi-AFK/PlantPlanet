#pragma once
#include<functional>

class Component_PlayerBehavior;

struct Salad
{
	std::function<bool(Component_PlayerBehavior*)> effect_0;
	std::function<bool(Component_PlayerBehavior*)> effect_1;
	std::function<bool(Component_PlayerBehavior*)> effect_2;
};

