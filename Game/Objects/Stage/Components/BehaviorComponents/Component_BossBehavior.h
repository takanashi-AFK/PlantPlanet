#pragma once

//�C���N���[�h
#include "../Component.h"

// effekseer: �C���N���[�h
#include "../../../../../EffekseeLib/EffekseerVFX.h"

enum BossState{
	BOSS_STATE_IDLE = 0,
	BOSS_STATE_SHOT,
	BOSS_STATE_TACKLE,
	BOSS_STATE_DEAD,
	BOSS_STATE_MAX
};

class Component_BossBehavior : public Component
{
private:
	BossState bNowState_;	// ���݂̏��

	StageObject* target_;	// �Ώ�
	string targetName_;		// �Ώۂ̖��O

	float shotInterval_;		// �ˌ��Ԋu
	float stateChangeDelay_;	// ��ԕύX�x��
	float shootHeight_;			// �ˌ��̍���
	float tackleDamage_;		// �ːi�̃_���[�W

	bool isActive_;				// �A�N�e�B�u�t���O
	bool isGameStart_;			// �Q�[���J�n�t���O
	bool isDeadStart_;			// ���S�J�n�t���O



	std::shared_ptr<EFFEKSEERLIB::EFKTransform> effectModelTransform_; // effekseer: ���f���̈ʒu���
	EFFEKSEERLIB::EFKTransform effekseerTransform;// effekseer: �G�t�F�N�g�̈ʒu���

public:
	/// <summary> �R���X�g���N�^ </summary>
	Component_BossBehavior(string _name, StageObject* _holder, Component* _parent);

	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> ��� </summary>
	void Release() override;

	/// <summary> �Փˏ��� </summary>
	void OnCollision(GameObject* _target, Collider* _collider) override;

	/// <summary> �ۑ� </summary>
	void Save(json& _saveObj) override;

	/// <summary> �Ǎ� </summary>
	void Load(json& _loadObj) override;

	/// <summary> ImGui�p�l���\�� </summary>
	void DrawData() override;

	/// <summary>�X�e�[�g��Ԃ�</summary>
	BossState GetState() const { return (BossState)bNowState_; }

/*
setter :*/
	/// <param name="_state"> �{�X�̏�� </param>
	void SetState(BossState _state) { bNowState_ = _state; }
/*
predicate:*/
	bool IsDead();

private:
	/// <summary> �����_���ɏ�Ԃ𒊑I���鏈�� </summary>
	BossState RandomStatePick();

	/// <summary> �ˌ���Ԏ��̏��� </summary>
	void Shot();

	/// <summary> �ːi��Ԏ��̏��� </summary>
	void Tackle();

	/// <summary> �ҋ@��Ԏ��̏��� </summary>
	void Idle();	

	/// <summary> ���S��Ԏ��̏��� </summary>
	void Dead();
};