#pragma once
#include"../../Engine/GameObject/GameObject.h"

class Scene_End :public GameObject
{
private:
	float count_;		// �o�ߎ���

public:
	/// <summary> �R���X�g���N�^ </summary>
	Scene_End(GameObject* parent_);

	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> �`�� </summary>
	void Draw() override;

	/// <summary> ��� </summary>
	void Release() override;
};


