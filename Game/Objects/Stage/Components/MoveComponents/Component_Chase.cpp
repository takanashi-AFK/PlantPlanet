#include "../../../../../Engine/ImGui/imgui.h"
#include "../../Stage.h"
#include "../../StageObject.h"
#include "Component_Chase.h"

Component_Chase::Component_Chase(string _name, StageObject* _holder, Component* _parent)
	:Component(_holder,_name,Chase,_parent), speed_(0.0f),target_(nullptr),limitDistance_(0.5f)
{
}

void Component_Chase::Initialize()
{
	speed_ = 0.3f;
}

void Component_Chase::Update()
{
	if (target_ == nullptr)target_ = (StageObject*)holder_->FindObject(targetName_);
	if (target_ == nullptr || isActive_ == false)return;

	//対象と保有者のポジションを取得
	XMFLOAT3 targetPos = target_->GetPosition();
	XMFLOAT3 holderPos = holder_->GetPosition();

	// 追従する方向を計算
	XMVECTOR direction = XMVectorSetY(CalcDirection(holderPos, targetPos),0);

	// ホルダーの向きを計算
	float rotateAngle = XMConvertToDegrees(CalcRotateAngle(direction));

	// 距離を計算
	float distance = CalcDistance(holderPos, targetPos);

	if (distance > GetLimitDistance()) {
		
		//向きをを適応
		holder_->SetRotateY(rotateAngle);

		//移動
		Move(direction);
	}
	else
	{
		rotateAngle = 0;
	}

}

void Component_Chase::Release()
{
}

void Component_Chase::Save(json& _saveobj)
{
	_saveobj["speed_"] = speed_;
	_saveobj["limitDistance_"] = limitDistance_;
	if(target_ != nullptr)_saveobj["target_"] = target_->GetObjectName();
}

void Component_Chase::Load(json& _loadObj)
{
	if(_loadObj.contains("speed_"))speed_ = _loadObj["speed_"];
	if(_loadObj.contains("target_"))targetName_ = _loadObj["target_"];
	if(_loadObj.contains("limitDistance_"))limitDistance_ = _loadObj["limitDistance_"];
}

void Component_Chase::DrawData()
{

	// 速度の設定
	ImGui::DragFloat("speed", &speed_, 0.01f, 0.0f, 10.0f);

	// 追跡をやめる距離の設定
	ImGui::DragFloat("limitDistance", &limitDistance_, 0.01f, 0.0f, 10.0f);

	// ターゲットの選択
	{
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
		else target_ = (StageObject*)holder_->FindObject(objNames[select]);
	}
}

XMVECTOR Component_Chase::CalcDirection(XMFLOAT3 _holderPos, XMFLOAT3 _targetPos)
{
	// 方向を計算
	return XMVector3Normalize(XMLoadFloat3(&_targetPos) - XMLoadFloat3(&_holderPos));
}

void Component_Chase::Move(XMVECTOR _direction)
{
	// 所有者の位置を取得
	XMFLOAT3 holderPosition = holder_->GetPosition();

	// 移動
	XMStoreFloat3(&holderPosition, XMLoadFloat3(&holderPosition) + (_direction * speed_));
	holder_->SetPosition(holderPosition);
}

float Component_Chase::CalcDistance(XMFLOAT3 _holderPos, XMFLOAT3 _targetPos)
{
	// 所有者から対象までの距離を計算
	return XMVectorGetX(XMVector3Length(XMLoadFloat3(&_targetPos) - XMLoadFloat3(&_holderPos)));
}

float Component_Chase::CalcRotateAngle(XMVECTOR _direction)
{
	// 回転角度を計算
	return atan2(XMVectorGetX(-_direction), XMVectorGetZ(-_direction));
}
