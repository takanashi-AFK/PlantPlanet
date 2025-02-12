#pragma once

// �C���N���[�h�f�B���N�e�B�u
#include <unordered_map>
#include "../../Engine/Json/JsonReader.h"
#include "Plant.h"

// using�f�B���N�e�B�u
using std::unordered_map;

// ���O���
namespace PlantCollection
{
	/// <summary> �A������ǉ� </summary>
	void AddPlant(PlantData _plant);

	/// <summary> �A�������폜 </summary>
	void RemovePlant(int _id);

	/// <summary> �A�������擾 </summary>
	void ClearPlants();

	/// <summary> �A������ۑ� </summary>
	void Save(json &_saveObj);

	/// <summary> �A������Ǎ� </summary>
	void Load(json &_loadObj);

	/// <summary> �S�Ă̐A�������擾 </summary>
	unordered_map<int, PlantData> GetPlants();

	/// <summary> �A�������擾 </summary>
	PlantData GetPlant(int _id);
};

