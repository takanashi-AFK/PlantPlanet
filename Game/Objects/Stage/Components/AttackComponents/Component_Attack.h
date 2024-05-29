#pragma once

//�C���N���[�h
#include"../Component.h"

//�쐬��:����
class Component_Attack :public Component
{

protected:
	bool isAttack_;//�U���t���O
	int  power_;//�U����
private:

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	Component_Attack(StageObject* _holder);


	/// <summary>
	/// �ق��R���|�[�l���g���p�������ۂ̃R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	/// <param name="_name">���g�̃R���|�[�l���g��</param>
	/// <param name="_type">���g�̃R���|�[�l���gID</param>
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

	/// <summary>
	/// ImGui�p�l���\��
	/// </summary>
	void DrawData()override;

};

