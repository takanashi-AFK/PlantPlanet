#pragma once
#include "../../Component.h"
#include "../Component_Attack.h"
#include "../../../../../../Engine/GameObject/GameObject.h"
#include "../../../Bullet.h"
//�쐬��:����
class Component_CactanAttack :public Component_Attack
{
	int frame;
	float speed_;
	StageObject* target_;//�Ǐ]�̑ΏۃI�u�W�F�N�g
	XMVECTOR direction_;

	public:

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
	/// <param name="_saveobj">�f�[�^�̕ۑ����</param>
	void Save(json& _saveobj) override;
	
	/// <summary>
	/// �ǂݍ���
	/// </summary>
	/// <param name="_loadobj">�f�[�^�̓Ǎ����</param>
	void Load(json& _loadobj) override;
	
	void DrawData()override;

	void Attack();

	void SetTarget();
};
