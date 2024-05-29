#pragma once

// �C���N���[�h
#include "../Component.h"

// Y����]���s���R���|�[�l���g�̃N���X
class Component_RotationY : public Component
{
private:
	float rotateSpeed_;	// ��]���x
	float angle_;		// �p�x

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	Component_RotationY(StageObject* _holder);
	
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

	
	/// <returns>
	///��]�̑��x���擾
	///</returns>
	float GetRotateSpeed() { return rotateSpeed_; }

	
	/// <returns>
	///�A���O�����擾
	/// </returns>
	float GetAngle() { return angle_; }

	/// <summary>
	/// ��]���x��ݒ�
	/// </summary>
	/// <param name="_direction">��]���x</param>
	void SetRotationSpeed(float _rotateSpeed) { rotateSpeed_ = _rotateSpeed; }

	/// <summary>
	/// �A���O����ݒ�
	/// </summary>
	/// <param name="_angle">�A���O��</param>
	void SetAngle(float _angle) { angle_ = _angle; }

};

