#pragma once
#include "../../Engine/GameObject/GameObject.h"

class Scene_Ranking : public GameObject
{
private:
	/// <summary> �R���X�g���N�^ </summary>
	Scene_Ranking(GameObject* _parent);

	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> �`�� </summary>
	void Draw() override;

	/// <summary> ��� </summary>
	void Release() override;
};

