#pragma once

// インクルードディレクティブ
#include <unordered_map>
#include "../../Engine/Json/JsonReader.h"
#include "Plant.h"

// usingディレクティブ
using std::unordered_map;

// 名前空間
namespace PlantCollection
{
	/// <summary> 植物情報を追加 </summary>
	void AddPlant(PlantData _plant);

	/// <summary> 植物情報を削除 </summary>
	void RemovePlant(int _id);

	/// <summary> 植物情報を取得 </summary>
	void ClearPlants();

	/// <summary> 植物情報を保存 </summary>
	void Save(json &_saveObj);

	/// <summary> 植物情報を読込 </summary>
	void Load(json &_loadObj);

	/// <summary> 全ての植物情報を取得 </summary>
	unordered_map<int, PlantData> GetPlants();

	/// <summary> 植物情報を取得 </summary>
	PlantData GetPlant(int _id);
};

