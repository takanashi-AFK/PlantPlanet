#include "Component_RectangleDetector.h"

#include "../../../../../Engine/Global.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../Stage.h"
#include "../../StageObject.h"
Component_RectangleDetector::Component_RectangleDetector(string _name, StageObject* _holder, Component* _parent)
	: Component(_holder, _name, RectangleDetector, _parent),
	target_(nullptr),
	length_(XMFLOAT2(0,0)),
	isContains_(false)
{
}

void Component_RectangleDetector::Initialize()
{
}

void Component_RectangleDetector::Update()
{

	// �Ώۂ����݂��Ȃ��ꍇ�͏������s��Ȃ�
	if (target_ == nullptr)return;

	// ��`�͈͔���
	isContains_ = IsContains();
}

void Component_RectangleDetector::Release()
{
}

void Component_RectangleDetector::Save(json& _saveObj)
{
	_saveObj["length_.x"] = length_.x;
	_saveObj["length_.y"] = length_.y;
	if (target_ != nullptr)_saveObj["target_"] = target_->GetObjectName();
}

void Component_RectangleDetector::Load(json& _loadObj)
{
	// ���g�̏���ǂݍ���
	if (_loadObj.find("length_.x") != _loadObj.end())length_.x = _loadObj["length_.x"];
	if (_loadObj.find("length_.y") != _loadObj.end())length_.y = _loadObj["length_.y"];
	if (_loadObj.find("target_") != _loadObj.end())target_ = (StageObject*)holder_->FindObject(_loadObj["target_"]);
}

void Component_RectangleDetector::DrawData()
{
	// ImGui�Ńf�[�^��`��
	ImGui::Text("isContains_ : %s", isContains_ ? "true" : "false");

	// �����̓���
	ImGui::DragFloat2("length_", &length_.x);

	// �Ώۂ̑I��
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

bool Component_RectangleDetector::IsContains()
{
	//	�Ώۂ����݂��Ȃ��ꍇ�͏������s��Ȃ�
	if (target_ == nullptr)return false;

	XMFLOAT3 holderPos = holder_->GetPosition();
	// ��`�̋��E���W���v�Z
	float left = holderPos.x - length_.x / 2;
	float right = holderPos.x + length_.x / 2;
	float back = holderPos.z - length_.y / 2;
	float front = holderPos.z + length_.y / 2;

	XMFLOAT3 targetPos = target_->GetPosition();
	return (targetPos.x >= left && targetPos.x <= right &&
		targetPos.z >= back && targetPos.z <= front);
}
