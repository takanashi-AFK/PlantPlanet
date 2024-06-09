#pragma once
#include"../../Engine/GameObject/GameObject.h"

class UIButton;
class UIPanel;


class Scene_End :public GameObject
{
private:
	//�{�^���N���X�̃C���X�^���X
	UIButton* button;
	//UI�p�l���N���X�̃C���X�^���X
	UIPanel* panel;

public:
	//�R���X�g���N�^
	Scene_End(GameObject* parent_);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

};

