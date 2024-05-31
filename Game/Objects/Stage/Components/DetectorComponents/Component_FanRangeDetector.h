#pragma once

#include "../Component.h"
#include <DirectXMath.h>

using namespace DirectX;

/// <summary>
/// ��`�͈͂ɓ������Ώۂ����o����R���|�[�l���g
/// </summary>
class Component_FanRangeDetector : public Component
{
private:
	StageObject* target_;	// ���o�Ώ�
	float length_;			// ���o�͈͂̒���
	float angle_;			// ���o�͈͂̊p�x
	XMVECTOR direction_;	// ���o�͈͂̕���
	bool isContains_;		// ���o�t���O

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	Component_FanRangeDetector(string _name, StageObject* _holder, Component* _parent);

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
	/// ���o�͈͓��ɓ����Ă��邩�ǂ���
	/// </returns>
	bool IsContains();
};

