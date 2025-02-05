#include "../../../../../Engine/ImGui/imgui.h"
#include "../../Stage.h"
#include "../../StageObject.h"
#include "../../../../../Game/Objects/Stage/Components/RotationComponents/Component_RotationY.h"
#include "../../../../../Engine/DirectX/Direct3D.h"
#include "Component_TackleMove.h"
#include "../TimerComponent/Component_Timer.h"

namespace {
	const float LIMIT_DISTANCE = 0.5f; // この距離以下になったら追従をやめる
	const float SPEED = 0.1f; // 移動速度
}

Component_TackleMove::Component_TackleMove(string _name, StageObject* _holder, Component* _parent)
	:Component(_holder, _name, TackleMove, _parent), direction_(XMVectorSet(0, 0, 0, 0)), distance_(0), easingType_("Linear"), rate_(0), isFirstMove_(true), startPosition_(XMFLOAT3(0, 0, 0))
{
}

void Component_TackleMove::Initialize()
{
	volatile auto i = 0;
}

void Component_TackleMove::Update()
{
	// アクティブ状態でないなら処理を行わない
	if (isActive_ == false)return;

	// 動作開始位置を取得
	if (isFirstMove_ == true)startPosition_ = holder_->GetPosition();
	isFirstMove_ = false;

	// 保有者の位置を取得
	XMFLOAT3 holderPosition = startPosition_;

	// 移動ベクトルを作成
	XMVECTOR move = XMLoadFloat3(&startPosition_) + XMVector3Normalize(direction_) * ((distance_) * Direct3D::EaseFunc[easingType_](rate_) );

	// 保有者の位置を移動
	XMStoreFloat3(&holderPosition, move);

	// 保有者の位置を設定
	holder_->SetPosition(holderPosition);

	// 割合を進める
	rate_ += speed_;

	if (rate_ >= 1) {
		// 移動終了
		isActive_ = false;
		isFirstMove_ = true;
		rate_ = 0;
	}
}

void Component_TackleMove::Release()
{
}

void Component_TackleMove::Save(json& _saveObj)
{
	_saveObj["direction_"] = direction_.m128_f32[0];

	_saveObj["distance_"] = distance_;

	_saveObj["speed_"] = speed_;

	_saveObj["easingType_"] = easingType_;
}

void Component_TackleMove::Load(json& _loadObj)
{
	if (_loadObj.contains("direction_"))direction_.m128_f32[0] = _loadObj["direction_"];

	if (_loadObj.contains("distance_"))distance_ = _loadObj["distance_"];

	if (_loadObj.contains("speed_"))speed_ = _loadObj["speed_"];

	if (_loadObj.contains("easingType_"))easingType_ = _loadObj["easingType_"];
}

void Component_TackleMove::DrawData()
{
	// 実行する
	if (ImGui::Button("Execute"))Execute();

	// 移動方向を設定
	ImGui::DragFloat3("direction_", &direction_.m128_f32[0]);

	// 移動距離を設定
	ImGui::DragFloat("distance_", &distance_);

	// 移動速度を設定
	ImGui::DragFloat("speed_", &speed_,0.001,0,1);

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