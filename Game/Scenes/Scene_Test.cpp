#include "Scene_Test.h"

// �C���N���[�h
#include "../Objects/Stage/SkySphere.h"
#include "../Objects/UI/UIPanel.h"
#include "../Objects/Stage/Stage.h"
#include "../Otheres/GameEditor.h"

Scene_Test::Scene_Test(GameObject * parent)
	: GameObject(parent, "Scene_Test")
{
}

void Scene_Test::Initialize()
{
	// �X�J�C�X�t�B�A�̐���
	SkySphere* skySphere = Instantiate<SkySphere>(this);

	// UI�p�l���̐���
	UIPanel* uiPanel = Instantiate<UIPanel>(this); {
		json loadObj;
		JsonReader::Load("Datas/test_UIsave.json", loadObj);
		uiPanel->Load(loadObj);
	}

	// �X�e�[�W���쐬
	Stage* pStage = Instantiate<Stage>(this);
	
	// �X�e�[�W��ǂݍ���
	json stageData;
	JsonReader::Load("Datas/tkanaais.json", stageData);
	pStage->Load(stageData);


#ifdef _DEBUG
	// �Q�[���G�f�B�^�[�̐���
	GameEditor* gameEditor = Instantiate<GameEditor>(this);
	gameEditor->SetEditStage(pStage);
	gameEditor->SetEditUIPanel(uiPanel);
#endif // _DEBUG
}

void Scene_Test::Update()
{
}

void Scene_Test::Draw()
{

}

void Scene_Test::Release()
{
}
