#pragma once

// �C���N���[�h
#include "../Component_Attack.h"

/// <summary>
/// �J�N�^��(�������U��Enemy)�̍U���R���|�[�l���g
/// </summary>
class Component_CactanAttack :public Component_Attack
{
private:
	float bulletSpeed_;	 // �e�̑��x
	StageObject* target_;// �U���Ώ�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�R���|�[�l���g�̕ێ���</param>
	Component_CactanAttack(StageObject* _holder);
	
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
	/// <param name="_savebj">�f�[�^�̕ۑ����</param>
	void Save(json& _saveObj) override;
	
	/// <summary>
	/// �Ǎ�
	/// </summary>
	/// <param name="_loadObj">�f�[�^�̓Ǎ����</param>
	void Load(json& _loadObj) override;
	
	/// <summary>
	/// ImGui�p�l���\��
	/// </summary>
	void DrawData()override;

	/// <summary>
	/// �U�����s
	/// </summary>
	void Attack();

	/// <summary>
	/// �^�[�Q�b�g�̐ݒ�
	/// </summary>
	void SetTarget();
};

