#pragma once

// �C���N���[�h
#include "../../Engine/GameObject/GameObject.h"

class Scene_Test : public GameObject
{
public:
	/// <summary> �R���X�g���N�^ </summary>
	Scene_Test(GameObject* parent);

	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> �`�� </summary>
	void Draw() override;

	/// <summary> ��� </summary>
	void Release() override;
};