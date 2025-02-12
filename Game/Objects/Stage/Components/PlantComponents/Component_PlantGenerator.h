#pragma once
#include "../Component.h"

#include "../../../../Plants/PlantCollection.h"
#include <vector>

using std::vector;

class Component_PlantGenerator : public Component
{
private:
	float radius_;	// 半径
	int plantNum_;	// 生成数
	int maxRarity_;	// 最大稀少度
	int minRarity_;	// 最小稀少度
	int areaId_;	// 出現エリア

	float rare1Weight_;
	float rare2Weight_;
	float rare3Weight_;

	// 出現させる植物を固有名で制限する処理に使用
	bool useRestrictedPlants_;		 // 制限植物を使用するか
	vector<int> restrictedPlantIds_; // 制限植物のIDリスト

public:
	/// <summary> コンストラクタ </summary>
	Component_PlantGenerator(string _name, StageObject* _holder, Component* _parent);

	/// <summary> 初期化 </summary>
	void Initialize() override;

	/// <summary> 更新 </summary>
	void Update() override;

	/// <summary> 開放 </summary>
	void Release() override;

	/// <summary> 保存 </summary>
	void Save(json& _saveObj) override;

	/// <summary> 読込 </summary>
	void Load(json& _loadObj) override;

	/// <summary> ImGuiパネル表示 </summary>
	void DrawData() override;
private:
	/// <summary> ランダムな位置を生成 </summary>
	XMFLOAT3 GenerateRandomPosition();

	/// <summary> 重み付けされた選択 </summary>
	PlantData WeightedPickPlants(unordered_map<int,PlantData> _plants);

	/// <summary> レアリティの合計値を取得 </summary>
	int CalculateTotalResearchPoint(vector<PlantData>& randomPlants);
};

