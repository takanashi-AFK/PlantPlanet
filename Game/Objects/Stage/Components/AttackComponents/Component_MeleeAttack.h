#pragma once
#include "Component_Attack.h"
#include "../../../../../Engine/Collider/BoxCollider.h"

class Component_MeleeAttack : public Component_Attack
{
private:
	XMVECTOR forward_; //�O����
	XMVECTOR prevFrontVec_;
	bool isOneHit_;

	// �ǋL��
	XMVECTOR direction_;	// �U������
	XMFLOAT3 colliderSize_; // �R���C�_�[�̑傫��
	bool isHit_;			// �q�b�g�������ǂ���
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
	/// ��莞�Ԃ�������R���C�_�[���폜
	/// </summary>
	/// <param name="_time">���b��ɏ�����</param>
	bool AutoDelete(float _time);

	/// <summary>
	/// �Փ˔���
	/// </summary>
	/// <param name="_target"></param>
	void OnCollision(GameObject* _target) override;
	
	/// <summary>
	/// ���ʕ����̐ݒ�
	/// </summary>
	/// <param name="_forward">���ʕ����x�N�g��</param>
	void SetForward(XMVECTOR _forward) { forward_ = _forward; }
};

