#include "Component_CircleRangeDetector.h"

// �C���N���[�h
#include "../../../../../Engine/Global.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../Stage.h"
#include "../../StageObject.h"

Component_CircleRangeDetector::Component_CircleRangeDetector(string _name, StageObject* _holder)
    : Component(_holder, _name, CircleRangeDetector), radius_(0.0f), isContains_(false), target_(nullptr)
{
}

void Component_CircleRangeDetector::Initialize()
{
}

void Component_CircleRangeDetector::Update()
{
	// �Ώۂ����݂��Ȃ��ꍇ�͏������s��Ȃ�
	if (target_ == nullptr)return;

	// �~�`�͈͔���
	// �͈͓��ɓ����Ă��邩�ǂ����𔻒�
	isContains_ = IsContains();
}

void Component_CircleRangeDetector::Release()
{
}

void Component_CircleRangeDetector::Save(json& _saveObj)
{
	// ���g�̏���ۑ�
	_saveObj["radius_"] = radius_;
	if(target_ != nullptr)_saveObj["target_"] = target_->GetObjectName();
}

void Component_CircleRangeDetector::Load(json& _loadObj)
{
	// ���g�̏���ǂݍ���
	if(_loadObj.find("radius_") != _loadObj.end())radius_ = _loadObj["radius_"];
	if (_loadObj.find("isContains_") != _loadObj.end())isContains_ = _loadObj["isContains_"];
	if (_loadObj.find("target_") != _loadObj.end())target_ = (StageObject*)holder_->FindObject(_loadObj["target_"]);
}

void Component_CircleRangeDetector::DrawData()
{
	// ImGui�Ńf�[�^��`��
	
	ImGui::Text("isContains_ : %s", isContains_ ? "true" : "false");

	// ���a�̓���
	ImGui::DragFloat("radius_", &radius_);

	ImGui::Separator();
	
	//�Ώۂ̑I��
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

bool Component_CircleRangeDetector::IsContains()
{
	// �Ώۂ����݂��Ȃ��ꍇ�͏������s��Ȃ�
	if (target_ == nullptr)return false;

	// �͈͓��ɓ����Ă��邩�ǂ����𔻒�
	float a = target_->GetPosition().x - holder_->GetPosition().x;
	float b = target_->GetPosition().z - holder_->GetPosition().z;

	// 2�_�Ԃ̋��������a�ȉ��Ȃ�true
	return sqrt(a * a + b * b) <= radius_;

}
