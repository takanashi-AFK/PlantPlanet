#include"../Objects/Stage/Components/BehaviorComponents/Component_PlayerBehavior.h"
#include "../Objects/Stage/Components/GaugeComponents/Component_HealthGauge.h"
#include "../Objects/Stage/Components/AttackComponents/Component_ShootAttack.h"
#include "../Objects/Stage/Components/AttackComponents/Component_MeleeAttack.h"
#include "../Objects/Stage/Components/MoveComponents/Component_WASDInputMove.h"

#include "Plant.h"

PlantData::FuncValue PlantData::Effect(Component_PlayerBehavior* pb)
{
    return GetFunction(this->id_)(pb);
}

std::function<PlantData::FuncValue(Component_PlayerBehavior*)> PlantData::GetFunction(int id)
{
	constexpr float FRAME_RATE = 60.f;
	auto SecToFrame = [](float sec)->int
		{
			return sec * FRAME_RATE;
		};
	FuncValue ret{};

	switch (id)
	{
	case 1: return [&,time = SecToFrame(20.f)](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = time / FRAME_RATE;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/PickUp.png";
			ret.amount = 10;

			if (time >= SecToFrame(20.f))
			{
				auto waitTime = pb->GetTimeCollectPlant() * 0.9;

				pb->SetTimeCollectPlant(waitTime);
				
				--time;

				ret.isUsable = true;
			}

			else if (time > 0) 
			{
				ret.isUsable = true;
			}

			else
			{
				pb->SetTimeCollectPlant(pb->defaultTime_CollectPlant);
				ret.isUsable = false;
			}

			return ret;
		};

	case 2: return[&](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = -1;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/LifeUp.png";
			ret.amount = 5;
			auto* hp = static_cast<Component_HealthGauge*>(pb->GetChildComponent("PlayerHealthGauge"));
			if (!hp); return ret;

			auto healedHP = (hp->GetMax() * 0.05f) + (hp->GetNow());
			hp->SetNow(healedHP);

			return ret;
		};

	case 3: return[&,time = SecToFrame(30)](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = time / FRAME_RATE;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/MeleeUp.png";
			ret.amount = 8;

			auto* mel = static_cast<Component_MeleeAttack*>(pb->GetChildComponent("MeleeAttack"));

			if (time > 0)
			{
				if (!mel)	return ret;

				mel->SetPower(pb->defaultPow_Melee * 1.08);
				--time;

				ret.isUsable = true;
			}

			else
			{
				mel->SetPower(pb->defaultPow_Melee);
				ret.isUsable = false;
			}
			return ret;
		};

	case 4: return[&](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = -1;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/LifeUp.png";
			ret.amount = 10;

			auto* hp = static_cast<Component_HealthGauge*>(pb->GetChildComponent("PlayerHealthGauge"));
			if (!hp);// return false;

			auto healedHP = (hp->GetMax() * 0.1f) + (hp->GetNow());
			hp->SetNow(healedHP);

			return ret;
		};

	case 5: return[&,time = SecToFrame(30)](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = time / FRAME_RATE;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/RangeUp.png";
			ret.amount = 8;
			auto* rng = static_cast<Component_ShootAttack*>(pb->GetChildComponent("ShootAttack"));

			if (time > 0)
			{
				if (!rng)	return ret ;

				rng->SetPower(pb->defaultPow_Range * 1.08);
				--time;
				ret.isUsable = true;
			}

			else
			{
				rng->SetPower(pb->defaultPow_Melee);
				ret.isUsable = false;
			}

			return ret;
		};

	case 6: return[&](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = -1;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/LifeDown.png";
			ret.amount = 20;

			auto* hp = static_cast<Component_HealthGauge*>(pb->GetChildComponent("PlayerHealthGauge"));
			if (!hp)	ret;

			auto healedHP = (hp->GetMax() * -0.2f) + (hp->GetNow());
			hp->SetNow(healedHP);

			return ret;
		};

	case 7: return[&](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = -1;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/MaxLifeUp.png";
			ret.amount = 25;

			auto* hp = static_cast<Component_HealthGauge*>(pb->GetChildComponent("PlayerHealthGauge"));
			if (!hp)	;

			auto fixedHP = (hp->GetMax() * 1.25);
			hp->SetMax(fixedHP);
			hp->SetNow(fixedHP);

			return ret;
		};

	case 8: return[&,time = SecToFrame(100)](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = time / FRAME_RATE;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/SpeedUp.png";
			ret.amount = 50;

			auto* move = static_cast<Component_WASDInputMove*>(pb->GetChildComponent("InputMove"));

			if (time > 0)
			{
				if (!move)	return ret;

				move->SetSpeed(pb->defaultSpeed_Walk *1.5);
				--time;
				ret.isUsable = true;
			}

			else
			{
				move->SetSpeed(pb->defaultSpeed_Walk);
				ret.isUsable = false;
			}
			return ret;
		};

	case 9: return[&](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = -1;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/LifeUp.png";
			ret.amount = 100;

			auto* hp = static_cast<Component_HealthGauge*>(pb->GetChildComponent("PlayerHealthGauge"));
			if (!hp)	return ret;

			auto healedHP = (hp->GetMax());
			hp->SetMax(healedHP);
			hp->SetNow(healedHP);

			return ret;
		};

	case 10: return[&,time = SecToFrame(20)](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = time / FRAME_RATE;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/MeleeUp.png";
			ret.amount = 20;

			auto* mel = static_cast<Component_MeleeAttack*>(pb->GetChildComponent("MeleeAttack"));

			if (time > 0)
			{
				if (!mel)	return ret;

				mel->SetPower(pb->defaultPow_Melee * 2.f);
				--time;
				ret.isUsable = true;
			}

			else
			{
				mel->SetPower(pb->defaultPow_Melee);
				ret.isUsable = false;
			}

			return ret;

		};


	default:return[&](Component_PlayerBehavior* pb)->PlantData::FuncValue 
		{
			ret.isUsable = false;
			ret.time =-1;
			ret.id = id;
			ret.filePath = "";
			ret.amount = -1;
			return ret;
		};
	}
}
