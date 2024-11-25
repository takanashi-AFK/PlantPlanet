#include "Component_PlantGenerator.h"

// インクルードディレクティブ
#include "../../Stage.h"
#include "../../StageObject.h"
#include <numbers>
#include "../../../../../Engine/ImGui/imgui.h"
#include <random>
#include "Component_Plant.h"

namespace {
	const int MAX_RARITY = 3;
	const int MIN_RARITY = 1;
	const int MAX_RESERCH_POINT = 100;

}

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
	vector<PlantData> randomPlants;
	while (randomPlants.size() < plantNum_) {

		// プラントを重み付けで選択
		randomPlants.push_back(WeightedPickPlants(PlantCollection::GetPlants()));

	}

	// 植物オブジェクトをステージ上に生成
	for (int i = 0; i < plantNum_; i++) {

		// ステージオブジェクトを生成
		StageObject* plant = CreateStageObject("plant" + std::to_string(i), randomPlants[i].modelFilePath_, holder_->GetParent());
		
		// 生成位置を設定
		plant->SetPosition(ramdomPositions[i]);

		// プラントコンポーネントを作成
		Component_Plant* plantComponent = (Component_Plant*)plant->AddComponent(CreateComponent("Plant", Plant, plant, this));
		//plantComponent->SetData();
		plant->AddComponent(CreateComponent("Plant", Plant, plant, this));

		// サイズを設定
		plant->SetScale({ 0.3f,0.3f,0.3f });

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

PlantData Component_PlantGenerator::WeightedPickPlants(unordered_map<int,PlantData> _plants)
{
	// 希望する確率分布をレアリティごとに設定
	std::vector<double> rarityWeights = { 0.7, 0.25, 0.05 }; // レアリティ1, 2, 3の確率

	// 確率に基づいてランダムにレアリティを選ぶ
	std::random_device rd;
	std::mt19937 gen(rd());
	int selectedRarity;
	do {

	std::discrete_distribution<> dist(rarityWeights.begin() , rarityWeights.end());

	selectedRarity = dist(gen) + 1; // レアリティは1~3なので+1
	} while (selectedRarity < minRarity_ || selectedRarity > maxRarity_);

	// 選ばれたレアリティの中からランダムに1つ選ぶ
	std::vector<PlantData> candidates;
	for (const auto& plant : _plants) {
		if (plant.second.rarity_ == selectedRarity) {
			candidates.push_back(plant.second);
		}
	}

	if (!candidates.empty()) {
		// 候補があればその中からランダムに1つ返す
		std::uniform_int_distribution<> randomIndex(0, candidates.size() - 1);
		return candidates[randomIndex(gen)];
	}
	else {
		// 万が一候補がなければランダムに選択
		auto it = _plants.begin();
		std::advance(it, rand() % _plants.size());
		return it->second;
	}

}

int Component_PlantGenerator::CalculateTotalResearchPoint(vector<PlantData>& randomPlants)
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
