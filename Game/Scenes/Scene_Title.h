#pragma once

#include"../../Engine/GameObject/GameObject.h"

class Scene_Title:public GameObject
{
private:
	bool isFirstSelectButton_;
public:
	/// <summary> �R���X�g���N�^ </summary>
	Scene_Title(GameObject* parent);

	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> �`�� </summary>
	void Draw() override;

	/// <summary> ��� </summary>
	void Release() override;
};

