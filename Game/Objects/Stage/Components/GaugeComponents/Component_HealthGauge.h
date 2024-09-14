#pragma once
//�C���N���[�h
#include"Component_Gauge.h"

/// <summary>
/// �̗͂̑������Ǘ�����R���|�[�l���g
/// </summary>
class Component_HealthGauge :public Component_Gauge
{
private:
	float prev_;	//�O��̗̑�
	float shaderChangeTime_;	//�V�F�[�_�[��ύX���鎞��
	bool isLock_;	//���b�N�t���O
	bool isTakeDamage_;	//�_���[�W���󂯂���
public:
	/// <summary> �R���X�g���N�^ </summary>
	Component_HealthGauge(string _name, StageObject* _holder, Component* _parent);

	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> ��� </summary>
	void Release() override;
/*
setter :*/
	/// <summary> �_���[�W���󂯂� </summary>
	void TakeDamage(float _damageValue);

	/// <summary> �� </summary>
	void Heal(float _healValue);

	/// <summary> ���b�N </summary>
	void Lock() { isLock_ = true; }

	/// <summary> �A�����b�N </summary>
	void Unlock() { isLock_ = false; }
/*
predicate :*/
	/// <summary> ���S���� </summary>
	bool IsDead() const { return now_ <= 0; }

	/// <summary> ���b�N���� </summary>
	bool IsLock() const { return isLock_; }

	/// <summary> ���b�N������������ </summary>
	bool IsLockAndReduce()const { return isLock_ && isTakeDamage_; }

	/// <summary> �_���[�W���󂯂��� </summary>
	bool IsTakeDamage() const { return isTakeDamage_; }
};