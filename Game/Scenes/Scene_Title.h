#pragma once

#include"../../Engine/GameObject/GameObject.h"

class Scene_Title:public GameObject
{
public:
	//�R���X�g���N�^
	Scene_Title(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};

