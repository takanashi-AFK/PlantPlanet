#pragma once

#include"../../Engine/GameObject/GameObject.h"

class UIButton;
class UIPanel;

class Scene_Title:public GameObject
{
private:
	//�{�^���N���X�̃C���X�^���X
	UIButton* button;
	//UI�p�l���N���X�̃C���X�^���X
	UIPanel* panel;
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

