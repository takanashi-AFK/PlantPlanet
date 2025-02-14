#include"../Objects/Stage/Components/BehaviorComponents/Component_PlayerBehavior.h"
#include "../Objects/Stage/Components/GaugeComponents/Component_HealthGauge.h"
#include "../Objects/Stage/Components/AttackComponents/Component_ShootAttack.h"
#include "../Objects/Stage/Components/AttackComponents/Component_MeleeAttack.h"
#include "../Objects/Stage/Components/MoveComponents/Component_WASDInputMove.h"
#include "../Objects/Stage/Components/GaugeComponents/Component_StaminaGauge.h"
#include "../Objects/Stage/Components/MoveComponents/Component_TackleMove.h"
#include "Plant.h"
#include"../../Engine/DirectX/Direct3D.h"

PlantData::FuncValue PlantData::Effect(Component_PlayerBehavior* pb)
{
    return GetFunction(this->id_)(pb);
}

std::function<PlantData::FuncValue(Component_PlayerBehavior*)> PlantData::GetFunction(int id)
{
	auto SecToFrame = [](float sec)->int
		{
			return sec * FPS;
		};
	static FuncValue ret{};

	switch (id)
	{

		//採取時間減少(10%)
	case 0: return [&,time = SecToFrame(20.f),id ,isFirst = true](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = time / FPS;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/PickUp.png";
			ret.amount = 10;

			if (isFirst)
			{
				auto effect = Component_PlayerBehavior::defaultTime_CollectPlant * .1f;
				auto waitTime = pb->GetTimeCollectPlant()  - effect;

				pb->SetTimeCollectPlant(waitTime);
				
				--time;

				ret.isUsable = true;

				isFirst = false;
			}

			else if (time > 0) 
			{
				ret.isUsable = true;
				--time;
			}

			else
			{
				auto effect = Component_PlayerBehavior::defaultTime_CollectPlant * 0.1;
				auto waitTime = pb->GetTimeCollectPlant() + effect;

				pb->SetTimeCollectPlant(waitTime);
				ret.isUsable = false;
			}

			return ret;
		};

		  //回復(5%)
	case 1: return[&,id](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
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

		  //近接攻撃力増加(8%)
	case 2: return[&,time = SecToFrame(30),id , isFirst = true](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = time / FPS;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/MeleeUp.png";
			ret.amount = 8;

			auto* mel = static_cast<Component_MeleeAttack*>(pb->GetChildComponent("MeleeAttack"));
			if (!mel)	return ret;

			if (isFirst)
			{
				auto effect = Component_PlayerBehavior::defaultPow_Melee * .08f;
				auto force = mel->GetPower() + effect;
				mel->SetPower(force);
				--time;

				ret.isUsable = true;
				isFirst = false;
			}

			else if (time > 0)
			{
				--time;
				ret.isUsable = true;
			}

			else
			{
				auto effect = Component_PlayerBehavior::defaultPow_Melee * .08f;
				auto force = mel->GetPower() - effect;
				mel->SetPower(force);
				ret.isUsable = false;
			}
			return ret;
		};

		  //回復(10%)
	case 3: return[&,id](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = -1;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/LifeUp.png";
			ret.amount = 10;

			auto* hp = static_cast<Component_HealthGauge*>(pb->GetChildComponent("PlayerHealthGauge"));
			if (!hp) return ret;

			auto healedHP = (hp->GetMax() * 0.1f) + (hp->GetNow());
			hp->SetNow(healedHP);

			return ret;
		};

		  //遠距離攻撃力増加(8%)
	case 4: return[&,time = SecToFrame(30),id ,isFirst = true](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = time / FPS;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/RangeUp.png";
			ret.amount = 8;
			auto* rng = static_cast<Component_ShootAttack*>(pb->GetChildComponent("ShootAttack"));

			if (isFirst)
			{
				if (!rng)	return ret ;

				auto effect = Component_PlayerBehavior::defaultPow_Range * .08f;
				auto force = rng->GetPower() + effect;
				rng->SetPower(force);
				--time;
				ret.isUsable = true;
				isFirst = false;
			}

			else if (time > 0)
			{
				--time;
				ret.isUsable = true;
			}

			else
			{
				auto effect = Component_PlayerBehavior::defaultPow_Range * .08f;
				auto force = rng->GetPower() - effect;
				rng->SetPower(force);
				ret.isUsable = false;
			}

			return ret;
		};

		  //体力低下(20%)
	case 5: return[&,id](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
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

		  //最大体力増加(25%)
	case 6: return[&,id](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = -1;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/MaxLifeUp.png";
			ret.amount = 25;

			auto* hp = static_cast<Component_HealthGauge*>(pb->GetChildComponent("PlayerHealthGauge"));
			if (!hp)	return ret;

			auto effect = Component_PlayerBehavior::defaultMax_HP * 0.25;
			auto fixedHP = (hp->GetMax() + effect);
			hp->SetMax(fixedHP);
			hp->SetNow(fixedHP);

			return ret;
		};

		  //歩行速度増加(50%)
	case 7: return[&,time = SecToFrame(10),isFirst = true,id](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = time / FPS;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/SpeedUp.png";
			ret.amount = 50;

			auto* move = static_cast<Component_WASDInputMove*>(pb->GetChildComponent("InputMove"));
			if (!move)	return ret;

			if(isFirst)
			{
				auto effect = Component_PlayerBehavior::defaultSpeed_Walk * 0.5;
				auto speed = move->GetSpeed() + effect;
				move->SetSpeed(speed);
				--time;
				ret.isUsable = true;
				isFirst = false;
			}

			else if (time > 0)
			{
				--time;
				ret.isUsable = true;
			}

			else
			{
				auto effect = Component_PlayerBehavior::defaultSpeed_Walk * 0.5;
				auto speed = move->GetSpeed() - effect;
				move->SetSpeed(speed);
				ret.isUsable = false;
			}
			return ret;
		};

		  //回復(100%)
	case 8: return[&,id](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
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

		  //近接攻撃力増加(100%)
	case 9: return[&,time = SecToFrame(20),id,isFirst = true](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = time / FPS;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/MeleeUp.png";
			ret.amount = 100;

			auto* mel = static_cast<Component_MeleeAttack*>(pb->GetChildComponent("MeleeAttack"));
			if (!mel)	return ret;

			if (isFirst)
			{
				auto effect = Component_PlayerBehavior::defaultPow_Melee * 1.0f;
				auto force = mel->GetPower() + effect;
				mel->SetPower(force);
				--time;

				ret.isUsable = true;
				isFirst = false;
			}

			else if (time > 0)
			{
				--time;
				ret.isUsable = true;
			}

			else
			{
				auto effect = Component_PlayerBehavior::defaultPow_Melee * 1.0f;
				auto force = mel->GetPower() - effect;
				mel->SetPower(force);
				ret.isUsable = false;
			}
			return ret;

		};

		  //スタミナ回復量増加(100%)
	case 10: return[&, time = SecToFrame(30), id, isFirst = true](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = time / FPS;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/StaminaUp.png";
			ret.amount = 100;

			auto* st = static_cast<Component_StaminaGauge*>(pb->GetChildComponent("StaminaGauge"));
			if (!st)	return ret;

			if (isFirst)
			{
				auto effect = Component_StaminaGauge::STAMINA_RECOVERY_DEFAULT * 1.0f;
				auto force = st->GetRecoverValue() + effect;
				st->SetRecoverValue(force);
				--time;

				ret.isUsable = true;
				isFirst = false;
			}

			else if (time > 0)
			{
				--time;
				ret.isUsable = true;
			}

			else
			{
				auto effect = Component_StaminaGauge::STAMINA_RECOVERY_DEFAULT * 1.0f;
				auto force = st->GetRecoverValue() - effect;
				st->SetRecoverValue(force);
				ret.isUsable = false;
			}
			return ret;

		};

		   //スタミナ回復量増加(30%)
	case 11: return[&, time = SecToFrame(30), id, isFirst = true](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = time / FPS;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/StaminaUp.png";
			ret.amount = 30;

			auto* st = static_cast<Component_StaminaGauge*>(pb->GetChildComponent("StaminaGauge"));
			if (!st)	return ret;

			if (isFirst)
			{
				auto effect = Component_StaminaGauge::STAMINA_RECOVERY_DEFAULT * .3f;
				auto force = st->GetRecoverValue() + effect;
				st->SetRecoverValue(force);
				--time;

				ret.isUsable = true;
				isFirst = false;
			}

			else if (time > 0)
			{
				--time;
				ret.isUsable = true;
			}

			else
			{
				auto effect = Component_StaminaGauge::STAMINA_RECOVERY_DEFAULT * .3f;
				auto force = st->GetRecoverValue() - effect;
				st->SetRecoverValue(force);
				ret.isUsable = false;
			}
			return ret;

		};

		   //スライディング距離増加(30%)
	case 12: return[&, time = SecToFrame(30), id, isFirst = true](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = time / FPS;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/SlidingUp.png";
			ret.amount = 80;

			if (isFirst)
			{
				auto effect = Component_PlayerBehavior::defaultDodge_Distance * .3f;
				auto force = pb->GetDodgeDistance() + effect;
				pb->SetDodgeDistance(force);
				--time;

				ret.isUsable = true;
				isFirst = false;
			}

			else if (time > 0)
			{
				--time;
				ret.isUsable = true;
			}

			else
			{
				auto effect = Component_PlayerBehavior::defaultDodge_Distance * .3f;
				auto force = pb->GetDodgeDistance() - effect;
				pb->SetDodgeDistance(force);
				ret.isUsable = false;
			}
			return ret;


		};

		   //超強化(30秒)
	case 13: return[&, time = SecToFrame(30), id, isFirst = true](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = true;
			ret.time = time / FPS;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/Special.png";
			ret.amount = 100;

			if (isFirst)
			{
				auto* hp = static_cast<Component_HealthGauge*>(pb->GetChildComponent("PlayerHealthGauge"));
				hp->SetNow(hp->GetMax());
				isFirst = false;
			}

			pb->GetHolder()->SetShader(Direct3D::SHADER_RAREFLOWER);
			pb->SetDodgeDistance(10.f);
			static_cast<Component_StaminaGauge*>(pb->GetChildComponent("StaminaGauge"))->SetRecoverValue(10.f);
			static_cast<Component_WASDInputMove*>(pb->GetChildComponent("InputMove"))->SetSpeed(.16f);
			static_cast<Component_MeleeAttack*>(pb->GetChildComponent("MeleeAttack"))->SetPower(140.f);
			static_cast<Component_ShootAttack*>(pb->GetChildComponent("ShootAttack"))->SetPower(80.f);

			if (time > SecToFrame(10))
			{
				pb->GetHolder()->SetShader(Direct3D::SHADER_RAREFLOWER);
			}

			else if (time > SecToFrame(3))
			{
				pb->GetHolder()->SetShader(Direct3D::SHADER_BOSS);
			}

			else
			{
				pb->GetHolder()->SetShader(Direct3D::SHADER_3D);
				pb->SetDodgeDistance(Component_PlayerBehavior::defaultDodge_Distance);
				static_cast<Component_StaminaGauge*>(pb->GetChildComponent("StaminaGauge"))
					->SetRecoverValue(Component_PlayerBehavior::defaultStamina_Recovery);
				static_cast<Component_WASDInputMove*>(pb->GetChildComponent("InputMove"))
					->SetSpeed(Component_PlayerBehavior::defaultSpeed_Walk);
				static_cast<Component_MeleeAttack*>(pb->GetChildComponent("MeleeAttack"))
					->SetPower(Component_PlayerBehavior::defaultPow_Melee);
				static_cast<Component_ShootAttack*>(pb->GetChildComponent("ShootAttack"))
					->SetPower(Component_PlayerBehavior::defaultPow_Range);
				ret.isUsable = false;
			}

			return ret;
		};

		   //スライディング距離増加(30%)
	case 14: [[fallthrough]];
	case 15: return[&, time = SecToFrame(15), id, isFirst = true](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = time / FPS;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/SlidingUp.png";
			ret.amount = 30;

			if (isFirst)
			{
				auto effect = Component_PlayerBehavior::defaultDodge_Distance * .3f;
				auto force = pb->GetDodgeDistance() + effect;
				pb->SetDodgeDistance(force);
				--time;

				ret.isUsable = true;
				isFirst = false;
			}

			else if (time > 0)
			{
				--time;
				ret.isUsable = true;
			}

			else
			{
				auto effect = Component_PlayerBehavior::defaultDodge_Distance * .3f;
				auto force = pb->GetDodgeDistance() - effect;
				pb->SetDodgeDistance(force);
				ret.isUsable = false;
			}
			return ret;

		};

		   //歩行速度増加(10%)
	case 16: return[&, time = SecToFrame(30), id, isFirst = true](Component_PlayerBehavior* pb) mutable ->PlantData::FuncValue
		{
			ret.isUsable = false;
			ret.time = time / FPS;
			ret.id = id;
			ret.filePath = "Images/SaladEffectLogo/SpeedUp.png";
			ret.amount = 10;

			auto* move = static_cast<Component_WASDInputMove*>(pb->GetChildComponent("InputMove"));
			if (!move)	return ret;

			if (isFirst)
			{
				auto effect = Component_PlayerBehavior::defaultSpeed_Walk * 0.1f;
				auto speed = move->GetSpeed() + effect;
				move->SetSpeed(speed);
				--time;
				ret.isUsable = true;
				isFirst = false;
			}

			else if (time > 0)
			{
				--time;
				ret.isUsable = false;
			}

			else
			{
				auto effect = Component_PlayerBehavior::defaultSpeed_Walk * 0.1f;
				auto speed = move->GetSpeed() - effect;
				move->SetSpeed(speed);
				ret.isUsable = false;
			}
			return ret;
		};

	default:return[&,id](Component_PlayerBehavior* pb)->PlantData::FuncValue 
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
