#include "Component_PlayerBehavior.h"

// �C���N���[�h
#include "../../../../../Engine/Collider/BoxCollider.h"
#include "../../StageObject.h"
#include "../HealthManagerComponents/Component_HealthManager.h"
#include "../../../../../Engine/ImGui/imgui.h"

Component_PlayerBehavior::Component_PlayerBehavior(string _name, StageObject* _holder, Component* _parent)
	: Component(_holder, _name, PlayerBehavior,_parent)
{
}

void Component_PlayerBehavior::Initialize()
{
	// �R���C�_�[�̒ǉ�
	holder_->AddCollider(new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1)));

	// �q�R���|�[�l���g�̒ǉ�
	if (FindChildComponent("InputMove") == false)AddChildComponent(CreateComponent("InputMove", WASDInputMove, holder_, this));
	if (FindChildComponent("HealthManager") == false)AddChildComponent(CreateComponent("HealthManager", HealthManager, holder_, this));
	//if (FindChildComponent("ShootAttack") == false)AddChildComponent(CreateComponent("ShootAttack", ShootAttack, holder_, this));
}

void Component_PlayerBehavior::Update()
{
	auto hm = dynamic_cast<Component_HealthManager*>(GetChildComponent("HealthManager"));

	ImGui::Text("Player HP : %f", hm->GetHP());
}

void Component_PlayerBehavior::Release()
{
}

void Component_PlayerBehavior::OnCollision(GameObject* _target)
{
}
