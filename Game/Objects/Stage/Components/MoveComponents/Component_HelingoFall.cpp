#include "Component_HelingoFall.h"

// �C���N���[�h
#include "../../../../../Engine/ImGui/imgui.h"

#include "../../StageObject.h"
#include "../TimerComponent/Component_Timer.h"
#include "../MoveComponents/Component_Fall.h"
#include "../MoveComponents/Component_Rise.h"


namespace
{
	float riseWaitTime_ = 5 ;			//�㏸
	float fallWaitTime_ = 0.8;			//�~��
}

// �R���X�g���N�^
Component_HelingoFall::Component_HelingoFall(string _name, StageObject* _holder, Component* _parent)
	: Component(_holder, _name, Fall, _parent),
	fallSpeed_(), riseSpeed_(), fallDistance_(),startHeight_(),
	isActive_(false), nowState_(WAIT), prevState_(RISE)
{
}

// ������
void Component_HelingoFall::Initialize()
{
	if (FindChildComponent("Timer") == false)AddChildComponent(CreateComponent("Timer", Timer, holder_, this));
	if (FindChildComponent("OnlyFall") == false)AddChildComponent(CreateComponent("OnlyFall", OnlyFall, holder_, this));
	if (FindChildComponent("OnlyRise") == false)AddChildComponent(CreateComponent("OnlyRise", OnlyRise, holder_, this));
}

// �X�V
void Component_HelingoFall::Update()
{   
	auto fall = dynamic_cast<Component_Fall*>(GetChildComponent("OnlyFall"));
	if (fall == nullptr) return;

	auto rise = dynamic_cast<Component_Rise*>(GetChildComponent("OnlyRise"));
	if (rise == nullptr) return;

	// ���s�������ǂ���
	if (isActive_) {
		XMFLOAT3 holderPos = holder_->GetPosition();
		
		switch (nowState_)
		{
		case FALL:
			fall->Execute(); 
			if (fall->IsActive()) {
				auto timer = dynamic_cast<Component_Timer*>(GetChildComponent("Timer"));
				timer->Reset();
				SetState(WAIT);
			}
			break;	// �~�����̏���

		case RISE:
			rise->Execute();
			if (rise->IsEnd()) {
				auto timer = dynamic_cast<Component_Timer*>(GetChildComponent("Timer"));
				timer->Reset();
				Stop();
				SetState(WAIT);
			}
			break;	// �㏸���̏���
		case WAIT:
			Wait();				 break;	// �ҋ@���̏���
		}

		// �ʒu���m��
		holder_->SetPosition(holderPos);
	}
}

// �J��
void Component_HelingoFall::Release()
{
}

// �ۑ�
void Component_HelingoFall::Save(json& _saveObj)
{
	// �ۑ�
	_saveObj["fallSpeed_"] = fallSpeed_;
	_saveObj["riseSpeed_"] = riseSpeed_;
	_saveObj["fallDistance_"] = fallDistance_;
	_saveObj["riseWaitTime_"] = riseWaitTime_;
	_saveObj["fallWaitTime_"] = fallWaitTime_;
}

// �Ǎ�
void Component_HelingoFall::Load(json& _loadObj)
{
	// �Ǎ�
	if (_loadObj.contains("fallSpeed_"))fallSpeed_ = _loadObj["fallSpeed_"];
	if (_loadObj.contains("riseSpeed_"))riseSpeed_ = _loadObj["riseSpeed_"];
	if (_loadObj.contains("fallDistance_"))fallDistance_ = _loadObj["fallDistance_"];
	if (_loadObj.contains("riseWaitTime_"))riseWaitTime_ = _loadObj["riseWaitTime_"];
	if (_loadObj.contains("fallWaitTime_"))fallWaitTime_ = _loadObj["fallWaitTime_"];
}

// ImGui�\��
void Component_HelingoFall::DrawData()
{
	// ImGui�`��
	ImGui::Text("Component_Fall");
	ImGui::Text(nowState_ == FALL ? "FALL" : nowState_ == RISE ? "RISE" : "WAIT");
	ImGui::Text(prevState_ == FALL ? "FALL" : prevState_ == RISE ? "RISE" : "WAIT");
	ImGui::Text("isActive_ : %s", isActive_ ? "true" : "false");

	ImGui::DragFloat("riseWaitTime_", &riseWaitTime_, 0.1f);
	ImGui::DragFloat("fallWaitTime_", &fallWaitTime_, 0.1f);

	if (ImGui::Button("Falling")) {
		Execute();
	}
}

void Component_HelingoFall::FallMove(float& _height)
{
	// �O��̏�Ԃ��ҋ@��Ԃ̎��A�J�n�̍������L�^
	// 2�T�ڂł���if�ɓ���Ȃ��悤�ɂ��邽�߁AprevState��FALL�ɕύX
	if (prevState_ == WAIT) {
		startHeight_ = _height;
		prevState_ = FALL;
	}
	// �ڕW�̍���
	float  targetHeight = startHeight_ - fallDistance_;
	
	// �������ڕW�̍����ɒB���Ă��Ȃ�������...
	if (_height > targetHeight) {
		
		// �~�����x������������������
		_height -= (fallSpeed_);

	}
	// ���������̍����ɒB������...
	else {
		// �^�C�}�[�����Z�b�g
		auto timer = dynamic_cast<Component_Timer*>(GetChildComponent("Timer"));
		timer->Reset();

		// �ڕW�̍����ɐݒ�
		_height = targetHeight;
		
		// �ҋ@��ԂɑJ��
		SetState(WAIT);
	}
}

void Component_HelingoFall::RiseMove(float& _height)
{
	// �ڕW�̍���
	float targetHeight = startHeight_;

	// �������ڕW�̍����ɒB���Ă��Ȃ�������...
	if (_height < targetHeight) {

		// �㏸���x�������������グ��
		_height += riseSpeed_;

	}
	else {
		// �^�C�}�[�����Z�b�g
		auto timer = dynamic_cast<Component_Timer*>(GetChildComponent("Timer"));
		timer->Reset();

		// �ڕW�̍����ɐݒ�
		_height = targetHeight;

		// �ҋ@��ԂɑJ��
		SetState(WAIT);

		// ������I��
		Stop();
	}
}

void Component_HelingoFall::Wait()
{
	// �^�C�}�[���擾
	auto timer = dynamic_cast<Component_Timer*>(GetChildComponent("Timer"));
	
	if (prevState_ == FALL) {

		// �^�C�}�[���J�n
		timer->SetTime(riseWaitTime_);
		timer->Start();

		auto rise = dynamic_cast<Component_Rise*>(GetChildComponent("OnlyRise"));
		if (rise == nullptr) return;
		rise->SetIsEnd(false);

		// �^�C�}�[���I��������A�㏸��ԂɑJ��
		if(timer->GetIsEnd())SetState(RISE);
	}

	else if (prevState_ == RISE) {

		// �^�C�}�[���J�n
		timer->SetTime(fallWaitTime_);
		timer->Start();

		// �^�C�}�[���I��������A�~����ԂɑJ��
		if (timer->GetIsEnd())SetState(FALL);
	}

	float time = timer->GetNowTime();
	ImGui::Text("time : %f", time);
}
