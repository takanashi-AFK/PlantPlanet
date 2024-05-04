#include "MoveXComponent.h"

// �C���N���[�h
#include "../../StageObject.h"
#include "../../../../../Engine/ImGui/imgui.h"

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
	// �ړ����x��ۑ�
	_saveObj["moveSpeedX_"] = moveSpeedX_;
}

void MoveXComponent::Load(json& _loadObj)
{
	// �ړ����x��Ǎ�
	moveSpeedX_ = _loadObj["moveSpeedX_"];
}

void MoveXComponent::DrawData()
{
	// �ړ����x��ҏW
	ImGui::DragFloat("moveSpeedX_",&moveSpeedX_,0.1f);
	
	// �ړ������Z�b�g
	if (ImGui::Button("reset")) {
		moveSpeedX_ = 0; holder_->SetPosition(0, 0, 0);
	}
}
