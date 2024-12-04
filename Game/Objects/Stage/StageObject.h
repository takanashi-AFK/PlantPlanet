#pragma once

// �C���N���[�h
#include "../../../Engine/DirectX/Direct3D.h"
#include "../../../Engine/GameObject/GameObject.h"
#include "Components/Component.h"
#include <vector>

#ifdef _DEBUG
#include<mutex>
#endif

// using�錾
using std::vector;


class StageObject : public GameObject
{
protected:
	vector<Component*> myComponents_;   // ���g���ۗL����R���|�[�l���g�Q

	string modelFilePath_;              // ���f���̃t�@�C���p�X
	int modelHandle_;                   // ���f���ԍ�

	bool isOnGround_;                   // �n�ʂɐݒu���邩�ǂ���
	bool isCollisionWall_;              // �ǂɓ����������ǂ���
	bool isColliding_;                  // �ق��̃I�u�W�F�N�g�ƏՓ˂��邩�ǂ���
	XMFLOAT3 onGroundOffset_;           // �n�ʂɐݒu����ۂ̃I�t�Z�b�g
	XMFLOAT3 raycastDirection_;
	float fallSpeed_;                   // �������x

	Direct3D::SHADER_TYPE shaderType_;   // �V�F�[�_�[�^�C�v

#ifdef _DEBUG
	std::mutex mutex_;
#endif

public:
	/// <summary> �R���X�g���N�^ </summary>
	StageObject(string _name,string _modelFilePath,GameObject* _parent);

	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> �`�� </summary>
	void Draw() override;

	/// <summary> ��� </summary>
	void Release() override;

	/// <summary> �Փˏ��� </summary>
	void OnCollision(GameObject* _target, Collider* _collider) override;

	/// <summary> �ۑ� </summary>
	virtual void Save(json& _saveObj);

	/// <summary> �Ǎ� </summary>
	virtual  void Load(json& _loadObj);

	/// <summary> ImGui�\�� </summary>
	virtual void DrawData();

	/// <summary> �R���|�[�l���g�̒ǉ� </summary>
	bool AddComponent(Component* _comp);

	/// <summary> �R���|�[�l���g�̍폜 </summary>
	bool DeleteComponent(Component* _comp);

	/// <summary> �S�R���|�[�l���g�̍폜 </summary>
	bool DeleteAllComponent();

	/// <summary> �R���|�[�l���g��T�� </summary>
	Component* FindComponent(string _name);
	vector<Component*> FindComponent(ComponentType _type);

	/// <summary> �A�j���[�V�������Đ� </summary>
	void PlayAnimation(int _startFrame, int _endFrame, float _speed);
/*
getter :*/
	/// <summary> ���f���ԍ��̎擾 </summary>
	int GetModelHandle() const { return modelHandle_; }

	/// <summary> ���f���t�@�C���p�X�̎擾 </summary>
	string GetModelFilePath() const { return modelFilePath_; }

	/// <summary> �n�ʂɐݒu����ۂ̃I�t�Z�b�g�̎擾 </summary>
	XMFLOAT3 GetOnGroundOffset() const { return onGroundOffset_; }

	XMFLOAT3 GetRayCastDirection()const { return this->raycastDirection_; };

	/// <summary> �������x�̎擾 </summary>
	float GetFallSpeed() const { return fallSpeed_; }

	/// <summary> �V�F�[�_�[�^�C�v�̎擾 </summary>
	Direct3D::SHADER_TYPE GetShaderType() const { return shaderType_; }

	/// <summary> �Փ˂��邩�ǂ������擾 </summary>
	bool GetIsColliding() { return isColliding_; }
/*
setter :*/
	/// <summary> ���f���ԍ��̐ݒ� </summary>
	void SetModelHandle(int _handle) { modelHandle_ = _handle; }

	/// <summary> ���f���t�@�C���p�X�̐ݒ� </summary>
	void SetModelFilePath(string _path) { modelFilePath_ = _path; }

	/// <summary> �n�ʂɐݒu����ۂ̃I�t�Z�b�g�̐ݒ� </summary>
	void SetOnGroundOffset(XMFLOAT3 _offset) { onGroundOffset_ = _offset; }

	/// <summary> �Փ˂��邩�ǂ�����ݒ� </summary>
	void SetIsColliding(bool _isColliding) { isColliding_ = _isColliding; }

	/// <summary> �������x�̐ݒ� </summary>
	void SetFallSpeed(float _speed) { fallSpeed_ = _speed; }

	/// <summary> �V�F�[�_�[�^�C�v�̐ݒ� </summary>
	void SetShader(Direct3D::SHADER_TYPE _type) { shaderType_ = _type; };

	void SetRayCastDirection(XMFLOAT3 dir) { this->raycastDirection_ = dir; };
/*
predicate :*/
	/// <summary> �n�ʂɐݒu���邩�ǂ����𔻒� </summary>
	bool IsOnGround() const { return isOnGround_; }

	/// <summary> �ǂɓ����������ǂ����𔻒�</summary>
	bool IsCollisionWall() const { return isCollisionWall_; }

private:
	/// <summary> �ڒn���� </summary>
	void OnGround(float _fallSpeed);

	/// <summary> �ǂ߂荞�ݖh�~���� </summary>
	void CollisionWall();
};

/// <summary>
/// �X�e�[�W�I�u�W�F�N�g�𐶐�����֐�
/// </summary>
/// <param name="_name"> �I�u�W�F�N�g��</param>
/// <param name="_modelFilePath"> ���f���t�@�C���p�X</param>
/// <param name="_parent"> �e�I�u�W�F�N�g</param>
/// <returns>�쐬�����I�u�W�F�N�g�̃|�C���^</returns>
StageObject* CreateStageObject(string _name, string _modelFilePath, GameObject* _parent);