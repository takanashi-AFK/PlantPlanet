#pragma once
#include "../../Otheres/UIObject.h"
#include <list>

class UIPanel : public UIObject
{
protected:

	std::list<UIObject*> childList;	//�q�I�u�W�F�N�g���X�g

private:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_name">UI�I�u�W�F�N�g�̖��O</param>
	/// <param name="_parent">�e�̃I�u�W�F�N�g</param>
	UIPanel(string _name,GameObject* _parent);

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
	/// <param name="_saveUiobj">�ۑ��f�[�^</param>
	void Save(json& _saveUiobj) override;

	/// <summary>
	/// �ǂݍ���
	/// </summary>
	/// <param name="_loadUiobj">�ǂݍ��݃f�[�^</param>
	void Load(json& _loadUiobj) override;

	/// <summary>
	/// �p�l���Ɏq�I�u�W�F�N�g��ǉ�
	/// </summary>
	/// <param name="_child">�ǉ��������q�I�u�W�F�N�g</param>
	void AddChild(UIObject* _child);

	/// <summary>
	/// �p�l������w�肵���q�I�u�W�F�N�g���폜
	/// </summary>
	/// <param name="_child">�폜�������q�I�u�W�F�N�g</param>
	void RemoveChild(UIObject* _child);

	/// <summary>
	/// �p�l�����̎q�I�u�W�F�N�g��S�č폜
	/// </summary>
	void ClearChild();
};

