#pragma once

// �C���N���[�h
#include "../../Engine/GameObject/GameObject.h"

// �O���錾
class Stage;

//�e�X�g�V�[�����Ǘ�����N���X
class TestScene : public GameObject
{
private:
	Stage* pStage_;	//�X�e�[�W�N���X�ւ̃|�C���^

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="parent">�e�I�u�W�F�N�g</param>
	TestScene(GameObject* parent);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �J��
	/// </summary>
	void Release() override;
};