#pragma once
//�C���N���[�h
#include"../Component.h"

//�쐬��-����
class Component_HealthManager :public Component {

private:

	int max_;
	int hp_;

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
	void SetHP(int _newHP);

	/// <summary>
	/// �_���[�W���󂯂��ۂ̌v�Z
	/// </summary>
	/// <param name="_damageValue">�_���[�W�l</param>
	void TakeDamage(int _damageValue);

	/// <summary>
	/// �񕜂̌v�Z
	/// </summary>
	/// <param name="_healValue">�񕜒l</param>
	void Heal(int _healValue);



};