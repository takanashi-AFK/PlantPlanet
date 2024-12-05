#include "Component_MeleeEnemyBehavior.h"

Component_MeleeEnemyBehavior::Component_MeleeEnemyBehavior(string _name, StageObject* _holder, Component* _parent):
	Component(_holder, _name, MeleeEnemyBehavior, _parent),
	enemyState_(E_IDLE)
{
}

void Component_MeleeEnemyBehavior::Initialize()
{
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
}

void Component_MeleeEnemyBehavior::Idle()
{
	// PlayerBehaviorを持ってるオブジェクトを探す
	// 属性でもいいかも？
	// 見つけたらMoveに変更
}

void Component_MeleeEnemyBehavior::Move()
{
	// 見つけたPlayerBehaviorを持ってるオブジェクトの方向に対して進む
	// プレイヤーと一定の距離まで近づいたらAttackに変更

	// 一定距離離れたらIdleに変更
}

void Component_MeleeEnemyBehavior::Attack()
{
	// 前方方向扇形に攻撃判定を出す
	// 攻撃判定が当たったらプレイヤーのHPを減らす

	// 攻撃判定が当たらなかったらMoveに変更
	// プレイヤーが一定距離以上離れたらidleに変更
}

void Component_MeleeEnemyBehavior::Dead()
{
	// 死亡アニメ
}
