#pragma once

//�C���N���[�h
#include"../Component.h"
#include <DirectXMath.h>
using namespace DirectX;


/// <summary>
/// �^�[�Q�b�g�ɑ΂��ĒǏ]����R���|�[�l���g�̃N���X
/// </summary>
class Component_Chase:public Component
{
private:
	StageObject* target_;	//�Ǐ]�̑ΏۃI�u�W�F�N�g
	string targetName_;		//�Ǐ]�̑ΏۃI�u�W�F�N�g��

	float speed_;			//���x
	float limitDistance_;	//�Ǐ]�̌��E����
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	Component_Chase(string _name, StageObject* _holder, Component* _parent);

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
	void Save(json& _saveObj) override;

	/// <summary>
	/// �Ǎ�
	/// </summary>
	/// <param name="_loadobj">�f�[�^�̓Ǎ����</param>
	void Load(json& _loadObj) override;

	/// <summary>
	/// ImGui�p�l���\��
	/// </summary>
	void DrawData()override;

	/// <summary>
	/// �^�[�Q�b�g�̐ݒ�
	/// </summary>
	void SetTarget(StageObject* _target) { target_ = _target; }

	/// <summary>
	/// �ǐՂ���߂鋗���̐ݒ�
	/// </summary>
	/// <returns></returns>
	float GetLimitDistance() { return limitDistance_; }

private:

	/// <summary>
	/// �����̌v�Z
	/// </summary>
	XMVECTOR CalcDirection(XMFLOAT3 _holderPos, XMFLOAT3 _targetPos);

	/// <summary>
	/// ���������߂�
	/// </summary>
	float CalcDistance(XMFLOAT3 _holderPos, XMFLOAT3 _targetPos);
	
	/// <summary>
	/// �z���_�[�̌�������]������
	/// </summary>
	float CalcRotateAngle(XMVECTOR _direction);
	
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move(XMVECTOR _direction);
};

