#include "Component_MeleeAttack.h"
#include "../../StageObject.h"
#include "../../../../../Engine/Collider/BoxCollider.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../TimerComponent/Component_Timer.h"
#include "../GaugeComponents/Component_HealthGauge.h"
#include "../../../../Engine/GameObject/Camera.h"
#include "../../../../../Engine/DirectX/Direct3D.h"

namespace
{
	const float MOVE_SPEED = 0.1f;
	const float MOVE_DISTANCE = 0.5f;


}

Component_MeleeAttack::Component_MeleeAttack(string _name, StageObject* _holder, Component* _parent)
	:Component_Attack(_holder, _name, MeleeAttack, _parent), 
	isHit_(false),
	isFirstMove_(true),
	startPosition_(XMFLOAT3(0, 0, 0))
{
}

void Component_MeleeAttack::Initialize()
{
	if (!FindChildComponent("Timer")) {
		AddChildComponent(CreateComponent("Timer", Timer, holder_, this));
	}

	holder_->AddCollider(new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1)));
	power_ = 20;
}

void Component_MeleeAttack::Update()
{

	if (!isActive_) return;

	// ボタンが押された瞬間だけ、初期位置を取得
	if (isFirstMove_ == true) {
		startPosition_ = holder_->GetPosition();
		isFirstMove_ = false;
	}

	forward_ = Camera::GetSightLine();

	XMVECTOR moveVec = (XMLoadFloat3(&startPosition_) * XMVector3Normalize(forward_)) * MOVE_SPEED;

	XMFLOAT3 holderPosition = startPosition_;
	XMStoreFloat3(&holderPosition, moveVec);

	holder_->SetPosition(holderPosition);

	if (XMVectorGetX(XMVector3Length(XMLoadFloat3(&holderPosition) - XMLoadFloat3(&startPosition_)))) {
		Stop();
	}
}

void Component_MeleeAttack::Release()
{
}

void Component_MeleeAttack::DrawData()
{
	ImGui::Text(isHit_ ? "true" : "false");

	ImGui::DragInt("power_", &power_);
	if (ImGui::Button("Execute")) Execute();
}

