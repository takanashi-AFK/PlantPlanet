#include "Component_StaminaGauge.h"
#include "../TimerComponent/Component_Timer.h"
#include "../../../Engine/ImGui/imgui.h"

namespace {
	const int STAMINA_DECREASE_SHOOT = 10;
	const int STAMINA_DECREASE_MELEE = 20;
	const int STAMINA_DECREASE_DODGE = 30;
	const float STAMINA_RECOVERY = 0.17f;
	const float STAMINA_MAX = 100.f;

}

Component_StaminaGauge::Component_StaminaGauge(string _name, StageObject* _holder, Component* _parent)
	:Component_Gauge(_holder, _name, StaminaGauge, _parent)
{
}

void Component_StaminaGauge::Initialize()
{
	timer = (Component_Timer*)GetChildComponent("Timer");

	if (FindChildComponent("Timer") == false)AddChildComponent(CreateComponent("Timer", Timer, holder_, this));
}

void Component_StaminaGauge::Update()
{
	if(timer == nullptr)return;
	if (isLock_ == true) {
		timer->SetTime(3);
		timer->Start();

		if (timer->GetIsEnd()) {
			RecoverUnlock();
			timer->Reset();
		}
	}
	else
	{
		RecoverStamina(STAMINA_RECOVERY);
	}
}

void Component_StaminaGauge::Release()
{
}

void Component_StaminaGauge::UseStamina(float _useValue)
{
	RecoverLock();
	timer->Reset();
	if (now_ >= _useValue) {
		now_ -= _useValue;
		return ;
	}
	return;
}

bool Component_StaminaGauge::CanUseStamina(float _useValue)
{
	return now_ >= _useValue; 
}

void Component_StaminaGauge::RecoverStamina(float _recoverValue)
{
	if(now_ <= STAMINA_MAX)
		now_ += _recoverValue;
}
