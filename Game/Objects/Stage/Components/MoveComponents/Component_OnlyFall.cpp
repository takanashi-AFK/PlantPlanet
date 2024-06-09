#include "Component_OnlyFall.h"
#include "../../StageObject.h"
#include "../../Components/TimerComponent/Component_Timer.h"
#include "../../../../../Engine/ImGui/imgui.h"
Component_OnlyFall::Component_OnlyFall(string _name, StageObject* _holder, Component* _parent)
	: Component(_holder, _name, OnlyFall, _parent), isActive_(false), isFirst_(true), fallSpeed_(0.0f), fallDistance_(0.0f), height_(0.0f), startHeight_(0.0f)
{
}

void Component_OnlyFall::Initialize()
{
}

void Component_OnlyFall::Update()
{
	if (isActive_) {

		// ����H����
		// TargetHeight����ImGui�Őݒ�ł���΂����悤�Ȃ�������
	/*	if (isFirst_) {
			startHeight_ = holder_->GetPosition().y;
			isFirst_ = false;
		}*/

		height_ = holder_->GetPosition().y;
		// �ڕW�̍���
		float  targetHeight = startHeight_ - fallDistance_;

		// �������ڕW�̍����ɒB���Ă��Ȃ�������...
		if (height_ > targetHeight) {

			// �~�����x������������������
			height_ -= (fallSpeed_);

		}
		// ���������̍����ɒB������...
		else {
			
			// �ڕW�̍����ɐݒ�
			height_ = targetHeight;
			isActive_ = false;
		}
		holder_->SetPosition(holder_->GetPosition().x,height_,holder_->GetPosition().z);
	}
}

void Component_OnlyFall::Release()
{
}

void Component_OnlyFall::Save(json& _saveObj)
{
	_saveObj["startHeight_"] = startHeight_;
	_saveObj["fallDistance_"] = fallDistance_;
	_saveObj["fallSpeed_"] = fallSpeed_;
	_saveObj["height_"] = height_;
}

void Component_OnlyFall::Load(json& _loadObj)
{
	if (_loadObj.contains("startHeight_"))startHeight_ = _loadObj["startHeight_"];
	if (_loadObj.contains("fallDistance_"))fallDistance_ = _loadObj["fallDistance_"];
	if (_loadObj.contains("fallSpeed_"))fallSpeed_ = _loadObj["fallSpeed_"];
	if (_loadObj.contains("height_"))height_ = _loadObj["height_"];
}

void Component_OnlyFall::DrawData()
{
	ImGui::DragFloat("Fall Distance", &fallDistance_, 0.1f, 0.0f, 100.0f);
	ImGui::DragFloat("Fall Speed", &fallSpeed_, 0.1f, 0.0f, 100.0f);
	if (ImGui::Button("Execute"))Execute();
}
