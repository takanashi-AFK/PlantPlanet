#pragma once

#include"../../Engine/GameObject/GameObject.h"

class UIPanel;

class Scene_Title:public GameObject
{
	UIPanel* pUIPanel_;
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

