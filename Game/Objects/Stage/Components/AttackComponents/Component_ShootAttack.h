#pragma once

// �C���N���[�h
#include "Component_Attack.h"
#include <DirectXMath.h>
#include "../../../Engine/Global.h"

using namespace DirectX;

/// <summary>
/// ���Z���������U���R���|�[�l���g
/// </summary>
class Component_ShootAttack :public Component_Attack
{
private:
	float shootingSpeed_;			// �e�̑��x
	XMVECTOR shootingDirection_;	// �e�̕���
	XMFLOAT3 shootingPosition_;		// �e�̔��ˈʒu

	bool isShootPositionSet_;		// ���ˈʒu���ݒ肳��Ă��邩
	float bulletLifeTime_;				// �e�̐�������
	EffectData effectData;				// �G�t�F�N�g�f�[�^
	float bulletColliderRadius_;	// �e�̓����蔻��̔��a

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�R���|�[�l���g�̕ێ���</param>
	Component_ShootAttack(string _name, StageObject* _holder, Component* _parent);
	
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

/*
setter :*/
	/// <param name="_speed"> �e�̑��x</param>
	void SetShootingSpeed(float _speed) { shootingSpeed_ = _speed; }
	
	/// <param name="_direction"> �e�̕���</param>
	void SetShootingDirection(XMVECTOR _direction) { shootingDirection_ = _direction; }
	
	/// <param name="_position"> �e�̔��ˈʒu</param>
	void SetShootingPosition(XMFLOAT3 _position) { shootingPosition_ = _position; isShootPositionSet_ = true;}
	
	/// <param name="_data"> �G�t�F�N�g�f�[�^</param>
	void SetEffectData(EffectData _data) { effectData = _data; }
	
	/// <param name="_time"> �e�̐�������</param>
	void SetBulletLifeTime(float _time) { bulletLifeTime_ = _time; }
	
	/// <param name="_radius"> �e�̓����蔻��̔��a</param>
	void SetBulletColliderRadius(float _radius) { bulletColliderRadius_ = _radius; }
/*
getter :*/
	/// <returns> �e�̑��x</returns>
	float GetShootingSpeed() const { return shootingSpeed_; }
	
	/// <returns> �e�̕���</returns>
	XMVECTOR GetShootingDirection() const { return shootingDirection_; }
	
	/// <returns> �e�̔��ˈʒu</returns>
	XMFLOAT3 GetShootingPosition() const { return shootingPosition_; }
	
	/// <returns> �G�t�F�N�g�f�[�^</returns>
	float GetBulletLifeTime() const { return bulletLifeTime_; }
	
	/// <returns> �e�̐�������</returns>
	float GetBulletColliderRadius() const { return bulletColliderRadius_; }
};

