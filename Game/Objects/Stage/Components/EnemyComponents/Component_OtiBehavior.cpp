#include "Component_OtiBehavior.h"

#include "../../StageObject.h"
#include "../DetectorComponents/Component_CircleRangeDetector.h"
#include "../RotationComponents/Component_RotationY.h"
#include "../TimerComponent/Component_Timer.h"
#include "../MoveComponents/Component_Fall.h"
#include "../MoveComponents/Component_Chase.h"

Component_OtiBehavior::Component_OtiBehavior(string _name, StageObject* _holder)
	: Component(_holder, _name, OtiBehavior)
{
}

void Component_OtiBehavior::Initialize()
{
}

void Component_OtiBehavior::Update()
{
	(((Component_CircleRangeDetector*)GetChildComponent(CircleRangeDetector))->SetRadius(5));
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

	// �͈͓��Ƀv���C���[�����邩�ǂ���
	(((Component_CircleRangeDetector*)GetChildComponent(CircleRangeDetector))->SetRadius(1));
	if (((Component_CircleRangeDetector*)GetChildComponent(CircleRangeDetector))->IsContains())
	{
		// �����s�����J�n
		((Component_Fall*)GetChildComponent(Fall))->Start();
	}
	else
	{
		// �����s�����~
		((Component_Fall*)GetChildComponent(Fall))->Stop();
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
