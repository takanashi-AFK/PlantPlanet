#pragma once
#include "../Component.h"

class Component_CircleRangeDetector : public Component
{
private:
	
	StageObject* target_;	// �ΏۃI�u�W�F�N�g
	float radius_;			// ���a
	bool isContains_;		// �����蔻��t���O

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	Component_CircleRangeDetector(StageObject* _holder);

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

// setter
	
	/// <param name="_radius">�{���͈͂̔��a</param>
	void SetRadius(float _radius) { radius_ = _radius; }

	/// <param name="_target">�{���Ώ�</param>
	void SetTarget(StageObject* _target) { target_ = _target; }

// getter

	/// <returns>�{���͈͂̔��a</returns>
	float GetRadius() { return radius_; }

	/// <returns>�{���Ώ�</returns>
	StageObject* GetTarget() { return target_; }

	/// <returns>�{���Ώۂ��{���͈͓��ɂ�����true</returns>
	bool IsContains();

};

