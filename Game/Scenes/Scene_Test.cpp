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
	: GameObject(parent, "Scene_Test"), panel(nullptr), button(nullptr)
{

}

void Scene_Test::Initialize()
{
	// �X�J�C�X�t�B�A��W���Ő���
	Instantiate<SkySphere>(this);

	//UIPanel�N���X�̃C���X�^���X�𐶐�
	//panel = Instantiate<UIPanel>(this); //this ��Scene_Test�̂��Ƃ��w���Ă�
	//button = Instantiate<UIButton>(panel);//button��UIPanel�̐e�ɐݒ肷��

	////�p�l���̃��X�g��button��ǉ�
	//panel->AddChild(button);


	

	// �X�e�[�W���쐬
	Stage* pStage = Instantiate<Stage>(this);
	
	// �X�e�[�W��ǂݍ���
	json stageData;
	JsonReader::Load("Datas/stage_Test00_sugawara.json", stageData);
	pStage->Load(stageData);


#ifdef _DEBUG

	// �X�e�[�W�G�f�B�^�[�����s
	StageEditor* pSe = Instantiate<StageEditor>(this); 

	// �ҏW�Ώۂ�ݒ�
	pSe->SetEditStage(pStage);
	
#endif // _DEBUG

	
}


void Scene_Test::Update()
{
	// if (button->ClickButton())PostQuitMessage(0);
}

void Scene_Test::Draw()
{
}

void Scene_Test::Release()
{

}