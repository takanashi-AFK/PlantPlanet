#pragma once
//�C���N���[�h
#include"../Component.h"

//�쐬��-����,����

/// <summary>
/// �̗͂̑������Ǘ�����R���|�[�l���g
/// </summary>
class Component_HealthManager :public Component 
{
private:

	float max_;
	float hp_;

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	Component_HealthManager(StageObject* _holder);

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
	/// HP�̒l���擾
	/// </summary>
	/// <returns></returns>
	float GetHP() const;

	/// <summary>
	/// HP�̒l��ݒ�
	/// </summary>
	/// <param name="_newHP">�^����ꂽHP</param>
	void SetHP(float _newHP);

	/// <returns>�̗͂̍ő�l</returns>
	float GetMax() const;

	/// <summary>
	/// �̗͂̍ő�l��ݒ�
	/// </summary>
	void SetMax(float _newMax);

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