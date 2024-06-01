#include "Component_CactanBehavior.h"
#include "../TimerComponent/Component_Timer.h"
#include "../AttackComponents/Component_ShootAttack.h"
#include "../DetectorComponents/Component_CircleRangeDetector.h"

Component_CactanBehavior::Component_CactanBehavior(string _name, StageObject* _holder, Component* _parent)
	:Component(_holder, _name, CactanBihavior, _parent)
{
}

void Component_CactanBehavior::Initialize()
{
	// �K�v�ȃR���|�[�l���g��ǉ�
	if (FindChildComponent("CircleRangeDetector") == false)AddChildComponent(CreateComponent("CircleRangeDetector",CircleRangeDetector,holder_,this));
	if (FindChildComponent("ShootAttack") == false)AddChildComponent(CreateComponent("ShootAttack", ShootAttack, holder_, this));
	if (FindChildComponent("Timer") == false)AddChildComponent(CreateComponent("Timer", Timer, holder_, this));
}

void Component_CactanBehavior::Update()
{
	auto detector = dynamic_cast<Component_CircleRangeDetector*>(GetChildComponent("CircleRangeDetector"));
	if (detector == nullptr)return;

	// �͈͓��Ƀv���C���[������ꍇ
	if (detector->IsContains())
	{
		// �^�C�}�[���J�n
		auto timer = dynamic_cast<Component_Timer*>(GetChildComponent("Timer"));
		if (timer == nullptr)return;

		timer->Start();
		if (timer->IsIntervalTime(3.f))
		{
			// �V���[�g�A�^�b�N���J�n
			auto shoot = dynamic_cast<Component_ShootAttack*>(GetChildComponent("ShootAttack"));
			if (shoot == nullptr)return;
			shoot->Execute();
		}
	}
	else
	{
		// �^�C�}�[���~
		auto timer = dynamic_cast<Component_Timer*>(GetChildComponent("Timer"));
		if (timer == nullptr)return;
		timer->Stop();
	}
}

void Component_CactanBehavior::Release()
{
}

void Component_CactanBehavior::Save(json& _saveObj)
{
}

void Component_CactanBehavior::Load(json& _loadObj)
{
}

void Component_CactanBehavior::DrawData()
{
}
