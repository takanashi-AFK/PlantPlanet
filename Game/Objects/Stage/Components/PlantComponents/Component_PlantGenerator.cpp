#include "Component_PlantGenerator.h"

// �C���N���[�h�f�B���N�e�B�u
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
	
	vector<Plant> randomPlants;
	//while (randomPlants.size() < plantNum_ && CalculateTotalResearchPoint(randomPlants) < 100) {

	//	// �v�����g���d�ݕt���őI��
	//	randomPlants.push_back(WeightedPickPlants(PlantCollection::GetPlants()));

	//	// ���������ő吔�𒴂��A���������|�C���g��100�����̏ꍇ�A���Z�b�g
	//	if (randomPlants.size() >= plantNum_ && CalculateTotalResearchPoint(randomPlants) < 100)randomPlants.clear();
	//}

	// Collection���̐A�����ォ�璊�o
	/*for (int i = 0; i < plantNum_; i++) {
		randomPlants.push_back(PlantCollection::GetPlants()[i]);
	}*/

	// �A���I�u�W�F�N�g���X�e�[�W��ɐ���
	for (int i = 0; i < plantNum_; i++) {

		// �X�e�[�W�I�u�W�F�N�g�𐶐�
		StageObject* plant = CreateStageObject("plant" + i, randomPlants[i].modelFilePath_, holder_->GetParent());
		
		// �����ʒu��ݒ�
		plant->SetPosition(ramdomPositions[i]);

		// �v�����g�R���|�[�l���g��ǉ�
		// plant->AddComponent(CreateComponent("Plant", Plant, plant, this));

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

Plant Component_PlantGenerator::WeightedPickPlants(unordered_map<int,Plant> _plants)
{
	// �d�݂̍��v���v�Z
	int totalWeight = 0;
	for (const auto& plant : _plants) totalWeight += plant.second.rarity_;

	// �d�݂ɉ����������_���I��
	if (totalWeight > 0) {
		int random = rand() % totalWeight;
		for (const auto& plant : _plants) {
			random -= plant.second.rarity_;
			if (random < 0) return plant.second;
		}
	}

	// ������A�d�݂����ׂ�0�Ȃ烉���_���ɑI��
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
