#include "Scene_Test.h"

// �C���N���[�h
#include "../Objects/Stage/SkySphere.h"
#include "../Objects/Stage/Stage.h"
#include "../Otheres/StageEditor.h"

//UIButton�ǉ��C���N���[�h
#include "../Objects/UI/UIButton.h"
#include "../../Engine/DirectX/Input.h"
#include "../../Engine/ImGui/imgui.h"

//UIPanel�ǉ��C���N���[�h
#include "../Objects/UI/UIPanel.h"

Scene_Test::Scene_Test(GameObject * parent)
	: GameObject(parent, "Scene_Test"), panel(nullptr)
{
}

void Scene_Test::Initialize()
{
	// �X�J�C�X�t�B�A��W���Ő���
	Instantiate<SkySphere>(this);

	panel = Instantiate<UIPanel>(this);
	panel->Initialize();

	//UIButton�N���X�̃C���X�^���X�𐶐�
	UIButton * button = new UIButton(panel);//UIButton��UIPanel�̎q�I�u�W�F�N�g�ɐݒ�
	button->Initialize();

	//UIButton��UIPanel�̎q�I�u�W�F�N�g�ɒǉ�
	panel->AddChild(button);


	//button = Instantiate<UIButton>(this);

	/*

	// �X�e�[�W���쐬
	Stage* pStage = Instantiate<Stage>(this);

#ifdef _DEBUG

	// �X�e�[�W�G�f�B�^�[�����s
	StageEditor* pSe = Instantiate<StageEditor>(this); 

	// �ҏW�Ώۂ�ݒ�
	pSe->SetEditTarget(pStage);
	
#endif // _DEBUG

	*/
}


void Scene_Test::Update()
{
	panel->Update();
}

void Scene_Test::Draw()
{
	panel->Draw();
}

void Scene_Test::Release()
{

}