#pragma once
#include "../Component.h"
#include "../../../../Plants/PlantCollection.h"

class Component_Plant : public Component
{
private:
	PlantData data_;	// �f�[�^

public:
	/// <summary> �R���X�g���N�^ </summary>
	Component_Plant(string _name, StageObject* _holder, Component* _parent);
	
	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> ��� </summary>
	void Release() override;

	/// <summary> �ۑ� </summary>
	void Save(json& _saveObj) override;

	/// <summary> �Ǎ� </summary>
	void Load(json& _loadObj) override;

	/// <summary> ImGui�p�l���\�� </summary>
	void DrawData() override;

/*
getter :*/
	/// <summary> �f�[�^���擾 </summary>
	PlantData GetData() const { return data_; }

/*
setter :*/
	/// <summary> �f�[�^��ݒ� </summary>
	void SetData(PlantData _data) { data_ = _data; }
};

