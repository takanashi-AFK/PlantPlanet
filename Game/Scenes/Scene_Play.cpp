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
#include "../Otheres/UserManager.h"

namespace {
	// UIレイアウトのjsonファイルパス
	const string ADVENTURE_MODE_UI_LAYOUT_JSON = "Datas/NoInv.json";
	const string SCOREATTACK_MODE_UI_LAYOUT_JSON = "Datas/SceneLayout/PlayScene/scoreAttackMode.json";

	// ステージのjsonファイルパス
	const string ADVENTURE_MODE_STAGE_JSON = "Datas/Test/TentativeGameData20241210.json";
	const string SCOREATTACK_MODE_STAGE_JSON = "Datas/StageLayouts/stage_normal.json";
}

using namespace Constants;

Scene_Play::Scene_Play(GameObject* parent)
	:GameObject(parent, "Scene_Play"),
	pStage_(nullptr),
	tpsCamera_(nullptr), 
	fixedCursorPos_(true), 
	cursorVisible_(true),
	isShowInventoryFirstTime_(false),
	isOpenInventoryUI_(false),
	fade_{ 1.f }
{
	UICursor::ToHide(true);
}

void Scene_Play::Initialize()
{
	ScoreManager::justAvoidance = 0;
	start_ = std::chrono::system_clock::now();
	// ゲームモードによる初期化処理
	switch (g_gameMode) 
	{
	case MODE_ADVENTURE:	InitAdventureMode();	break;
	case MODE_SCOREATTACK:	InitScoreAttackMode();	break;
	}
}

void Scene_Play::Update()
{
	// カーソル固定化処理
	auto playTime = std::chrono::system_clock::now() - start_;
	g_playTime = std::chrono::duration_cast<std::chrono::seconds>(playTime).count();
	
	SetCursorMode();

	// カメラのアクティブ化
	if(tpsCamera_ != nullptr)tpsCamera_->SetActive(true);

	// プレイヤー情報を取得
	Component_PlayerBehavior* playerBehavior = nullptr; {
		if (pStage_ != nullptr)
		for (auto pb : pStage_->FindComponents(ComponentType::PlayerBehavior))playerBehavior = (Component_PlayerBehavior*)pb;
	}

	// ボス情報を取得
	Component_BossBehavior* bossBehavior = nullptr; {
		if (pStage_ != nullptr)
		for (auto bb : pStage_->FindComponents(ComponentType::BossBehavior))bossBehavior = (Component_BossBehavior*)bb;
	}

	// ゲームモードによる更新処理
	switch (g_gameMode)
	{
	case MODE_ADVENTURE:	UpdateAdventureMode(playerBehavior, bossBehavior);	break;
	case MODE_SCOREATTACK:	UpdateScoreAttackMode(playerBehavior, bossBehavior);break;
	}

	// debug
	/*static bool isDebugMode = false;
	if (Input::IsKeyDown(DIK_0))isDebugMode = !isDebugMode;
	if (isDebugMode)DrawDebugDataEditWindow();*/
	// debug
}

void Scene_Play::Draw()
{
}

void Scene_Play::Release()
{
	UserManager& um = UserManager::GetInstance();
	auto user = um.GetLoggedInUser();

	um.UpdatePlayTotalTime(user->userName, user->playTotalTime + g_playTime);
}

void Scene_Play::InitUIPanel(const string& _fileName)
{
	// UIパネル & レイアウトの読み込み
	json loadData;
	if (JsonReader::Load(_fileName, loadData))UIPanel::GetInstance()->Load(loadData);
}

void Scene_Play::InitStage(const string& _fileName)
{
	// スカイスフィアの生成
	Instantiate<SkySphere>(this);
	// ※ スカイスフィアの時間帯をランダムで設定する処理を追記予定

	// ステージデータの読み込み
	json loadData;
	if (JsonReader::Load(_fileName, loadData)) {

		// ステージを生成
		pStage_ = Instantiate<Stage>(this);

		// ステージの読み込み
		pStage_->Load(loadData);
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
		fixedCursorPos_ = !fixedCursorPos_;

		// カーソルの表示状態を切り替える
		cursorVisible_ = !fixedCursorPos_;
		UICursor::ToHide(!cursorVisible_);
	}

	// カーソルの位置を中央に固定
	if (fixedCursorPos_) {
		SetCursorPos(Direct3D::screenWidth_ / 2, Direct3D::screenHeight_ / 2);
	}
}

void Scene_Play::DrawDebugDataEditWindow()
{
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

void Scene_Play::InitAdventureMode()
{
	// UIパネルの初期化処理 ※アドベンチャーモード用
	InitUIPanel(ADVENTURE_MODE_UI_LAYOUT_JSON);

	// ステージの初期化処理 ※アドベンチャーモード用
	InitStage(ADVENTURE_MODE_STAGE_JSON);

	// ステージ上に植物をランダムに生成
	if (pStage_ != nullptr)for (auto pg : (pStage_->GetStageObject("generator"))->FindComponent(ComponentType::PlantGenerator))pg->Execute();

	// カメラの初期化処理
	InitCamera();

	// UIインベントリの初期化
	UIInventory::Initialize();
}

void Scene_Play::UpdateAdventureMode(Component_PlayerBehavior* _playerBehavior, Component_BossBehavior* _bossBehavior)
{
	// UIの表示状態による更新処理
	if (isOpenInventoryUI_ == true) UpdateInventoryUI();
	else							UpdateNormalUI(_playerBehavior, _bossBehavior);


	// シーン遷移条件検知処理
	bool isSceneChange = false; {

		// プレイヤーが死亡した場合...
		for (auto comp_healthGauge : _playerBehavior->GetChildComponent(ComponentType::HealthGauge)) {
			if (((Component_HealthGauge*)comp_healthGauge)->IsDead() == true) {

				// プレイヤーの状態を`死亡`に変更
				_playerBehavior->SetState(PlayerState::PLAYER_STATE_DEAD);

				// シーン遷移フラグを立てる
				isSceneChange = true;
			}
		}

		// 調査度がMAX かつ 帰還ゲートにインタラクトした場合...
		constexpr int RESEARCH_POINT_MAX = 100;
		if (_playerBehavior->GetResearchPoint() >= RESEARCH_POINT_MAX && _playerBehavior->IsExchangeScene())
			isSceneChange = true;
		
		// debug 終了ボタンが押された場合...
		if (Input::IsKeyDown(DIK_ESCAPE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_START))isSceneChange = true;
	}

	// シーン遷移処理
	if (isSceneChange == true) {

		for (auto getPlantData : _playerBehavior->GetMyPlants()) {
			bool exists = false;
			for (const auto& plantData : g_playerPlantData) {
				if (plantData.name_ == getPlantData.name_) {
					exists = true;
					break;
				}
			}
			if (!exists) {
				g_playerPlantData.push_back(getPlantData);
			}
		}

		g_thisPlayGetPlantData.clear();
		g_thisPlayGetPlantData = _playerBehavior->GetMyPlants();
		UIInventory::Release();
		isOpenInventoryUI_ = false;
		SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
		sceneManager->ChangeScene(SCENE_ID_RESULT, TID_BLACKOUT);
	}
}

void Scene_Play::UpdateInventoryUI()
{
	// インベントリUIを閉じる(非表示にする)処理
	if (Input::IsKeyDown(DIK_Q) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B) || UIInventory::IsShowInventory() == false) CloseInventoryUI();

	// UIインベントリの更新、フェード処理
	UIInventory::SetFade(fade_);
	UIInventory::Update();
	fade_ -= FADESPEED;
	fade_ = std::clamp(fade_, 0.f, 1.f);
}

void Scene_Play::UpdateNormalUI(Component_PlayerBehavior* _playerBehavior, Component_BossBehavior* _bossBehavior)
{
	// プレイヤーの情報が取得できた場合...
	if (_playerBehavior != nullptr) {

		// カメラのターゲットをプレイヤーに設定
		if(tpsCamera_ != nullptr)tpsCamera_->SetTarget(_playerBehavior->GetHolder());

		// プレイヤーのHP情報を取得 & 反映
		Component_HealthGauge* playerHealthGauge = (Component_HealthGauge*)_playerBehavior->GetChildComponent("PlayerHealthGauge");
		UIProgressBar* playerHPBar = (UIProgressBar*)UIPanel::GetInstance()->FindObject("HPBar");
		if (playerHPBar != nullptr && playerHealthGauge != nullptr)playerHPBar->SetProgress(playerHealthGauge->now_, playerHealthGauge->max_);

		// プレイヤーのスタミナ情報を取得 & 反映
		Component_StaminaGauge* playerStaminaGauge = (Component_StaminaGauge*)_playerBehavior->GetChildComponent("StaminaGauge");
		UIProgressBar* playerSTBar = (UIProgressBar*)UIPanel::GetInstance()->FindObject("STBar");
		if (playerSTBar != nullptr && playerStaminaGauge != nullptr)playerSTBar->SetProgress(playerStaminaGauge->now_, playerStaminaGauge->max_);

		// プレイヤーの調査度情報を取得 & 反映
		UIProgressCircle* playerResearchPointCircle = (UIProgressCircle*)UIPanel::GetInstance()->FindObject("ResearchCircle");
		if (playerResearchPointCircle != nullptr)playerResearchPointCircle->SetProgress(_playerBehavior->GetResearchPoint(), 100);
	}

	fade_ += FADESPEED;
	UIInventory::ShowInventory(false);

	//フェード中ならUIInventoryを可視化しフェーディングする
	if (fade_ < 1.f)
	{
		UIInventory::ShowInventory(true);
		UIInventory::SetFade(fade_);
		UIInventory::Update();
	}
	fade_ = std::clamp(fade_, 0.f, 1.f);

	// インベントリUIを開く(表示する)処理
	if (Input::IsKeyDown(DIK_Q) || Input::IsPadButtonDown(XINPUT_GAMEPAD_B))
		OpenInventoryUI();
}

void Scene_Play::CloseInventoryUI()
{
	isOpenInventoryUI_ = false;			// インベントリUIを閉じる(非表示にする)処理
	UIInventory::ShowInventory(true);	// インベントリUIを非表示にする
	EnterOtherObject(this);				// 他のオブジェクトを有効化
	isShowInventoryFirstTime_ = true;	// インベントリUIを初めて表示したかのフラグを立てる

	// カーソル固定化の切り替え
	fixedCursorPos_ = !fixedCursorPos_;

	// カーソルの表示状態を切り替える
	cursorVisible_ = !fixedCursorPos_;
	UICursor::ToHide(!cursorVisible_);

	UIInventory::SetFade(1.0);
	UIInventory::Update();
}

void Scene_Play::OpenInventoryUI()
{
	isOpenInventoryUI_ = true;			// インベントリUIを開く(表示する)処理
	UIInventory::SetStage(pStage_);		// ステージ情報をインベントリUIに設定
	UIInventory::InventoryDataSet();	// インベントリUIのデータセット
	UIInventory::ShowInventory(true);	// インベントリUIを表示する
	LeaveOtherObject(this);				// 他のオブジェクトを無効化
	isShowInventoryFirstTime_ = true;	// インベントリUIを初めて表示したかのフラグを立てる

	// カーソル固定化の切り替え
	fixedCursorPos_ = !fixedCursorPos_;

	// カーソルの表示状態を切り替える
	cursorVisible_ = !fixedCursorPos_;
	UICursor::ToHide(!cursorVisible_);

	UIInventory::SetFade(1.0);
	UIInventory::Update();
}

void Scene_Play::InitScoreAttackMode()
{
	// UIパネルの初期化
	InitUIPanel(SCOREATTACK_MODE_UI_LAYOUT_JSON);

	// ステージの初期化
	InitStage(SCOREATTACK_MODE_STAGE_JSON);

	// カメラの初期化
	InitCamera();

	// カウントダウンの生成
	countDown_ = Instantiate<CountDown>(this);
	countDown_->Start();

	// カーソルの非表示化
	ShowCursor(false);
}

void Scene_Play::UpdateScoreAttackMode(Component_PlayerBehavior* _playerBehavior, Component_BossBehavior* _bossBehavior)
{
	Component_HealthGauge* playerHealthGauge = (Component_HealthGauge*)_playerBehavior->GetChildComponent("PlayerHealthGauge");
	UIProgressBar* playerHPBar = (UIProgressBar*)UIPanel::GetInstance()->FindObject("HPBar");
	if (playerHPBar != nullptr && playerHealthGauge != nullptr)playerHPBar->SetProgress(playerHealthGauge->now_, playerHealthGauge->max_);

	// プレイヤーのスタミナ情報を取得 & 反映
	Component_StaminaGauge* playerStaminaGauge = (Component_StaminaGauge*)_playerBehavior->GetChildComponent("StaminaGauge");
	UIProgressBar* playerSTBar = (UIProgressBar*)UIPanel::GetInstance()->FindObject("STBar");
	if (playerSTBar != nullptr && playerStaminaGauge != nullptr)playerSTBar->SetProgress(playerStaminaGauge->now_, playerStaminaGauge->max_);

	// タイマーの取得
	UITimer* uiTimer = (UITimer*)UIPanel::GetInstance()->FindObject(PLAY_SCENE_TIMER_NAME);

	// カーソル固定化処理
	SetCursorMode();

	// ゲーム開始処理
	if (countDown_->IsFinished() && isGameStart_ == false) {

		// カメラのアクティブ化
		tpsCamera_->SetActive(true);

		// ゲーム開始フラグを立てる
		isGameStart_ = true;

		// タイマーの開始
		if (uiTimer != nullptr) uiTimer->StartTimer();
	}

	// シーン切替処理
	{
		// シーン切替フラグを用意
		bool isSceneChange = false;
		Component_PlayerBehavior* pb = nullptr;
		Component_BossBehavior* bb = nullptr;

		// プレイヤーが死んだ場合、切替フラグを立てる
		for (auto& playerBehavior : pStage_->FindComponents(ComponentType::PlayerBehavior))if (((Component_PlayerBehavior*)playerBehavior)->IsDead()) { ScoreManager::isClear = false; isSceneChange = true; pb = dynamic_cast<Component_PlayerBehavior*>(playerBehavior); }

		// ボスが死んだ場合、切替フラグを立てる
		for (auto& bossBehavior : pStage_->FindComponents(ComponentType::BossBehavior)) if (((Component_BossBehavior*)bossBehavior)->IsDead()) { ScoreManager::isClear = true; isSceneChange = true; bb = dynamic_cast<Component_BossBehavior*>(bossBehavior); }

		// タイマーが終了した場合、切替フラグを立てる
		if (uiTimer != nullptr)
			if (uiTimer->IsEnd())
			{ ScoreManager::isClear = false; isSceneChange = true; }

		// シーン切替フラグが立っている場合
		if (isSceneChange == true) {

			//コンポーネントの取得
			for (auto& playerBehavior : pStage_->FindComponents(ComponentType::PlayerBehavior))
				pb = static_cast<Component_PlayerBehavior*>(playerBehavior);
			for (auto& bossBehavior : pStage_->FindComponents(ComponentType::BossBehavior))
				bb = static_cast<Component_BossBehavior*>(bossBehavior);

			// タイマーの最終値を取得
			ScoreManager::time = uiTimer->GetSeconds();

			//ユーザー名の取得
			ScoreManager::userName = (UserManager::GetInstance().GetLoggedInUser())->userName;
			{
				//受けたダメージの計算、設定
				Component_HealthGauge* hg = nullptr;
				for (auto& itr : pb->GetChildComponent(ComponentType::HealthGauge))
					hg = static_cast<Component_HealthGauge*>(itr);

				ScoreManager::recieveDMG = hg->GetMax() - hg->GetNow();
				ScoreManager::playerHp = hg->GetNow();
			}

			{
				//ボスに与えたダメージの計算、設定
				Component_HealthGauge* hge = nullptr;
				hge = static_cast<Component_HealthGauge*>(bb->GetChildComponent("HealthGauge"));

				ScoreManager::dealtDMG = hge->GetMax() - hge->GetNow();
			}

			// シーンを切り替える
			SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
			sceneManager->ChangeScene(SCENE_ID_RESULT, TID_BLACKOUT);
		}
	}

	// プレイヤーをカメラのターゲットに設定
	for (auto playerBehavior : pStage_->FindComponents(ComponentType::PlayerBehavior))tpsCamera_->SetTarget(playerBehavior->GetHolder());
}
