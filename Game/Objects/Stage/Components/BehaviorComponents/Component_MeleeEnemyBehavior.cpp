
// インクルード
#include "../../../Engine/ImGui/imgui.h"
#include "../../Components/DetectorComponents/Component_CircleRangeDetector.h"
#include "../../Components/MoveComponents/Component_Chase.h"
#include "../../Stage.h"
#include "../../StageObject.h"
#include "../DetectorComponents/Component_FanRangeDetector.h"
#include "../GaugeComponents/Component_HealthGauge.h"
#include "../TimerComponent/Component_Timer.h"
#include "Component_MeleeEnemyBehavior.h"
#include "../../../../Plants/PlantCollection.h"
#include <unordered_map>
#include "../PlantComponents/Component_Plant.h"


Component_MeleeEnemyBehavior::Component_MeleeEnemyBehavior(string _name, StageObject* _holder, Component* _parent):
	Component(_holder, _name, MeleeEnemyBehavior, _parent),
	enemyState_(E_IDLE),
	playerObjects_(nullptr),
	deltaDistance_(1.f),
	power_(5.f),
	isAttacked_(false),
	isFlowerSpawned_(false)
{
}

void Component_MeleeEnemyBehavior::Initialize()
{
	holder_->AddCollider(new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1)));
	holder_->SetObjectType(StageObject::TYPE_ENEMY);
	EFFEKSEERLIB::gEfk->AddEffect("enemyDead", "Effects/EnemyDead.efk");


	// 子コンポーネントを追加
	if (FindChildComponent("CircleRangeDetector") == false)AddChildComponent(CreateComponent("CircleRangeDetector", CircleRangeDetector, holder_, this));
	if (FindChildComponent("Chase") == false)AddChildComponent(CreateComponent("Chase", Chase, holder_, this));
	if (FindChildComponent("FanRangeDetector") == false)AddChildComponent(CreateComponent("FanRangeDetector", FanRangeDetector, holder_, this));
	if (FindChildComponent("timer")==false)AddChildComponent(CreateComponent("timer", Timer, holder_, this));
	if (FindChildComponent("HealthGauge") == false)AddChildComponent(CreateComponent("HealthGauge", HealthGauge, holder_, this));
}

void Component_MeleeEnemyBehavior::Update()
{
	//各Stateごとの処理
	switch (enemyState_)
	{
	case E_IDLE:	Idle();		break; // 待機状態 プレイヤーが近くにいるかどうかを検知
	case E_MOVE:	Move();		break; // 移動状態 プレイヤーに近づく
	case E_ATTACK:	Attack();	break; // 攻撃状態 プレイヤーに攻撃
	case E_DEAD:	Dead();		break; // 死亡状態
	}
}

void Component_MeleeEnemyBehavior::Release()
{
}

void Component_MeleeEnemyBehavior::Save(json& _saveObj)
{
	_saveObj["speed_"] = speed_;
	_saveObj["power_"] = power_;
	_saveObj["attackCoolTime_"] = attackCoolTime_;
	_saveObj["dropFlowerName_"] = dropFlowerName_;
}

void Component_MeleeEnemyBehavior::Load(json& _loadObj)
{
	if (_loadObj.contains("speed_"))speed_ = _loadObj["speed_"];
	if (_loadObj.contains("power_"))power_ = _loadObj["power_"];
	if (_loadObj.contains("attackCoolTime_"))attackCoolTime_ = _loadObj["attackCoolTime_"];
	if (_loadObj.contains("dropFlowerName_"))dropFlowerName_ = _loadObj["dropFlowerName_"];
}

void Component_MeleeEnemyBehavior::DrawData()
{
	// 移動速度
	ImGui::DragFloat("speed_", &speed_, 0.1, 0);
	// 与ダメージ
	ImGui::DragFloat("power_", &power_, 0.1, 0);
	// 攻撃クールタイム
	ImGui::DragFloat("attackCoolTime_",&attackCoolTime_, 0.1, 0);

	static int select = 1;

	unordered_map<int, PlantData> plant = PlantCollection::GetPlants();

	if (plant.size() == 0)ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "No plant data!"); \
	else {
		// コンボボックス
		if (ImGui::BeginCombo("FlowerSetData", plant[select].name_.c_str())) {
			for (const auto& [key, data] : plant) { // `unordered_map` をループ
				bool is_selected = (select == key); // 現在選択中か確認
				if (ImGui::Selectable(data.name_.c_str(), is_selected)) {
					select = key; // キーを更新
					dropFlowerName_ = data.name_;
				}
				if (is_selected) {
					ImGui::SetItemDefaultFocus(); // デフォルトフォーカス
				}
			}
			ImGui::EndCombo();
		}
	}

	ImGui::Text("nowState %d", enemyState_);
}

void Component_MeleeEnemyBehavior::Idle()
{
	// ステージ情報を取得
	Stage* pStage = (Stage*)(holder_->FindObject("Stage"));
	if (pStage == nullptr)return;

	// 各コンポーネントを取得
	Component_CircleRangeDetector* detector = (Component_CircleRangeDetector*)(GetChildComponent("CircleRangeDetector"));
	if (detector == nullptr)return;

	Component_Timer* timer = (Component_Timer*)(GetChildComponent("timer"));
	if (timer == nullptr)return;

	// 前回のループで攻撃していたらクールタイムを設定
	if (isAttacked_ == true) {

		timer->SetTime(attackCoolTime_);
		timer->Start();
		if (timer->GetIsEnd()) {
			isAttacked_ = false;
			timer->Reset();
		}
	}
	else if(isAttacked_ == false){
		// プレイヤーが近くにいるかどうかを検知
		if (detector->IsContains()) {
			for (StageObject* object : pStage->GetStageObjects()) {
				// Playerの属性をもったオブジェクトを取得
				if (object->GetObjectType() == StageObject::TYPE_PLAYER) {
					playerObjects_ = object;
				}
			}
			if (playerObjects_ == nullptr) {
				SetState(E_IDLE);
				return;
			}
			else {
				SetState(E_MOVE);
				return;
			}
		}
	}
	Component_HealthGauge *hg = (Component_HealthGauge*)(GetChildComponent("HealthGauge"));
	if (hg == nullptr)return;

	if (hg->IsDead())
		SetState(E_DEAD);

}

void Component_MeleeEnemyBehavior::Move()
{
	// 各コンポーネントを取得
	Component_Chase* chase = (Component_Chase*)(GetChildComponent("Chase"));
	if (chase == nullptr)return;

	Component_CircleRangeDetector* detector = (Component_CircleRangeDetector*)(GetChildComponent("CircleRangeDetector"));
	if (detector == nullptr)return;

	// ターゲットを設定、追跡開始
	chase->SetTarget(playerObjects_);
	chase->SetSpeed(speed_);
	chase->Execute();

	XMFLOAT3 playerPos = playerObjects_->GetPosition();
	XMFLOAT3 enemyPos = holder_->GetPosition();
	XMVECTOR deltaDist = XMLoadFloat3(&playerPos) - XMLoadFloat3(&enemyPos);

	// 距離が一定以下になったら攻撃状態に遷移
	if (XMVectorGetX(XMVector3Length(deltaDist)) <= deltaDistance_) {
		chase->Stop();
		SetState(E_ATTACK);
	};

	// 探知範囲外に出たら待機状態に遷移
	if (!detector->IsContains()) {
		chase ->Stop();
		SetState(E_IDLE);
	}

	Component_HealthGauge* hg = (Component_HealthGauge*)(GetChildComponent("HealthGauge"));
	if (hg == nullptr)return;

	if (hg->IsDead())SetState(E_DEAD);


}

void Component_MeleeEnemyBehavior::Attack()
{
	// 各コンポーネントを取得
	Component_FanRangeDetector* attackRange = (Component_FanRangeDetector*)(GetChildComponent("FanRangeDetector"));
	if (attackRange == nullptr)return;

	// プレイヤーが攻撃範囲内にいるかどうかを検知
	attackRange->SetTarget(playerObjects_);
	if (attackRange->IsContains()) {
		// プレイヤーにダメージを与える
		Component_HealthGauge* healthGauge = (Component_HealthGauge*)playerObjects_->FindComponent("PlayerHealthGauge");
		if (healthGauge == nullptr)return;
		healthGauge->TakeDamage(power_);
		// フラグを立てる
		isAttacked_ = true;
	}
	if (isAttacked_ == true) {
		SetState(E_IDLE);
	}

	Component_HealthGauge* hg = (Component_HealthGauge*)(GetChildComponent("HealthGauge"));
	if (hg == nullptr)return;

	if (hg->IsDead())SetState(E_DEAD);
}

void Component_MeleeEnemyBehavior::Dead()
{
	// その場に指定した花を出す
	if (isFlowerSpawned_ == true)return;
	if (dropFlowerName_ == "")return;

	unordered_map<int, PlantData> plantData = PlantCollection::GetPlants();
	if (plantData.size() <= 0)return;

	EFFEKSEERLIB::EFKTransform t;
	DirectX::XMStoreFloat4x4(&(t.matrix), holder_->GetWorldMatrix());
	t.isLoop = false;
	t.maxFrame = 60;
	t.speed = 0.1;
	effectModelTransform = EFFEKSEERLIB::gEfk->Play("enemyDead", t);

	holder_->KillMe();

	for (auto& plant : plantData) {
		if (plant.second.name_ == dropFlowerName_) {
			// その場に花を生成
			Stage* pStage = (Stage*)(holder_->FindObject("Stage"));
			if (pStage == nullptr)return;

			StageObject* plantObject = CreateStageObject("EnemyDropPlant1", plant.second.modelFilePath_, pStage);

			// 当たり判定を設定
			plantObject->SetIsColliding(false);

			// プラントコンポーネントを作成
			Component_Plant* plantComponent = (Component_Plant*)CreateComponent("Plant", Plant, plantObject, this);

			// 植物情報を設定
			plantComponent->SetData(plant.second);

			// コンポーネントを追加
			plantObject->AddComponent(plantComponent);

			// サイズを設定
			plantObject->SetScale({ 0.3f,0.3f,0.3f });
			plantObject->SetPosition(holder_->GetPosition());

			// 属性を設定
			plantObject->SetObjectType(StageObject::TYPE_PLANT);

			pStage->DeleteStageObject(holder_);
			// ステージに追加
			((Stage*)holder_->GetParent())->AddStageObject(plantObject);

			isFlowerSpawned_ = true;
			break;
		}
	}
}