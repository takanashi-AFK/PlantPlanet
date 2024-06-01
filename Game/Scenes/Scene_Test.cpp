#include "Scene_Test.h"

// �C���N���[�h
#include "../Objects/Stage/SkySphere.h"
#include "../Objects/Stage/Stage.h"
#include "../Otheres/StageEditor.h"

Scene_Test::Scene_Test(GameObject * parent)
	: GameObject(parent, "Scene_Test")
{
}

void Scene_Test::Initialize()
{
	// �X�J�C�X�t�B�A��W���Ő���
	Instantiate<SkySphere>(this);

	// �X�e�[�W���쐬
	Stage* pStage = Instantiate<Stage>(this);
	
	// �X�e�[�W��ǂݍ���
	json stageData;
	JsonReader::Load("Datas/test_DebugStage01.json", stageData);
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
}

void Scene_Test::Draw()
{
}

void Scene_Test::Release()
{
}
