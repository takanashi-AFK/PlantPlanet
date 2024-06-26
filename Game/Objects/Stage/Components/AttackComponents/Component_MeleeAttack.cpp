#include "Component_MeleeAttack.h"
#include "../../StageObject.h"
#include "../../../../../Engine/Collider/BoxCollider.h"
#include "../../Knuckle.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../TimerComponent/Component_Timer.h"
Component_MeleeAttack::Component_MeleeAttack(string _name, StageObject* _holder, Component* _parent)
	:Component_Attack(_holder, _name, MeleeAttack, _parent)
{
}

void Component_MeleeAttack::Initialize()
{
	//�^�C�}�[�̒ǉ�
	if (FindChildComponent("Timer") == false)AddChildComponent(CreateComponent("Timer", Timer, holder_, this));
	
	//�R���C�_�[�̒ǉ�
	holder_->AddCollider(new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1)));
}

void Component_MeleeAttack::Update()
{
	// �L���łȂ��ꍇ�͏������s��Ȃ�
	if (isActive_ == false)return;

	// �R���C�_�[�̍X�V
	Collider* collider = dynamic_cast<BoxCollider*>((holder_->GetCollider(0)));
	if (collider == nullptr) return;
	{
		//�O�����̎擾
		XMFLOAT3 front{};
		XMStoreFloat3(&front, forward_);
		
		// �ʒu�̍X�V
		collider->SetCenter(front);
	}
}

void Component_MeleeAttack::Release()
{
}

void Component_MeleeAttack::DrawData()
{
	ImGui::Text("MeleeAttack");
	if (ImGui::Button("Execute"))Execute();
}

void Component_MeleeAttack::AutoDelete(float _time)
{
	auto timer = dynamic_cast<Component_Timer*>(GetChildComponent("Timer"));
	if (timer == nullptr) return;
	timer->SetTime(_time);
	timer->Start();
	if (timer->GetIsEnd()) {
		holder_->ClearCollider();
		timer->Reset();
	}
}
