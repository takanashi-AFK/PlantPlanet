#pragma once
#include"../../Engine/GameObject/GameObject.h"

class UIPanel;
class Scene_Result :public GameObject
{
private:
	UIPanel* panel;

public:
	//�R���X�g���N�^
	Scene_Result(GameObject* parent_);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

};

