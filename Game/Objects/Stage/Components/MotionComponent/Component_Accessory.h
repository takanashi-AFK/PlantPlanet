#pragma once
#include "Component_Motion.h"
#include <vector>
#include "../../Components/BehaviorComponents/Component_PlayerBehavior.h"

class Component_Accessory : public Component_Motion
{
public:
	/// <summary> コンストラクタ </summary>
	Component_Accessory(string _name, StageObject* _holder, Component* _parent);

	/// <summary> 初期化 </summary>
	void Initialize() override;

	/// <summary> 更新 </summary>
	void Update() override;

	/// <summary> 解放 </summary>
	void Release() override;

	/// <summary> ImGuiパネル表示 </summary>
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

