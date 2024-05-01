#pragma once
#include "../../Engine/GameObject/GameObject.h"

class Stage;

//�e�X�g�V�[�����Ǘ�����N���X
class TestScene : public GameObject
{
	Stage* pStage_;
public:
	//�R���X�g���N�^
	//�����Fparent  �e�I�u�W�F�N�g�iSceneManager�j
	TestScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};