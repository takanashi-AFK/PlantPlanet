#include "Component_ShootAttack.h"

// �C���N���[�h
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../Bullet.h"
#include "../../Stage.h"

Component_ShootAttack::Component_ShootAttack(string _name, StageObject* _holder, Component* _parent):
	Component_Attack(_holder, _name, ShootAttack,_parent),
	bulletSpeed_(0.5f)
{
}

void Component_ShootAttack::Initialize()
{
}

void Component_ShootAttack::Update()
{	
	if (isActive_ == false)return;

	// �������v���n�u�𐶐�
	Bullet* bulletPrefab = Instantiate<Bullet>(holder_->GetParent());
	
	// ���������x��ݒ�
	bulletPrefab->SetSpeed(bulletSpeed_);

	// ������������ݒ�
	bulletPrefab->SetDirection(bulletDirection_);

	// �������ʒu��ݒ�
	bulletPrefab->SetPosition(holder_->GetPosition());

	// ��������
	bulletPrefab->Execute();
	
	// �U�����~
	Stop();
}

void Component_ShootAttack::Release()
{
}

void Component_ShootAttack::Save(json& _saveObj)
{
}

void Component_ShootAttack::Load(json& _loadObj)
{
}

void Component_ShootAttack::DrawData()
{
	// ���x�̐ݒ�
	ImGui::DragFloat("Speed", &bulletSpeed_, 0.1f, 0, 2.f);
	
	// �����̐ݒ�
	ImGui::DragFloat3("Direction", (float*)&bulletDirection_, 0.1f);

	// �U���{�^��
	if (ImGui::Button("Execute"))this->Execute();
}
