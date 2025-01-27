#pragma once

// �C���N���[�h
#include "StageObject.h"

// effekseer: �C���N���[�h
#include "../../../EffekseeLib/EffekseerVFX.h"
#include "../../../Engine/Global.h"

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
	StageObject* shooter_;	// ���˂����I�u�W�F�N�g
	int power_;

	// effekseer: �ό`�s��
	std::shared_ptr<EFFEKSEERLIB::EFKTransform> effectModelTransform_;/*������*/
	EffectData data_;

	float lifeTime_;	// ��������
	float colliderRadius_;	// �����蔻��̔��a
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
	void OnCollision(GameObject* _target, Collider* _collider) override;

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

	/// <summary>
	/// �Ȃ�̃I�u�W�F�N�g�����˂�������ݒ�
	/// </summary>
	/// <param name="_shooter"></param>
	void SetShooter(StageObject* _shooter) { shooter_ = _shooter; }

	void SetEffectData(EffectData _data) { data_ = _data; }

	void SetLifeTime(float _time) { lifeTime_ = _time; }

	void SetPower(int _power) { power_ = _power; }

	void SetColliderRadius(float _radius) { colliderRadius_ = _radius; }
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
};

Bullet* CreateBullet(GameObject* _parent, EffectData _data, float _colderRadius); 
Bullet* CreateBullet(GameObject* _parent,EffectData _data);