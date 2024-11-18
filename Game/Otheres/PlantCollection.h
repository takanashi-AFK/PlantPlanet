#pragma once
#include <vector>
#include "../../Engine/Json/JsonReader.h"
using std::vector;
class Plant;

namespace PlantCollection
{
	void AddPlant(Plant* _plant);
	void RemovePlant(int _index);
	void ClearPlantKind();
	vector<Plant*> GetPlantsKind();

	void Save(json& _saveObj);
	void Load(json& _loadObj);
};

