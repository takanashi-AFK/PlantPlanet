#include "Component_OtiBehavior.h"

#include "../../StageObject.h"
#include "../DetectorComponents/Component_CircleRangeDetector.h"
#include "../DetectorComponents/Component_FanRangeDetector.h"
#include "../RotationComponents/Component_RotationY.h"
#include "../TimerComponent/Component_Timer.h"

Component_OtiBehavior::Component_OtiBehavior(StageObject* _holder)
	: Component(_holder, "Component_OtiBehavior", OtiBehavior)
{
}

void Component_OtiBehavior::Initialize()
{
	// �q�R���|�[�l���g�̒ǉ�
	if (FindChildComponent(CircleRangeDetector) == false)AddChildComponent(CreateComponent(CircleRangeDetector, holder_));
	if (FindChildComponent(FanRangeDetector) == false)AddChildComponent(CreateComponent(FanRangeDetector, holder_));
	if (FindChildComponent(RotationY) == false)AddChildComponent(CreateComponent(RotationY, holder_));
	if (FindChildComponent(Timer) == false)AddChildComponent(CreateComponent(Timer, holder_));
}

void Component_OtiBehavior::Update()
{
	// �~�`�͈͓��ɓ����Ă��邩�ǂ����𔻒�
	if (((Component_CircleRangeDetector*)GetChildComponent(CircleRangeDetector))->IsContains()){

		// �^�C�}�[���J�n
		((Component_Timer*)GetChildComponent(Timer))->Reset();
		((Component_Timer*)GetChildComponent(Timer))->Start();

		// ��]���x��ݒ�
		((Component_RotationY*)GetChildComponent(RotationY))->SetRotationSpeed(5.f);
	}
	else{
		// �^�C�}�[���T�b���ϑ�������
		if (((Component_Timer*)GetChildComponent(Timer))->IsOnTime(5)) {
			
			// �^�C�}�[���~
			((Component_Timer*)GetChildComponent(Timer))->Stop();

			// ��]���x��ݒ�
			((Component_RotationY*)GetChildComponent(RotationY))->SetRotationSpeed(0.f);
		}		
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
