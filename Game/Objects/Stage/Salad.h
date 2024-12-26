#pragma once
#include<functional>
#include"../../Plants/Plant.h"

class Component_PlayerBehavior;

struct Salad
{
	std::function<PlantData::FuncValue(Component_PlayerBehavior*)> effect_0;
	std::function<PlantData::FuncValue(Component_PlayerBehavior*)> effect_1;
	std::function<PlantData::FuncValue(Component_PlayerBehavior*)> effect_2;
};

