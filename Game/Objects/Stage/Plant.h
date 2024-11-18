#pragma once
#include "StageObject.h"
class Plant :
    public StageObject
{
private:
	int id_;
	int rarity_;
	string name_;
	string modelFilePath_;
	string imageFilePath_;
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_parent">親オブジェクト</param>
	Plant(GameObject* _parent);

	void SetID(int _id) { id_ = _id; }
	void SetRarity(int _rarity) { rarity_ = _rarity; }
	void SetName(string _name) { name_ = _name; }
	void SetModelFilePath(string _modelFilePath) { modelFilePath_ = _modelFilePath; }
	void SetImageFilePath(string _imageFilePath) { imageFilePath_ = _imageFilePath; }

	int GetID() { return id_; }
	int GetRarity() { return rarity_; }
	string GetName() { return name_; }

	void Save(json& _saveObj);
	void Load(json& _loadObj);

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 詳細をエディタに描画
	/// </summary>
	void DrawData() override;
};

