#include "Component_RangeEnemyBehavior.h"

#include "../../../../../Engine/Global.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../Stage.h"
#include "../../StageObject.h" 
#include "../AttackComponents/Component_ShootAttack.h"
#include "../DetectorComponents/Component_CircleRangeDetector.h"
#include "../GaugeComponents/Component_HealthGauge.h"

#include "../TimerComponent/Component_Timer.h"

#include"../../../Engine/Global.h"

Component_RangeEnemyBehavior::Component_RangeEnemyBehavior(string _name, StageObject* _holder, Component* _parent)
	:Component(_holder, _name, CactanBehavior, _parent), shotAmount_{}, shotInterval_{}, isFire_{ false }
{
	currentProcess_ = [this]() {SleepProcess(); };
}

void Component_RangeEnemyBehavior::Initialize()
{
	holder_->AddCollider(new SphereCollider({ 0,0,0 },1.f));
	holder_->SetObjectType(StageObject::TYPE_ENEMY);

	if (!FindChildComponent("CircleRangeDetector"))AddChildComponent(CreateComponent("CircleRangeDetector", CircleRangeDetector, holder_, this));
	if (!FindChildComponent("ShootAttack"))AddChildComponent(CreateComponent("ShootAttack", ShootAttack, holder_, this));
	if (!FindChildComponent("Timer"))AddChildComponent(CreateComponent("Timer", Timer, holder_, this));
	if (!FindChildComponent("HealthGauge")) AddChildComponent(CreateComponent("HealthGauge", HealthGauge, holder_, this));
}

void Component_RangeEnemyBehavior::Update()
{
	if (isDead())	currentProcess_ = [this](){DeadProcess();};
	currentProcess_();

	
}

void Component_RangeEnemyBehavior::Release()
{
}

void Component_RangeEnemyBehavior::Save(json& _saveObj)
{
}

void Component_RangeEnemyBehavior::Load(json& _loadObj)
{
}

void Component_RangeEnemyBehavior::DrawData()
{
	// アクティブフラグを表示
	ImGui::Checkbox("isActive_", &isActive_);
	// 対象を設定
	{
		// ステージ上に存在するオブジェクトの名前を全て取得
		vector<string> nameList{}; nameList.push_back("None");
		for (auto obj : ((Stage*)holder_->FindObject("Stage"))->GetStageObjects()) nameList.push_back(obj->GetObjectName());

		// コンボボックスで選択
		static int select = 0;
		if (ImGui::BeginCombo("target_", nameList[select].c_str())) {
			for (int i = 0; i < nameList.size(); i++) {
				bool is_selected = (select == i);
				if (ImGui::Selectable(nameList[i].c_str(), is_selected)) select = i;
				if (is_selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		// 選択された名前から対象を設定
		if (select != 0) target_ = (StageObject*)holder_->FindObject(nameList[select]);
	}
}

void Component_RangeEnemyBehavior::OnCollision(GameObject* _target, Collider* _collider)
{
}

bool Component_RangeEnemyBehavior::isSerchTargetSuccess()
{
	if (!target_)
	{
		target_ = (StageObject*)holder_->FindObject(targetName_);
		if (!target_)return false;
	}

	return true;
}

bool Component_RangeEnemyBehavior::isDetectTarget()
{
	Component_CircleRangeDetector* detector = (Component_CircleRangeDetector*)(GetChildComponent("CircleRangeDetector"));
	if (!detector)return false;
	detector->SetRadius(5.f);
	detector->SetTarget(target_);

	return detector->IsContains();
}
bool Component_RangeEnemyBehavior::isDead()
{
	Component_HealthGauge* hp = dynamic_cast<Component_HealthGauge*>(GetChildComponent("HealthGauge"));
	if (!hp)return false;

	if (hp->GetNow() <= 0)return true;

	return false;
}

float Component_RangeEnemyBehavior::GetLengthTo(XMFLOAT3 tgt)
{
	auto position = holder_->GetPosition();

	auto getPow = [&](uint8_t index)->float
		{
			float pos{};
			float tg{};

			switch (index)
			{
			case 0:pos = position.x; tg = tgt.x; break;
			case 1:pos = position.y; tg = tgt.y; break;
			case 2:pos = position.z; tg = tgt.z; break;
			default:break;
			}

			auto len = tg - pos;

			return (len*len);
		};

	float powLength = getPow(0) + getPow(1) + getPow(2);//x y z

	return std::sqrtf(powLength);
}

void Component_RangeEnemyBehavior::WalkTo(XMFLOAT3 dir)
{
	constexpr float moveAmount = 0.05f;
	float length = sqrt(std::powf(dir.x, 2) + std::powf(dir.y, 2) + std::powf(dir.z, 2));
	dir = dir * (1 / length);
	
	//raycast--
	// 
	//--------
	
	XMFLOAT3 vec = dir * moveAmount;
	XMFLOAT3 pos = vec + holder_->GetPosition() ;
	pos.y = 0;
	holder_->SetPosition(pos);
}

void Component_RangeEnemyBehavior::Attack()
{
	++shotInterval_;

	constexpr int burstInterval = 10;

	if (shotInterval_ >= burstInterval)
	{
		Component_ShootAttack* shoot = (Component_ShootAttack*)(GetChildComponent("ShootAttack"));
		if (!shoot)return;

		XMVECTOR dir = { 0,0,1,0 };

		shoot->SetPower(1);
		shoot->SetShootingSpeed(.1f);
		shoot->SetShootingDirection(XMVector3TransformCoord(dir, XMMatrixRotationY(XMConvertToRadians(holder_->GetRotate().y))));
		shoot->SetShootingPosition(holder_->GetPosition());
		shoot->SetBulletColliderRadius(0.5f);
		shoot->SetBulletLifeTime(100);
		shoot.Set
		shoot->Execute();

		constexpr int rapidAmount = 5;

		++shotAmount_;
		isFire_ = shotAmount_ >= rapidAmount ? false : true;
		shotInterval_ = 0;

		if (!isFire_) shotAmount_ = 0;
	}
}

//----------------------------------------------------
void Component_RangeEnemyBehavior::SleepProcess()
{
	if (!isSerchTargetSuccess())return;

	if (isDetectTarget()) currentProcess_ = [this]() {CombatProcess(); };
}

void Component_RangeEnemyBehavior::CombatProcess()
{
	constexpr float stalkbleLength = 7.0f;
	constexpr float wishDistance = 5.0f;

	float toTargetLength = GetLengthTo(target_->GetPosition());
	Component_Timer* timer = dynamic_cast<Component_Timer*>(GetChildComponent("Timer"));
	if (!timer)	return;
	timer->Start();

	if ( toTargetLength >= stalkbleLength)
	{
		currentProcess_ = [this]() {SleepProcess(); };
		timer->Stop();
		timer->Reset();

		return;
	}

	WalkTo
	(
		toTargetLength < wishDistance?
		target_->GetPosition() - holder_->GetPosition() *-1:
		target_->GetPosition() - holder_->GetPosition()//inverse direction

	);

	isFire_ = timer->IsIntervalTime(3.f) || isFire_;
	if(isFire_)		Attack();
}

void Component_RangeEnemyBehavior::DeadProcess()
{
}
