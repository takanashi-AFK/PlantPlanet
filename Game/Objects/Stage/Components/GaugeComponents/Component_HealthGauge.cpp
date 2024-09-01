#include "Component_HealthGauge.h"

// �C���N���[�h
#include "../../../../../Engine/ImGui/imgui.h"


Component_HealthGauge::Component_HealthGauge(string _name, StageObject* _holder, Component* _parent)
	:Component_Gauge(_holder, _name, HealthGauge,_parent)
{

}

void Component_HealthGauge::Initialize()
{
}

void Component_HealthGauge::Update()
{
}

void Component_HealthGauge::Release()
{
}

void Component_HealthGauge::TakeDamage(float _damageValue)
{
	now_ -= _damageValue;
	//HP��0�ȉ��ɂȂ�Ȃ��悤�ɂ���
	if (now_ < 0)now_ = 0;
}

void Component_HealthGauge::Heal(float _healValue)
{
	//HP���ő�l�𒴂��Ȃ��悤�ɂ���
	if (now_ + _healValue <= max_)now_ += _healValue;
}