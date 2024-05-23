#include "Component_OtiBehavior.h"

#include "../../StageObject.h"
#include "../DetectorComponents/Component_CircleRangeDetector.h"
#include "../RotationComponents/Component_RotationY.h"
#include "../TimerComponent/Component_Timer.h"
#include "../MoveComponents/Component_Fall.h"
#include "../MoveComponents/Component_Chase.h"

Component_OtiBehavior::Component_OtiBehavior(StageObject* _holder)
	: Component(_holder, "Component_OtiBehavior", OtiBehavior)
{
}

void Component_OtiBehavior::Initialize()
{
	// �q�R���|�[�l���g�̒ǉ�
	if (FindChildComponent(CircleRangeDetector) == false)AddChildComponent(CreateComponent(CircleRangeDetector, holder_));
	//if (FindChildComponent(Timer) == false)AddChildComponent(CreateComponent(Timer, holder_));
	//if (FindChildComponent(Fall) == false)AddChildComponent(CreateComponent(Fall, holder_));
	if (FindChildComponent(Chase) == false)AddChildComponent(CreateComponent(Chase, holder_));
}

void Component_OtiBehavior::Update()
{
	// �͈͓��Ƀv���C���[�����邩�ǂ���
	if (((Component_CircleRangeDetector*)GetChildComponent(CircleRangeDetector))->IsContains()) 
	{
		// �Ǐ]�s�����J�n
		((Component_Chase*)GetChildComponent(Chase))->Start();
	}
	else 
	{
		// �Ǐ]�s�����~
		((Component_Chase*)GetChildComponent(Chase))->Stop();
	}
}

void Component_OtiBehavior::Release()
{
}

void Component_OtiBehavior::Save(json& _saveObj)
{
}

void Component_OtiBehavior::Load(json& _loadObj)
{
}

void Component_OtiBehavior::DrawData()
{
}
