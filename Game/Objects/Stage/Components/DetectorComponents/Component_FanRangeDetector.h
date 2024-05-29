#pragma once

#include "../Component.h"
#include <DirectXMath.h>

using namespace DirectX;

class Component_FanRangeDetector : public Component
{
private:
	StageObject* target_;	// �ΏۃI�u�W�F�N�g
	float length_;			// ����
	float angle_;			// �p�x
	XMVECTOR direction_;	// ����
	bool isContains_;		// �����蔻��t���O

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	Component_FanRangeDetector(StageObject* _holder);

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

	// �͈͓��ɓ����Ă��邩�ǂ����𔻒�
	bool IsContains();
};

