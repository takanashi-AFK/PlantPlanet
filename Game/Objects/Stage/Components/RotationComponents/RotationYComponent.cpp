#include "RotationYComponent.h"

// �C���N���[�h
#include "../../StageObject.h"
#include "../../../../../Engine/ImGui/imgui.h"

RotationYComponent::RotationYComponent(StageObject* _holeder)
	:Component(_holeder,"RotationYComponent",RotationY),angle_(0),rotateSpeed_(0)
{
}

void RotationYComponent::Initialize()
{
}

void RotationYComponent::Update()
{
	// Y���ɉ�]������
	angle_ += rotateSpeed_;
	if (rotateSpeed_ > 0)holder_->SetRotateY(angle_);
}

void RotationYComponent::Release()
{
}

void RotationYComponent::Save(json& _saveObj)
{
	// ��]���x��ۑ�
	_saveObj["rotateSpeed_"] = rotateSpeed_;

	// �p�x��ۑ�
	_saveObj["angle_"] = angle_;
}

void RotationYComponent::Load(json& _loadObj)
{
	// ��]���x��Ǎ�
	rotateSpeed_ = _loadObj["rotateSpeed_"];

	// �p�x��Ǎ�
	angle_ = _loadObj["angle_"];
}

void RotationYComponent::DrawData()
{
	// ��]���x��\��
	ImGui::DragFloat("rotateSpeed_", &rotateSpeed_,0.1f);
}
