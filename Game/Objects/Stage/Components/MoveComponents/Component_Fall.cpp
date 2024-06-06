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
	isFalling_(false), isFirstTime_(true), isActive_(false), nowState_(WAIT), prevState_(RISE)
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
		
		switch (nowState_)
		{
		case FALL:FallMove(holderPos.y); break;	// �~�����̏���
		case RISE:RiseMove(holderPos.y); break;	// �㏸���̏���
		case WAIT:Wait();				 break;	// �ҋ@���̏���
		}

		// �ʒu���m��
		holder_->SetPosition(holderPos);

		// ���݂̏�Ԃ�ۑ�
		

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
		Execute();
	}
}

void Component_Fall::FallMove(float& _height)
{
	// �O��̏�Ԃ��ҋ@��Ԃ̎��A�J�n�̍������L�^
	if (prevState_ == WAIT) {
		startHeight_ = _height;
		prevState_ = FALL;
	}
	// �ڕW�̍���
	float  targetHeight = startHeight_ - fallDistance_;
	
	// �����@���@�ڕW�̍����@�ɒB�����Ă��Ȃ�������...
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

void Component_Fall::RiseMove(float& _height)
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

void Component_Fall::Wait()
{
	// �^�C�}�[���擾
	auto timer = dynamic_cast<Component_Timer*>(GetChildComponent("Timer"));
	
	if (prevState_ == FALL) {

		// �^�C�}�[���J�n
		timer->SetTime(riseWaitTime_);
		timer->Start();

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
