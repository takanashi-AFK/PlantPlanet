#pragma once
#include "../Component.h"
#include <DirectXMath.h>
#include "../../StageObject.h"
#include <vector>
#include "../../../../../Engine/ResourceManager/Model.h"
#include "../../../EffekseeLib/EffekseerVFX.h"/*������*/
#include "../../../../../Engine/Global.h"


// �O���錾
class CountDown;
class Component_BossBehavior;
// �v���C���[�̏��
enum PlayerState {
	PLAYER_STATE_IDLE = 0,	/* �ҋ@ */
	PLAYER_STATE_WALK,		/* ���s */
	PLAYER_STATE_SHOOT,		/* �ˌ� */
	PLAYER_STATE_DODGE,		/* ��� */
	PLAYER_STATE_DEAD,		/* ���S */
	PLAYER_STATE_SHOOT_WALK_LEFT,/*��������ɏe���\���č��ɓ���*/
	PLAYER_STATE_SHOOT_WALK_RIGHT,/*��������ɏe���\���ĉE�ɓ���*/
	PLAYER_STATE_SHOOT_WALK_BACK,/*��������ɏe���\���Č���ɓ���*/
	PLAYER_STATE_SHOOT_WALK_FORWARD,/*��������ɏe���\���đO���ɓ���*/
	PLAYER_STATE_MAX
};

class Component_PlayerBehavior : public Component
{
private:
	PlayerState nowState_, prevState_;	// ���݂̏�ԁA�O�̏��
	float shootHeight_;					// �ˌ��̍���
	float stamina_;						// �X�^�~�i
	XMFLOAT3 prevAngles_;				// �O�t���[���̉�]
	Component_BossBehavior* bossBehavior;
	int invincibilityFrame_;			// ���G�t���[��
	int walkingFrame_;					// �A�����ĕ����Ă��鎞��
	int lockRotateTime_;				// ��]���Œ肷�鎞��

	// effekseer: �ό`�s��
	std::shared_ptr<EFFEKSEERLIB::EFKTransform> effectModelTransform;/*������*/
	EffectData effectData_;

	bool isShootStart_ : 1;					// �ˌ��J�n�t���O
	bool isDodgeStart_ : 1;					// ����J�n�t���O
	bool isGameStart_  : 1;					// �Q�[���J�n�t���O
	bool isUseStamina_ : 1;					// �X�^�~�i���g�p������
	bool isShootAttack_: 1;					

public:
	/// <summary> �R���X�g���N�^ </summary>
	Component_PlayerBehavior(string _name,StageObject* _holder,Component* _parent);

	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> ��� </summary>
	void Release() override;
	
	/// <summary> �ۑ� </summary>
	void Save(json& _saveObj) override;

	/// <summary> �Ǎ� </summary>
	void Load(json& _loadObj) override;

	/// <summary> ImGui�p�l���\�� </summary>
	void DrawData() override;

/*
setter :*/
	/// <param name="_state"> �v���C���[�̏�� </param>
	void SetState(PlayerState _state) { prevState_ = nowState_; nowState_ = _state; }

	/// <param name="_height"> �ˌ��̍��� </param>
	void SetShootHeight(float _height) { shootHeight_ = _height; }

	/// <param name="_flag"> �Q�[���J�n�t���O </param>
	void SetGameStart(bool _flag) { isGameStart_ = _flag; }

/*
getter :*/
	/// <returns> �v���C���[�̏�� </returns>
	PlayerState GetState() const { return nowState_; }

	/// <returns> �O�̃v���C���[�̏�� </returns>
	PlayerState GetPrevState() const { return prevState_; }

	/// <returns> �ˌ��̍��� </returns>
	float GetShootHeight() const { return shootHeight_; }

	/// <returns> �Q�[���J�n�t���O </returns>
	bool GetGameStart() const { return isGameStart_; }

	float GetLockRotateTime()const { return lockRotateTime_; };
/*
predicate :*/
	/// <returns> �v���C���[������ł��邩 </returns>
	bool IsDead();

	/// <returns> �ˌ����J�n������ </returns>
	bool IsShootStart() const { return isShootStart_; }

	/// <returns> ������J�n������ </returns>
	bool IsDodgeStart() const { return isDodgeStart_; }

	/// <returns> ���݂̏�Ԃ��w�肵����Ԃ� </returns>
	bool IsState(PlayerState _state) const { return nowState_ == _state; }
private:
	/// <summary> �ˌ������̌v�Z </summary>
	XMVECTOR CalcShootDirection();

	/// <summary> �ҋ@��Ԏ��̏��� </summary>
	void Idle();
	
	/// <summary> ���s��Ԏ��̏��� </summary>
	void Walk();
	
	/// <summary> �ˌ���Ԏ��̏��� </summary>
	void Shoot();

	/// <summary> �����Ԏ��̏��� </summary>
	void Dodge();

	/// <summary> ���S��Ԏ��̏��� </summary>
	void Dead();
};

