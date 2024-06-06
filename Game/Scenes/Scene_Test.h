#pragma once

// �C���N���[�h
#include "../../Engine/GameObject/GameObject.h"


//�e�X�g��
//�O���錾
class UIButton;
class UIPanel;


/// <summary>
/// �e�X�g�V�[�����Ǘ�����N���X
/// </summary>
class Scene_Test : public GameObject
{

private:

	//�e�X�g��
	//�{�^���N���X�̃C���X�^���X
	UIButton* button;

	//UI�p�l���N���X�̃C���X�^���X
	UIPanel* panel;

	//Scene_Test�N���X�̃C���X�^���X
	Scene_Test * scene_test_;

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