#pragma once
#include"../../Engine/GameObject/GameObject.h"

class Scene_Play:public GameObject
{
public:

	Scene_Play(GameObject* parent);
	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};

