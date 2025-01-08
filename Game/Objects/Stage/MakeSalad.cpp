#include "MakeSalad.h"
#include<algorithm>
#include"../Stage/Components/BehaviorComponents/Component_PlayerBehavior.h"

MakeSalad::MakeSalad() :salad_{}, pArray_{ nullptr }, recipe_{}
{
}

MakeSalad::~MakeSalad()
{
	auto breakableWall = [](Component_PlayerBehavior* player)->PlantData::FuncValue
		{
			PlantData::FuncValue ret;

			ret.amount = -1;
			ret.time = -1;
			ret.isUsable = false;

			player->SetBreakableWall(true);

			return ret;
		};

	specialMenuFuncs.push_back(breakableWall);
}

void MakeSalad::SetRecipeDatum(type p, int index)
{
	index %= NEED_PLANT_NUM;

	recipe_[index] = p;
}

MakeSalad::type MakeSalad::GetRecipeDatum(int index)
{
	index %= NEED_PLANT_NUM;

	return recipe_[index];
}

std::array<MakeSalad::type, MakeSalad::NEED_PLANT_NUM> MakeSalad::GetRecipeData()
{
	return recipe_;
}

void MakeSalad::SetArray(typeArray* arr)
{
	pArray_ = arr;
}

void MakeSalad::Make()
{
	for (auto itr : specialMenu_) {
		auto& [menu , index] = itr;
		if (MakeSpecialMenu({menu,specialMenuFuncs[index]})) return;
	}

	salad_.effect_0 = PlantData::GetFunction(recipe_[0].id_);
	salad_.effect_1 = PlantData::GetFunction(recipe_[1].id_);
	salad_.effect_2 = PlantData::GetFunction(recipe_[2].id_);
}

Salad MakeSalad::GetSalad()
{
	return salad_;
}

void MakeSalad::DrawData()
{
}

void MakeSalad::Save(json& _saveObj)
{
	for (auto i = 0u; i < specialMenu_.size(); ++i) {

		auto&[menu, func] = specialMenu_[i];
		_saveObj["specialMenu"][i]["0"] = menu[0].id_;
		_saveObj["specialMenu"][i]["1"] = menu[1].id_;
		_saveObj["specialMenu"][i]["2"] = menu[2].id_;
		_saveObj["FuncIndex"][i] = func;
	}
}

void MakeSalad::Load(json& _loadObj)
{
	Delete();

	int index = 0;
	while (true) {

		if(_loadObj["specialMenu"][index].is_null()) break;

		std::tuple< Recipe, SPECIAL_FUNC> menu =
		{
			{
				
			},
			static_cast<SPECIAL_FUNC>(_loadObj["FuncIndex"][index].get<int>())
		};

		auto& [recipe,func] = menu;
		recipe[0].id_ = _loadObj["specialMenu"][index]["0"];
	    recipe[1].id_ = _loadObj["specialMenu"][index]["1"];
		recipe[2].id_ = _loadObj["specialMenu"][index]["2"];

		specialMenu_.push_back(menu);

		++index;
	}
}

void MakeSalad::Add()
{
	std::tuple< Recipe, SPECIAL_FUNC> menu
		=
	{
		{
		},
		BREAKABLE_WALL
	};
	
	specialMenu_.push_back(menu);
}

void MakeSalad::Delete(int index)
{
	specialMenu_.erase(specialMenu_.begin() + index);
}

void MakeSalad::Delete()
{
	specialMenu_.clear();
}

bool MakeSalad::MakeSpecialMenu(std::tuple<Recipe, std::function<PlantData::FuncValue(Component_PlayerBehavior*)>> specialMenu)
{
	auto&[menu, func] = specialMenu;
	auto tempRecipe = recipe_;

	auto sortAlg = [](MakeSalad::type &left, MakeSalad::type& right)-> bool
		{
			
			return left.id_ < right.id_;
		};

	std::sort(menu.begin(), menu.end(),sortAlg);
	std::sort(tempRecipe.begin(), tempRecipe.end(),sortAlg);

	if (tempRecipe[0].id_ == menu[0].id_ &&
		tempRecipe[1].id_ == menu[1].id_ &&
		tempRecipe[2].id_ == menu[2].id_)
	{
		salad_.effect_0 = func;
		salad_.effect_1 = PlantData::GetFunction(-1);
		salad_.effect_2 = PlantData::GetFunction(-1);

		return true;
	}

	return false;
}
