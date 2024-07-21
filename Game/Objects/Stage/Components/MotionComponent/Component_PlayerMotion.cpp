#include "../../../../../Engine/ResourceManager/Model.h"
#include "Component_PlayerMotion.h"
#include "../../Stage.h"

Component_PlayerMotion::Component_PlayerMotion(string _name, StageObject* _holder,  Component* _parent)
	:Component_Motion(_name, _holder, PlayerMotion, _parent)
{
}

void Component_PlayerMotion::Initialize()
{
	motionModelMap_.insert(std::make_pair(PSTATE_WALK, Model::Load("Models/Player/Running.fbx")));
	// ���f���̃��[�h
	auto stageObjectList = ((Stage*)holder_)->GetStageObjects();

	for (auto stageObject : stageObjectList) {
		auto componentVector = stageObject->FindComponent(PlayerBehavior);
		if (componentVector.empty()) continue;
		for (auto pl : componentVector) {
			if (pl == nullptr)return;
			playerBehavior_ = ((Component_PlayerBehavior*)pl);
		}
	}
}

void Component_PlayerMotion::Update()
{
	static 	bool isAnimationNow_ = false;
	if (playerBehavior_ == nullptr)return;
	// ���݂�State���擾
	state_ = playerBehavior_->GetState();

	// �������܂������ĂȂ�����m�F���肢
	if (state_ == PSTATE_WALK ) {
		// playerBehavior�̃��f���n���h����ς���
		if (((StageObject*)playerBehavior_)->GetModelHandle() == motionModelMap_[PSTATE_WALK])return;
		((StageObject*)playerBehavior_)->SetModelHandle(motionModelMap_[PSTATE_WALK]);

		// ���[�V�����Đ�
		if (isAnimationNow_ == false) 
		Model::SetAnimFrame(((StageObject*)playerBehavior_)->GetModelHandle(), 0, 40, 1);
		isAnimationNow_ = true;
		
	}
	else if(state_ != PSTATE_WALK) {
		isAnimationNow_ = false;
	}
}

void Component_PlayerMotion::Release()
{
}

void Component_PlayerMotion::DrawData()
{
}

// playerState�̃A�h���X���擾
// 