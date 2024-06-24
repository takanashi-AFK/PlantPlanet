#include "Component_WASDInputMove.h"
#include "../../../../../Engine/DirectX/Input.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../StageObject.h"
#include "../../../../../Engine/GameObject/Camera.h"

namespace
{
	float speed = 0.1f;
}

Component_WASDInputMove::Component_WASDInputMove(string _name, StageObject* _holder, Component* _parent)
	:Component(_holder, _name, WASDInputMove, _parent),isActive_(true)
{
}

void Component_WASDInputMove::Initialize()
{
}

void Component_WASDInputMove::Update()
{
    // ���̃R���|�[�l���g���A�N�e�B�u�łȂ��ꍇ�A�������I��
    if (!isActive_) return;

    // ��{�̃x�N�g����p�ӁA������
    XMVECTOR dir = XMVectorZero();

    // �J�����̎����x�N�g�����擾�AY������0�ɂ��Đ��K��
    XMVECTOR sightLine = Camera::GetSightLine();
    sightLine = XMVectorSetY(sightLine, 0);
    sightLine = XMVector3Normalize(sightLine);

    // ���͂ɉ����ĕ����x�N�g����ݒ�
    if (Input::IsKey(DIK_W)) dir += sightLine;
    if (Input::IsKey(DIK_A)) dir += XMVector3Transform(sightLine, XMMatrixRotationY(XMConvertToRadians(-90)));
    if (Input::IsKey(DIK_S)) dir -= sightLine;
    if (Input::IsKey(DIK_D)) dir += XMVector3Transform(sightLine, XMMatrixRotationY(XMConvertToRadians(90)));
    dir = XMVector3Normalize(dir);
    XMVECTOR move = dir * speed;

    // ���݂̈ʒu���擾
    XMFLOAT3 pos = holder_->GetPosition();

    // �ړ��x�N�g����XMFLOAT3�ɕϊ�
    XMFLOAT3 moveVector;
    XMStoreFloat3(&moveVector, move);

    // �V�����ʒu���v�Z
    pos.x += moveVector.x;
    pos.y += moveVector.y;
    pos.z += moveVector.z;

    // �V�����ʒu��ݒ�
    holder_->SetPosition(pos);
}

void Component_WASDInputMove::Release()
{
}

void Component_WASDInputMove::DrawData()
{
	ImGui::Checkbox("Active", &isActive_);
	ImGui::SliderFloat("Speed", &speed, 0.01f, 1.0f);
}

void Component_WASDInputMove::Move(XMVECTOR _dir, float _speed)
{
	XMFLOAT3 pos = holder_->GetPosition();
	XMStoreFloat3(&pos, XMLoadFloat3(&pos) + _dir * _speed);
	holder_->SetPosition(pos);
}
