#include "Component_BossBehavior.h"

// インクルード
#include "../../StageObject.h"
#include "../TimerComponent/Component_Timer.h"
#include "../AttackComponents/Component_ShootAttack.h"
#include "../DetectorComponents/Component_CircleRangeDetector.h"
#include "../MoveComponents/Component_TackleMove.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../Stage.h"
#include "../../../../../Engine/Global.h"
#include <random>
#include "../../../UI/CountDown.h"
#include "../GaugeComponents/Component_HealthGauge.h"
#include "../../../UI/UIPanel.h"
#include "../../../UI/UIProgressBar.h"

#include "../../../../../Engine/ResourceManager/Model.h"

#include "../../../../Constants.h"
#include "../../../../../Engine/ResourceManager/Audio.h"

using namespace Constants;

namespace
{
	const float SHOT_RATE = 0.2f;
	const float SHOT_ANGLE = 15;
	const float MODEL_SIZE = 4.0f;
	const float MODEL_SIZE_HALF = MODEL_SIZE / 2;
	const int SHOT_TIME = 5;
	const float SMALL_VEROSITY = 0.02f;
	const XMFLOAT3 BOSS_COLLIDER_SIZE = { 5.0f, 5.0f, 5.0f };
	const int ROTATE_RANDOM_VALUE = 15;
	const int EFFECT_FRAME = 60;
	const int EFFECT_SPEED = 1;
}

Component_BossBehavior::Component_BossBehavior(string _name, StageObject* _holder, Component* _parent)
	: Component(_holder, _name, BossBehavior, _parent), 
	bNowState_(BOSS_STATE_IDLE),
	target_(nullptr), 
	targetName_(""), 
	shotInterval_(0), 
	stateChangeDelay_(0),
	shootHeight_(0), 
	isGameStart_(false), 
	isDeadStart_(true),
	tackleDamage_(0)
{
}

void Component_BossBehavior::Initialize()
{
	// 必要なコンポーネントを追加
	if (!FindChildComponent("ShootAttack")) AddChildComponent(CreateComponent("ShootAttack", ShootAttack, holder_, this));
	if (!FindChildComponent("Timer")) AddChildComponent(CreateComponent("Timer", Timer, holder_, this));
	if (!FindChildComponent("TackleMove")) AddChildComponent(CreateComponent("TackleMove", TackleMove, holder_, this));
	if (!FindChildComponent("HealthGauge")) AddChildComponent(CreateComponent("HealthGauge", HealthGauge, holder_, this));
	if(!FindChildComponent("CircleRangeDetector")) AddChildComponent(CreateComponent("CircleRangeDetector", CircleRangeDetector, holder_, this));

	// effekseer: :Effectの読み込み
	EFFEKSEERLIB::gEfk->AddEffect("sword", "Effects/Salamander12.efk");
	EFFEKSEERLIB::gEfk->AddEffect("fire", "Effects/Fire3.efk");

	// コライダーの追加
	// fix: コライダーのサイズを今後データから読み込むように変更
	holder_->AddCollider(new BoxCollider({}, BOSS_COLLIDER_SIZE));

	//holder_->SetShader(Direct3D::SHADER_BOSS);
}

void Component_BossBehavior::Update()
{

	CountDown* countDown = (CountDown*)(holder_->FindObject("CountDown"));
	if (countDown != nullptr && isGameStart_ == false) {

		// カウントダウンが終了した場合
		if (countDown->IsFinished()) {

			//移動を可能にする
			isActive_ = true;

			// ゲームスタートフラグを立てる
			isGameStart_ = true;
		}
		else {
			// 移動を不可能にする
			isActive_ = false;
			return;
		}
	}

	// ターゲットの取得処理
	if (target_ == nullptr) target_ = (StageObject*)holder_->GetParent()->FindChildObject(targetName_);

	// 範囲内にプレイヤーが存在するかどうか
	Component_CircleRangeDetector* circleRangeDetector = (Component_CircleRangeDetector*)GetChildComponent("CircleRangeDetector");
	if (circleRangeDetector != nullptr) {
		circleRangeDetector->SetRadius(20.f);
		circleRangeDetector->SetTarget(target_);
	}

	if(circleRangeDetector->IsContains() == true) Execute();


	// 対象が存在しない または アクティブでない場合は処理を行わない
	if (target_ == nullptr || isActive_ == false) return;

	{
		// プレイヤーのHPゲージコンポーネントを取得
		Component_HealthGauge* hg = (Component_HealthGauge*)(GetChildComponent("HealthGauge"));

		// UIProgressBarを取得
		UIProgressBar* hpBar = (UIProgressBar*)UIPanel::GetInstance()->FindObject(PLAY_SCENE_BOSS_HP_GAUGE_NAME);

		// HPの値を移動
		ScoreManager::playerHp = (int)hg->now_;

		// HPバーの値を設定
		if (hpBar != nullptr && hg != nullptr)hpBar->SetProgress(hg->now_, hg->max_);

		if(hg->now_ <= 0) SetState(BOSS_STATE_DEAD);
	}


	// 状態によって処理を分岐
	switch (bNowState_)
	{
	case BOSS_STATE_IDLE:Idle(); break;
	case BOSS_STATE_SHOT:Shot(); break;
	case BOSS_STATE_TACKLE:Tackle(); break;
	case BOSS_STATE_DEAD:Dead(); break;
	}
}

void Component_BossBehavior::Release()
{
}

void Component_BossBehavior::OnCollision(GameObject* _target, Collider* _collider)
{
	// 突進攻撃によるダメージ処理
	{
		for (auto tackleMove : GetChildComponent(ComponentType::TackleMove)){

			// 突進攻撃がアクティブでない または 対象が存在しない場合は処理を行わない
			if (!((Component_TackleMove*)tackleMove)->IsActived() || _target == nullptr) continue;

			// 対象のHPを減少
			for (auto hg : ((StageObject*)_target)->FindComponent(HealthGauge))((Component_HealthGauge*)hg)->TakeDamage(tackleDamage_);
		}
	}
}

void Component_BossBehavior::Save(json& _saveObj)
{
	// 対象の名前を保存
	if (target_ != nullptr) _saveObj["target_"] = target_->GetObjectName();
	
	// 発射間枠を保存
	_saveObj["shotInterval_"] = shotInterval_;
	
	// 射撃の高さを保存
	_saveObj["shootHeight_"] = shootHeight_;
	
	// 状態変更遅延を保存
	_saveObj["stateChangeDelay_"] = stateChangeDelay_;

	// 突進のダメージを保存
	_saveObj["tackleDamage_"] = tackleDamage_;
}

void Component_BossBehavior::Load(json& _loadObj)
{
	// 対象の名前を読み込み
	if (_loadObj.contains("target_")) targetName_ = _loadObj["target_"];
	
	// 発射間隔を読み込み
	if (_loadObj.contains("shotInterval_")) shotInterval_ = _loadObj["shotInterval_"];

	// 射撃の高さを読み込み
	if(_loadObj.contains("shootHeight_")) shootHeight_ = _loadObj["shootHeight_"];
	
	// 状態変更遅延を読み込み
	if (_loadObj.contains("stateChangeDelay_")) stateChangeDelay_ = _loadObj["stateChangeDelay_"];

	// 突進のダメージを読み込み
	if (_loadObj.contains("tackleDamage_")) tackleDamage_ = _loadObj["tackleDamage_"];
}

void Component_BossBehavior::DrawData()
{
	// アクティブフラグを表示
	ImGui::Checkbox("isActive_", &isActive_);
	// 対象を設定
	{
		// ステージ上に存在するオブジェクトの名前を全て取得
		vector<string> nameList{};nameList.push_back("None");
		for (auto obj : ((Stage*)holder_->FindObject("Stage"))->GetStageObjects()) nameList.push_back(obj->GetObjectName());

		// コンボボックスで選択
		static int select = 0;
		if (ImGui::BeginCombo("target_", nameList[select].c_str())) {
			for (int i = 0; i < nameList.size(); i++) {
				bool is_selected = (select == i);
				if (ImGui::Selectable(nameList[i].c_str(), is_selected)) select = i;
				if (is_selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		// 選択された名前から対象を設定
		if (select != 0) target_ = (StageObject*)holder_->FindObject(nameList[select]);
	}

	// 発射間隔を表示
	ImGui::DragFloat("shotInterval_", &shotInterval_);
	
	// 射撃の高さを表示
	ImGui::DragFloat("shootHeight_", &shootHeight_);
	
	// 状態変更遅延を表示
	ImGui::DragFloat("stateChangeDelay_", &stateChangeDelay_);

	// 突進のダメージを表示
	ImGui::DragFloat("tackleDamage_", &tackleDamage_);
}

bool Component_BossBehavior::IsDead()
{
	// ボスの状態がDEADであり、大きさが0以下の場合
	return (bNowState_ == BOSS_STATE_DEAD && holder_->GetScale().x <= 0);
}

BossState Component_BossBehavior::RandomStatePick()
{
	// 抽選結果を格納する変数を用意
	BossState res = BOSS_STATE_MAX;

	// 状態を抽選ただし、IDLEとDEADは除外
	do res = (BossState)(rand() % BOSS_STATE_MAX);
	while (res == BOSS_STATE_IDLE || res == BOSS_STATE_DEAD);

	// 抽選結果を返す
	return res;
}

void Component_BossBehavior::Shot()
{
	// 射撃攻撃コンポーネントの取得 & nullチェック
	Component_ShootAttack* shoot = (Component_ShootAttack*)GetChildComponent("ShootAttack");

	// タイマーの取得 & nullチェック
	Component_Timer* timer = (Component_Timer*)GetChildComponent("Timer");
	if (timer == nullptr || shoot == nullptr) return;

	// タイマーの設定 & 開始
	timer->SetTime(SHOT_TIME);
	timer->Start();

	// 角度を格納する変数を用意
	static float angle = 0;
	static float rotateSpeed = 1.f + (rand() % ROTATE_RANDOM_VALUE);

	// 所有者の角度を更新
	angle += rotateSpeed;
	holder_->SetRotateY(angle);

	// タイマーが一定時間経過した場合
	if (timer->IsIntervalTime(shotInterval_)) {

		// 発射方向を設定
		XMVECTOR dir = { 0,0,1,0 };
		shoot->SetShootingDirection(XMVector3TransformCoord(dir, XMMatrixRotationY(XMConvertToRadians(holder_->GetRotate().y))));

		// 発射位置を設定
		XMFLOAT3 shootPosition = holder_->GetPosition();
		shootPosition.y += shootHeight_;
		shoot->SetShootingPosition(shootPosition);

		// 発射
		shoot->Execute();
		Audio::Play(Audio::Load("Audios/風魔法1.wav",false));
	}

	// タイマーが終了した場合
	if (timer->GetIsEnd()) {
		
		// 待機状態に遷移
		SetState(BOSS_STATE_IDLE);

		// タイマーをリセット
		timer->Reset();

		// 角度をリセット
		angle = 0;

		// 回転速度を 1.fから15.fの範囲でランダムに設定
		rotateSpeed = 1.f + (rand() % ROTATE_RANDOM_VALUE);
	}
}

void Component_BossBehavior::Tackle()
{
	// 初回フラグを用意
	static bool isFirst = true;

	// 突進行動コンポーネントの取得 & nullチェック
	Component_TackleMove* tackleMove = (Component_TackleMove*)GetChildComponent("TackleMove");
	if (tackleMove == nullptr) return;

	// 初回のみ処理を行う
	if (isFirst) {
		// 突進攻撃の初回フラグをfalseに設定
		isFirst = false;

		// プレイヤーの方向を向く
		{
			// 所有者の位置を取得
			XMFLOAT3 holderPos = holder_->GetPosition();

			// 対象の位置を取得
			XMFLOAT3 targetPos = target_->GetPosition();

			// 所有者の位置を対象の位置に向ける
			holder_->SetRotateY(atan2f(targetPos.z - holderPos.z, targetPos.x - holderPos.x) * 180 / XM_PI);
		}
		
		// 突進攻撃を実行
		{
			// 情報の取得
			// 所有者の位置を取得
			XMFLOAT3 holderPos = holder_->GetPosition();

			// 対象の位置を取得
			XMFLOAT3 targetPos = target_->GetPosition();

			// 突進方向を設定 
			XMVECTOR direction = XMVector3Normalize(XMLoadFloat3(&targetPos) - XMLoadFloat3(&holderPos));
			tackleMove->SetDirection(XMVectorSetY(direction, 0));


			// ステージ情報を取得
			Stage* pStage = (Stage*)(holder_->FindObject("Stage"));
			if (pStage == nullptr) return;
			std::vector<StageObject*> stageObj = pStage->GetStageObjects();
			float dodgeDistance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&targetPos) - XMLoadFloat3(&holderPos)));



			// 距離を設定
			tackleMove->SetDistance(dodgeDistance - MODEL_SIZE_HALF );

			// 突進を実行
			tackleMove->Execute();
		}

		// サウンドを再生
		Audio::Play(Audio::Load("Audios/火炎魔法1.wav", false));

		// エフェクトを再生
		{
			// 情報の設定
			DirectX::XMStoreFloat4x4(&(effekseerTransform.matrix), holder_->GetWorldMatrix());
			effekseerTransform.isLoop = false;
			effekseerTransform.maxFrame = EFFECT_FRAME;
			effekseerTransform.speed = EFFECT_SPEED;

			// 再生
			effectModelTransform_ = EFFEKSEERLIB::gEfk->Play("sword", effekseerTransform);
		}

	}

	// 突進攻撃が終了した場合
	if (tackleMove->IsActived() == false) { SetState(BOSS_STATE_IDLE); isFirst = true; }
}

void Component_BossBehavior::Idle()
{
	// タイマーの取得 & nullチェック
	Component_Timer* timer = (Component_Timer*)GetChildComponent("Timer");
	if (timer == nullptr) return;

	// タイマーの設定
	{
		// 状態を遷移するまでの時間を設定
		timer->SetTime(stateChangeDelay_);

		// タイマーを開始
		timer->Start();
	}

	// タイマーが終了した場合
	if (timer->GetIsEnd()) {

		// ランダムに状態を遷移
		SetState(RandomStatePick());

		// タイマーをリセット
		timer->Reset();
	}
}

void Component_BossBehavior::Dead()
{
	Transform effectTransform;
	effectTransform.position_ = holder_->GetPosition();

	// 初回のみ処理を行う
	if (isDeadStart_) {
		// 初回フラグをfalseに設定
		isDeadStart_ = false;

		// サウンドを再生
		Audio::Play(Audio::Load("Audios/DJのスクラッチ2.wav", false));
	}

	// 大きさが０でない場合
	if (holder_->GetScale().x >= 0) {
	// キャラクターの大きさを徐々に小さくする
	holder_->SetScale(holder_->GetScale().x - SMALL_VEROSITY);
	}
	else {
		// 情報の設定
		DirectX::XMStoreFloat4x4(&(effekseerTransform.matrix), effectTransform.GetWorldMatrix());
		effekseerTransform.isLoop = false;
		effekseerTransform.maxFrame = EFFECT_FRAME;
		effekseerTransform.speed = EFFECT_SPEED;

		// 再生
		effectModelTransform_ = EFFEKSEERLIB::gEfk->Play("fire", effekseerTransform);
		
		// サウンドを再生
		Audio::Play(Audio::Load("Audios/爆発1.wav", false));
	}
}