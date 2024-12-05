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
	// PlayerBehavior�������Ă�I�u�W�F�N�g��T��
	// �����ł����������H
	// ��������Move�ɕύX
}

void Component_MeleeEnemyBehavior::Move()
{
	// ������PlayerBehavior�������Ă�I�u�W�F�N�g�̕����ɑ΂��Đi��
	// �v���C���[�ƈ��̋����܂ŋ߂Â�����Attack�ɕύX

	// ��苗�����ꂽ��Idle�ɕύX
}

void Component_MeleeEnemyBehavior::Attack()
{
	// �O��������`�ɍU��������o��
	// �U�����肪����������v���C���[��HP�����炷

	// �U�����肪������Ȃ�������Move�ɕύX
	// �v���C���[����苗���ȏ㗣�ꂽ��idle�ɕύX
}

void Component_MeleeEnemyBehavior::Dead()
{
	// ���S�A�j��
}
