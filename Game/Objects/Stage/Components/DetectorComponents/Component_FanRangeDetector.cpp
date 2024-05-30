#include "Component_FanRangeDetector.h"

#include "../../../../../Engine/Global.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../Stage.h"
#include "../../StageObject.h"

Component_FanRangeDetector::Component_FanRangeDetector(string _name, StageObject* _holder)
	: Component(_holder, _name, FanRangeDetector)
	, target_(nullptr), length_(0), angle_(0), direction_(XMVectorSet(0, 0, 0, 0)), isContains_(false)
{
}

void Component_FanRangeDetector::Initialize()
{
}

void Component_FanRangeDetector::Update()
{
	// �Ώۂ����݂��Ȃ��ꍇ�͏������s��Ȃ�
	if (target_ == nullptr)return;
	
	// ��`�͈͔���
	isContains_ = IsContains();
}

void Component_FanRangeDetector::Release()
{
}

void Component_FanRangeDetector::Save(json& _saveObj)
{
	// ���g�̏���ۑ�
	_saveObj["length_"] = length_;
	_saveObj["angle_"] = angle_;
	if (target_ != nullptr)_saveObj["target_"] = target_->GetObjectName();
}

void Component_FanRangeDetector::Load(json& _loadObj)
{
	// ���g�̏���ǂݍ���
	if (_loadObj.find("length_") != _loadObj.end())length_ = _loadObj["length_"];
	if (_loadObj.find("angle_") != _loadObj.end())angle_ = _loadObj["angle_"];
	if (_loadObj.find("target_") != _loadObj.end())target_ = (StageObject*)holder_->FindObject(_loadObj["target_"]);
}

void Component_FanRangeDetector::DrawData()
{
	
	// ImGui�Ńf�[�^��`��
	ImGui::Text("isContains_ : %s", isContains_ ? "true" : "false");
	
	// �����̓���
	ImGui::DragFloat("length_", &length_);
	
	// �p�x�̓���
	ImGui::DragFloat("angle_", &angle_);
	
	// �����x�N�g���̓���
	XMFLOAT3 direction;
	XMStoreFloat3(&direction, direction_);
	ImGui::DragFloat3("direction_",&direction.x,0.1f);
	direction_ = XMLoadFloat3(&direction);

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

bool Component_FanRangeDetector::IsContains()
{
	//	�Ώۂ����݂��Ȃ��ꍇ�͏������s��Ȃ�
	if (target_ == nullptr)return false;

	// ��`�͈͔���
	// �ۗL�҂̈ʒu�������x�N�g���i�Q�����j
	XMFLOAT3 holderPos = holder_->GetPosition();
	XMVECTOR vecA = XMVectorSetY(XMLoadFloat3(&holderPos), 0);

	// �Ώۂ̈ʒu�������x�N�g���i�Q�����j
	XMFLOAT3 targetPos = target_->GetPosition();
	XMVECTOR vecB = XMVectorSetY(XMLoadFloat3(&targetPos), 0);

	// �ۗL�҂̈ʒu����Ώۂ̈ʒu�ւ̃x�N�g���i�Q�����j
	XMVECTOR vecAB = vecB - vecA;
		
	// �ۗL�҂̈ʒu����Ώۂ̈ʒu�ւ̃x�N�g���̒���
	float vecABLength = XMVectorGetX(XMVector3Length(vecAB));

	// �������r
	if (vecABLength > length_)return false;

	// ���ς����߂�
	float dot = XMVectorGetX(XMVector2Dot(direction_, XMVector2Normalize(vecAB)));

	// ��͈̔͂���cos�̒l�����߂�
	float angleCos = cos(XMConvertToRadians(angle_ / 2.0f));
	
	// cos�̒l���r
	return !(angleCos > dot);
}
