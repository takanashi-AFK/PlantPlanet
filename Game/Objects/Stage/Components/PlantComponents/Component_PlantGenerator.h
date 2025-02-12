#pragma once
#include "../Component.h"

#include "../../../../Plants/PlantCollection.h"
#include <vector>

using std::vector;

class Component_PlantGenerator : public Component
{
private:
	float radius_;	// ���a
	int plantNum_;	// ������
	int maxRarity_;	// �ő�H���x
	int minRarity_;	// �ŏ��H���x
	int areaId_;	// �o���G���A

	float rare1Weight_;
	float rare2Weight_;
	float rare3Weight_;

	// �o��������A�����ŗL���Ő������鏈���Ɏg�p
	bool useRestrictedPlants_;		 // �����A�����g�p���邩
	vector<int> restrictedPlantIds_; // �����A����ID���X�g

public:
	/// <summary> �R���X�g���N�^ </summary>
	Component_PlantGenerator(string _name, StageObject* _holder, Component* _parent);

	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> �J�� </summary>
	void Release() override;

	/// <summary> �ۑ� </summary>
	void Save(json& _saveObj) override;

	/// <summary> �Ǎ� </summary>
	void Load(json& _loadObj) override;

	/// <summary> ImGui�p�l���\�� </summary>
	void DrawData() override;
private:
	/// <summary> �����_���Ȉʒu�𐶐� </summary>
	XMFLOAT3 GenerateRandomPosition();

	/// <summary> �d�ݕt�����ꂽ�I�� </summary>
	PlantData WeightedPickPlants(unordered_map<int,PlantData> _plants);

	/// <summary> ���A���e�B�̍��v�l���擾 </summary>
	int CalculateTotalResearchPoint(vector<PlantData>& randomPlants);
};

