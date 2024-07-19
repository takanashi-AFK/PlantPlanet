#pragma once

//�C���N���[�h
#include"../Component.h"
#include <DirectXMath.h>
using namespace DirectX;


/// <summary>
/// �^�[�Q�b�g�ɑ΂��ĒǏ]����R���|�[�l���g�̃N���X
/// </summary>
class Component_TackleMove :public Component
{
private:
	bool isActive_;				// �Ǐ]���L�����ǂ���
	XMVECTOR direction_;		// �ړ�����
	float distance_;			// �ړ�����
	float speed_;				// �ړ����x
	string easingType_;			// �C�[�W���O�̎��

	float rate_;				// �C�[�W���O�̊���
	bool isFirstMove_;			// ����ړ����ǂ���
	XMFLOAT3 startPosition_;	// ����J�n�̈ʒu

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_holder">�ۗL��</param>
	Component_TackleMove(string _name, StageObject* _holder, Component* _parent);

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
	/// <param name="_saveobj">�f�[�^�̕ۑ����</param>
	void Save(json& _saveObj) override;

	/// <summary>
	/// �Ǎ�
	/// </summary>
	/// <param name="_loadobj">�f�[�^�̓Ǎ����</param>
	void Load(json& _loadObj) override;

	/// <summary>
	/// �ړ�������ݒ�
	/// </summary>
	/// <param name="_direction">����</param>
	void SetDirection(XMVECTOR _direction) { direction_ = _direction; }

	/// <summary>
	/// �ړ�������ݒ�
	/// </summary>
	/// <param name="_direction">����</param>
	void SetDistance(float _distance) { distance_ = _distance; }

	/// <summary>
	/// ImGui�p�l���\��
	/// </summary>
	void DrawData()override;

	void Execute() { isActive_ = true; }
	void Stop() { isActive_ = false; }

	/// <summary>
	/// ���s����Ă��邩�ǂ���
	/// </summary>
	bool IsActived() { return isActive_; }
};