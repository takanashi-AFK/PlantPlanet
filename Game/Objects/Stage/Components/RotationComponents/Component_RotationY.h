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
	/// <param name="_holeder">�ۗL��</param>
	Component_RotationY(StageObject* _holeder);
	
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
	/// ImGui�\��
	/// </summary>
	void DrawData() override;
};

