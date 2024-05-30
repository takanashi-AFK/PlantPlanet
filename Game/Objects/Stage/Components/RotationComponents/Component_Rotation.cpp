#include "Component_Rotation.h"

Component_Rotation::Component_Rotation(string _name, StageObject* _holder)
	:Component(_holder,_name,Rotation)
{
}

void Component_Rotation::Initialize()
{
	// �e���̉�]�p�R���|�[�l���g��ǉ�
	if (FindChildComponent(RotationX) == false)AddChildComponent(CreateComponent("RotationX01", RotationX, holder_));
	if (FindChildComponent(RotationY) == false)AddChildComponent(CreateComponent("RotationY01", RotationY, holder_));
	if (FindChildComponent(RotationZ) == false)AddChildComponent(CreateComponent("RotationZ01", RotationZ, holder_));
}

void Component_Rotation::Update()
{
}

void Component_Rotation::Release()
{
}

void Component_Rotation::Save(json& _saveObj)
{
}

void Component_Rotation::Load(json& _loadObj)
{
}

void Component_Rotation::DrawData()
{
}
