#pragma once

//�C���N���[�h
#include"../Component.h"
#include <DirectXMath.h>
using namespace DirectX;


//�쐬��:�����B�l
class Component_Chase:public Component
{
private:
	StageObject* target_;	//�Ǐ]�̑ΏۃI�u�W�F�N�g
	float speed_;			//���x
	bool isActive_;			//�Ǐ]���L�����ǂ���
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	Component_Chase(StageObject* _holder);

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

	// �^�[�Q�b�g�̐ݒ�
	void SetTarget(StageObject* _target) { target_ = _target; }

	// �Ǐ]�̊J�n
	void Start() { isActive_ = true; }

	// �Ǐ]�̒�~
	void Stop() { isActive_ = false; }

	// �Ǐ]���L�����ǂ���
	bool IsActive() { return isActive_; }

private:

	// �����̌v�Z
	XMVECTOR CalcDirection(XMFLOAT3 _holderPos, XMFLOAT3 _targetPos);

	// ���������߂�
	float CalcDistance(XMFLOAT3 _holderPos, XMFLOAT3 _targetPos);
	
	// �z���_�[�̌�������]������
	float CalcRotateAngle(XMVECTOR _direction);
	
	// �ړ�
	void Move(XMVECTOR _direction);
};

