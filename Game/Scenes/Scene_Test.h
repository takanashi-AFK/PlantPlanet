#pragma once

// �C���N���[�h
#include "../../Engine/GameObject/GameObject.h"


//�O���錾
class UIButton;

/// <summary>
/// �e�X�g�V�[�����Ǘ�����N���X
/// </summary>
class Scene_Test : public GameObject
{

private:

	//�e�X�g��
	//�{�^���N���X�̃C���X�^���X
	UIButton* button;

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="parent">�e�I�u�W�F�N�g</param>
	Scene_Test(GameObject* parent);

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