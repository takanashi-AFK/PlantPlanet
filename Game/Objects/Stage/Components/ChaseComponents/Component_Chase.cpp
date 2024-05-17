#include "Component_Chase.h"
#include "../../StageObject.h"
#include "../../Stage.h"
#include "../../../../../Engine/ImGui/imgui.h"
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
	ChaseMove();

}

void Component_Chase::Release()
{
}

void Component_Chase::Save(json& _saveobj)
{
	_saveobj[" movingdistance_"] = speed_;
	if (target_ != nullptr)_saveobj["targetName"] = target_->GetObjectName();
}

void Component_Chase::Load(json& _loadobj)
{
	if (_loadobj.find("move_") != _loadobj.end()) speed_ = _loadobj[" movingdistance_"];
	if (_loadobj.find("target_") != _loadobj.end())target_ = (StageObject*)holder_->FindObject(_loadobj["target_"]);
}

void Component_Chase::DrawData()
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


void Component_Chase::ChaseMove()
{
	if(target_ == nullptr)return;

	//�ΏۂƕۗL�҂̃|�W�V�������擾
	XMFLOAT3 targetPos = target_->GetPosition();
	XMFLOAT3 holderPos = holder_->GetPosition();
	targetPos.y = 0;
	//�|�W�V������Vector�^�ɕύX���Ǐ]��������ƒ��������߂�
	XMVECTOR targetVec = XMLoadFloat3(&targetPos);
	XMVECTOR holderVec = XMLoadFloat3(&holderPos);
	float distance = XMVectorGetX(XMVector3Length(targetVec - holderVec));
	XMVECTOR direction = XMVector3Normalize(XMVectorSetY(targetVec - holderVec, 0));

	//�Ǐ]��������ɑ̂̌�������]������
	double rotateAngle = atan2(XMVectorGetX(-direction), XMVectorGetZ(-direction));

	XMStoreFloat3(&holderPos, holderVec + (direction * speed_));
	if (distance > LIMIT_DISTANCE){
		//�ړ���̈ʒu��K��
		holder_->SetRotateY(rotateAngle);
		holder_->SetPosition(holderPos);
	}
	else
	{
		rotateAngle = 0;
	}

	
}
