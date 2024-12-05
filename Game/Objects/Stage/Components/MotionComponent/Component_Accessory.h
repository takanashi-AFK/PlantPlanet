#pragma once
#include "Component_Motion.h"
#include <vector>
#include "../../Components/BehaviorComponents/Component_PlayerBehavior.h"

class Component_Accessory : public Component_Motion
{
public:
	/// <summary> �R���X�g���N�^ </summary>
	Component_Accessory(string _name, StageObject* _holder, Component* _parent);

	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> ��� </summary>
	void Release() override;

	/// <summary> ImGui�p�l���\�� </summary>
	void DrawData() override;

	void ExchangeModel(std::string filePath);

	void Load(json& _loadObj);
	void Save(json& _saveObj);

	char bone_[256];

protected:
	StageObject* gun_;
	int gunModelHandle_;

	XMFLOAT3 originalRotate_;
	XMFLOAT3 originalScale_;
	XMFLOAT3 originalPosition_;

};

