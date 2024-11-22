#pragma once
#include <map>
#include "../../Engine/Json/JsonReader.h"
using std::unordered_map;

struct Plant {
	int id_;
	int rarity_;
	string name_;
	string modelFilePath_;
	string imageFilePath_;
};

namespace PlantCollection
{
	void AddPlant(Plant _plant);
	void RemovePlant(int _id);
	void ClearPlants();
	unordered_map<int, Plant> GetPlants();
	void Save(json &_saveObj);
	void Load(json &_loadObj);
};

