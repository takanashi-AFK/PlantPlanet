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
	:Component(_holder, _name, PlantGenerator, _parent),
	radius_(10),
	plantNum_(10), 
	maxRarity_(MAX_RARITY),
	minRarity_(MIN_RARITY),
	areaId_(0), 
	rare1Weight_(0.7), 
	rare2Weight_(0.25), 
	rare3Weight_(0.05)
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
		StageObject* stagePlantObject = CreateStageObject("plant" + std::to_string(i), randomPlants[i].modelFilePath_, holder_->GetParent());

		// �����ʒu��ݒ�
		stagePlantObject->SetPosition(ramdomPositions[i]);

		// �����蔻���ݒ�
		stagePlantObject->SetIsColliding(false);

		// �v�����g�R���|�[�l���g���쐬
		Component_Plant* plantComponent = (Component_Plant*)CreateComponent("Plant", Plant, stagePlantObject, this);

		// �A������ݒ�
		plantComponent->SetData(randomPlants[i]);

		// �R���|�[�l���g��ǉ�
		stagePlantObject->AddComponent(plantComponent);

		// �T�C�Y��ݒ�
		stagePlantObject->SetScale({ 0.3f,0.3f,0.3f });

		// ������ݒ�
		stagePlantObject->SetObjectType(StageObject::TYPE_PLANT);

		// �X�e�[�W�ɒǉ�
		((Stage*)holder_->GetParent())->AddStageObject(stagePlantObject);
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

	// �d�݂�ۑ�
	_saveObj["rare1Weight_"] = rare1Weight_;
	_saveObj["rare2Weight_"] = rare2Weight_;
	_saveObj["rare3Weight_"] = rare3Weight_;
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

	// �d�݂�Ǎ�
	rare1Weight_ = _loadObj["rare1Weight_"];
	rare2Weight_ = _loadObj["rare2Weight_"];
	rare3Weight_ = _loadObj["rare3Weight_"];
}

void Component_PlantGenerator::DrawData()
{
	bool isExecute = true;
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

	ImGui::Separator();

	// �m���̐ݒ�
	ImGui::DragFloat("rare1Weight_", &rare1Weight_, 0.01f, 0.f, 1.f - (rare2Weight_ + rare3Weight_));
	ImGui::DragFloat("rare2Weight_", &rare2Weight_, 0.01f, 0.f, 1.f - (rare1Weight_ + rare3Weight_));
	ImGui::DragFloat("rare3Weight_", &rare3Weight_, 0.01f, 0.f, 1.f - (rare1Weight_ + rare2Weight_));

	// ���v��1����Ȃ�������{��
	if (rare1Weight_ + rare2Weight_ + rare3Weight_ != 1.f) {
		ImGui::Text("Please set so that the total is 1");
		isExecute = false;
	}
	// �{�^����\��
	if (ImGui::Button("Generate") && isExecute)Execute();
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

PlantData Component_PlantGenerator::WeightedPickPlants(unordered_map<int, PlantData> _plants)
{
	// ��]����m�����z�����A���e�B���Ƃɐݒ�
	std::vector<double> rarityWeights = { rare1Weight_, rare2Weight_, rare3Weight_ }; // ���A���e�B1, 2, 3�̊m��

	// �m���Ɋ�Â��ă����_���Ƀ��A���e�B��I��
	std::random_device rd;
	std::mt19937 gen(rd());
	int selectedRarity;
	do {
		std::discrete_distribution<> dist(rarityWeights.begin(), rarityWeights.end());
		selectedRarity = dist(gen) + 1; // ���A���e�B��1~3�Ȃ̂�+1
	} while (selectedRarity < minRarity_ || selectedRarity > maxRarity_);

	// �I�΂ꂽ���A���e�B���� isSpawn �� true �̒����烉���_����1�I��
	std::vector<PlantData> candidates;
	for (const auto& plant : _plants) {
		if (plant.second.rarity_ == selectedRarity && plant.second.isSpawn_) {
			candidates.push_back(plant.second);
		}
	}

	if (!candidates.empty()) {
		// ��₪����΂��̒����烉���_����1�Ԃ�
		std::uniform_int_distribution<> randomIndex(0, candidates.size() - 1);
		return candidates[randomIndex(gen)];
	}
	else {
		// �������₪�Ȃ���� isSpawn �� true �̒����烉���_���ɑI��
		std::vector<PlantData> fallbackCandidates;
		for (const auto& plant : _plants) {
			if (plant.second.isSpawn_) {
				fallbackCandidates.push_back(plant.second);
			}
		}

		if (!fallbackCandidates.empty()) {
			std::uniform_int_distribution<> randomIndex(0, fallbackCandidates.size() - 1);
			return fallbackCandidates[randomIndex(gen)];
		}
		else {
			// �Ō�̎�i�Ƃ��āA�S�̂��烉���_���ɑI��
			auto it = _plants.begin();
			std::advance(it, rand() % _plants.size());
			return it->second;
		}
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
