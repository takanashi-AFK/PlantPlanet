#include "Component_MoveX.h"

// �C���N���[�h
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../StageObject.h"

Component_MoveX::Component_MoveX(string _name, StageObject* _holder)
	:Component(_holder,_name,MoveX),moveSpeedX_(0)
{
}

void Component_MoveX::Initialize()
{
}

void Component_MoveX::Update()
{
	// ���݂̈ʒu���擾
	XMFLOAT3 position = holder_->GetPosition();
	
	// ����������
	XMVECTOR dir = XMVectorSet(1, 0, 0, 0);

	// �ړ��������
	XMStoreFloat3(&position, XMLoadFloat3(&position) + XMVector3Normalize(dir) * moveSpeedX_);

	// �ړ�
	holder_->SetPosition(position);
}

void Component_MoveX::Release()
{
}

void Component_MoveX::Save(json& _saveObj)
{
	// �ړ����x��ۑ�
	_saveObj["moveSpeedX_"] = moveSpeedX_;
}

void Component_MoveX::Load(json& _loadObj)
{
	// �ړ����x��Ǎ�
	if(_loadObj.contains("moveSpeedX_"))moveSpeedX_ = _loadObj["moveSpeedX_"];
}

void Component_MoveX::DrawData()
{
	// �ړ����x��ҏW
	ImGui::DragFloat("moveSpeedX_",&moveSpeedX_,0.1f);
	
	// �ړ������Z�b�g
	if (ImGui::Button("reset")) {
		moveSpeedX_ = 0; holder_->SetPosition(0, 0, 0);
	}
}
