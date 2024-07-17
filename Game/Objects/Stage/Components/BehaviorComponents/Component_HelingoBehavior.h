#pragma once
#include "../Component.h"

class Component_HelingoBehavior : public Component
{
private:
	StageObject* target_;	// �^�[�Q�b�g
	string targetName_;		// �^�[�Q�b�g��

	float fallrange_;		// �����͈�
	float discoveryrange_;			// �����͈�
	bool isTimerActive_;	// �^�C�}�[���A�N�e�B�u���ǂ���

	bool oneHit_;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>	
	/// <param name="_holder">�ۗL��</param>
	Component_HelingoBehavior(string _name, StageObject* _holder, Component* _parent);
	
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
	/// �Փˏ���
	/// </summary>
	void OnCollision(GameObject* _target, Collider* _collider) override;

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
};

