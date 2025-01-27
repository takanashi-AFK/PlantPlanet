#include<array>
#include<vector>
#include<utility>
#include"Salad.h"
#include "../../Plants/Plant.h"
#include "../../../Engine/Json/JsonReader.h"

class MakeSalad
{
public:

	static constexpr int NEED_PLANT_NUM = 3;
	using typeArray = std::vector<PlantData>;
	using type = typeArray::value_type;

public:

	MakeSalad();
	~MakeSalad();

	void SetRecipeDatum(type p, int index);
	type GetRecipeDatum(int index);
	std::array<type, NEED_PLANT_NUM> GetRecipeData();
	void SetArray(typeArray* arr);
	void Make();
	Salad GetSalad();

	void DrawData();
	void Save(json& _saveObj);
	void Load(json& _loadObj);
	void Add();		// add special menu
	void Delete(int index);  // delete(erase) special menu
	void Delete();  // delete all special menu

private:
	using Recipe = std::array<type, NEED_PLANT_NUM>;

	bool MakeSpecialMenu(std::tuple<Recipe, std::function<PlantData::FuncValue(Component_PlayerBehavior*)>> specialMenu);

	typeArray* pArray_;
	Recipe recipe_;

	enum SPECIAL_FUNC
	{
		BREAKABLE_WALL,
		AMOUNT
	};
	//特殊レシピと関数を持った構造体の配列
	std::vector<std::tuple<Recipe , SPECIAL_FUNC>> specialMenu_;
	Salad salad_;

private:

	//特殊レシピの関数


	std::vector<std::function<PlantData::FuncValue(Component_PlayerBehavior*)>> specialMenuFuncs;
};