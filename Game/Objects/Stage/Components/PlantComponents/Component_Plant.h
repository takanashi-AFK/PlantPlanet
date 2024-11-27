#pragma once
#include "../Component.h"
#include "../../../../Plants/PlantCollection.h"

class Component_Plant : public Component
{
private:
	PlantData data_;	// データ

public:
	/// <summary> コンストラクタ </summary>
	Component_Plant(string _name, StageObject* _holder, Component* _parent);
	
	/// <summary> 初期化 </summary>
	void Initialize() override;

	/// <summary> 更新 </summary>
	void Update() override;

	/// <summary> 解放 </summary>
	void Release() override;

	/// <summary> 保存 </summary>
	void Save(json& _saveObj) override;

	/// <summary> 読込 </summary>
	void Load(json& _loadObj) override;

	/// <summary> ImGuiパネル表示 </summary>
	void DrawData() override;

/*
getter :*/
	/// <summary> データを取得 </summary>
	PlantData GetData() const { return data_; }

/*
setter :*/
	/// <summary> データを設定 </summary>
	void SetData(PlantData _data) { data_ = _data; }
};

