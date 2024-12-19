#include "Plant.h"
#include"../Objects/Stage/Components/BehaviorComponents/Component_PlayerBehavior.h"

bool PlantData::Effect(Component_PlayerBehavior* pb)
{
    GetFunction(this->id_)(pb);
}

std::function<bool(Component_PlayerBehavior*)> PlantData::GetFunction(int id)
{
	switch (id)
	{
	case 0: return [](Component_PlayerBehavior* pb)->bool
		{

		};


	default :return[](Component_PlayerBehavior* pb)->bool{return false;};
	}
}
