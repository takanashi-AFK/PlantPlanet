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
#include "../Objects/UI/UIInventory.h"
#include "../../Game/Objects/UI/UICursor.h"


using namespace Constants;

Scene_Play::Scene_Play(GameObject* parent)
	:GameObject(parent, "Scene_Play"),
	pStage_(nullptr),
	tpsCamera_(nullptr), 
	fixedCursorPos(true), 
	cursorVisible(true),
	isGameStart_(false), 
	isBossSpawn_(false),
	isDebugDataEditWindowOpen_(false),
	isShowInventoryFirstTime_(false),
	currentState_(PlaySceneState::PlaySceneState_Play)
{
	UICursor::ToHide(true);
}

void Scene_Play::Initialize()
{
	// UIパネルの初期化
	InitUIPanel();

	// ステージの初期化
	InitStage();

	// カメラの初期化
	InitCamera();

	UIInventory::Initialize();
}

void Scene_Play::Update()
{
	// カーソル固定化処理
	SetCursorMode();

	//	// カメラのアクティブ化
	tpsCamera_->SetActive(true);

	// プレイヤー情報を取得
	Component_PlayerBehavior* playerBehavior = nullptr; {
		for (auto pb : pStage_->FindComponents(ComponentType::PlayerBehavior))playerBehavior = (Component_PlayerBehavior*)pb;
	}

	// ボス情報を取得
	Component_BossBehavior* bossBehavior = nullptr; {
		for (auto bb : pStage_->FindComponents(ComponentType::BossBehavior))bossBehavior = (Component_BossBehavior*)bb;
	}

	if (currentState_ == PlaySceneState::PlaySceneState_Play) {
		// プレイヤーをカメラのターゲットに設定
		if (playerBehavior != nullptr)tpsCamera_->SetTarget(playerBehavior->GetHolder());

		// プレイ情報の表示処理
		SetPlayInfo();

		// シーン遷移処理
		bool isSceneChange = false;
		{
			// ボスに関わる処理
			if (bossBehavior != nullptr) {

				// ボスのHPが0になったら
				for (auto healthGauge : bossBehavior->GetChildComponent(ComponentType::HealthGauge)) {
					if (((Component_HealthGauge*)healthGauge)->now_ <= 0) {

						// ボスの状態を死亡に変更
						bossBehavior->SetState(BossState::BOSS_STATE_DEAD);

						// シーン遷移フラグを立てる
						isSceneChange = true;
					}
				}

				// プレイヤーのHPが0になったら
				for (auto healthGauge : playerBehavior->GetChildComponent(ComponentType::HealthGauge)) {
					if (((Component_HealthGauge*)healthGauge)->now_ <= 0) {

						// プレイヤーの状態を死亡に変更
						playerBehavior->SetState(PlayerState::PLAYER_STATE_DEAD);

						// シーン遷移フラグを立てる
						isSceneChange = true;
					}
				}

			}

			// 終了ボタンが押されたら
			if (Input::IsKeyDown(DIK_ESCAPE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_START)) {

				isSceneChange = true;

			}

			if (isSceneChange == true) {

				// プレイヤーが取得した植物情報を取得
				g_playerPlantData = playerBehavior->GetMyPlants();

				SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
				sceneManager->ChangeScene(SCENE_ID_RESULT, TID_BLACKOUT);
			}

		}

		// ボス出現処理
		{

			if (playerBehavior->GetResearchPoint() >= 100 && isBossSpawn_ == false) {
				// ボス敵の生成
				SpawnBossEnemy(); isBossSpawn_ = true;
			}
			// 
			//else playerBehavior->SetResearchPoint(playerBehavior->GetResearchPoint() + 1);
		}

		if (Input::IsKeyDown(DIK_I)) {
			SetState(PlaySceneState::PlaySceneState_Inventory);
			UIInventory::SetStage(pStage_);
			UIInventory::InventoryDataSet();

			isShowInventoryFirstTime_ = true;
		}
	}
	else if (currentState_ == PlaySceneState::PlaySceneState_Inventory) {
		if (isShowInventoryFirstTime_ == true) {
			UIInventory::SetStage(pStage_);

			UIInventory::InventoryDataSet();
			UIInventory::SwitchInventory(true);
			isShowInventoryFirstTime_ = false;
		}


		if (Input::IsKeyDown(DIK_I)) {
			SetState(PlaySceneState::PlaySceneState_Play);
			UIInventory::SwitchInventory(false);
			isShowInventoryFirstTime_ = true;
		}
	UIInventory::Update();
	}


	if (Input::IsKeyDown(DIK_O)) {
		// カーソル固定化の切り替え
		fixedCursorPos = !fixedCursorPos;

		// カーソルの表示状態を切り替える
		cursorVisible = !fixedCursorPos;
		UICursor::ToHide(!cursorVisible);
		isDebugDataEditWindowOpen_ = !isDebugDataEditWindowOpen_;
	}

	DrawDebugDataEditWindow();
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
	if (JsonReader::Load("Datas/Test/Inventory.json", loadData))
		UIPanel::GetInstance()->Load(loadData);
}

void Scene_Play::InitStage()
{
	// スカイスフィアの生成
	Instantiate<SkySphere>(this);

	// ステージデータの読み込み
	json loadData;
	if (JsonReader::Load("Datas/Test/TentativeGameData20241210.json", loadData)) {

		// ステージを生成
		pStage_ = Instantiate<Stage>(this);

		// ステージの読み込み
		pStage_->Load(loadData);
	}
	
	// 植物の生成
	{
		// 植物データの読み込み
		json loadData;
		JsonReader::Load("Datas/PlantData/TentativeFlowers.json", loadData);
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
		UICursor::ToHide(!cursorVisible);
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

void Scene_Play::SpawnBossEnemy()
{
	// ボスの情報を読込 ※読込失敗時は処理を中断
	json loadData;
	if (JsonReader::Load("Datas/Test/testBoss.json", loadData) == false) return;

	// ステージオブジェクトの生成
	StageObject* stageObject_Boss = CreateStageObject(loadData.begin().key(), loadData.begin().value()["modelFilePath_"], pStage_);

	// 読み込んだデータを反映
	stageObject_Boss->Load(loadData.begin().value());

	// 位置を設定
	stageObject_Boss->SetPosition(27.5, 2, 27.5);

	// ターゲットを設定
	for (auto bossBehavior : stageObject_Boss->FindComponent(ComponentType::BossBehavior)) 
		((Component_BossBehavior*)bossBehavior)->SetTarget(pStage_->GetStageObject("00_Player"));
	
	// ステージに追加
	pStage_->AddStageObject(stageObject_Boss);
}

void Scene_Play::DrawDebugDataEditWindow()
{
	if (!isDebugDataEditWindowOpen_) return;



	Component_PlayerBehavior* playerBehavior = nullptr;
	for (auto pb : pStage_->FindComponents(ComponentType::PlayerBehavior))
		playerBehavior = (Component_PlayerBehavior*)pb;
	if (playerBehavior == nullptr)return;

	Component_HealthGauge* playerHealthGauge = (Component_HealthGauge*)
		playerBehavior->GetChildComponent("PlayerHealthGauge");
	if (playerHealthGauge == nullptr)return;

	Component_StaminaGauge* playerStaminaGauge = (Component_StaminaGauge*)
		playerBehavior->GetChildComponent("StaminaGauge");
	if (playerStaminaGauge == nullptr)return;

	ImGui::Begin("Debug Mode"); {
		ImGui::Text("Player param Edit");
		ImGui::Text("Player HP : %f / %f", playerHealthGauge->now_, playerHealthGauge->max_);
		ImGui::Text("Player ST : %f / %f", playerStaminaGauge->now_, playerStaminaGauge->max_);
		ImGui::Text("Player Research Point : %d", playerBehavior->GetResearchPoint());

		static int playerMAXHP = 100;
		static int playerHP = 100;
		static int playerMAXST = 100;
		static int playerST = 100;
		static int playerResearchPoint = 0;

		ImGui::DragInt("Player MAX HP", &playerMAXHP, 1, 0, 1000);
		ImGui::DragInt("Player HP", &playerHP, 1, 0, playerMAXHP);
		ImGui::DragInt("Player MAX ST", &playerMAXST, 1, 0, 1000);
		ImGui::DragInt("Player ST", &playerST, 1, 0, playerMAXST);
		ImGui::DragInt("Player Research Point", &playerResearchPoint, 1, 0, 100);

		if (ImGui::Button("MAXHP APPLY"))playerHealthGauge->max_ = playerMAXHP;                          ImGui::SameLine();
		if (ImGui::Button("HP APPLY"))playerHealthGauge->now_ = playerHP;                                ImGui::SameLine();
		if (ImGui::Button("MAXST APPLY"))playerStaminaGauge->max_ = playerMAXST;                         ImGui::SameLine();
		if (ImGui::Button("ST APPLY"))playerStaminaGauge->now_ = playerST;                               ImGui::SameLine();
		if (ImGui::Button("Research Point APPLY"))playerBehavior->SetResearchPoint(playerResearchPoint);

		ImGui::Separator();
		ImGui::Text("ST Decrease Edit");
		ImGui::Text("Player Decrease ST SHOOT : %f", playerBehavior->GetStaminaDecrease_Shoot());
		ImGui::Text("Player Decrease ST DODGE : %f", playerBehavior->GetStaminaDecrease_Dodge());
		ImGui::Text("Player Decrease ST MELEE : %f", playerBehavior->GetStaminaDecrease_Melee());

		static int playerDecreaseST_Shoot = 10;
		static int playerDecreaseST_Melee = 20;
		static int playerDecreaseST_Dodge = 30;

		ImGui::DragInt("Player Decrease ST SHOOT", &playerDecreaseST_Shoot, 1, 0, 1000);
		ImGui::DragInt("Player Decrease ST MELEE", &playerDecreaseST_Melee, 1, 0, 1000);
		ImGui::DragInt("Player Decrease ST DODGE", &playerDecreaseST_Dodge, 1, 0, 1000);

		if (ImGui::Button("ST Decrease SHOOT APPLY"))playerBehavior->SetStaminaDecrease_Shoot(playerDecreaseST_Shoot);   ImGui::SameLine();
		if (ImGui::Button("ST Decrease MELEE APPLY"))playerBehavior->SetStaminaDecrease_Melee(playerDecreaseST_Melee);   ImGui::SameLine();
		if (ImGui::Button("ST Decrease DODGE APPLY"))playerBehavior->SetStaminaDecrease_Dodge(playerDecreaseST_Dodge);
		ImGui::Separator();
		ImGui::Text("Camera Edit");
		static float sens = 0.1f;
		ImGui::Text("Camera Sensitivity %f", tpsCamera_->GetSensitivity());
		ImGui::DragFloat("Camera Sensitivity", &sens, 0.01f, 0.0f, 1.0f);
		if (ImGui::Button("Apply Camera Sensitivity"))tpsCamera_->SetSensitivity(sens);
	}
	ImGui::End();
}

void Scene_Play::ShowInventory()
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

	// インベントリの表示
	{
		ImGui::Begin("Inventory"); {
			ImGui::Text("Inventory");

			// プレイヤーの情報を取得
			Component_PlayerBehavior* playerBehavior = nullptr;
			for (auto pb : pStage_->FindComponents(ComponentType::PlayerBehavior))
				playerBehavior = (Component_PlayerBehavior*)pb;

			// プレイヤーが取得した
			std::vector<PlantData> playerPlantData = playerBehavior->GetMyPlants();

		}
	}
}
