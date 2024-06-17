#pragma once

// �C���N���[�h
#include"../../Engine/GameObject/GameObject.h"

// �O���錾
class Component_HealthManager;

class Scene_Play:public GameObject
{
private:
	Component_HealthManager* playerHealth_; //�v���C���[��HP���Ǘ�����R���|�[�l���g

public:
	//�R���X�g���N�^
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

