#include "Plant.h"
#include"../Objects/Stage/Components/BehaviorComponents/Component_PlayerBehavior.h"
#include "../Objects/Stage/Components/GaugeComponents/Component_HealthGauge.h"
#include "../Objects/Stage/Components/AttackComponents/Component_ShootAttack.h"
#include "../Objects/Stage/Components/AttackComponents/Component_MeleeAttack.h"
#include "../Objects/Stage/Components/MoveComponents/Component_WASDInputMove.h"

bool PlantData::Effect(Component_PlayerBehavior* pb)
{
    return GetFunction(this->id_)(pb);
}

std::function<bool(Component_PlayerBehavior*)> PlantData::GetFunction(int id)
{
	auto SecToFrame = [](float sec)->int
		{
			return sec * 60;
		};
	switch (id)
	{
	case 1: return [time = SecToFrame(20.f)](Component_PlayerBehavior* pb) mutable ->bool
		{
			if (time > 0)
			{
				auto time = pb->GetTimeCollectPlant() * 0.9;

				pb->SetTimeCollectPlant(time);

				--time;
				return true;
			}

			else
			{
				pb->SetTimeCollectPlant(pb->defaultTime_CollectPlant);
				return false;
			}
		};

	case 2: return[](Component_PlayerBehavior* pb) mutable ->bool
		{
			Component_HealthGauge* hp = static_cast<Component_HealthGauge*>(pb->GetChildComponent("PlayerHealthGauge"));
			if (!hp)	return false;

			auto healedHP = (hp->GetMax() * 0.05f) + (hp->GetNow());
			hp->SetNow(healedHP);

			return false;
		};

	case 3: return[time = SecToFrame(30)](Component_PlayerBehavior* pb) mutable ->bool
		{

			auto* mel = static_cast<Component_MeleeAttack*>(pb->GetChildComponent("MeleeAttack"));

			if (time > 0)
			{
				if (!mel)	return false;

				mel->SetPower(pb->defaultPow_Melee * 1.08);
				--time;
				return true;
			}

			else
			{
				mel->SetPower(pb->defaultPow_Melee);
				return false;
			}
		};

	case 4: return[](Component_PlayerBehavior* pb) mutable ->bool
		{
			Component_HealthGauge* hp = static_cast<Component_HealthGauge*>(pb->GetChildComponent("PlayerHealthGauge"));
			if (!hp)	return false;

			auto healedHP = (hp->GetMax() * 0.1f) + (hp->GetNow());
			hp->SetNow(healedHP);

			return false;
		};

	case 5: return[time = SecToFrame(30)](Component_PlayerBehavior* pb) mutable ->bool
		{

			auto* rng = static_cast<Component_ShootAttack*>(pb->GetChildComponent("ShootAttack"));

			if (time > 0)
			{
				if (!rng)	return false;

				rng->SetPower(pb->defaultPow_Range * 1.08);
				--time;
				return true;
			}

			else
			{
				rng->SetPower(pb->defaultPow_Melee);
				return false;
			}
		};

	case 6: return[](Component_PlayerBehavior* pb) mutable ->bool
		{
			Component_HealthGauge* hp = static_cast<Component_HealthGauge*>(pb->GetChildComponent("PlayerHealthGauge"));
			if (!hp)	return false;

			auto healedHP = (hp->GetMax() * -0.2f) + (hp->GetNow());
			hp->SetNow(healedHP);

			return false;
		};

	case 7: return[](Component_PlayerBehavior* pb) mutable ->bool
		{
			Component_HealthGauge* hp = static_cast<Component_HealthGauge*>(pb->GetChildComponent("PlayerHealthGauge"));
			if (!hp)	return false;

			auto fixedHP = (hp->GetMax() * 1.25);
			hp->SetMax(fixedHP);
			hp->SetNow(fixedHP);

			return false;
		};

	case 8: return[time = SecToFrame(100)](Component_PlayerBehavior* pb) mutable ->bool
		{

			auto* move = static_cast<Component_WASDInputMove*>(pb->GetChildComponent("InputMove"));

			if (time > 0)
			{
				if (!move)	return false;

				move->SetSpeed(pb->defaultSpeed_Walk *1.5);
				--time;
				return true;
			}

			else
			{
				move->SetSpeed(pb->defaultSpeed_Walk);
				return false;
			}
		};

	case 9: return[](Component_PlayerBehavior* pb) mutable ->bool
		{
			Component_HealthGauge* hp = static_cast<Component_HealthGauge*>(pb->GetChildComponent("PlayerHealthGauge"));
			if (!hp)	return false;

			auto healedHP = (hp->GetMax());
			hp->SetMax(healedHP);
			hp->SetNow(healedHP);

			return false;
		};

	case 10: return[time = SecToFrame(20)](Component_PlayerBehavior* pb) mutable ->bool
		{

			auto* mel = static_cast<Component_MeleeAttack*>(pb->GetChildComponent("MeleeAttack"));

			if (time > 0)
			{
				if (!mel)	return false;

				mel->SetPower(pb->defaultPow_Melee * 2.f);
				--time;
				return true;
			}

			else
			{
				mel->SetPower(pb->defaultPow_Melee);
				return false;
			}
		};


	default :return[](Component_PlayerBehavior* pb)->bool{return false;};
	}
}
