#include "Component_MeleeEnemyBehavior.h"
#include "../../Stage.h"
#include "../../StageObject.h"
#include "../../Components/DetectorComponents/Component_CircleRangeDetector.h"
#include "../../Components/MoveComponents/Component_Chase.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../DetectorComponents/Component_FanRangeDetector.h"
#include "../GaugeComponents/Component_HealthGauge.h"

Component_MeleeEnemyBehavior::Component_MeleeEnemyBehavior(string _name, StageObject* _holder, Component* _parent):
	Component(_holder, _name, MeleeEnemyBehavior, _parent),
	enemyState_(E_MAX),
	playerObjects_(nullptr),
	deltaDistance_(1.f),
	power_(5.f)
{
}

void Component_MeleeEnemyBehavior::Initialize()
{
	if (FindChildComponent("CircleRangeDetector") == false)AddChildComponent(CreateComponent("CircleRangeDetector", CircleRangeDetector, holder_, this));
	if (FindChildComponent("Chase") == false)AddChildComponent(CreateComponent("Chase", Chase, holder_, this));
	if (FindChildComponent("FanRangeDetector") == false)AddChildComponent(CreateComponent("FanRangeDetector", FanRangeDetector, holder_, this));
}

void Component_MeleeEnemyBehavior::Update()
{

	switch (enemyState_)
	{
	case E_IDLE:	Idle();		break;
	case E_MOVE:	Move();		break;
	case E_ATTACK:	Attack();	break;
	case E_DEAD:	Dead();		break;
	}
}

void Component_MeleeEnemyBehavior::Release()
{
}

void Component_MeleeEnemyBehavior::Save(json& _saveObj)
{
}

void Component_MeleeEnemyBehavior::Load(json& _loadObj)
{
}

void Component_MeleeEnemyBehavior::DrawData()
{
	if (ImGui::Button("Ecxecute"))SetState(E_IDLE);
}

void Component_MeleeEnemyBehavior::Idle()
{
	// PlayerBehaviorを持ってるオブジェクトを探す

	// ステージ情報の取得
	Stage* pStage = (Stage*)(holder_->FindObject("Stage"));
	if (pStage == nullptr)return;

	Component_CircleRangeDetector* detector = (Component_CircleRangeDetector*)(GetChildComponent("CircleRangeDetector"));
	if (detector == nullptr)return;

	if (detector->IsContains()) {
		ImGui::Text("Player is found");
		// ステージオブジェクトの取得
		for (StageObject* object : pStage->GetStageObjects()) {

			// 敵オブジェクトだったらリストに追加
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

void Component_MeleeEnemyBehavior::Move()
{
	// 見つけたPlayerBehaviorを持ってるオブジェクトの方向に対して進む
	// プレイヤーと一定の距離まで近づいたらAttackに変更
	Component_Chase* chase = (Component_Chase*)(GetChildComponent("Chase"));
	if (chase == nullptr)return;

	chase->SetTarget(playerObjects_);
	chase->Execute();

	// プレイヤーとの距離を取得
	XMFLOAT3 playerPos = playerObjects_->GetPosition();
	XMFLOAT3 enemyPos = holder_->GetPosition();
	XMVECTOR deltaDist = XMLoadFloat3(&playerPos) - XMLoadFloat3(&enemyPos);
	if (XMVectorGetX(XMVector3Length(deltaDist)) <= deltaDistance_) {
		chase->Stop();
		SetState(E_ATTACK);
	};

}

void Component_MeleeEnemyBehavior::Attack()
{

	Component_FanRangeDetector* attackRange = (Component_FanRangeDetector*)(GetChildComponent("FanRangeDetector"));
	if (attackRange == nullptr)return;
	// 前方方向扇形に攻撃判定を出す
	// 攻撃判定が当たったらプレイヤーのHPを減らす
	attackRange->SetTarget(playerObjects_);
	if (attackRange->IsContains()) {

		Component_HealthGauge* healthGauge = (Component_HealthGauge*)playerObjects_->FindComponent("PlayerHealthGauge");
		if (healthGauge == nullptr)return;
		healthGauge->TakeDamage(power_);
		SetState(E_IDLE);
	}


	// 攻撃判定が当たらなかったらMoveに変更
	// プレイヤーが一定距離以上離れたらidleに変更
}

void Component_MeleeEnemyBehavior::Dead()
{
	// その場に指定した花を出す
}
