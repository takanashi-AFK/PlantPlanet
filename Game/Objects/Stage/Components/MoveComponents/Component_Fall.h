#pragma once

//�C���N���[�h
#include "../Component.h"
#include <DirectXMath.h>

/// <summary>
/// �I�u�W�F�N�g�̍~���E�㏸���s���R���|�[�l���g�̃N���X
/// </summary>
class Component_Fall : public Component
{
private:
	
	float fallSpeed_;		//�~�����x
	float riseSpeed_;		//�㏸���x
	float fallDistance_;	//�~��������܂ł̋���

	DirectX::XMFLOAT3 startRisePosition_; //�㏸���n�߂̈ʒu
	DirectX::XMFLOAT3 startFallPosition_; //�~�����n�߂̈ʒu
	
	bool isFirstTime_ = true;	//�ŏ��̏�Ԃ�ۑ����邩
	bool isFalling_ = false;	//�������邩�ǂ���
	bool isActive_;				//���s���ꂽ���ǂ���
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
	void Start() { isActive_ = true; }

	/// <summary>
	/// �~���̒�~
	/// </summary>
	void Stop() { isActive_ = false; }

	/// <summary>
	/// ���s����Ă��邩�ǂ���
	/// </summary>
	void Execute() { isActive_ = true; }

	bool IsActived() { return isActive_; }

	bool IsFalling() { return isFalling_; }
};

