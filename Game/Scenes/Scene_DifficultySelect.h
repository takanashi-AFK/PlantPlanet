#pragma once
#include"../../Engine/GameObject/GameObject.h"

class Scene_DifficultySelect :public GameObject
{
public:
	/// <summary> �R���X�g���N�^ </summary>
	Scene_DifficultySelect(GameObject* parent);

	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> �`�� </summary>
	void Draw() override;

	/// <summary> ��� </summary>
	void Release() override;
};

