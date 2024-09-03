#pragma once

// �C���N���[�h
#include"../../Engine/GameObject/GameObject.h"
#include "../Objects/UI/UIPanel.h"
#include "../Objects/UI/UIImage.h"

// �O���錾
class Stage;
class CountDown;
class TPSCamera;
class StageObject;
class Component;
class Scene_Play:public GameObject
{
private:
	Stage* pStage_;

	CountDown* countDown_;
	TPSCamera* tpsCamera_;
	StageObject* player_;

	Component* phm;
	Component* bhm;
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

