#include "Component_PlantGenerator.h"

// インクルードディレクティブ
#include "../../Stage.h"
#include "../../StageObject.h"
#include <numbers>
#include "../../../../../Engine/ImGui/imgui.h"

Component_PlantGenerator::Component_PlantGenerator(string _name, StageObject* _holder, Component* _parent)
	:Component(_holder, _name, PlantGenerator, _parent)
{
}

void Component_PlantGenerator::Initialize()
{
}

void Component_PlantGenerator::Update()
{
	// アクティブでない場合、処理を行わない
	if(isActive_ == false)return;

	// 位置
	// 生成数分だけ繰り返す
	vector<XMFLOAT3> ramdomPositions;
	while (ramdomPositions.size() < plantNum_) {

		// 位置をランダムに生成
		XMFLOAT3 generatedPosition = GenerateRandomPosition();

		// 生成位置が他の生成位置と重なっていないかチェック
		bool isOverlap = false;
		for (auto& position : ramdomPositions) {
			if (sqrt(pow(position.x - generatedPosition.x, 2) + pow(position.z - generatedPosition.z, 2)) < 1.f) {
				isOverlap = true;
				break;
			}
		}

		// 重なっていない場合、生成位置を追加
		if (isOverlap == false)ramdomPositions.push_back(generatedPosition);
	}

	// レアリティ
	
	vector<Plant> randomPlants;
	//while (randomPlants.size() < plantNum_ && CalculateTotalResearchPoint(randomPlants) < 100) {

	//	// プラントを重み付けで選択
	//	randomPlants.push_back(WeightedPickPlants(PlantCollection::GetPlants()));

	//	// 生成数が最大数を超え、かつ総研究ポイントが100未満の場合、リセット
	//	if (randomPlants.size() >= plantNum_ && CalculateTotalResearchPoint(randomPlants) < 100)randomPlants.clear();
	//}

	// Collection内の植物を上から抽出
	/*for (int i = 0; i < plantNum_; i++) {
		randomPlants.push_back(PlantCollection::GetPlants()[i]);
	}*/

	// 植物オブジェクトをステージ上に生成
	for (int i = 0; i < plantNum_; i++) {

		// ステージオブジェクトを生成
		StageObject* plant = CreateStageObject("plant" + i, randomPlants[i].modelFilePath_, holder_->GetParent());
		
		// 生成位置を設定
		plant->SetPosition(ramdomPositions[i]);

		// プラントコンポーネントを追加
		// plant->AddComponent(CreateComponent("Plant", Plant, plant, this));

		// ステージに追加
		((Stage*)holder_->GetParent())->AddStageObject(plant);
	}

	// アクティブをオフにする
	isActive_ = false;
}

void Component_PlantGenerator::Release()
{
}

void Component_PlantGenerator::Save(json& _saveObj)
{
	// 半径を保存
	_saveObj["radius_"] = radius_;

	// 生成数を保存
	_saveObj["plantNum_"] = plantNum_;

	// 最大稀少度を保存
	_saveObj["maxRarity_"] = maxRarity_;

	// 最小稀少度を保存
	_saveObj["minRarity_"] = minRarity_;

	// 出現エリアを保存
	_saveObj["areaId_"] = areaId_;
}

void Component_PlantGenerator::Load(json& _loadObj)
{
	// 半径を読込
	radius_ = _loadObj["radius_"];

	// 生成数を読込
	plantNum_ = _loadObj["plantNum_"];

	// 最大稀少度を読込
	maxRarity_ = _loadObj["maxRarity_"];

	// 最小稀少度を読込
	minRarity_ = _loadObj["minRarity_"];

	// 出現エリアを読込
	areaId_ = _loadObj["areaId_"];
}

void Component_PlantGenerator::DrawData()
{
	// 半径を設定
	ImGui::DragFloat("radius_", &radius_);

	// 生成数を設定
	ImGui::DragInt("plantNum_", &plantNum_);

	// 最大稀少度を設定
	ImGui::DragInt("maxRarity_", &maxRarity_);

	// 最小稀少度を設定
	ImGui::DragInt("minRarity_", &minRarity_);

	// 出現エリアを設定
	ImGui::DragInt("areaId_", &areaId_);

	// ボタンを表示
	if (ImGui::Button("Generate"))Execute();
}

XMFLOAT3 Component_PlantGenerator::GenerateRandomPosition()
{
	// 方向をランダムに決定
	float angle = static_cast<float>(rand()) / RAND_MAX * 2.f * std::numbers::pi;

	// 距離をランダムに決定
	float distance = sqrt(static_cast<float>(rand()) / RAND_MAX) * radius_;

	// 位置を計算して返す
	return {
		holder_->GetPosition().x + distance * cos(angle),
		0,
		holder_->GetPosition().z + distance * sin(angle)
	};
}

Plant Component_PlantGenerator::WeightedPickPlants(unordered_map<int,Plant> _plants)
{
	// 重みの合計を計算
	int totalWeight = 0;
	for (const auto& plant : _plants) totalWeight += plant.second.rarity_;

	// 重みに応じたランダム選択
	if (totalWeight > 0) {
		int random = rand() % totalWeight;
		for (const auto& plant : _plants) {
			random -= plant.second.rarity_;
			if (random < 0) return plant.second;
		}
	}

	// 万が一、重みがすべて0ならランダムに選択
	auto it = _plants.begin();
	std::advance(it, rand() % _plants.size());
	return it->second;
}

int Component_PlantGenerator::CalculateTotalResearchPoint(vector<Plant>& randomPlants)
{
	int totalResearchPoint = 0;
	for (const auto& plant : randomPlants) {
		switch (plant.rarity_){
		case 1: totalResearchPoint += 5;	break;
		case 2: totalResearchPoint += 10;	break;
		case 3: totalResearchPoint += 30;	break;
		}
	}
	return totalResearchPoint;
}
