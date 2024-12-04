#include "Component_FanRangeDetector.h"

#include "../../../../../Engine/Global.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../Stage.h"
#include "../../StageObject.h"

Component_FanRangeDetector::Component_FanRangeDetector(string _name, StageObject* _holder, Component* _parent)
	: Component(_holder, _name, FanRangeDetector,_parent),
	target_(nullptr),
	length_(0), 
	angle_(0), 
	direction_(XMVectorSet(0, 0, 0, 0)), 
	isContains_(false)
{
}

void Component_FanRangeDetector::Initialize()
{
}

void Component_FanRangeDetector::Update()
{

	// 対象が存在しない場合は処理を行わない
	if (target_ == nullptr)return;
	
	// 扇形範囲判定
	isContains_ = IsContains();
}

void Component_FanRangeDetector::Release()
{
}

void Component_FanRangeDetector::Save(json& _saveObj)
{
	// 自身の情報を保存
	_saveObj["length_"] = length_;
	_saveObj["angle_"] = angle_;
	if (target_ != nullptr)_saveObj["target_"] = target_->GetObjectName();
}

void Component_FanRangeDetector::Load(json& _loadObj)
{
	// 自身の情報を読み込み
	if (_loadObj.find("length_") != _loadObj.end())length_ = _loadObj["length_"];
	if (_loadObj.find("angle_") != _loadObj.end())angle_ = _loadObj["angle_"];
	if (_loadObj.find("target_") != _loadObj.end())target_ = (StageObject*)holder_->FindObject(_loadObj["target_"]);
}

void Component_FanRangeDetector::DrawData()
{
	
	// ImGuiでデータを描画
	ImGui::Text("isContains_ : %s", isContains_ ? "true" : "false");
	
	// 長さの入力
	ImGui::DragFloat("length_", &length_);
	
	// 角度の入力
	ImGui::DragFloat("angle_", &angle_);
	
	// 方向ベクトルの入力
	XMFLOAT3 direction;
	XMStoreFloat3(&direction, direction_);
	ImGui::DragFloat3("direction_",&direction.x,0.1f);
	direction_ = XMLoadFloat3(&direction);

	// 対象の選択
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

	if (target_ != nullptr) {
		XMFLOAT3 pos = target_->GetPosition();
		ImGui::Text("position_ = %f,%f,%f", REFERENCE_XMFLOAT3(pos));
	}
	
}

bool Component_FanRangeDetector::IsContains()
{
	//	対象が存在しない場合は処理を行わない
	if (target_ == nullptr)return false;

	// 扇形範囲判定
	// 保有者の位置を示すベクトル（２次元）
	XMFLOAT3 holderPos = holder_->GetPosition();
	XMVECTOR vecA = XMVectorSetY(XMLoadFloat3(&holderPos), 0);

	// 対象の位置を示すベクトル（２次元）
	XMFLOAT3 targetPos = target_->GetPosition();
	XMVECTOR vecB = XMVectorSetY(XMLoadFloat3(&targetPos), 0);

	// 保有者の位置から対象の位置へのベクトル（２次元）
	XMVECTOR vecAB = vecB - vecA;
		
	// 保有者の位置から対象の位置へのベクトルの長さ
	float vecABLength = XMVectorGetX(XMVector3Length(vecAB));

	// 長さを比較
	if (vecABLength > length_)return false;

	// 内積を求める
	float dot = XMVectorGetX(XMVector2Dot(direction_, XMVector2Normalize(vecAB)));

	// 扇の範囲からcosの値を求める
	float angleCos = cos(XMConvertToRadians(angle_ / 2.0f));
	
	// cosの値を比較
	return !(angleCos > dot);
}
