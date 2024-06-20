#pragma once

// �C���N���[�h
#include "StageObject.h"
#include "../../../Engine/EffekseerVFX.h"
/// <summary>
/// ���˂���e���Ǘ�����N���X
/// </summary>
class Bullet :public StageObject
{
private:
	bool isActive_;			// �e�����쒆���ǂ���
	int frame_;				// �o�߃t���[��	
	float speed_;			// �ړ����x
	XMVECTOR direction_;	// �ړ�����

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_parent">�e�I�u�W�F�N�g</param>
	Bullet(GameObject* _parent);

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
	/// �Փˏ���
	/// </summary>
	void OnCollision(GameObject* _target) override;

	/// <summary>
	/// �ړ����x��ݒ�
	/// </summary>
	/// <param name="_speed">���x</param>
	void SetSpeed(float _speed) { speed_ = _speed; }
	
	/// <summary>
	/// �ړ�������ݒ�
	/// </summary>
	/// <param name="_direction">����</param>
	void SetDirection(XMVECTOR _direction) { direction_ = _direction; }

	/// <summary>
	/// ���s
	/// </summary>
	void Execute() { isActive_ = true; }

private:
	/// <summary>
	/// �ړ�
	/// </summary>
	/// <param name="_dir">����</param>
	/// <param name="_speed">���x</param>
	void Move(XMVECTOR _dir, float _speed);

	/// <summary>
	/// ��莞�Ԍ�Ɏ��g���폜
	/// </summary>
	/// <param name="_sec">������܂ł̎��ԁi�b�j</param>
	void AutoDelete(float _sec);

	std::shared_ptr<EFFEKSEERLIB::EFKTransform>mt;
};



