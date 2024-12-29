#include "Component_PlayerBehavior.h"

// インクルード
#include "../../../../../Engine/Collider/BoxCollider.h"
#include "../../../../../Engine/DirectX/Direct3D.h"
#include "../../../../../Engine/DirectX/Input.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../../../../Game/Objects/Stage/Components/GaugeComponents/Component_HealthGauge.h"
#include "../../../../Constants.h"
#include "../../../Camera/TPSCamera.h"
#include "../../../Engine/Global.h"
#include "../../../Game/Objects/UI/UIPanel.h"
#include "../../../Game/Objects/UI/UIProgressBar.h"
#include "../../../UI/CountDown.h"
#include "../../Stage.h"
#include "../../StageObject.h"
#include "Component_BossBehavior.h"
#include <algorithm> 
#include <directxmath.h> 
#include "../../../UI/UIProgressCircle.h"
#include "../../../UI/UIImage.h"
#include "../../../UI/Components/Component_UIEasing.h"
#include "../../../UI/UIInventory.h"


// child components include
#include "../AttackComponents/Component_MeleeAttack.h"
#include "../AttackComponents/Component_ShootAttack.h"
#include "../MotionComponent/Component_PlayerMotion.h"
#include "../MoveComponents/Component_TackleMove.h"
#include "../MoveComponents/Component_WASDInputMove.h"
#include "../TimerComponent/Component_Timer.h"
#include "../DetectorComponents/Component_CircleRangeDetector.h"
#include "../PlantComponents/Component_Plant.h"
#include "../GaugeComponents/Component_StaminaGauge.h"
#include "../../Salad.h"
#include "../../MakeSalad.h"

using namespace Constants;

namespace {
	constexpr int SHOOT_FRAME = 115;
	constexpr XMFLOAT3 PLAYER_COLLIDER_SIZE = { 1,1,1 };
	constexpr XMFLOAT3 PLAYER_COLLIDER_POSITION = { 0,0.5,0 };
	constexpr XMVECTOR INITIALIZE_DIRECTION_Z = { 0,0,1,0 };

	constexpr float DODGE_DISTANCE = 5.0f;
	constexpr float DODGE_RAY_OFFSET = 0.5f;
	constexpr float DODGE_DISTANCE_LIMIT = 0.7;

	constexpr int EFFECT_FRAME = 60;
	constexpr int EFFECT_SPEED = 1;

	constexpr float BOSS_TACKLE_DISTANCE = 2.0f;

	constexpr int STAMINA_DECREASE_SHOOT = 10;
	constexpr int STAMINA_DECREASE_MELEE = 20;
	constexpr int STAMINA_DECREASE_DODGE = 30;
	constexpr float STAMINA_RECOVERY = 0.17f;
	constexpr float STAMINA_MAX = 100.f;

	const int INCLEACE_RESEARCH_POINT_RARITY_3 = 30;
	const int INCLEACE_RESEARCH_POINT_RARITY_2 = 15;
	const int INCLEACE_RESEARCH_POINT_RARITY_1 = 5;

	bool IsXMVectorZero(XMVECTOR _vec) {
		return XMVector3Equal(_vec, XMVectorZero());
	}

	bool IsWASDKey() {
		return Input::IsKey(DIK_W) || Input::IsKey(DIK_A) || Input::IsKey(DIK_S) || Input::IsKey(DIK_D);
	}

	struct CompareDist {
		bool operator()(const RayCastData& lhs, const RayCastData& rhs) const {
			return lhs.dist < rhs.dist;
		}
	};

}

Component_PlayerBehavior::Component_PlayerBehavior(string _name, StageObject* _holder, Component* _parent)
	: Component(_holder, _name, PlayerBehavior, _parent),
	shootHeight_(1.0f),
	isGameStart_(false),
	nowState_(PLAYER_STATE_IDLE),
	prevState_(PLAYER_STATE_IDLE),
	invincibilityFrame_(60),
	isShootStart_(false),
	isDodgeStart_(false),
	bossBehavior(nullptr),
	effectModelTransform(nullptr),
	effectData_(),
	researchPoint_(0),
	myPlants_(),
	isMeleeStart_(true),
	stamina_decrease_dodge_(30),
	stamina_decrease_melee_(20),
	stamina_decrease_shoot_(10),
	timeCollectPlant(defaultTime_CollectPlant),
	saladEffects_{},
	isEatSaladEnd_(false)
{
}

void Component_PlayerBehavior::Initialize()
{
	// コライダーの追加
	holder_->AddCollider(new BoxCollider(PLAYER_COLLIDER_POSITION, PLAYER_COLLIDER_SIZE));

	holder_->SetObjectType(StageObject::TYPE_PLAYER);


	// effekseer: :Effectの読み込み
	EFFEKSEERLIB::gEfk->AddEffect("dodge", "Effects/Lazer01.efk");
	EFFEKSEERLIB::gEfk->AddEffect("impact", "Effects/Attack_Impact.efk");

	// 子コンポーネントの追加
	if (FindChildComponent("InputMove") == false)AddChildComponent(CreateComponent("InputMove", WASDInputMove, holder_, this));
	if (FindChildComponent("ShootAttack") == false)AddChildComponent(CreateComponent("ShootAttack", ShootAttack, holder_, this));
	if (FindChildComponent("PlayerHealthGauge") == false)AddChildComponent(CreateComponent("PlayerHealthGauge", HealthGauge, holder_, this));
	if (FindChildComponent("PlayerMotion") == false)AddChildComponent(CreateComponent("PlayerMotion", PlayerMotion, holder_, this));
	if (FindChildComponent("TackleMove") == false)AddChildComponent(CreateComponent("TackleMove", TackleMove, holder_, this));
	if (FindChildComponent("InteractTimer") == false)AddChildComponent(CreateComponent("InteractTimer", Timer, holder_, this));
	if (FindChildComponent("IsInteractableDetector") == false)AddChildComponent(CreateComponent("IsInteractableDetector", CircleRangeDetector, holder_, this));
	if (FindChildComponent("StaminaGauge") == false)AddChildComponent(CreateComponent("StaminaGauge", StaminaGauge, holder_, this));
	if (FindChildComponent("MeleeAttack") == false)AddChildComponent(CreateComponent("MeleeAttack", MeleeAttack, holder_, this));

	UIProgressCircle* interactTimeCircle = (UIProgressCircle*)UIPanel::GetInstance()->FindObject("interactTimeCircle");
	if (interactTimeCircle != nullptr) {
		interactTimeCircle->SetVisible(false);
		interactTimeCircle->SetProgress(0, timeCollectPlant);
	}

	popUpInfo_.backGround_ = static_cast<UIImage*>(UIPanel::GetInstance()->FindObject("PopUp-Image-BackGround"));
	popUpInfo_.images_[0] = static_cast<UIImage*>(UIPanel::GetInstance()->FindObject("PopUp-Image-Effect-Icon0"));
	popUpInfo_.images_[1] = static_cast<UIImage*>(UIPanel::GetInstance()->FindObject("PopUp-Image-Effect-Icon1"));
	popUpInfo_.images_[2] = static_cast<UIImage*>(UIPanel::GetInstance()->FindObject("PopUp-Image-Effect-Icon2"));

	popUpInfo_.info_ = static_cast<UIText*>(UIPanel::GetInstance()->FindObject("PopUp-Text-Title"));
	popUpInfo_.texts_[0] = static_cast<UIText*>(UIPanel::GetInstance()->FindObject("PopUp-Text-Effect0"));
	popUpInfo_.texts_[1] = static_cast<UIText*>(UIPanel::GetInstance()->FindObject("PopUp-Text-Effect1"));
	popUpInfo_.texts_[2] = static_cast<UIText*>(UIPanel::GetInstance()->FindObject("PopUp-Text-Effect2"));

	saladEffectLogo_.images_[0]= static_cast<UIImage*>(UIPanel::GetInstance()->FindObject("INV-Effect-Logo0"));
	saladEffectLogo_.images_[1] = static_cast<UIImage*>(UIPanel::GetInstance()->FindObject("INV-Effect-Logo1"));
	saladEffectLogo_.images_[2] = static_cast<UIImage*>(UIPanel::GetInstance()->FindObject("INV-Effect-Logo2"));

	saladEffectLogo_.texts_[0] = static_cast<UIText*>(UIPanel::GetInstance()->FindObject("INV-Effect-Text0"));
	saladEffectLogo_.texts_[1] = static_cast<UIText*>(UIPanel::GetInstance()->FindObject("INV-Effect-Text1"));
	saladEffectLogo_.texts_[2] = static_cast<UIText*>(UIPanel::GetInstance()->FindObject("INV-Effect-Text2"));

	auto* move = static_cast<Component_WASDInputMove*>(GetChildComponent("InputMove"));
	move->SetSpeed(this->defaultSpeed_Walk);
}

void Component_PlayerBehavior::Update()
{
	ResetSaladEffectLogo();
	ApplyEffects();
	DrawPopUp();

	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// カウント制御されている場合の処理
	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	CountDown* countDown = (CountDown*)(holder_->FindObject("CountDown"));
	if (countDown != nullptr && isGameStart_ == false) {

		// 移動コンポーネントの取得 & 有無の確認
		Component_WASDInputMove* move = (Component_WASDInputMove*)(GetChildComponent("InputMove"));
		if (GetChildComponent("InputMove") == nullptr)return;

		// カウントダウンが終了した場合
		if (countDown->IsFinished()) {

			//移動を可能にする
			move->Execute();
	

			// ゲームスタートフラグを立てる
			isGameStart_ = true;
		}
		else {
			// 移動を不可能にする
			move->Stop();
			return;
		}
	}

	// HP関連処理
	{
		// プレイヤーのHPゲージコンポーネントを取得
		Component_HealthGauge* hg = (Component_HealthGauge*)(GetChildComponent("PlayerHealthGauge"));

		// UIProgressBarを取得
		UIProgressBar* hpBar = (UIProgressBar*)UIPanel::GetInstance()->FindObject(PLAY_SCENE_PLAYER_HP_GAUGE_NAME);

		// HPの値を移動
		ScoreManager::playerHp = (int)hg->now_;

		// HPバーの値を設定
		if (hpBar != nullptr && hg != nullptr)hpBar->SetProgress(hg->now_, hg->max_);

		// HPが0以下になったら... DEAD状態に遷移
		if (hg != nullptr)if (hg->IsDead() == true)SetState(PLAYER_STATE_DEAD);
	}

	// スタミナ関連処理
	{
		// プレイヤーのスタミナゲージコンポーネントを取得
		Component_StaminaGauge* sg = (Component_StaminaGauge*)(GetChildComponent("StaminaGauge"));

		// UIProgressBarを取得
		UIProgressBar* staminaBar = (UIProgressBar*)UIPanel::GetInstance()->FindObject("staminaGauge");

		// スタミナバーの値を設定
		if (staminaBar != nullptr && sg != nullptr)staminaBar->SetProgress(sg->now_, sg->max_);
	}

	// 移動コンポーネントの取得 & 有無の確認
	Component_WASDInputMove* move = (Component_WASDInputMove*)(GetChildComponent("InputMove"));
	if (GetChildComponent("InputMove") != nullptr)move->Execute();

	UIProgressCircle* interactTimeCircle = (UIProgressCircle*)UIPanel::GetInstance()->FindObject("interactTimeCircle");
	bool isVisible{};
	if (IsInteractable()) {
		UIImage* interactTimeCircleFrame = (UIImage*)UIPanel::GetInstance()->FindObject("interactTimeCircleFrame");
		UIProgressCircle* interactTimeCircle = (UIProgressCircle*)UIPanel::GetInstance()->FindObject("interactTimeCircle");
		interactTimeCircleFrame->SetVisible(true);
		interactTimeCircle->SetVisible(true);
	}
	else {
		UIImage* interactTimeCircleFrame = (UIImage*)UIPanel::GetInstance()->FindObject("interactTimeCircleFrame");
		UIProgressCircle* interactTimeCircle = (UIProgressCircle*)UIPanel::GetInstance()->FindObject("interactTimeCircle");
		interactTimeCircleFrame->SetVisible(false);
		interactTimeCircle->SetVisible(false);
	}

	if (interactTimeCircle)
	{
		interactTimeCircle->SetVisible(isVisible);
		interactTimeCircle->SetVisible(isVisible);
	}

	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// 状態ごとの処理
	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	move->SetRotateLock(true);
	prevAngles_= holder_->GetRotate();

	switch (nowState_)
	{
	case PLAYER_STATE_IDLE:					Idle();         break;  // 現在の状態がIDLEの場合
	case PLAYER_STATE_SHOOT_WALK_LEFT:
	case PLAYER_STATE_SHOOT_WALK_RIGHT:
	case PLAYER_STATE_SHOOT_WALK_FORWARD:
	case PLAYER_STATE_SHOOT_WALK_BACK:
	case PLAYER_STATE_WALK:					Walk();         break;  // 現在の状態がWALKの場合
	case PLAYER_STATE_DODGE:				Dodge();         break;  // 現在の状態がDASHの場合
	case PLAYER_STATE_DEAD:					Dead();         break;  // 現在の状態がDEADの場合
	case PLAYER_STATE_INTRACT:				Interact();      break;  // 現在の状態がINTRACTの場合
	case PLAYER_STATE_MELEE:				Melee();     break;  // 現在の状態がMELEEの場合
	case PLAYER_STATE_MADESALAD:			
		MadeSalad();     break;  // 現在の状態がMADESALADの場合
	}

	if (isShootAttack_)	Shoot();

	if (lockRotateFrameLeft_ < lockRotateFrame_)
	{
		move->SetRotateLock(false);
		
		holder_->SetRotate(prevAngles_);
		++lockRotateFrameLeft_;
		return;
	}
	
	lockRotateFrameLeft_ = NULL;
	lockRotateFrame_ = NULL;

}

void Component_PlayerBehavior::Release()
{
}

void Component_PlayerBehavior::Save(json& _saveObj)
{
	_saveObj["shootHeight"] = shootHeight_;

	_saveObj["invincibilityFrame"] = invincibilityFrame_;
}

void Component_PlayerBehavior::Load(json& _loadObj)
{
	if (_loadObj.contains("shootHeight"))shootHeight_ = _loadObj["shootHeight"];

	if (_loadObj.contains("invincibilityFrame"))invincibilityFrame_ = _loadObj["invincibilityFrame"];
}

void Component_PlayerBehavior::DrawData()
{
	ImGui::DragFloat("ShootHeight", &shootHeight_, 0.1f);
	
	ImGui::DragInt("invincibilityFrame", &invincibilityFrame_, 1);
}

void Component_PlayerBehavior::EatSalad(Salad salad)
{
	saladEffects_.clear();

	saladEffects_.push_back(salad.effect_0);
	saladEffects_.push_back(salad.effect_1);
	saladEffects_.push_back(salad.effect_2);

	isRenewalPopUp_ = true;

	isEatSaladEnd_ = true;

}

void Component_PlayerBehavior::SetTimeCollectPlant(float time)
{
	timeCollectPlant = time;
}

void Component_PlayerBehavior::Idle()
{
	// 移動コンポーネントの取得 & 有無の確認
	Component_WASDInputMove* move = (Component_WASDInputMove*)(GetChildComponent("InputMove"));
	if (move == nullptr)return;
	Component_StaminaGauge* sg = (Component_StaminaGauge*)(GetChildComponent("StaminaGauge"));
	if (sg == nullptr)return;
	// 状態優先度：歩行 > 射撃
	// `InputMove`コンポーネントの移動フラグが立っていたら...歩行状態に遷移
	if (move->IsMove()) SetState(PLAYER_STATE_WALK);

	// マウスの左ボタンが押されていたまたは、マウスの左ボタンが押されてたら、射撃状態に遷移
	else if (Input::IsMouseButtonDown(0) || Input::IsPadTriggerDownR(0)) {
		if (!sg->CanUseStamina(STAMINA_DECREASE_SHOOT)) {

			// 状態を遷移
			IsWASDKey() ? SetState(PLAYER_STATE_WALK) : SetState(PLAYER_STATE_IDLE);
			return;
		}
		//SetState(PLAYER_STATE_SHOOT);
		isShootAttack_ = true;
	}
	// スペースキーが押されていたら...ダッシュ状態に遷移
	else if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadTriggerDownL(0)) {
		if (!sg->CanUseStamina(STAMINA_DECREASE_DODGE)) {

			// 状態を遷移
			IsWASDKey() ? SetState(PLAYER_STATE_WALK) : SetState(PLAYER_STATE_IDLE);
			return;
		}
		SetState(PLAYER_STATE_DODGE);
	}
	// スペースキーが押されていたら...ダッシュ状態に遷移
	else if (Input::IsKeyDown(DIK_V) || Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		if (!sg->CanUseStamina(stamina_decrease_melee_)) {
			// 状態を遷移
			IsWASDKey() ? SetState(PLAYER_STATE_WALK) : SetState(PLAYER_STATE_IDLE);
			return;
		}
		SetState(PLAYER_STATE_MELEE);
	}
	// Aボタン もしくは Eキー が押されていたら...インタラクト状態に遷移
	else if (Input::IsKeyDown(DIK_E) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A) && IsInteractable()) {
	
		SetState(PLAYER_STATE_INTRACT);
	}
	else if (nowState_ != PLAYER_STATE_DEAD && isEatSaladEnd_ == true) {
		SetState(PLAYER_STATE_MADESALAD);
	}
}

void Component_PlayerBehavior::Walk()
{
	// 移動コンポーネントの取得 & 有無の確認
	Component_WASDInputMove* move = (Component_WASDInputMove*)(GetChildComponent("InputMove"));
	if (move == nullptr)return;
	Component_StaminaGauge* sg = (Component_StaminaGauge*)(GetChildComponent("StaminaGauge"));
	if (sg == nullptr)return;

	// 移動コンポーネントが移動していなかったら...IDLE状態に遷移
	if (move->IsMove() == false) {
		SetState(PLAYER_STATE_IDLE);
		return; // ここで処理を終了
	}

	// マウスの左ボタンが押されていたかつ、マウスの左ボタンが押されてたら、射撃状態に遷移
	else if (Input::IsMouseButtonDown(0) || Input::IsPadTriggerDownR(0)) {
		if (!sg->CanUseStamina(STAMINA_DECREASE_SHOOT)) {

			// 状態を遷移
			IsWASDKey() ? SetState(PLAYER_STATE_WALK) : SetState(PLAYER_STATE_IDLE);
			return;
		}
		isShootAttack_ = true;
	}
	// スペースキーが押されていたら...ダッシュ状態に遷移
	else if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadTriggerDownL(0)) {
		if (!sg->CanUseStamina(STAMINA_DECREASE_DODGE)) {

			// 状態を遷移
			IsWASDKey() ? SetState(PLAYER_STATE_WALK) : SetState(PLAYER_STATE_IDLE);
			return;
		}
		SetState(PLAYER_STATE_DODGE);
	}
	// Vが押されていたら...近接攻撃状態に遷移
	else if (Input::IsKeyDown(DIK_V) || Input::IsPadButton(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		if (!sg->CanUseStamina(stamina_decrease_melee_)) {
			// 状態を遷移
			IsWASDKey() ? SetState(PLAYER_STATE_WALK) : SetState(PLAYER_STATE_IDLE);
			return;
		}
		SetState(PLAYER_STATE_MELEE);
	}
	// Aボタン もしくは Eキー が押されていたら...インタラクト状態に遷移
	else if (Input::IsKeyDown(DIK_E) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A) && IsInteractable()) {
		SetState(PLAYER_STATE_INTRACT);
	}
	else if (nowState_ != PLAYER_STATE_DEAD && isEatSaladEnd_ == true) {
		SetState(PLAYER_STATE_MADESALAD);
	}
}

void Component_PlayerBehavior::Shoot()
{
	isShootAttack_ = false;

	{
		constexpr int loopTime_ = 1;
		constexpr int lockFrameShoot = Component_PlayerMotion::shotRecoilFrame_ + Component_PlayerMotion::shotWalkFrame_ * loopTime_;
		lockRotateFrame_ = lockFrameShoot;
		lockRotateFrameLeft_ = 0;
	}

	Component_StaminaGauge* sg = (Component_StaminaGauge*)(GetChildComponent("StaminaGauge"));
	if (sg == nullptr)return;

	// モーションコンポーネントの取得 & 有無の確認
	Component_PlayerMotion* motion = (Component_PlayerMotion*)(GetChildComponent("PlayerMotion"));
	if (motion == nullptr)return;

	// TPSカメラの方向を取得
	TPSCamera* tpsCamera = (TPSCamera*)holder_->FindObject("TPSCamera");
	if (tpsCamera != nullptr)prevAngles_.y = tpsCamera->GetAngle().y; // 向きの更新

	// 射撃モーションのアニメーションの現在の再生時間を取得
	//float nowFrame = motion->GetNowFrame();

	// 現在のフレームが射撃アニメーションのちょうどいいタイミングを過ぎたら...


	//if (nowFrame >= SHOOT_FRAME && isShootStart_ == false)
	{

		// 発射オプションを設定
		Component_ShootAttack* shoot = (Component_ShootAttack*)(GetChildComponent("ShootAttack"));
		{
			// 射撃コンポーネントの有無の確認
			if (shoot == nullptr)return;

			// 発射位置を設定
			XMFLOAT3 shootPosition = holder_->GetPosition();
			shootPosition.y += shootHeight_;
			shoot->SetShootingPosition(shootPosition);

			// 発射方向を設定
			shoot->SetShootingDirection(CalcShootDirection());
		}

		// 射撃処理を実行
		shoot->Execute();

		// 射撃フラグを立てる
		isShootStart_ = true;
	}

	// 移動コンポーネントの取得 & 有無の確認
	//Component_WASDInputMove* move = (Component_WASDInputMove*)(GetChildComponent("InputMove"));
	//if (move != nullptr);

	// NOTE: 終了するためのフラグ
	bool isEnd = false;

	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadTriggerDownL(0)) {
		if (!sg->CanUseStamina(STAMINA_DECREASE_DODGE)) {

			// 状態を遷移
			IsWASDKey() ? SetState(PLAYER_STATE_WALK) : SetState(PLAYER_STATE_IDLE);
			return;
		}
		SetState(PLAYER_STATE_DODGE);
	}
	// アニメーションが終わったら...
	//if (motion->IsEnd())
	{
		isEnd = true; SetState(PLAYER_STATE_IDLE);
	}

	if (isEnd == true) {

		// 射撃フラグをリセット
		isShootStart_ = false;

		sg->UseStamina(stamina_decrease_shoot_);

		// 移動コンポーネントの再開
		//if (move != nullptr) move->Execute();
	}

}

void Component_PlayerBehavior::Dodge()
{

	Component_StaminaGauge* sg = (Component_StaminaGauge*)(GetChildComponent("StaminaGauge"));
	if (sg == nullptr)return;

	static float frameCount = 0;
	static float dodgeDistance = DODGE_DISTANCE;

	// プレイヤーのHPゲージコンポーネントを取得
	Component_HealthGauge* hg = (Component_HealthGauge*)(GetChildComponent("PlayerHealthGauge"));
	if (hg == nullptr)return;

	// 移動コンポーネントの取得 & 有無の確認
	Component_WASDInputMove* move = (Component_WASDInputMove*)(GetChildComponent("InputMove"));
	if (move == nullptr)return;


	// 突進コンポーネントの取得 & 有無の確認
	Component_TackleMove* tackle = (Component_TackleMove*)(GetChildComponent("TackleMove"));
	if (tackle != nullptr && isDodgeStart_ == false) {

		//向きを固定する時間を無しにする
		lockRotateFrame_ = 0;

		// 突進方向を設定
		XMVECTOR dir = -INITIALIZE_DIRECTION_Z; {
			// 移動を不可能にする
			move->Stop();

			// 移動方向がゼロベクトルでなければ、移動方向を取得
			if (IsXMVectorZero(move->GetMoveDirection()) == false)dir = move->GetMoveDirection();
			else dir = XMVector3Normalize(XMVectorSetY(Camera::GetSightLine(),0));
		}

		// 突進方向を設定
		tackle->SetDirection(dir);

		// ステージ情報を取得
		Stage* pStage = (Stage*)(holder_->FindObject("Stage"));
		if (pStage == nullptr) return;
		std::vector<StageObject*> stageObj = pStage->GetStageObjects();

		// ステージオブジェクトすべてにレイを撃つ
		for (auto obj : stageObj) {
			// 自分自身のオブジェクトだったらスキップ
			if (obj->GetObjectName() == holder_->GetObjectName() || !obj->GetIsColliding())
				continue;

			// モデルハンドルを取得
			int hGroundModel = obj->GetModelHandle();
			if (hGroundModel < 0) continue;

			// 正面方向にレイを発射
			RayCastData tackleRayData; {
				tackleRayData.start = holder_->GetPosition(); // レイの発射位置
				tackleRayData.start.y += DODGE_RAY_OFFSET;
				XMStoreFloat3(&tackleRayData.dir, dir); // レイの方向
				Model::RayCast(hGroundModel, &tackleRayData); // レイを発射
			}

			// レイが何かに当たったら且つ、その距離が突進距離以下だったら突進距離の再設定
			if (tackleRayData.hit && tackleRayData.dist <= dodgeDistance) {

				// 近すぎたら0にする
				if (tackleRayData.dist <= DODGE_DISTANCE_LIMIT) {
					dodgeDistance = 0;
					break;
				}
				else {
					dodgeDistance = tackleRayData.dist;
					break;
				}
			}
			else
				dodgeDistance = DODGE_DISTANCE;
		}

		// 突進距離を設定
		tackle->SetDistance(dodgeDistance);

		// 突進処理を実行
		tackle->Execute();

		// 無敵へ変更
		frameCount = 0;
		hg->Lock();

		// ダッシュフラグを立てる
		isDodgeStart_ = true;
	}

	//モデルの向きの設定
	{
		auto dir = tackle->GetDirection();

		float cita = XMConvertToDegrees(std::atan2f(XMVectorGetX(dir), XMVectorGetZ(dir)));

		prevAngles_.y = cita;
		holder_->SetRotate(prevAngles_);
	}

	// エフェクトの再生処理
	{
		EFFEKSEERLIB::EFKTransform t;
		DirectX::XMStoreFloat4x4(&(t.matrix), holder_->GetWorldMatrix());
		t.isLoop = false;
		t.maxFrame = EFFECT_FRAME;
		t.speed = EFFECT_SPEED;
		effectModelTransform = EFFEKSEERLIB::gEfk->Play("dodge", t);
	}

	// nフレーム経過語に、無敵状態を解除
	{
		frameCount++;

		if (frameCount >= invincibilityFrame_) {
			hg->Unlock();
		}
	}	

	// ボス衝突判定
	BossState bossState = BOSS_STATE_MAX;
	if (bossBehavior != nullptr) {

		// 情報の取得
		XMFLOAT3 holderPos = holder_->GetPosition();
		XMFLOAT3 bossPos = bossBehavior->GetHolder()->GetPosition();
		XMFLOAT3 bossToPlayer = bossPos - holderPos;
		XMVECTOR vBossToPlayer = XMLoadFloat3(&bossToPlayer);

		// ボスが突進してきたら且つ、ボスとプレイヤーの距離が2以下だったら跳ね返る
		if (bossState == BossState::BOSS_STATE_TACKLE && XMVectorGetZ(XMVector3Length(vBossToPlayer)) < BOSS_TACKLE_DISTANCE) {

			Component_TackleMove* a = (Component_TackleMove*)bossBehavior->GetChildComponent("TackleMove");
			a->SetDistance(0);

			tackle->SetDistance(0);

			// エフェクトの再生処理
			{
				EFFEKSEERLIB::EFKTransform effectTransform;
				DirectX::XMStoreFloat4x4(&(effectTransform.matrix), holder_->GetWorldMatrix());
				effectTransform.isLoop = false;
				effectTransform.maxFrame = EFFECT_FRAME;
				effectTransform.speed = EFFECT_SPEED;
				effectModelTransform = EFFEKSEERLIB::gEfk->Play("impact", effectTransform);
			}
		}

	}
	else {
		// ボスが存在しない場合は、ボスがいないか探し情報を取得
		BossState bossState = BOSS_STATE_MAX;
		for (auto boss : (((Stage*)holder_->FindObject("Stage"))->FindComponents(BossBehavior))) {

			// ボスの情報を取得
			bossBehavior = (Component_BossBehavior*)boss;

			// ボスの状態を取得
			bossState = (BossState)((Component_BossBehavior*)boss)->GetState();
		}
	}

	// 突進処理が終了していたら...
	if (tackle->IsActived() == false) {

		// ダッシュフラグをリセット
		isDodgeStart_ = false;

		//移動を可能にする
		move->Execute();

		dodgeDistance = DODGE_DISTANCE;

		sg->UseStamina(stamina_decrease_dodge_);

		// 状態を遷移
		IsWASDKey() ? SetState(PLAYER_STATE_WALK) : SetState(PLAYER_STATE_IDLE);
	}
}

void Component_PlayerBehavior::Dead()
{
	// 移動コンポーネントの取得 & 有無の確認後、移動を不可能にする
	Component_WASDInputMove* move = (Component_WASDInputMove*)(GetChildComponent("InputMove"));
	if (move != nullptr)move->Stop();
}

void Component_PlayerBehavior::Interact()
{
	// 必要情報の取得 & 宣言定義
	Component_Timer* interactTimer = (Component_Timer*)(GetChildComponent("InteractTimer"));
	bool isInteractNow = true;

	// 移動コンポーネントの取得 & 有無の確認
	Component_WASDInputMove* move = (Component_WASDInputMove*)(GetChildComponent("InputMove"));
	if (move != nullptr) move->Stop();

	// UIProgressBarを取得
	UIProgressCircle* interactTimeCircle = (UIProgressCircle*)UIPanel::GetInstance()->FindObject("interactTimeCircle");
	interactTimeCircle->SetProgress(interactTimer->GetNowTime(), timeCollectPlant);

	UIImage* interactTimeCircleFrame = (UIImage*)UIPanel::GetInstance()->FindObject("interactTimeCircleFrame");

	StageObject* nearestPlant = nullptr;

	//ImGui::Text("InteractTimer: %f", intractTimer->GetNowTime());



	// タイマーコンポーネントが存在する場合、カウントを開始
	if (interactTimer != nullptr)interactTimer->Start();

	// Aボタン もしくは Eキー が押されている場合...
	if (Input::IsKey(DIK_E) || Input::IsPadButton(XINPUT_GAMEPAD_A)) {
		interactTimeCircle->SetVisible(true);
		interactTimeCircleFrame->SetVisible(true);

		//指定した秒数経過しているか
		if (interactTimer->IsOnTime(timeCollectPlant)) {


			// 最も近い植物オブジェクトを取得
			PlantData plantData;
			nearestPlant = GetNearestPlant(plantData);
			if (nearestPlant != nullptr) {
				// 所持植物リストに追加
				myPlants_.push_back(plantData);

				// 調査ポイントを加算
				researchPoint_ += GetResearchPointByRarity(plantData);

				Stage* pStage = ((Stage*)holder_->FindObject("Stage"));


				// 植物オブジェクトを削除
				pStage->DeleteStageObject(nearestPlant);
				nearestPlant->KillMe();

				interactTimeCircle->SetVisible(false);
				interactTimeCircleFrame->SetVisible(false);
			}

			// インタラクト状態を終了
			isInteractNow = false;
		}
	}
	// それ以外の場合... インタラクト状態を終了
	else {
		isInteractNow = false;

		interactTimeCircle->SetVisible(false);
		interactTimeCircleFrame->SetVisible(false);
		interactTimer->Reset();
		interactTimeCircle->SetProgress(interactTimer->GetNowTime(), timeCollectPlant);
	}
	// 終了処理
	if (isInteractNow == false) {

		// タイマーをリセット
		interactTimer->Reset();
		interactTimeCircle->SetProgress(interactTimer->GetNowTime(), timeCollectPlant);
		// 移動を可能にする

		interactTimeCircle->SetVisible(false);
		interactTimeCircleFrame->SetVisible(false);

		if (move != nullptr) move->Execute();
		// 待機状態に遷移
		SetState(PLAYER_STATE_IDLE);
	}
}

void Component_PlayerBehavior::Melee()
{
	Component_MeleeAttack* melee = (Component_MeleeAttack*)(GetChildComponent("MeleeAttack"));
	if (melee == nullptr)return;

	// 移動コンポーネントの取得 & 有無の確認
	Component_WASDInputMove* move = (Component_WASDInputMove*)(GetChildComponent("InputMove"));
	if (move == nullptr) return;

	Component_StaminaGauge* sg = (Component_StaminaGauge*)(GetChildComponent("StaminaGauge"));
	if (sg == nullptr)return;

	if (isMeleeStart_ == true) {
		melee->Execute();
		move->Stop();
		isMeleeStart_ = false;
	}

	// 攻撃処理が終了していたら...
	if (melee->IsActive() == false) {

		// 攻撃フラグをリセット
		isMeleeStart_ = true;

		// 移動を可能にする
		move->Execute();

		sg->UseStamina(stamina_decrease_melee_);

		// 状態を遷移
		IsWASDKey() ? SetState(PLAYER_STATE_WALK) : SetState(PLAYER_STATE_IDLE);
	}
}

void Component_PlayerBehavior::MadeSalad()
{
	Component_PlayerMotion* motion = (Component_PlayerMotion*)(GetChildComponent("PlayerMotion"));
	if (motion == nullptr)return;
	// 移動コンポーネントの取得 & 有無の確認
	Component_WASDInputMove* move = (Component_WASDInputMove*)(GetChildComponent("InputMove"));
	if (move == nullptr)return;

	move->Stop();
	if (motion->IsEnd() == true) {
		move->Execute();
		isEatSaladEnd_ = false;
		IsWASDKey() ? SetState(PLAYER_STATE_WALK) : SetState(PLAYER_STATE_IDLE);
	}
}

void Component_PlayerBehavior::ApplyEffects()
{

	int index = 0;
	for (auto itr = saladEffects_.begin(); itr != saladEffects_.end();) {
		
		auto data = (*itr)(this);
		if (!data.isUsable) itr = saladEffects_.erase(itr);
		else ++itr;

		if (isRenewalPopUp_)
		{

			popUpInfo_.images_[index]->SetImage(data.filePath);

			string info = data.time != -1 ?
				std::format("{}% ,{}sec", data.amount, data.time) :
				std::format("{}%", data.amount);
			popUpInfo_.texts_[index]->SetText(info);

			popUpInfo_.time = (3 + 0.5 * 2) * FPS;

			++index;
		}

		else
		{
			saladEffectLogo_.images_[index]->SetImage(data.filePath);
			string info = data.time != -1 ?
				std::format("{}% ,{}sec", data.amount, data.time) :
				std::format("{}%", data.amount);
			saladEffectLogo_.texts_[index]->SetText(info);

			++index;
		}
		
	}
	isRenewalPopUp_ = false;
}

void Component_PlayerBehavior::ResetSaladEffectLogo()
{
	for (auto i = 0u; i < MakeSalad::NEED_PLANT_NUM; ++i) {
		saladEffectLogo_.images_[i]->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
		saladEffectLogo_.texts_[i]->SetText("");
	}
}

void Component_PlayerBehavior::DrawPopUp()
{
	bool flag = false;
	if (popUpInfo_.time <= 0)
	{
		popUpInfo_.backGround_->SetVisible(flag);
		popUpInfo_.info_->SetVisible(flag);

		for (auto i = 0u; i < MakeSalad::NEED_PLANT_NUM; ++i) {
			popUpInfo_.images_[i]->SetVisible(flag);
			popUpInfo_.texts_[i]->SetVisible(flag);
		}

		return;
	}

	flag = true;
	popUpInfo_.backGround_->SetVisible(flag);
	popUpInfo_.info_->SetVisible(flag);

	for (auto i = 0u; i < MakeSalad::NEED_PLANT_NUM; ++i) {
		popUpInfo_.images_[i]->SetVisible(flag);
		popUpInfo_.texts_[i]->SetVisible(flag);
	}
	
	constexpr float FALLING = FPS * .5f;
	constexpr float STABLE = FPS * (.5f + 3.f);
	constexpr float RISING = FPS * (.5f + 3.f + .5f);

	auto SetEasingValue = [&](float val)
		{
			popUpInfo_.backGround_->GetEasing()->GetEasing()->pile_ = val;
			
			popUpInfo_.info_->GetEasing()->GetEasing()->pile_ = val;

			for (auto i = 0u; i < MakeSalad::NEED_PLANT_NUM; ++i) {
				popUpInfo_.images_[i]->GetEasing()->GetEasing()->pile_ = val;
				popUpInfo_.texts_[i]->GetEasing()->GetEasing()->pile_ = val;
			}

		};
	auto RisePopUp = [&]()
		{
			auto time = popUpInfo_.time - STABLE;
			auto thr = RISING - STABLE;
			SetEasingValue(1-(time/thr));
		};

	auto StablePopUp = [&]()
		{
			SetEasingValue(1.f);
		};

	auto FallPopUp = [&]()
		{
			SetEasingValue((popUpInfo_.time / FALLING));
		};

	if (popUpInfo_.time >= STABLE)RisePopUp();
	else if (popUpInfo_.time >= FALLING) StablePopUp();
	else FallPopUp();

	--popUpInfo_.time;
}

int Component_PlayerBehavior::GetResearchPointByRarity(PlantData _plantData)
{
	if (_plantData.rarity_ == 3) {
		return INCLEACE_RESEARCH_POINT_RARITY_3;
	}
	else if (_plantData.rarity_ == 2) {
		return INCLEACE_RESEARCH_POINT_RARITY_2;
	}
	else if (_plantData.rarity_ == 1) {
		return INCLEACE_RESEARCH_POINT_RARITY_1;
	}
	return 0;
}

float Component_PlayerBehavior::GetTimeCollectPlant()
{
	return timeCollectPlant;
}

bool Component_PlayerBehavior::IsDead()
{
	Component_PlayerMotion* motion = (Component_PlayerMotion*)(GetChildComponent("PlayerMotion"));

	if (motion != nullptr) return motion->IsEnd() && nowState_ == PLAYER_STATE_DEAD;
	return false;
}

bool Component_PlayerBehavior::IsInteractable()
{
	PlantData plantData;
	return (GetNearestPlant(plantData) != nullptr);
}

XMVECTOR Component_PlayerBehavior::CalcShootDirection()
{
	// FIX: 下記の処理が正常に動作しないため、カメラの視線ベクトルを返すように一時的に変更
	return Camera::GetSightLine();

	// レイキャストデータを作成
	RayCastData data; {
		// レイの発射位置 NOTE: ワールド座標系でのカメラの位置
		data.start = Camera::GetPosition();

		// レイの方向 NOTE: カメラの視線ベクトルを正規化したもの,座標系に依存しないものと考える
		XMStoreFloat3(&data.dir, XMVector3Normalize(Camera::GetSightLine()));
	}

	// ヒットしたレイキャストデータを格納するリストを用意
	vector<RayCastData> hitRayCastDatalist;

	// ステージオブジェクトの取得
	Stage* pStage = ((Stage*)holder_->FindObject("Stage"));
	vector<StageObject*> objects = pStage != nullptr ? pStage->GetStageObjects() : vector<StageObject*>();

	for (auto obj : objects) {

		// 自分自身のオブジェクトだったらスキップ
		if (obj->GetObjectName() == holder_->GetObjectName()) continue;

		// レイキャストを実行
		Model::RayCast(obj->GetModelHandle(), &data);

		// レイが当たったら
		if (data.hit) {

			// レイキャストが当たった位置をワールド座標系に変換
			XMStoreFloat3(&data.pos, XMVector3TransformCoord(XMLoadFloat3(&data.pos), obj->GetWorldMatrix()));

			// レイキャストデータをリストに追加
			hitRayCastDatalist.push_back(data);
		}
	}

	// レイキャストデータリストが空でなければ
	if (hitRayCastDatalist.empty() == false) {

		// 配列の中身を比較して最も近いものを取得
		// プレイヤーの位置からの距離を計算し、shootDirに代入
		auto min_iter = std::min_element(hitRayCastDatalist.begin(), hitRayCastDatalist.end(), CompareDist());

		// イテレータが有効か確認して最小要素の dist を出力
		if (min_iter != hitRayCastDatalist.end()) {
			XMFLOAT3 holderPos = holder_->GetPosition();
			return XMVector3Normalize(XMLoadFloat3(&min_iter->pos) - XMLoadFloat3(&holderPos));
		}
	}

	// レイキャストデータリストが空だったら
	return Camera::GetSightLine();
}

StageObject* Component_PlayerBehavior::GetNearestPlant(PlantData& _plantData)
{
	// 範囲チェックコンポーネントの取得
	Component_CircleRangeDetector* detector = (Component_CircleRangeDetector*)(GetChildComponent("IsInteractableDetector"));
	if (detector == nullptr)return nullptr;

	// 全植物オブジェクトを取得
	vector<StageObject*> plantObjects; {

		// ステージ情報の取得
		Stage* pStage = (Stage*)(holder_->FindObject("Stage"));
		if (pStage == nullptr)return nullptr;

		// ステージオブジェクトの取得
		for (StageObject* object : pStage->GetStageObjects()) {

			// 植物オブジェクトだったらリストに追加
			if(object->GetObjectType() == StageObject::TYPE_PLANT)
				plantObjects.push_back(object);
		}
	}

	// コンポーネント保有者と一番近い植物オブジェクトを取得
	StageObject* nearPlant = nullptr; {

		// 一番近い植物オブジェクトを取得
		float minDist = FLT_MAX;
		for (StageObject* plant : plantObjects) {

			detector->SetRadius(1.2f);
			detector->SetTarget(plant);
			if (detector->IsContains()) nearPlant = plant;

			//// 植物オブジェクトの位置を取得
			//XMFLOAT3 plantPos = plant->GetPosition();
			//// コンポーネント保有者と植物オブジェクトの距離を計算
			//float dist = XMVectorGetX(XMVector3Length(XMLoadFloat3(&plantPos) - XMLoadFloat3(&holder_->GetPosition())));
			//// 一番近い植物オブジェクトを取得
			//if (dist < minDist) {
			//	minDist = dist;
			//	nearPlant = plant;
			//}
		}
	}

	// 植物コンポーネント情報から植物データを取得
	if(nearPlant!=nullptr)
	for (auto plant : nearPlant->FindComponent(Plant)) 
		_plantData = ((Component_Plant*)plant)->GetData();

	// 一番近い植物オブジェクトを返す
	return nearPlant;
}