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

#include"../../../Engine/ResourceManager/Model.h"
#include "../../../../Plants/PlantCollection.h"
#include "../PlantComponents/Component_Plant.h"

#include<limits>

Component_RangeEnemyBehavior::Component_RangeEnemyBehavior(string _name, StageObject* _holder, Component* _parent)
	:Component(_holder, _name, WeakRangeEnemy, _parent), shotAmount_{}, shotInterval_{}, isFire_{ false },isFlowerSpawned_{false}
	, moveAmount_(.05f), burstInterval_(10), rapidAmount_(5), stalkbleLength_(7.0f), wishDistance_(5.0f), fireInterval_(3.f), detectorRange_(10.f)
{
	currentProcess_ = [this]() {SleepProcess(); };
	isActive_ = true;

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
	_saveObj["TargetName"] = targetName_;
	_saveObj["DropFlowerName"] = dropFlowerName_;

	_saveObj["MoveAmount"] = moveAmount_;
	_saveObj["BurstInterval"] = burstInterval_;
	_saveObj["RapidAmount"] = rapidAmount_;
	_saveObj["StalkableLength"] = stalkbleLength_;
	_saveObj["WishDistance"] = wishDistance_;
	_saveObj["FireInterval"] = fireInterval_;
	_saveObj["detectorRange_"] = detectorRange_;


}

void Component_RangeEnemyBehavior::Load(json& _loadObj)
{
	if (_loadObj.contains("TargetName"))	targetName_ = _loadObj["TargetName"].get<string>();
	target_ = ((StageObject*)holder_->FindObject(targetName_));
	if (_loadObj.contains("DropFlowerName"))	dropFlowerName_ = _loadObj["DropFlowerName"].get<string>();
	if (_loadObj.contains("FireInterval"))		fireInterval_ = _loadObj["FireInterval"].get<float>();

	if (_loadObj.contains("MoveAmount"))		const_cast<float&>(moveAmount_)		 = _loadObj["MoveAmount"].get<float>();
	if (_loadObj.contains("BurstInterval"))		const_cast<uint16_t&>(burstInterval_)= _loadObj["BurstInterval"].get<uint16_t>();
	if (_loadObj.contains("RapidAmount"))		const_cast<uint8_t&>(rapidAmount_)	 = _loadObj["RapidAmount"].get<uint8_t>();
	if (_loadObj.contains("StalkableLength"))	const_cast<float&>(stalkbleLength_)	 = _loadObj["StalkableLength"].get<float>();
	if (_loadObj.contains("WishDistance"))		const_cast<float&>(wishDistance_)	 = _loadObj["WishDistance"].get<float>();
	if (_loadObj.contains("detectorRange_"))	const_cast<float&>(detectorRange_) = _loadObj["detectorRange_"].get<float>();
}

void Component_RangeEnemyBehavior::DrawData()
{

	// アクティブフラグを表示
	ImGui::Checkbox("isActive_", &isActive_);
	// 対象を設定
	{
		// ステージ上に存在するオブジェクトの名前を全て取得

		static int selectTarget = 0;
		vector<string> nameList{}; nameList.push_back("None");
		for (auto obj : ((Stage*)holder_->FindObject("Stage"))->GetStageObjects()) {
			
			nameList.push_back(obj->GetObjectName());
			if (obj->GetObjectName() == targetName_) selectTarget = nameList.size() - 1;
		}

		// コンボボックスで選択
		
		if (ImGui::BeginCombo("target_", nameList[selectTarget].c_str())) {
			for (int i = 0; i < nameList.size(); i++) {
				bool is_selected = (selectTarget == i);
				if (ImGui::Selectable(nameList[i].c_str(), is_selected)) selectTarget = i;
				if (is_selected) ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		// 選択された名前から対象を設定
		if (selectTarget != 0)
		{
			target_ = (StageObject*)holder_->FindObject(nameList[selectTarget]);
			targetName_ = target_->GetObjectName();
		}
	}

	static int selectFlower = 0;
	unordered_map<int, PlantData> plant = PlantCollection::GetPlants();
	if (plant.size() == 0)ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "No plant data!"); \
	else {
		// コンボボックス
		if (ImGui::BeginCombo("FlowerSetData", plant[selectFlower].name_.c_str())) {
			for (const auto& [key, data] : plant) { // `unordered_map` をループ
				bool is_selected = (selectFlower == key); // 現在選択中か確認
				if (ImGui::Selectable(data.name_.c_str(), is_selected)) {
					selectFlower = key; // キーを更新
					dropFlowerName_ = data.name_;
				}
				if (is_selected) {
					ImGui::SetItemDefaultFocus(); // デフォルトフォーカス
				}
			}
			ImGui::EndCombo();
		}
	}

	ImGui::DragFloat("MoveAmount     :", &const_cast<float&>(moveAmount_), 0.01, .0f);
	//お行儀良くないけどReleaseでは
	//constで挙動してほしいのでcosnt cast

	int tempBurstInterval = burstInterval_;
	int tempRapidAmount = rapidAmount_;

	ImGui::DragInt  ("BurstInterval  :", &const_cast<int&>(tempBurstInterval), 1, .0f,(std::numeric_limits<decltype(burstInterval_)>::max)());
	ImGui::DragInt  ("RapidAmount    :", &const_cast<int&>(tempRapidAmount), 1, .0f, (std::numeric_limits<decltype(rapidAmount_)>::max)());
	ImGui::DragFloat("StalkableLength:", &const_cast<float&>(stalkbleLength_), 0.01, .0f);
	ImGui::DragFloat("WishDistance   :", &const_cast<float&>(wishDistance_), 0.01, .0f);
	ImGui::DragFloat("FireInterval   :", &(fireInterval_), 0.01, .0f);
	ImGui::DragFloat("DetectorRange  :", &const_cast<float&>(detectorRange_), 0.01, .0f);

	const_cast<uint16_t&>(burstInterval_) = tempBurstInterval;
	const_cast<uint8_t&>(rapidAmount_) = tempRapidAmount;

}

void Component_RangeEnemyBehavior::OnCollision(GameObject* _target, Collider* _collider)
{
}

bool Component_RangeEnemyBehavior::isSearchTargetSuccess()
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
	detector->SetRadius(detectorRange_);
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
	dir.y = 0;
	float length = sqrt(std::powf(dir.x, 2) + std::powf(dir.y, 2) + std::powf(dir.z, 2));
	dir = dir * (1 / length);
	
	//raycast-----------
	Stage* pStage = (Stage*)(holder_->FindObject("Stage"));
	if (pStage == nullptr) return;
	std::vector<StageObject*> stageObj = pStage->GetStageObjects();

	for (auto obj : stageObj) {
		// 自分自身のオブジェクトだったらスキップ
		if (obj->GetObjectName() == holder_->GetObjectName() || !obj->GetIsColliding())
			continue;

		// モデルハンドルを取得
		int hGroundModel = obj->GetModelHandle();
		if (hGroundModel < 0) continue;

		RayCastData RayData; {
			RayData.start = holder_->GetPosition(); // レイの発射位置
			RayData.start.y += .5f;
			RayData.dir = dir;
			Model::RayCast(hGroundModel, &RayData); // レイを発射
		}

		// レイが何かに当たったら且つ、その距離が突進距離以下だったら突進距離の再設定
		if (RayData.hit && RayData.dist <= moveAmount_) {

			return;
		}
		
	}
	//--------
	
	XMFLOAT3 vec = dir * moveAmount_;
	XMFLOAT3 pos = vec + holder_->GetPosition();
	pos.y = 0;
	holder_->SetPosition(pos);
}

void Component_RangeEnemyBehavior::Attack()
{
	++shotInterval_;

	if (shotInterval_ >= burstInterval_)
	{
		Component_ShootAttack* shoot = (Component_ShootAttack*)(GetChildComponent("ShootAttack"));
		if (!shoot)return;

		XMVECTOR dir = { 0,0,1,0 };

		shoot->SetPower(1);
		shoot->SetShootingSpeed(.1f);

		shoot->SetShootingDirection(XMVector3TransformCoord(dir, XMMatrixRotationY(XMConvertToRadians(holder_->GetRotate().y))));
		shoot->SetShootingPosition(holder_->GetPosition());
		shoot->SetBulletColliderRadius(0.5f);
		shoot->SetBulletLifeTime(1.5);
		//shoot->SetEffectData();
		shoot->Execute();

		++shotAmount_;
		isFire_ = shotAmount_ >= rapidAmount_ ? false : true;
		shotInterval_ = 0;

		if (!isFire_) shotAmount_ = 0;
	}
}

void Component_RangeEnemyBehavior::FaceToTarget()
{
	XMFLOAT3 angle = target_->GetPosition() - holder_->GetPosition();
	holder_->SetRotateY(XMConvertToDegrees(atan2f(angle.x, angle.z)));
}

//----------------------------------------------------
void Component_RangeEnemyBehavior::SleepProcess()
{
	if (!isSearchTargetSuccess())return;

	if (isDetectTarget()) currentProcess_ = [this]() {CombatProcess(); };
}

void Component_RangeEnemyBehavior::CombatProcess()
{
	FaceToTarget();

	float toTargetLength = GetLengthTo(target_->GetPosition());
	Component_Timer* timer = dynamic_cast<Component_Timer*>(GetChildComponent("Timer"));
	if (!timer)	return;
	timer->Start();

	if ( toTargetLength >= stalkbleLength_)
	{
		currentProcess_ = [this]() {SleepProcess(); };
		timer->Stop();
		timer->Reset();

		return;
	}

	WalkTo
	(
		toTargetLength < wishDistance_?
		(target_->GetPosition() - holder_->GetPosition()) *-1:
		target_->GetPosition() - holder_->GetPosition()//inverse direction
	);

	isFire_ = timer->IsIntervalTime(fireInterval_) || isFire_;
	if(isFire_)		Attack();
}

void Component_RangeEnemyBehavior::DeadProcess()
{
	Stage* pStage = (Stage*)(holder_->FindObject("Stage"));
	if (pStage == nullptr)return;
	pStage->DeleteStageObject(holder_);

	if (isFlowerSpawned_ == true)return;
	if (dropFlowerName_ == "")return;

	std::unordered_map<int, PlantData> plantData = PlantCollection::GetPlants();
	if (plantData.size() <= 0)return;

	for (auto& plant : plantData) {
		if (plant.second.name_ == dropFlowerName_) {
			// その場に花を生成
			

			StageObject* plantObject = CreateStageObject("EnemyDropPlant1", plant.second.modelFilePath_, pStage);

			// 当たり判定を設定
			plantObject->SetIsColliding(false);

			// プラントコンポーネントを作成
			Component_Plant* plantComponent = (Component_Plant*)CreateComponent("Plant", Plant, plantObject, this);

			// 植物情報を設定
			plantComponent->SetData(plant.second);

			// コンポーネントを追加
			plantObject->AddComponent(plantComponent);

			// サイズを設定
			plantObject->SetScale({ 0.3f,0.3f,0.3f });
			plantObject->SetPosition(holder_->GetPosition());

			// 属性を設定
			plantObject->SetObjectType(StageObject::TYPE_PLANT);

			// ステージに追加
			((Stage*)holder_->GetParent())->AddStageObject(plantObject);

			isFlowerSpawned_ = true;
			return;
		}
	}
}
