#include "Component_MeleeAttack.h"
#include "../../StageObject.h"
#include "../../../../../Engine/Collider/BoxCollider.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../TimerComponent/Component_Timer.h"
#include "../HealthManagerComponents/Component_HealthManager.h"
#include "../../Stage.h"

Component_MeleeAttack::Component_MeleeAttack(string _name, StageObject* _holder, Component* _parent)
	:Component_Attack(_holder, _name, MeleeAttack, _parent), isHit_(false),colliderSize_(1.f,1.f,1.f)
{
}

void Component_MeleeAttack::Initialize()
{
	//�^�C�}�[�̒ǉ�
	if (!FindChildComponent("Timer")) {
		AddChildComponent(CreateComponent("Timer", Timer, holder_, this));
	}

	//�R���C�_�[�̒ǉ�
	holder_->AddCollider(new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1)));
	power_ = 20;
}

void Component_MeleeAttack::Update()
{
	// �L���łȂ��ꍇ�͏������s��Ȃ�
	if (isActive_ == false) return;

	// �R���C�_�[�̍X�V
	Collider* collider = dynamic_cast<BoxCollider*>(holder_->GetCollider(0));
	if (!collider) return;
	{
		//�O�����̎擾
		XMFLOAT3 front{};

		// �ړ��x�N�g����0�̏ꍇ�A�O��̃x�N�g�����g�p
		if (XMVector3Equal(forward_, XMVectorZero())) {
			forward_ = prevFrontVec_;
		}

		XMStoreFloat3(&front, forward_);

		prevFrontVec_ = forward_;

		// �ʒu�̍X�V
		collider->SetCenter(front);

		if (AutoDelete(0.03f)) {
			collider->SetCenter(XMFLOAT3{ 0, 0, 0 });
			Stop();
			isOneHit_ = false;  // AutoDelete��true�̏ꍇ�Ƀ��Z�b�g
		}
	}
}

void Component_MeleeAttack::Release()
{
}

void Component_MeleeAttack::DrawData()
{
	// ������������������������������������
	// �U���͂̐ݒ�
	// ������������������������������������
	ImGui::DragInt("power_", &power_);

	// ������������������������������������
	// �R���C�_�[�̑傫��
	// ������������������������������������
	if (ImGui::DragFloat3("colliderSize_", (float*)&colliderSize_, 0.01f)) {
		// �R���C�_�[�̑傫����ύX
		BoxCollider* collider = dynamic_cast<BoxCollider*>(holder_->GetCollider(0));
		if (collider != nullptr) collider->SetSize(colliderSize_);
	}

	// ������������������������������������
	// �U�������̐ݒ�
	// ������������������������������������
	ImGui::DragFloat3("direction_", (float*)&direction_, 0.01f);

	// ������������������������������������
	// ���s�{�^���̐ݒ�
	// ������������������������������������
	if (ImGui::Button("Execute")) Execute();

	// ������������������������������������
	// �ߋ����U�����q�b�g�������ǂ���
	// ������������������������������������
	ImGui::SameLine();
	ImGui::Text("isHit : %s",isOneHit_ ? "true" : "false");
}

bool Component_MeleeAttack::AutoDelete(float _time)
{
	auto timer = dynamic_cast<Component_Timer*>(GetChildComponent("Timer"));
	if (!timer) return false;
	timer->SetTime(_time);
	timer->Start();
	if (timer->GetIsEnd()) {
		timer->Reset();
		return true;
	}
	return false;
}

void Component_MeleeAttack::OnCollision(GameObject* _target)
{
	// �L���łȂ��ꍇ�͏������s��Ȃ�
	if (!isActive_ || isOneHit_) return;

	// �^�[�Q�b�g��StageObject�łȂ��ꍇ�͏������s��Ȃ�
	StageObject* target = dynamic_cast<StageObject*>(_target);
	if (!target) return;

	// �_���[�W����
	for (auto hm : target->FindComponent(HealthManager)) {
		((Component_HealthManager*)hm)->TakeDamage(power_);
		isOneHit_ = true;  // �_���[�W�����̒���Ƀt���O��ݒ�

		if (((Component_HealthManager*)hm)->GetHP() <= 0.f) {
			((Stage*)holder_->FindObject("Stage"))->DeleteStageObject((StageObject*)_target);
		}
	}
}
