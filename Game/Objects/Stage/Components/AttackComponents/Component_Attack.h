#pragma once

//�C���N���[�h
#include"../Component.h"

//�쐬��:����
class Component_Attack :public Component
{

protected:
	bool isAttack_;
	int  power_;
private:

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	Component_Attack(StageObject* _holder);

	Component_Attack(StageObject* _holder, string _name, ComponentType _type);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// ���
	/// </summary>
	void Release() override;

	/// <summary>
	/// �ۑ�
	/// </summary>
	/// <param name="_saveobj">�f�[�^�̕ۑ����</param>
	void Save(json& _saveobj) override;

	/// <summary>
	/// �ǂݍ���
	/// </summary>
	/// <param name="_loadobj">�f�[�^�̓Ǎ����</param>
	void Load(json& _loadobj) override;

	void DrawData()override;

};

