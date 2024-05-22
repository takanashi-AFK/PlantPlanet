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

	// �����̌v�Z
	XMVECTOR CalcDirection(XMFLOAT3 _holderPos, XMFLOAT3 _targetPos);

	// �ړ�
	void Move(XMVECTOR _direction);
	
	// ���������߂�
	float CalcDistance(XMFLOAT3 _holderPos, XMFLOAT3 _targetPos);
	
	// �z���_�[�̌�������]������
	float CalcRotateAngle(XMVECTOR _direction);

	// �^�[�Q�b�g�̐ݒ�
	void SetTarget(StageObject* _target) { target_ = _target; }
};

