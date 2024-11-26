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
#include "../../../Engine/Global.h"
#include "../../../Game/Objects/UI/UIPanel.h"
#include "../../../Game/Objects/UI/UIProgressBar.h"
#include "../../../UI/CountDown.h"
#include "../../Stage.h"
#include "../../StageObject.h"
#include "Component_BossBehavior.h"
#include <algorithm> 
#include <directxmath.h> 

// child components include
#include "../AttackComponents/Component_MeleeAttack.h"
#include "../AttackComponents/Component_ShootAttack.h"
#include "../GaugeComponents/Component_HealthGauge.h"
#include "../MotionComponent/Component_PlayerMotion.h"
#include "../MoveComponents/Component_TackleMove.h"
#include "../MoveComponents/Component_WASDInputMove.h"
#include "../TimerComponent/Component_Timer.h"
#include "../DetectorComponents/Component_CircleRangeDetector.h"
#include "../PlantComponents/Component_Plant.h"

using namespace Constants;

namespace {
	const int SHOOT_FRAME = 115;
	const float DODGE_DISTANCE = 5.0f;
	const XMFLOAT3 PLAYER_COLLIDER_SIZE = { 1,1,1 };
	const XMFLOAT3 PLAYER_COLLIDER_POSITION = { 0,0.5,0 };
	const XMVECTOR INITIALIZE_DIRECTION_Z = { 0,0,1,0 };
	const float DODGE_RAY_OFFSET = 0.5f;
	const float DODGE_DISTANCE_LIMIT = 0.7;
	const int EFFECT_FRAME = 60;
	const int EFFECT_SPEED = 1;
	const float BOSS_TACKLE_DISTANCE = 2.0f;

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
	effectData_()
{
}

void Component_PlayerBehavior::Initialize()
{
	// コライダーの追加
	holder_->AddCollider(new BoxCollider(PLAYER_COLLIDER_POSITION, PLAYER_COLLIDER_SIZE));

	// effekseer: :Effectの読み込み
	EFFEKSEERLIB::gEfk->AddEffect("dodge", "Effects/Lazer01.efk");/*★★★*/
	EFFEKSEERLIB::gEfk->AddEffect("impact", "Effects/Attack_Impact.efk");/*★★★*/

	// 子コンポーネントの追加
	if (FindChildComponent("InputMove") == false)AddChildComponent(CreateComponent("InputMove", WASDInputMove, holder_, this));
	if (FindChildComponent("ShootAttack") == false)AddChildComponent(CreateComponent("ShootAttack", ShootAttack, holder_, this));
	if (FindChildComponent("PlayerHealthGauge") == false)AddChildComponent(CreateComponent("PlayerHealthGauge", HealthGauge, holder_, this));
	if (FindChildComponent("PlayerMotion") == false)AddChildComponent(CreateComponent("PlayerMotion", PlayerMotion, holder_, this));
	if (FindChildComponent("TackleMove") == false)AddChildComponent(CreateComponent("TackleMove", TackleMove, holder_, this));
	if (FindChildComponent("IntractTimer") == false)AddChildComponent(CreateComponent("IntractTimer", Timer, holder_, this));
	if (FindChildComponent("IsInteractableDetector") == false)AddChildComponent(CreateComponent("IsInteractableDetector", CircleRangeDetector, holder_, this));
}

void Component_PlayerBehavior::Update()
{
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
		if (hpBar != nullptr && hg != nullptr)hpBar->SetProgress(&hg->now_, &hg->max_);

		// HPが0以下になったら... DEAD状態に遷移
		if (hg != nullptr)if (hg->IsDead() == true)SetState(PLAYER_STATE_DEAD);
	}

	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	// 状態ごとの処理
	// ＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
	switch (nowState_)
	{
	case PLAYER_STATE_IDLE:           Idle();         break;  // 現在の状態がIDLEの場合
	case PLAYER_STATE_WALK:           Walk();         break;  // 現在の状態がWALKの場合
	case PLAYER_STATE_SHOOT:          Shoot();        break;  // 現在の状態がSHOOTの場合
	case PLAYER_STATE_DODGE:          Dodge();         break;  // 現在の状態がDASHの場合
	case PLAYER_STATE_DEAD:            Dead();         break;  // 現在の状態がDEADの場合
	case PLAYER_STATE_INTRACT:        Intract();      break;  // 現在の状態がINTRACTの場合
	}
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
	// 高さの設定
	ImGui::DragFloat("ShootHeight", &shootHeight_, 0.1f);

	// 無敵フレームの設定
	ImGui::DragInt("invincibilityFrame", &invincibilityFrame_, 1);
}

void Component_PlayerBehavior::Idle()
{
	// 移動コンポーネントの取得 & 有無の確認
	Component_WASDInputMove* move = (Component_WASDInputMove*)(GetChildComponent("InputMove"));
	if (move == nullptr)return;

	// 状態優先度：歩行 > 射撃
	// `InputMove`コンポーネントの移動フラグが立っていたら...歩行状態に遷移
	if (move->IsMove()) SetState(PLAYER_STATE_WALK);

	// マウスの左ボタンが押されていたかつ、マウスの右ボタンが押されてたら、射撃状態に遷移
	else if (Input::IsMouseButtonDown(0) || Input::GetPadTriggerR(0)) SetState(PLAYER_STATE_SHOOT);

	// スペースキーが押されていたら...ダッシュ状態に遷移
	else if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) SetState(PLAYER_STATE_DODGE);
	
	// Aボタン もしくは Eキー が押されていたら...インタラクト状態に遷移
	else if (Input::IsKeyDown(DIK_E) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) SetState(PLAYER_STATE_INTRACT);
}

void Component_PlayerBehavior::Walk()
{
	// 移動コンポーネントの取得 & 有無の確認
	Component_WASDInputMove* move = (Component_WASDInputMove*)(GetChildComponent("InputMove"));
	if (move == nullptr)return;

	// 移動コンポーネントが移動していなかったら...IDLE状態に遷移
	if (move->IsMove() == false) {
		SetState(PLAYER_STATE_IDLE);
		return; // ここで処理を終了
	}

	// 状態優先度：ダッシュ > 射撃
	// スペースキーが押されていたら...ダッシュ状態に遷移
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) SetState(PLAYER_STATE_DODGE);

	// マウスの左ボタンが押されていたかつ、マウスの右ボタンが押されてたら、射撃状態に遷移
	else if (Input::IsMouseButtonDown(0) || Input::GetPadTriggerR(0)) SetState(PLAYER_STATE_SHOOT);

	// Aボタン もしくは Eキー が押されていたら...インタラクト状態に遷移
	else if (Input::IsKeyDown(DIK_E) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) SetState(PLAYER_STATE_INTRACT);
}

void Component_PlayerBehavior::Shoot()
{
	// モーションコンポーネントの取得 & 有無の確認
	Component_PlayerMotion* motion = (Component_PlayerMotion*)(GetChildComponent("PlayerMotion"));
	if (motion == nullptr)return;

	// TPSカメラの方向を取得
	TPSCamera* tpsCamera = (TPSCamera*)holder_->FindObject("TPSCamera");
	if (tpsCamera != nullptr)holder_->SetRotateY(tpsCamera->GetAngle().y);

	// 射撃モーションのアニメーションの現在の再生時間を取得
	float nowFrame = motion->GetNowFrame();

	// 現在のフレームが射撃アニメーションのちょうどいいタイミングを過ぎたら...
	if (nowFrame >= SHOOT_FRAME && isShootStart_ == false) {

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
	Component_WASDInputMove* move = (Component_WASDInputMove*)(GetChildComponent("InputMove"));
	if (move != nullptr) move->Stop();


	// NOTE: 終了するためのフラグ
	bool isEnd = false;

	// スペースキーが押されていたら...ダッシュ状態に遷移
	if (Input::IsKeyDown(DIK_SPACE) || Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) { isEnd = true; SetState(PLAYER_STATE_DODGE); }

	// アニメーションが終わったら...
	if (motion->IsEnd()) { isEnd = true; SetState(PLAYER_STATE_IDLE); }

	if (isEnd == true) {
		// 射撃フラグをリセット
		isShootStart_ = false;

		// 移動コンポーネントの再開
		if (move != nullptr) move->Execute();
	}
}

void Component_PlayerBehavior::Dodge()
{
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
	if(bossBehavior != nullptr){

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

void Component_PlayerBehavior::Intract()
{
	// 必要情報の取得 & 宣言定義
	Component_Timer* intractTimer = (Component_Timer*)(GetChildComponent("IntractTimer"));
	bool isInteractNow = true;

	// 移動コンポーネントの取得 & 有無の確認
	Component_WASDInputMove* move = (Component_WASDInputMove*)(GetChildComponent("InputMove"));
	if (move != nullptr) move->Stop();

	StageObject* nearestPlant = nullptr;

	//ImGui::Text("InteractTimer: %f", intractTimer->GetNowTime());

	// タイマーコンポーネントが存在する場合、カウントを開始
	if (intractTimer != nullptr)intractTimer->Start();

	// Aボタン もしくは Eキー が押されている場合...
	if (Input::IsKey(DIK_E) || Input::IsPadButton(XINPUT_GAMEPAD_A)) {

		// カウントが 5秒 経過していたら...
		if (intractTimer->IsOnTime(5)) {

			// 最も近い植物オブジェクトを取得
			PlantData plantData;
			nearestPlant = GetNearestPlant(plantData);
			if (nearestPlant != nullptr) myPlants_.push_back(plantData);

			// 植物オブジェクトを削除
			if (nearestPlant != nullptr) nearestPlant->KillMe();

			// インタラクト状態を終了
			isInteractNow = false;
		}
	}
	// それ以外の場合... インタラクト状態を終了
	else isInteractNow = false;

	// 終了処理
	if (isInteractNow == false) {

		// タイマーをリセット
		intractTimer->Reset();

		// 移動を可能にする
		if (move != nullptr) move->Execute();

		// 待機状態に遷移
		SetState(PLAYER_STATE_IDLE);
	}
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
			for (auto plant : object->FindComponent(Plant))
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
			if(detector->IsContains()) nearPlant = plant;

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
	//for(auto plant : nearPlant->FindComponent(Plant)) _plantData = ((Component_Plant*)plant)->GetData();

	// 一番近い植物オブジェクトを返す
	return nearPlant;
}
