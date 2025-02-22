#include "MakeSalad.h"
#include<algorithm>
#include"../Stage/Components/BehaviorComponents/Component_PlayerBehavior.h"
#include "../../../Engine/ImGui/imgui.h"
#include<format>

static int plantIDforSpecialFunc = 0;

MakeSalad::MakeSalad() :salad_{}, pArray_{ nullptr }, recipe_{}
{
	auto breakableWall = [&](Component_PlayerBehavior* player)->PlantData::FuncValue
		{
			PlantData::FuncValue ret;

			ret.amount = -1;
			ret.time = -1;
			ret.isUsable = false;
			ret.id = plantIDforSpecialFunc;
			ret.filePath = "Images/SaladEffectLogo/NONE.png";
			ret.specialText = "Breakable Wall!";
			player->SetBreakableWall(true);

			return ret;
		};

	specialMenuFuncs.push_back(breakableWall);
}

MakeSalad::~MakeSalad()
{
	
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
	for (auto i = 0u; i < specialMenu_.size(); ++i) {
		if (ImGui::TreeNode(std::format("Recipe:{}", i).c_str()))
		{
			auto& [recipe,func] = specialMenu_[i];

			auto funcValue = static_cast<int>(func);
			
			ImGui::DragInt("Recipe 0", &recipe[0].id_);
			ImGui::DragInt("Recipe 1", &recipe[1].id_);
			ImGui::DragInt("Recipe 2", &recipe[2].id_);
			ImGui::DragInt("Function", &funcValue);
			
			funcValue = std::clamp(funcValue, 0, static_cast<int>(specialMenuFuncs.size()) - 1);
			func = static_cast<SPECIAL_FUNC>(funcValue);

			if(ImGui::Button("Delete"))	Delete(i);
			
			ImGui::TreePop();
		}
	}
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
		plantIDforSpecialFunc = menu[0].id_;
		salad_.effect_0 = func;
		salad_.effect_1 = [=](Component_PlayerBehavior* pb)mutable->PlantData::FuncValue
			{
				PlantData::FuncValue ret;
				ret.isUsable = false;
				ret.time = -1;
				ret.id = tempRecipe[1].id_;
				ret.filePath = "Models/tentativeFlowers/BlankFlowerImage.png";
				ret.amount = -1;
				ret.specialText = "1";
				return ret;
			};
		salad_.effect_2 = [=](Component_PlayerBehavior* pb)mutable->PlantData::FuncValue
			{
				PlantData::FuncValue ret;
				ret.isUsable = false;
				ret.time = -1;
				ret.id = tempRecipe[2].id_;
				ret.filePath = "Models/tentativeFlowers/BlankFlowerImage.png";
				ret.amount = -1;
				ret.specialText = "1";
				return ret;
			};

		return true;
	}

	return false;
}
