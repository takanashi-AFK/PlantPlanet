#pragma once

// �C���N���[�h
#include "../../Engine/GameObject/GameObject.h"

// �X�v���b�V���V�[�����Ǘ�����N���X
class Scene_Splash : public GameObject
{
private:
	int imageHandle_;	// �摜�ԍ�
	float count_;		// �o�ߎ���
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_parent">�e�I�u�W�F�N�g</param>
	Scene_Splash(GameObject* _parent);

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

