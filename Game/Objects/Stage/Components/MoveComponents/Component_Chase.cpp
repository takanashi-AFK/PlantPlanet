#include "Component_Chase.h"
#include "../../StageObject.h"
#include "../../Stage.h"
#include "../../../../../Engine/ImGui/imgui.h"

namespace {
	const float LIMIT_DISTANCE = 0.5f; // ���̋����ȉ��ɂȂ�����Ǐ]����߂�
}

Component_Chase::Component_Chase(StageObject* _holder)
	:Component(_holder,"Component_Chase",Chase), speed_(0.0f),target_(nullptr)
{
}

void Component_Chase::Initialize()
{
	speed_ = 0.3f;
}

void Component_Chase::Update()
{
	if (target_ == nullptr || isActive_ == false)return;

	//�ΏۂƕۗL�҂̃|�W�V�������擾
	XMFLOAT3 targetPos = target_->GetPosition();
	XMFLOAT3 holderPos = holder_->GetPosition();

	// �Ǐ]����������v�Z
	XMVECTOR direction = XMVectorSetY(CalcDirection(holderPos, targetPos),0);

	// �z���_�[�̌������v�Z
	float rotateAngle = CalcRotateAngle(direction);

	// �������v�Z
	float distance = CalcDistance(holderPos, targetPos);

	if (distance > LIMIT_DISTANCE) {
		
		//��������K��
		holder_->SetRotateY(rotateAngle);

		//�ړ�
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
	_saveobj["isActive_"] = isActive_;
	if(target_ != nullptr)_saveobj["target_"] = target_->GetObjectName();
}

void Component_Chase::Load(json& _loadobj)
{
	if(_loadobj.contains("speed_"))speed_ = _loadobj["speed_"];
	if(_loadobj.contains("isActive_"))isActive_ = _loadobj["isActive_"];
	if(_loadobj.contains("target_"))target_ = (StageObject*)holder_->FindObject(_loadobj["target_"]);
}

void Component_Chase::DrawData()
{

	// �Ǐ]�̗L������
	ImGui::Checkbox("isActive", &isActive_);
	
	// ���x�̐ݒ�
	ImGui::DragFloat("speed", &speed_, 0.01f, 0.0f, 10.0f);

	// �^�[�Q�b�g�̑I��
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
	return XMVector3Normalize(XMLoadFloat3(&_targetPos) - XMLoadFloat3(&_holderPos));
}

void Component_Chase::Move(XMVECTOR _direction)
{
	XMFLOAT3 holderPosition = holder_->GetPosition();
	XMStoreFloat3(&holderPosition, XMLoadFloat3(&holderPosition) + (_direction * speed_));
	holder_->SetPosition(holderPosition);
}

float Component_Chase::CalcDistance(XMFLOAT3 _holderPos, XMFLOAT3 _targetPos)
{
	return XMVectorGetX(XMVector3Length(XMLoadFloat3(&_targetPos) - XMLoadFloat3(&_holderPos)));
}

float Component_Chase::CalcRotateAngle(XMVECTOR _direction)
{
	// ��]�p�x���v�Z
	return atan2(XMVectorGetX(-_direction), XMVectorGetZ(-_direction));
}
