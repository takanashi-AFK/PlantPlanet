#include "Component_Plant.h"

Component_Plant::Component_Plant(string _name, StageObject* _holder, Component* _parent)
	: Component(_holder, _name, ComponentType::Plant, _parent)
{
}

void Component_Plant::Initialize()
{
}

void Component_Plant::Update()
{
}

void Component_Plant::Release()
{
}

void Component_Plant::Save(json& _saveObj)
{
}

void Component_Plant::Load(json& _loadObj)
{
}

void Component_Plant::DrawData()
{
}
