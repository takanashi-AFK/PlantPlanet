#include "PlantCollection.h"

namespace PlantCollection
{
	unordered_map<int, Plant> plants_;
}

void PlantCollection::AddPlant(Plant _plant)
{
	plants_[_plant.id_] = _plant;
}

void PlantCollection::RemovePlant(int _id)
{
	auto it = plants_.find(_id);
	if (it != plants_.end()) {
		// �}�b�v����G���g�����폜
		plants_.erase(it);
	}
}

void PlantCollection::ClearPlants()
{
	plants_.clear();
}

unordered_map<int, Plant> PlantCollection::GetPlants()
{
	return plants_;
}

void PlantCollection::Save(json& _saveObj)
{

	if (plants_.size() > 0) {
		int index = 1;
		for (auto plant : plants_) {
			_saveObj[index]["PlantID"] = index;
			_saveObj[index]["PlantRarity"] = plant.second.rarity_;
			_saveObj[index]["PlantName"] = plant.second.name_;
			_saveObj[index]["PlantModelFilePath"] = plant.second.modelFilePath_;
			_saveObj[index]["PlantImageFilePath"] = plant.second.imageFilePath_;
			index++;
		}
	}
}

void PlantCollection::Load(json& _loadObj)
{
	// ���݂�plants_���N���A���Ă���ǂݍ���
	plants_.clear();
	int index = 0;
	// JSON�I�u�W�F�N�g�����[�v���ăf�[�^��ǂݍ���
	for (auto it = _loadObj.begin(); it != _loadObj.end(); ++it) {
		PlantCollection::AddPlant({
			index ,
			it.value()["PlantRarity"],
			it.value()["PlantName"],
			it.value()["PlantModelFilePath"],
			it.value()["PlantImageFilePath"]
			});
		index++;
	}
}





