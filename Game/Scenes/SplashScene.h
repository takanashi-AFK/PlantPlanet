#pragma once

// �C���N���[�h
#include "../../Engine/GameObject/GameObject.h"

// �X�v���b�V���V�[�����Ǘ�����N���X
class SplashScene : public GameObject
{
private:
	int imageHandle_;	// �摜�ԍ�
	float count_;
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_parent">�e�I�u�W�F�N�g</param>
	SplashScene(GameObject* _parent);

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

