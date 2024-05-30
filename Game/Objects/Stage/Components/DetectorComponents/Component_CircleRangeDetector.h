#pragma once
// �C���N���[�h
#include "../Component.h"

/// <summary>
/// �~�`�͈͂ɓ������Ώۂ����o����R���|�[�l���g
/// </summary>
class Component_CircleRangeDetector : public Component
{
private:
	
	StageObject* target_;	// ���o�Ώ�
	float radius_;			// ���o�͈͂̔��a
	bool isContains_;		// ���o�t���O

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	Component_CircleRangeDetector(string _name, StageObject* _holder);

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
	
	/// <param name="_radius">
	/// �{���͈͂̔��a
	/// </param>
	void SetRadius(float _radius) { radius_ = _radius; }

	/// <param name="_target">
	/// �{���Ώ�
	/// </param>
	void SetTarget(StageObject* _target) { target_ = _target; }

	/// <returns>
	/// �{���͈͂̔��a
	/// </returns>
	float GetRadius() { return radius_; }

	/// <returns>
	/// �{���Ώ�
	/// </returns>
	StageObject* GetTarget() { return target_; }

	/// <returns>
	/// �{���Ώۂ��{���͈͓��ɂ�����true
	/// </returns>
	bool IsContains();

};

