#include "Component_Fall.h"

// �C���N���[�h
#include "../../../../../Engine/ImGui/imgui.h"

#include "../../StageObject.h"
#include "../TimerComponent/Component_Timer.h"

namespace
{
	float riseWaitTime_ = 5 ;			//�㏸
	float fallWaitTime_ = 0.8;			//�~��
}

// �R���X�g���N�^
Component_Fall::Component_Fall(string _name, StageObject* _holder, Component* _parent)
	: Component(_holder, _name, Fall, _parent)
	, fallSpeed_(), riseSpeed_(), fallDistance_(),
	isFalling_(false), isFirstTime_(true), isActive_(false), startRisePosition_(), startFallPosition_(), state_(WAIT), prevState_(RISE)
{
}

// ������
void Component_Fall::Initialize()
{
	if (FindChildComponent("Timer") == false)AddChildComponent(CreateComponent("Timer", Timer, holder_, this));
}

// �X�V
void Component_Fall::Update()
{   
	// ���s�������ǂ���
	if (isActive_) {
		XMFLOAT3 holderPos = holder_->GetPosition();
		auto timer = dynamic_cast<Component_Timer*>(GetChildComponent("Timer"));
		if (timer == nullptr) return;
		if (isFirstTime_) {
			// ���~�E�㏸�̊J�n�ʒu��ێ�����ϐ�
			startRisePosition_.y = holderPos.y;
			startFallPosition_.y = holderPos.y;

			isFirstTime_ = false;
			isRised_ = false;
		}

		switch (state_)
		{
		case FALL:
			// �~�����̏���
			if (holderPos.y > startFallPosition_.y - fallDistance_) {
				holderPos.y -= (fallSpeed_ );
				isFalling_ = true;
			}
			else {
				// ���̂����̍����ɒB������㏸���J�n����
				timer->Reset();
				holderPos.y = startFallPosition_.y - fallDistance_;
				prevState_ = FALL;
				state_ = WAIT;
			}
			break;

		case RISE:
			// �㏸���̏���
			if (holderPos.y < startRisePosition_.y) {
				holderPos.y += riseSpeed_;
				isFalling_ = false;
			}
			else {
				// ���̂���荂���ɒB������㏸���~���A�~�����J�n����
				timer->Reset();
				prevState_ = RISE;
				state_ = WAIT;
				isActive_ = false;
				isFirstTime_ = true;
				holderPos.y = startRisePosition_.y;  // �㏸�J�n�ʒu�ɖ߂�
				startFallPosition_.y = holderPos.y;  // �~���J�n�ʒu���L�^
			}
			break;

		case WAIT:
			isFalling_ = false;
			if (prevState_ == FALL) {
				timer->SetTime(riseWaitTime_);
				timer->Start();
				if (timer->GetIsEnd()) {
					state_ = RISE;
				}
			}
			else if (prevState_ == RISE) {
				timer->SetTime(fallWaitTime_);
				timer->Start();
				if (timer->GetIsEnd()) {
					state_ = FALL;
				}
			}
			ImGui::Text("Time:%f", timer->GetNowTime());

			break;

		}

		// �ʒu���m��
		holder_->SetPosition(holderPos);
	}
}

// �J��
void Component_Fall::Release()
{
}

// �ۑ�
void Component_Fall::Save(json& _saveObj)
{
	// �ۑ�
	_saveObj["fallSpeed_"] = fallSpeed_;
	_saveObj["riseSpeed_"] = riseSpeed_;
	_saveObj["fallDistance_"] = fallDistance_;
	_saveObj["riseWaitTime_"] = riseWaitTime_;
	_saveObj["fallWaitTime_"] = fallWaitTime_;
}

// �Ǎ�
void Component_Fall::Load(json& _loadObj)
{
	// �Ǎ�
	if (_loadObj.contains("fallSpeed_"))fallSpeed_ = _loadObj["fallSpeed_"];
	if (_loadObj.contains("riseSpeed_"))riseSpeed_ = _loadObj["riseSpeed_"];
	if (_loadObj.contains("fallDistance_"))fallDistance_ = _loadObj["fallDistance_"];
	if (_loadObj.contains("riseWaitTime_"))riseWaitTime_ = _loadObj["riseWaitTime_"];
	if (_loadObj.contains("fallWaitTime_"))fallWaitTime_ = _loadObj["fallWaitTime_"];
}

// ImGui�\��
void Component_Fall::DrawData()
{
	// ImGui�`��
	ImGui::Text("Component_Fall");
	ImGui::DragFloat("fallSpeed_", &fallSpeed_, 0.1f);
	ImGui::DragFloat("riseSpeed_", &riseSpeed_, 0.1f);
	ImGui::DragFloat("fallDistance_", &fallDistance_, 0.1f);
	ImGui::DragFloat("riseWaitTime_", &riseWaitTime_, 0.1f);
	ImGui::DragFloat("fallWaitTime_", &fallWaitTime_, 0.1f);

	if (ImGui::Button("Falling")) {
		Start();
	}
}
