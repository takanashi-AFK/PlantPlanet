#pragma once

//�C���N���[�h
#include "../Component.h"
#include <DirectXMath.h>

// using�f�B���N�e�B�u
using namespace DirectX;

/// <summary>
/// �I�u�W�F�N�g�̍~���E�㏸���s���R���|�[�l���g�̃N���X
/// </summary>
class Component_Fall : public Component
{
private:
	enum STATE { FALL, RISE, WAIT, };	// ���
	STATE nowState_ ;					// ���݂̏��	
	STATE prevState_;					// �P�t���[���O�̏��

	float fallSpeed_;					// �~�����x
	float riseSpeed_;					// �㏸���x
	float fallDistance_;				// �~�����鋗��

	float startHeight_;					// �J�n�̍���

	float startRiseHeight_;				// �㏸�J�n�̍���
	float startFallHeight_;				// �~���J�n�̍���
	bool isActive_;						// ���s���ꂽ���ǂ���

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	Component_Fall(string _name, StageObject* _holder, Component* _parent);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �J��
	/// </summary>
	void Release() override;

	/// <summary>
	/// �ۑ�
	/// </summary>
	/// <param name="_saveObj">�ۑ����</param>
	void Save(json& _saveObj) override;

	/// <summary>
	/// �Ǎ�
	/// </summary>
	/// <param name="_loadObj">�Ǎ����</param>
	void Load(json& _loadObj) override;

	/// <summary>
	/// ImGui�p�l���\��
	/// </summary>
	void DrawData() override;

	/// <summary>
	/// �~���̊J�n
	/// </summary>
	void Execute() { isActive_ = true; }

	/// <summary>
	/// �~���̒�~
	/// </summary>
	void Stop() { isActive_ = false; }

	/// <summary>
	/// ���s����Ă��邩�ǂ���
	/// </summary>
	bool IsActived() { return isActive_; }

	void SetState(STATE _state) { prevState_ = nowState_; nowState_ = _state; }

	bool IsFalling() { return nowState_==FALL ? true:false; }
private:
	void FallMove(float& _height);	//�~��
	void RiseMove(float& _height);	//�㏸
	void Wait();	//�ҋ@
};

