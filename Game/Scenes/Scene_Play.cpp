#include "Scene_Play.h"

// インクルード
#include "../../Engine/DirectX/Input.h"
#include "../../Engine/Global.h"
#include"../../Engine/SceneManager.h"
#include "../Constants.h"
#include "../Objects/Camera/TPSCamera.h"
#include "../Objects/Stage/Components/BehaviorComponents/Component_BossBehavior.h"
#include"../Objects/Stage/Components/BehaviorComponents/Component_PlayerBehavior.h"
#include"../Objects/Stage/Components/GaugeComponents/Component_HealthGauge.h"
#include"../Objects/Stage/SkySphere.h"
#include"../Objects/Stage/Stage.h"
#include "../Objects/UI/CountDown.h"
#include "../Objects/UI/UIPanel.h"
#include "../Objects/UI/UITimer.h"
#include "../Plants/PlantCollection.h"

#include "../Objects/UI/UIProgressBar.h"
#include "../Objects/Stage/Components/GaugeComponents/Component_StaminaGauge.h"
#include "../Objects/UI/UIProgressCircle.h"

using namespace Constants;

Scene_Play::Scene_Play(GameObject* parent)
	:GameObject(parent, "Scene_Play"), pStage_(nullptr), tpsCamera_(nullptr), fixedCursorPos(true), cursorVisible(true), isGameStart_(false)
{
}

void Scene_Play::Initialize()
{
	// UIパネルの初期化
	InitUIPanel();

	// ステージの初期化
	InitStage();

	// カメラの初期化
	InitCamera();

	// カーソルの非表示化
	ShowCursor(false);
}

void Scene_Play::Update()
{
	// カーソル固定化処理
	SetCursorMode();

	//	// カメラのアクティブ化
	tpsCamera_->SetActive(true);

	// プレイヤーをカメラのターゲットに設定
	for (auto playerBehavior : pStage_->FindComponents(ComponentType::PlayerBehavior))tpsCamera_->SetTarget(playerBehavior->GetHolder());

	// プレイ情報の表示処理
	SetPlayInfo();


	// シーン遷移処理
	{
		if (Input::IsKeyDown(DIK_L)){

			// プレイヤーが取得した植物情報を取得
			Component_PlayerBehavior* playerBehavior = nullptr;
			for (auto pb : pStage_->FindComponents(ComponentType::PlayerBehavior))playerBehavior = (Component_PlayerBehavior*)pb;
			g_playerPlantData = playerBehavior->GetMyPlants();

			SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
			sceneManager->ChangeScene(SCENE_ID_RESULT, TID_BLACKOUT);
		}
	}
}

void Scene_Play::Draw()
{
}

void Scene_Play::Release()
{
}

void Scene_Play::InitUIPanel()
{
	// UIパネル & レイアウトの読み込み
	json loadData;
	if (JsonReader::Load("Datas/Test/Prottype_PlayScene_Layout.json", loadData))
		UIPanel::GetInstance()->Load(loadData);
}

void Scene_Play::InitStage()
{
	// スカイスフィアの生成
	Instantiate<SkySphere>(this);

	// ステージデータの読み込み
	json loadData;
	if (JsonReader::Load("Datas/Test/testStage_Intract.json", loadData)) {

		// ステージを生成
		pStage_ = Instantiate<Stage>(this);

		// ステージの読み込み
		pStage_->Load(loadData);
	}
	
	// 植物の生成
	{
		// 植物データの読み込み
		json loadData;
		JsonReader::Load("Datas/PlantData/plant.json", loadData);
		PlantCollection::Load(loadData);

		// generatorの起動
		for (auto pg : (pStage_->GetStageObject("generator"))->FindComponent(ComponentType::PlantGenerator))pg->Execute();
	}
}

void Scene_Play::InitCamera()
{
	// カメラ情報の読み込み
	json loadData;
	if (JsonReader::Load(PLAY_SCENE_CAMERA_LAYOUT_JSON, loadData)) {

		// カメラの生成
		tpsCamera_ = Instantiate<TPSCamera>(this);

		// カメラの読み込み
		tpsCamera_->Load(loadData);

		// カメラのアクティブ化
		tpsCamera_->SetActive(false);
	}
}

void Scene_Play::SetCursorMode()
{
	// F3キーが押されたら
	if (Input::IsKeyDown(DIK_F3)) {

		// カーソル固定化の切り替え
		fixedCursorPos = !fixedCursorPos;

		// カーソルの表示状態を切り替える
		cursorVisible = !fixedCursorPos;
		ShowCursor(cursorVisible);
	}

	// カーソルの位置を中央に固定
	if (fixedCursorPos) {
		SetCursorPos(Direct3D::screenWidth_ / 2, Direct3D::screenHeight_ / 2);
	}
}

void Scene_Play::SetPlayInfo()
{
	// プレイヤーの情報を設定しUIパネルに反映
	{
		// `PlayerBehavior`コンポーネントの取得
		Component_PlayerBehavior* playerBehavior = nullptr;
		for (auto pb : pStage_->FindComponents(ComponentType::PlayerBehavior))playerBehavior = (Component_PlayerBehavior*)pb;

		// プレイヤーのHP情報を取得 & 反映
		Component_HealthGauge* playerHealthGauge = (Component_HealthGauge*)playerBehavior->GetChildComponent("PlayerHealthGauge");
		UIProgressBar* playerHPBar = (UIProgressBar*)UIPanel::GetInstance()->FindObject("HPBar");
		if (playerHPBar != nullptr && playerHealthGauge != nullptr)playerHPBar->SetProgress(playerHealthGauge->now_, playerHealthGauge->max_);
	
		// プレイヤーのスタミナ情報を取得 & 反映
		Component_StaminaGauge* playerStaminaGauge = (Component_StaminaGauge*)playerBehavior->GetChildComponent("StaminaGauge");
		UIProgressBar* playerSTBar = (UIProgressBar*)UIPanel::GetInstance()->FindObject("STBar");
		if (playerSTBar != nullptr && playerStaminaGauge != nullptr)playerSTBar->SetProgress(playerStaminaGauge->now_, playerStaminaGauge->max_);

		// プレイヤーの調査度情報を取得 & 反映
		UIProgressCircle* playerResearchPointCircle = (UIProgressCircle*)UIPanel::GetInstance()->FindObject("ResearchCircle");
		if (playerResearchPointCircle != nullptr)playerResearchPointCircle->SetProgress(playerBehavior->GetResearchPoint(), 100);
	}

}
