#include "Scene_Test.h"

// インクルード
#include "../../Engine/GameObject/Camera.h"
#include "../../Engine/Global.h"
#include "../../Engine/SceneManager.h"
#include "../Constants.h"
#include "../Objects/Camera/TPSCamera.h"
#include "../Objects/Stage/SkySphere.h"
#include "../Objects/Stage/Stage.h"
#include "../Objects/UI/UIPanel.h"
#include "../Otheres/GameEditor.h"
#include"../Generators/EnemyGenerator.h"

using namespace Constants;

Scene_Test::Scene_Test(GameObject * parent)
	: GameObject(parent, "Scene_Test")
{
}

void Scene_Test::Initialize()
{
	// スカイスフィアの生成
	Instantiate<SkySphere>(this);

	// ステージを作成
	Stage* pStage = Instantiate<Stage>(this);

	// デフォルトカメラ情報の読み込み
	json camData;
	if (JsonReader::Load(TEST_SCENE_CAMERA_LAYOUT_JSON, camData)) {
		Camera::SetPosition(JFLOAT3(camData["position"]));
		Camera::SetTarget(JFLOAT3(camData["target"]));
	}

	// TPSカメラの生成
	TPSCamera* tpsCamera = Instantiate<TPSCamera>(this);

#ifdef _DEBUG
	// ゲームエディターの生成
	GameEditor* gameEditor = Instantiate<GameEditor>(this);
	gameEditor->SetEditStage(pStage);
	gameEditor->SetEditUIPanel(UIPanel::GetInstance());
	gameEditor->SetTPSCamera(tpsCamera);
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
	JsonReader::Save(TEST_SCENE_CAMERA_LAYOUT_JSON, camData);
}
