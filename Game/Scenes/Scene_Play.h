#pragma once
#include"../../Engine/GameObject/GameObject.h"

class Component_HealthManager;

class Scene_Play:public GameObject
{
	Component_HealthManager* health_;

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

