#include "Component_PlantGenerator.h"

// �C���N���[�h�f�B���N�e�B�u
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
	// �A�N�e�B�u�łȂ��ꍇ�A�������s��Ȃ�
	if(isActive_ == false)return;

	// �ʒu
	// �������������J��Ԃ�
	vector<XMFLOAT3> ramdomPositions;
	while (ramdomPositions.size() < plantNum_) {

		// �ʒu�������_���ɐ���
		XMFLOAT3 generatedPosition = GenerateRandomPosition();

		// �����ʒu�����̐����ʒu�Əd�Ȃ��Ă��Ȃ����`�F�b�N
		bool isOverlap = false;
		for (auto& position : ramdomPositions) {
			if (sqrt(pow(position.x - generatedPosition.x, 2) + pow(position.z - generatedPosition.z, 2)) < 1.f) {
				isOverlap = true;
				break;
			}
		}

		// �d�Ȃ��Ă��Ȃ��ꍇ�A�����ʒu��ǉ�
		if (isOverlap == false)ramdomPositions.push_back(generatedPosition);
	}

	// ���A���e�B
	vector<PlantData> randomPlants;
	while (randomPlants.size() < plantNum_) {

		// �v�����g���d�ݕt���őI��
		randomPlants.push_back(WeightedPickPlants(PlantCollection::GetPlants()));

	}

	// �A���I�u�W�F�N�g���X�e�[�W��ɐ���
	for (int i = 0; i < plantNum_; i++) {

		// �X�e�[�W�I�u�W�F�N�g�𐶐�
		StageObject* plant = CreateStageObject("plant" + std::to_string(i), randomPlants[i].modelFilePath_, holder_->GetParent());
		
		// �����ʒu��ݒ�
		plant->SetPosition(ramdomPositions[i]);

		// �v�����g�R���|�[�l���g���쐬
		Component_Plant* plantComponent = (Component_Plant*)plant->AddComponent(CreateComponent("Plant", Plant, plant, this));
		//plantComponent->SetData();
		plant->AddComponent(CreateComponent("Plant", Plant, plant, this));

		// �T�C�Y��ݒ�
		plant->SetScale({ 0.3f,0.3f,0.3f });

		// �X�e�[�W�ɒǉ�
		((Stage*)holder_->GetParent())->AddStageObject(plant);
	}

	// �A�N�e�B�u���I�t�ɂ���
	isActive_ = false;
}

void Component_PlantGenerator::Release()
{
}

void Component_PlantGenerator::Save(json& _saveObj)
{
	// ���a��ۑ�
	_saveObj["radius_"] = radius_;

	// ��������ۑ�
	_saveObj["plantNum_"] = plantNum_;

	// �ő�H���x��ۑ�
	_saveObj["maxRarity_"] = maxRarity_;

	// �ŏ��H���x��ۑ�
	_saveObj["minRarity_"] = minRarity_;

	// �o���G���A��ۑ�
	_saveObj["areaId_"] = areaId_;
}

void Component_PlantGenerator::Load(json& _loadObj)
{
	// ���a��Ǎ�
	radius_ = _loadObj["radius_"];

	// ��������Ǎ�
	plantNum_ = _loadObj["plantNum_"];

	// �ő�H���x��Ǎ�
	maxRarity_ = _loadObj["maxRarity_"];

	// �ŏ��H���x��Ǎ�
	minRarity_ = _loadObj["minRarity_"];

	// �o���G���A��Ǎ�
	areaId_ = _loadObj["areaId_"];
}

void Component_PlantGenerator::DrawData()
{
	// ���a��ݒ�
	ImGui::DragFloat("radius_", &radius_);

	// ��������ݒ�
	ImGui::DragInt("plantNum_", &plantNum_);

	// �ő�H���x��ݒ�
	ImGui::DragInt("maxRarity_", &maxRarity_);

	// �ŏ��H���x��ݒ�
	ImGui::DragInt("minRarity_", &minRarity_);

	// �o���G���A��ݒ�
	ImGui::DragInt("areaId_", &areaId_);

	// �{�^����\��
	if (ImGui::Button("Generate"))Execute();
}

XMFLOAT3 Component_PlantGenerator::GenerateRandomPosition()
{
	// �����������_���Ɍ���
	float angle = static_cast<float>(rand()) / RAND_MAX * 2.f * std::numbers::pi;

	// �����������_���Ɍ���
	float distance = sqrt(static_cast<float>(rand()) / RAND_MAX) * radius_;

	// �ʒu���v�Z���ĕԂ�
	return {
		holder_->GetPosition().x + distance * cos(angle),
		0,
		holder_->GetPosition().z + distance * sin(angle)
	};
}

PlantData Component_PlantGenerator::WeightedPickPlants(unordered_map<int,PlantData> _plants)
{
	// ��]����m�����z�����A���e�B���Ƃɐݒ�
	std::vector<double> rarityWeights = { 0.7, 0.25, 0.05 }; // ���A���e�B1, 2, 3�̊m��

	// �m���Ɋ�Â��ă����_���Ƀ��A���e�B��I��
	std::random_device rd;
	std::mt19937 gen(rd());
	int selectedRarity;
	do {

	std::discrete_distribution<> dist(rarityWeights.begin() , rarityWeights.end());

	selectedRarity = dist(gen) + 1; // ���A���e�B��1~3�Ȃ̂�+1
	} while (selectedRarity < minRarity_ || selectedRarity > maxRarity_);

	// �I�΂ꂽ���A���e�B�̒����烉���_����1�I��
	std::vector<PlantData> candidates;
	for (const auto& plant : _plants) {
		if (plant.second.rarity_ == selectedRarity) {
			candidates.push_back(plant.second);
		}
	}

	if (!candidates.empty()) {
		// ��₪����΂��̒����烉���_����1�Ԃ�
		std::uniform_int_distribution<> randomIndex(0, candidates.size() - 1);
		return candidates[randomIndex(gen)];
	}
	else {
		// �������₪�Ȃ���΃����_���ɑI��
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
