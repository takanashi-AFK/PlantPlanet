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
	// plants_����̏ꍇ�͉������Ȃ�
	if (plants_.size() <= 0) return;
	
	// JSON�I�u�W�F�N�g�����[�v���ăf�[�^��ۑ�
	int index = 0;
	for (auto& plant : plants_) _saveObj[index++] = {
		{"id_",index},
		{"rarity_",plant.second.rarity_},
		{"name_",plant.second.name_},
		{"areaId_",plant.second.areaId_},
		{"modelFilePath_",plant.second.modelFilePath_},
		{"imageFilePath_",plant.second.imageFilePath_}
	};
}

void PlantCollection::Load(json& _loadObj)
{
	// ���݂̐A������S�ăN���A����
	ClearPlants();

	// _loadObj����̏ꍇ�͉������Ȃ�
	if (_loadObj.size() <= 0) return;

	// JSON�I�u�W�F�N�g�����[�v���ăf�[�^��Ǎ�
	int index = 0;
	for (auto it = _loadObj.begin(); it != _loadObj.end(); ++it) {
		PlantCollection::AddPlant({
			index ,
			it.value()["rarity_"],
			it.value()["name_"],
			it.value()["areaId_"],
			it.value()["modelFilePath_"],
			it.value()["imageFilePath_"]
			}
		);
		index++;
	}
}




