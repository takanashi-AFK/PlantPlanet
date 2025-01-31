#include "PlantCollection.h"

namespace PlantCollection
{
	unordered_map<int, PlantData> plants_;
}

void PlantCollection::AddPlant(PlantData _plant)
{
	plants_[_plant.id_] = _plant;
}

void PlantCollection::RemovePlant(int _id)
{
	plants_.erase(_id);
}

void PlantCollection::ClearPlants()
{
	plants_.clear();
}

unordered_map<int, PlantData> PlantCollection::GetPlants()
{
	return plants_;
}

void PlantCollection::Save(json& _saveObj)
{
	// plants_が空の場合は何もしない
	if (plants_.size() <= 0) return;
	
	// JSONオブジェクトをループしてデータを保存
	int index = 0;
	for (auto& plant : plants_) _saveObj[index++] = {
		{"id_",index},
		{"rarity_",plant.second.rarity_},
		{"name_",plant.second.name_},
		{"areaId_",plant.second.areaId_},
		{"isSpawn_",plant.second.isSpawn_},
		{"modelFilePath_",plant.second.modelFilePath_},
		{"imageFilePath_",plant.second.imageFilePath_},
		{"descriptionImageFilePath_Complete_",plant.second.descriptionImageFilePath_Complete_},
		{"descriptionImageFilePath_InComplete_",plant.second.descriptionImageFilePath_InComplete_}
	};
}

void PlantCollection::Load(json& _loadObj)
{
	// 現在の植物情報を全てクリアする
	ClearPlants();

	// _loadObjが空の場合は何もしない
	if (_loadObj.size() <= 0) return;

	// JSONオブジェクトをループしてデータを読込
	int index = 0;
	for (auto it = _loadObj.begin(); it != _loadObj.end(); ++it) {
		PlantCollection::AddPlant({
			index ,
			it.value()["rarity_"],
			it.value()["name_"],
			it.value()["areaId_"],
			it.value()["isSpawn_"],
			it.value()["modelFilePath_"],
			it.value()["imageFilePath_"],
			it.value()["descriptionImageFilePath_Complete_"],
			it.value()["descriptionImageFilePath_InComplete_"]
			}
		);
		index++;
	}
}





