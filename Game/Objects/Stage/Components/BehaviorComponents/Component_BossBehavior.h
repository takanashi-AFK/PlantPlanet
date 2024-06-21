#pragma once

//�C���N���[�h
#include "../Component.h"

// effekseer: �C���N���[�h
#include "../../../../../EffekseeLib/EffekseerVFX.h"

class Component_BossBehavior : public Component
{
private:

	enum STATE { SHOT, TACKLE, SHOCK, WAIT, };	// ���
	STATE nowState_;					// ���݂̏��	
	STATE prevState_;					// �P�t���[���O�̏��

	StageObject* target_;
	string targetName_;

	float shotrange_;
	float tacklerange_;

	float shotRate_;
	float rotateSpeed_;
	float nextStateTime_;

	bool isActive_;

	// effekseer: �ό`�s��
	std::shared_ptr<EFFEKSEERLIB::EFKTransform> mt;/*������*/


	void RandomTransition(); // �����_���J�ڂ̂��߂̃��\�b�h
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>	
	/// <param name="_holder">�ۗL��</param>
	Component_BossBehavior(string _name, StageObject* _holder, Component* _parent);

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
	void OnCollision(GameObject* _target) override;

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

	void Shot();

	void Tackle();

	void Shock();

	void Wait();					// �ҋ@
};