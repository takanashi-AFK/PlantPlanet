#include "Scene_Test.h"

// �C���N���[�h
#include "../Objects/Stage/SkySphere.h"
#include "../Objects/UI/UIPanel.h"
#include "../Objects/Stage/Stage.h"
#include "../Otheres/GameEditor.h"
#include "../../Engine/GameObject/Camera.h"
#include "../../Engine/Global.h"

Scene_Test::Scene_Test(GameObject * parent)
	: GameObject(parent, "Scene_Test")
{
}

void Scene_Test::Initialize()
{
	// �X�J�C�X�t�B�A�̐���
	SkySphere* skySphere = Instantiate<SkySphere>(this);

	// �X�e�[�W���쐬
	Stage* pStage = Instantiate<Stage>(this); {
		json stageData;
		JsonReader::Load("Datas/tkanaais.json", stageData);
		pStage->Load(stageData);
	}

	// UI�p�l���̐���
	UIPanel* uiPanel = Instantiate<UIPanel>(this); {
		json uiData;
		JsonReader::Load("Datas/test_UIsave.json", uiData);
		uiPanel->Load(uiData);
	}
	
	// �J�����̐ݒ�
	json camData;
	if (JsonReader::Load("Datas/camera.json", camData)) {
		Camera::SetPosition(JFLOAT3(camData["position"]));
		Camera::SetTarget(JFLOAT3(camData["target"]));
	}


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
	json camData;
	camData["position"] = { REFERENCE_XMFLOAT3(Camera::GetPosition()) };
	camData["target"] = { REFERENCE_XMFLOAT3(Camera::GetTarget()) };
	JsonReader::Save("Datas/camera.json", camData);
}
