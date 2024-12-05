#include "Component_MeleeEnemyBehavior.h"

Component_MeleeEnemyBehavior::Component_MeleeEnemyBehavior(string _name, StageObject* _holder, Component* _parent):
	Component(_holder, _name, MeleeEnemyBehavior, _parent)
{
}

void Component_MeleeEnemyBehavior::Initialize()
{
}

void Component_MeleeEnemyBehavior::Update()
{
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
