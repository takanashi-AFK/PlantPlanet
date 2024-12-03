#include "Component_MeleeAttack.h"
#include "../../StageObject.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../TimerComponent/Component_Timer.h"
#include "../GaugeComponents/Component_HealthGauge.h"
#include "../../../../Engine/GameObject/Camera.h"
#include "../../../../../Engine/DirectX/Direct3D.h"
#include "../DetectorComponents/Component_FanRangeDetector.h"

namespace
{
	const float MOVE_SPEED = 0.1f;
	const float MOVE_DISTANCE = 5.f;
}

Component_MeleeAttack::Component_MeleeAttack(string _name, StageObject* _holder, Component* _parent)
	:Component_Attack(_holder, _name, MeleeAttack, _parent), 
	isHit_(false),
	isFirstMove_(true),
	startPosition_(XMFLOAT3(0, 0, 0)),
	easingType_("Linear"), 
	rate_(0),
	speed_(0.1),
	distance_(5.f),
	sequence_(E_STEPFORWORD)
{
}

void Component_MeleeAttack::Initialize()
{
	if (!FindChildComponent("Timer")) {
		AddChildComponent(CreateComponent("Timer", Timer, holder_, this));
	}

	holder_->AddCollider(new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1)));
	power_ = 20;
	if (FindChildComponent("attackRange") == false)AddChildComponent(CreateComponent("attackRange", FanRangeDetector, holder_, this));
}

void Component_MeleeAttack::Update()
{

	if (!isActive_) return;

	switch (sequence_)
	{ 
		case E_STEPFORWORD: StepForward();  break;
		case E_SLASH:		Slash();		break;
	}
	StepForward();
}

void Component_MeleeAttack::Release()
{
}

void Component_MeleeAttack::DrawData()
{
	ImGui::Text(isHit_ ? "true" : "false");

	ImGui::DragInt("power_", &power_);
	if (ImGui::Button("Execute")) Execute();


	// 移動距離を設定
	ImGui::DragFloat("distance_", &distance_);

	// 移動速度を設定
	ImGui::DragFloat("speed_", &speed_, 0.001, 0, 1);


	// イージングの種類を設定
	if (ImGui::BeginCombo("EaseFunc", easingType_.c_str())) {
		for (const auto& pair : Direct3D::EaseFunc) {
			bool is_selected = (easingType_ == pair.first);
			if (ImGui::Selectable(pair.first.c_str(), is_selected)) easingType_ = pair.first;
			if (is_selected) ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

void Component_MeleeAttack::Save(json& _saveObj)
{
	_saveObj["distance_"] = distance_;
	_saveObj["speed_"] = speed_;
	_saveObj["easingType_"] = easingType_;
}

void Component_MeleeAttack::Load(json& _loadObj)
{
	if (_loadObj.contains("distance_")) distance_ = _loadObj["distance_"];
	if (_loadObj.contains("speed_")) speed_ = _loadObj["speed_"];
	if (_loadObj.contains("easingType_")) easingType_ = _loadObj["easingType_"];
}

void Component_MeleeAttack::StepForward()
{

	// ボタンが押された瞬間だけ、初期位置を取得
	if (isFirstMove_ == true) {
		startPosition_ = holder_->GetPosition();
		isFirstMove_ = false;
	}
	else {
		forward_ = Camera::GetSightLine();
		if (XMVector3Equal(forward_, XMVectorZero()))forward_ = XMVectorSet(0, 0, 1, 0);
		XMFLOAT3 holderPosition = startPosition_;

		XMVECTOR move = XMLoadFloat3(&holderPosition) + XMVector3Normalize(forward_) * ((distance_)*Direct3D::EaseFunc[easingType_](rate_));

		move = XMVectorSet(XMVectorGetX(move), 0, XMVectorGetZ(move), 0.f);

		XMStoreFloat3(&holderPosition, move);

		// 位置を更新
		holder_->SetPosition(holderPosition);

		rate_ += speed_;

		// MOVE_DISTANCEに到達したら停止
		if (rate_ >= 1) {
			Stop();
			isFirstMove_ = true;
			rate_ = 0;
			SetSequence(E_SLASH);
		}
	}
}

void Component_MeleeAttack::Slash()
{
	Component_FanRangeDetector * attackRange = (Component_FanRangeDetector*)(GetChildComponent("attackRange"));
	if (attackRange == nullptr)return;

	attackRange->SetAngle(180);
	attackRange->SetRange(5);
	attackRange->SetDirection(Camera::GetSightLine());

	// 範囲内にいるenemyの属性を持っているオブジェクトをターゲットにする

}

