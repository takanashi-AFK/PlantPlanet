#include "RotationXComponent.h"

// �C���N���[�h
#include "../../StageObject.h"
#include "../../../../../Engine/ImGui/imgui.h"

RotationXComponent::RotationXComponent(StageObject* _holeder)
	:Component(_holeder,"RotationXComponent",RotationX), angle_(0), rotateSpeed_(0)
{
}

void RotationXComponent::Initialize()
{
}

void RotationXComponent::Update()
{
	// X���ɉ�]������
	angle_ += rotateSpeed_;
	if (rotateSpeed_ > 0)holder_->SetRotateX(angle_);
}

void RotationXComponent::Release()
{
}

void RotationXComponent::Save(json& _saveObj)
{
	// ��]���x��ۑ�
	_saveObj["rotateSpeed_"] = rotateSpeed_;

	// �p�x��ۑ�
	_saveObj["angle_"] = angle_;
}

void RotationXComponent::Load(json& _loadObj)
{
	// ��]���x��Ǎ�
	rotateSpeed_ = _loadObj["rotateSpeed_"];

	// �p�x��Ǎ�
	angle_ = _loadObj["angle_"];
}

void RotationXComponent::DrawData()
{
	// ��]���x��\��
	ImGui::DragFloat("rotateSpeed_", &rotateSpeed_, 0.1f);
}
