#include "Component_OtiBehavior.h"

#include "../../StageObject.h"
#include "../DetectorComponents/Component_CircleRangeDetector.h"

Component_OtiBehavior::Component_OtiBehavior(StageObject* _holder)
	: Component(_holder, "Component_OtiBehavior", OtiBehavior)
{
}

void Component_OtiBehavior::Initialize()
{
	// �q�R���|�[�l���g�̒ǉ�
	AddChildComponent(CreateComponent(CircleRangeDetector, holder_));
}

void Component_OtiBehavior::Update()
{
	// �͈͓��ɓ����Ă��邩�ǂ����𔻒�
	if (((Component_CircleRangeDetector*)GetChildComponent(CircleRangeDetector))->IsContains())
	{
		// �͈͓��ɓ����Ă���ꍇ�̏���
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
