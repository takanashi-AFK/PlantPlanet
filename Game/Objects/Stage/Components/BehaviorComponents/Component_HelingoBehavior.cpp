#include "Component_HelingoBehavior.h"

#include "../../../../../Engine/Collider/BoxCollider.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../Stage.h"
#include "../../StageObject.h"
#include "../DetectorComponents/Component_CircleRangeDetector.h"
#include "../GaugeComponents/Component_HealthGauge.h"
#include "../MoveComponents/Component_Chase.h"
#include "../MoveComponents/Component_Fall.h"
#include "../MoveComponents/Component_HelingoFall.h"
#include "../RotationComponents/Component_RotationY.h"
#include "../TimerComponent/Component_Timer.h"

Component_HelingoBehavior::Component_HelingoBehavior(string _name, StageObject* _holder, Component* _parent)
	: Component(_holder, _name, HelingoBehavior,_parent)
{
}

void Component_HelingoBehavior::Initialize()
{
	// コライダーの追加
	holder_->AddCollider(new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1)));



	// 子コンポーネントの追加
	if (FindChildComponent("CircleRangeDetector") == false)AddChildComponent(CreateComponent("CircleRangeDetector", CircleRangeDetector, holder_, this));
	if (FindChildComponent("Timer") == false)AddChildComponent(CreateComponent("Timer", Timer, holder_, this));
	if (FindChildComponent("Chase") == false)AddChildComponent(CreateComponent("Chase", Chase, holder_, this));
	if (FindChildComponent("HelingoFall") == false)AddChildComponent(CreateComponent("HelingoFall", HelingoFall, holder_, this));
	if (FindChildComponent("HealthGauge") == false)AddChildComponent(CreateComponent("HelingoHealthGauge", HealthGauge, holder_, this));
}

void Component_HelingoBehavior::Update()
{
	if (target_ == nullptr) target_ = (StageObject*)holder_->FindObject(targetName_);
	if (target_ == nullptr) return;

	Component_CircleRangeDetector* detector = (Component_CircleRangeDetector*)(GetChildComponent("CircleRangeDetector"));
	if (detector == nullptr) return;

	Component_HelingoFall* fall = (Component_HelingoFall*)(GetChildComponent("HelingoFall"));
	if (fall == nullptr)return;

	// 検知範囲の設定
	detector->SetRadius(discoveryrange_);

	if (!fall->IsActived()&&detector->IsContains()) {
		Component_Chase* chase = (Component_Chase*)(GetChildComponent("Chase"));
		if (chase == nullptr) return;

		// 追跡ターゲットの設定、追跡開始
		chase->SetTarget(target_);
		chase->Execute();

		// 落下範囲の設定
		detector->SetRadius(fallrange_);

		// 落下範囲に入った場合
		if (detector->IsContains()) {
			// 追跡を停止
			chase->Stop();
			fall->Execute();
			oneHit_ = false;
		}
		else {
		}
	}
	else {
		Component_Chase* chase = (Component_Chase*)(GetChildComponent("Chase"));
		if (chase == nullptr) return;
		chase->Stop();
	}
	ImGui::Text("%f", detector->GetRadius());

}

void Component_HelingoBehavior::Release()
{
}

void Component_HelingoBehavior::OnCollision(GameObject* _target, Collider* _collider)
{
	// プレイヤーと衝突した場合
	if (_target->GetObjectName() == "Char_Player") {

		// プレイヤーのHPマネージャーコンポーネントを取得
        Component* hm = ((StageObject*)_target)->FindComponent("PlayerHealthGauge");
		if (hm == nullptr)return;

		// プレイヤーのHPを減らす
		Component_HelingoFall* helingoFall = (Component_HelingoFall*)(GetChildComponent("HelingoFall"));
		if (helingoFall == nullptr) return;

		Component_Fall* fall = (Component_Fall*)(helingoFall->GetChildComponent("Fall"));
		if (fall == nullptr)return;

		if (fall->IsActive() && oneHit_ == false) {
			((Component_HealthGauge*)hm)->TakeDamage(5);
			oneHit_ = true;
		}
		// プレイヤーのHPが0以下の場合
		if (((Component_HealthGauge*)hm)->GetNow() <= 0) {

			// プレイヤーを消す
			((Stage*)holder_->FindObject("Stage"))->DeleteStageObject((StageObject*)_target);
		}
	}
}

void Component_HelingoBehavior::Save(json& _saveObj)
{
	if (target_ != nullptr)_saveObj["target_"] = target_->GetObjectName();
	_saveObj["fallrange_"] = fallrange_;
	_saveObj["discoveryrange_"] = discoveryrange_;
}

void Component_HelingoBehavior::Load(json& _loadObj)
{
	if (_loadObj.contains("target_"))targetName_ = _loadObj["target_"];
	if (_loadObj.contains("fallrange_"))fallrange_ = _loadObj["fallrange_"];
	if (_loadObj.contains("discoveryrange_"))discoveryrange_ = _loadObj["discoveryrange_"];
}

void Component_HelingoBehavior::DrawData()
{
#ifdef _DEBUG
	
	

	ImGui::DragFloat("fallrange_", &fallrange_);
	ImGui::DragFloat("discoveryrange_", &discoveryrange_);


	//対象の選択
	vector<string> objNames;
	objNames.push_back("null");

	for (auto obj : ((Stage*)holder_->GetParent())->GetStageObjects())objNames.push_back(obj->GetObjectName());

	static int select = 0;
	if (ImGui::BeginCombo("target_", objNames[select].c_str())) {
		for (int i = 0; i < objNames.size(); i++) {
			bool is_selected = (select == i);
			if (ImGui::Selectable(objNames[i].c_str(), is_selected))select = i;
			if (is_selected)ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	if (select == 0)target_ = nullptr;
	else {
		target_ = (StageObject*)holder_->FindObject(objNames[select]);

		auto detector = dynamic_cast<Component_CircleRangeDetector*>(GetChildComponent("CircleRangeDetector"));
		detector->SetTarget(target_);
	}

	auto helingoFall = dynamic_cast<Component_HelingoFall*>(GetChildComponent("HelingoFall"));
	if (helingoFall == nullptr) return;

	if (ImGui::Button("EXE"))
		helingoFall->Execute();

	if (ImGui::Button("Stop"))
		helingoFall->Stop();

#endif // _DEBUG
}
