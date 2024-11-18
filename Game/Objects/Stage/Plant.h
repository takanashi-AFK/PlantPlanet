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
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_parent">�e�I�u�W�F�N�g</param>
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
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �ڍׂ��G�f�B�^�ɕ`��
	/// </summary>
	void DrawData() override;
};

