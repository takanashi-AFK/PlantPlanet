#pragma once
#include "StageObject.h"

// �X�J�C�X�t�B�A���Ǘ�����N���X
class SkySphere : public StageObject
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_parent">�e�I�u�W�F�N�g</param>
	SkySphere(GameObject* _parent);

	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;
};

