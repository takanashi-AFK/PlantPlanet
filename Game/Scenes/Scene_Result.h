#pragma once
#include"../../Engine/GameObject/GameObject.h"

class Scene_Result :public GameObject
{
	int scoreNum_;
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

