#include "RotationZComponent.h"

// �C���N���[�h
#include "../../StageObject.h"
#include "../../../../../Engine/ImGui/imgui.h"

RotationZComponent::RotationZComponent(StageObject* _holeder)
	:Component(_holeder, "RotationZComponent", RotationZ), angle_(0), rotateSpeed_(0)
{
}

void RotationZComponent::Initialize()
{
}

void RotationZComponent::Update()
{
	// Z���ɉ�]������
	angle_ += rotateSpeed_;

	// ��]���x�̒l�����̒l�ł��� ���� �p�x��360�x�����������A�p�x���O�ɖ߂�
	if (!std::signbit(rotateSpeed_) && angle_ >= 360.f)angle_ = 0.f;

	// ��]���x�̒l�����̒l�ł��� ���� �p�x��-360�x�����������A�p�x���O�ɖ߂�
	if (std::signbit(rotateSpeed_) && angle_ <= -360.f)angle_ = 0.f;

	// ��]���x���O�ł͂Ȃ����A�p�x��K��������
	if (rotateSpeed_ != 0.f)holder_->SetRotateZ(angle_);
}

void RotationZComponent::Release()
{
}

void RotationZComponent::Save(json& _saveObj)
{
	// ��]���x��ۑ�
	_saveObj["rotateSpeed_"] = rotateSpeed_;

	// �p�x��ۑ�
	_saveObj["angle_"] = angle_;
}

void RotationZComponent::Load(json& _loadObj)
{
	// ��]���x��Ǎ�
	rotateSpeed_ = _loadObj["rotateSpeed_"];

	// �p�x��Ǎ�
	angle_ = _loadObj["angle_"];
}

void RotationZComponent::DrawData()
{
	// ��]���x��ҏW
	ImGui::DragFloat("rotateSpeed_", &rotateSpeed_, 0.1f);
}
