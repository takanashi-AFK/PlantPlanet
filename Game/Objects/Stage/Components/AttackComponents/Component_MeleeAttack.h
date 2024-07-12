#pragma once
#include "Component_Attack.h"
#include "../../../../../Engine/Collider/BoxCollider.h"

class Component_MeleeAttack : public Component_Attack
{
private:
	XMVECTOR direction_;	// �U������
	XMFLOAT3 colliderSize_; // �R���C�_�[�̑傫��
	bool isHit_;			// �q�b�g�������ǂ���
	BoxCollider* collider_;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	/// <param name="_name">���g�̃R���|�[�l���g��</param>
	/// <param name="_type">���g�̃R���|�[�l���g�^�C�v</param>
	Component_MeleeAttack(string _name, StageObject* _holder, Component* _parent);

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
	void Release();

	/// <summary>
	/// ImGui�p�l���\��
	/// </summary>
	void DrawData() override;

	/// <summary>
	/// �Փ˔���
	/// </summary>
	/// <param name="_target"></param>
	void OnCollision(GameObject* _target, Collider* _collider) ;
	
	/// <summary>
	/// �U�������̐ݒ�
	/// </summary>
	/// <param name="_direction"></param>
	void SetDirection(XMVECTOR _direction) { direction_ = _direction; }
};

