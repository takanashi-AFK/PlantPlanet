#include "MoveXComponent.h"
#include "../StageObject.h"
#include "../../../../Engine/ImGui/imgui.h"

MoveXComponent::MoveXComponent(StageObject* _holeder)
	:Component(_holeder,"MoveXComponent",MoveX),moveSpeedX_(0)
{
}

void MoveXComponent::Initialize()
{
}

void MoveXComponent::Update()
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

void MoveXComponent::Release()
{
}

void MoveXComponent::Save(json& _saveObj)
{
	_saveObj["moveSpeedX_"] = moveSpeedX_;
}

void MoveXComponent::Load(json& _loadObj)
{
	moveSpeedX_ = _loadObj["moveSpeedX_"];
}

void MoveXComponent::DrawData()
{
	ImGui::DragFloat("moveSpeedX_",&moveSpeedX_,0.1f);
}