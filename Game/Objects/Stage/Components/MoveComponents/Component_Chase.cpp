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

	//�ΏۂƕۗL�҂̃|�W�V�������擾
	XMFLOAT3 targetPos = target_->GetPosition();
	XMFLOAT3 holderPos = holder_->GetPosition();

	// �Ǐ]����������v�Z
	XMVECTOR direction = XMVectorSetY(CalcDirection(holderPos, targetPos),0);

	// �z���_�[�̌������v�Z
	float rotateAngle = XMConvertToDegrees(CalcRotateAngle(direction));

	// �������v�Z
	float distance = CalcDistance(holderPos, targetPos);

	if (distance > GetLimitDistance()) {
		
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
	_saveobj["limitDistance_"] = limitDistance_;
	if(target_ != nullptr)_saveobj["target_"] = target_->GetObjectName();
}

void Component_Chase::Load(json& _loadObj)
{
	if(_loadObj.contains("speed_"))speed_ = _loadObj["speed_"];
	if(_loadObj.contains("isActive_"))isActive_ = _loadObj["isActive_"];
	if (_loadObj.contains("target_"))targetName_ = _loadObj["target_"];
	if(_loadObj.contains("limitDistance_"))limitDistance_ = _loadObj["limitDistance_"];
}

void Component_Chase::DrawData()
{

	// �Ǐ]�̗L������
	ImGui::Checkbox("isActive", &isActive_);
	
	// ���x�̐ݒ�
	ImGui::DragFloat("speed", &speed_, 0.01f, 0.0f, 10.0f);

	// �ǐՂ���߂鋗���̐ݒ�
	ImGui::DragFloat("limitDistance", &limitDistance_, 0.01f, 0.0f, 10.0f);

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
	// �������v�Z
	return XMVector3Normalize(XMLoadFloat3(&_targetPos) - XMLoadFloat3(&_holderPos));
}

void Component_Chase::Move(XMVECTOR _direction)
{
	// ���L�҂̈ʒu���擾
	XMFLOAT3 holderPosition = holder_->GetPosition();

	// �ړ�
	XMStoreFloat3(&holderPosition, XMLoadFloat3(&holderPosition) + (_direction * speed_));
	holder_->SetPosition(holderPosition);
}

float Component_Chase::CalcDistance(XMFLOAT3 _holderPos, XMFLOAT3 _targetPos)
{
	// ���L�҂���Ώۂ܂ł̋������v�Z
	return XMVectorGetX(XMVector3Length(XMLoadFloat3(&_targetPos) - XMLoadFloat3(&_holderPos)));
}

float Component_Chase::CalcRotateAngle(XMVECTOR _direction)
{
	// ��]�p�x���v�Z
	return atan2(XMVectorGetX(-_direction), XMVectorGetZ(-_direction));
}
