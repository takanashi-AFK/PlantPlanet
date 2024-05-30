#pragma once
#include "../../Otheres/UIObject.h"

class UIButton : public UIObject
{
private:

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_name">UI�I�u�W�F�N�g�̖��O</param>
	/// <param name="_modelFilePath">���f���t�@�C���p�X</param>
	/// <param name="_parent">�e�I�u�W�F�N�g</param>
	UIButton(string _name, string _modelFilePath, GameObject* _parent);

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
	/// ���
	/// </summary>
	void Release() override;

	/// <summary>
	/// �ۑ�
	/// </summary>
	/// <param name="_saveUiobj"></param>
	void Save(json& _saveUiobj) override;

	/// <summary>
	/// �ǂݍ���
	/// </summary>
	/// <param name="_loadUiobj"></param>
	void Load(json& _loadUiobj) override;


};

