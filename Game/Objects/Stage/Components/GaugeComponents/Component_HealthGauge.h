#pragma once
//�C���N���[�h
#include"Component_Gauge.h"

/// <summary>
/// �̗͂̑������Ǘ�����R���|�[�l���g
/// </summary>
class Component_HealthGauge :public Component_Gauge
{
private:

	float maxHp_; //�̗͂̏��
	float nowHp_;  //�̗�
	
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	Component_HealthGauge(string _name, StageObject* _holder, Component* _parent);

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

	/// <summary>
	/// �_���[�W���󂯂��ۂ̌v�Z
	/// </summary>
	/// <param name="_damageValue">�_���[�W�l</param>
	void TakeDamage(float _damageValue);

	/// <summary>
	/// �񕜂̌v�Z
	/// </summary>
	/// <param name="_healValue">�񕜒l</param>
	void Heal(float _healValue);

	/// <summary>
	/// �̗͂̃��Z�b�g(�ő�l�ɖ߂�)
	/// </summary>
	void Reset();

	
};