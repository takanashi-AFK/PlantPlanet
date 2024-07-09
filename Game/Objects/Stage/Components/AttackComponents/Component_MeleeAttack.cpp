#include "Component_MeleeAttack.h"
#include "../../StageObject.h"
#include "../../../../../Engine/Collider/BoxCollider.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../TimerComponent/Component_Timer.h"
#include "../GaugeComponents/Component_HealthGauge.h"
#include "../../Stage.h"

Component_MeleeAttack::Component_MeleeAttack(string _name, StageObject* _holder, Component* _parent)
	:Component_Attack(_holder, _name, MeleeAttack, _parent), isHit_(false), colliderSize_(1.f, 1.f, 1.f)
{
}

void Component_MeleeAttack::Initialize()
{
	// �R���|�[�l���g��ǉ�
	if (FindChildComponent("Timer") == false) AddChildComponent(CreateComponent("Timer", Timer, holder_, this));

	//�R���C�_�[�̒ǉ�
	holder_->AddCollider(new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1)));
}

void Component_MeleeAttack::Update()
{
	// �^�C�}�[���擾
	auto timer = dynamic_cast<Component_Timer*>(GetChildComponent("Timer"));
	if (timer == nullptr) return;

	// �L���łȂ��ꍇ�͏������s��Ȃ�
	if (isActive_ == false) {
		timer->Reset();
		timer->SetTime(0.03f);
		return;
	}

	// �^�C�}�[���J�n
	timer->Start();

	// �R���C�_�[���擾
	Collider* collider = dynamic_cast<BoxCollider*>(holder_->GetCollider(0));
	
	// �R���C�_�[���擾�ł��Ȃ��ꍇ�͏������s��Ȃ�
	if (collider == nullptr) return;

	// holder�̈ʒu�ƍU����������A�R���C�_�[�̈ʒu��ݒ�
	{
		// �ۗL�҂̈ʒu�ɍU�����������Z
		XMFLOAT3 pos;
		XMStoreFloat3(&pos,XMVector3Normalize(direction_));

		// �R���C�_�[�̈ʒu��ݒ�
		collider->SetCenter(pos);
	}

	// �^�C�}�[���I�����Ă���ꍇ�́A�U�����I��
	if (timer->GetIsEnd() == true) {
		Stop();
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
	ImGui::Text("isHit : %s",isHit_ ? "true" : "false");

	// ������������������������������������
	// �A�N�e�B�u��Ԃ��ǂ����̐ݒ�
	// ������������������������������������
	ImGui::Text("isActive_ : %s", isActive_ ? "true" : "false");

}

void Component_MeleeAttack::OnCollision(GameObject* _target)
{
	// �L���łȂ��ꍇ�͏������s��Ȃ�
	if (!isActive_ || isHit_) return;

	// �^�[�Q�b�g��StageObject�łȂ��ꍇ�͏������s��Ȃ�
	StageObject* target = dynamic_cast<StageObject*>(_target);
	if (target == nullptr) return;

	// �_���[�W����
	for (auto hm : target->FindComponent(HealthGauge)) {
		((Component_HealthGauge*)hm)->TakeDamage(power_);
		isHit_ = true;  // �_���[�W�����̒���Ƀt���O��ݒ�

		if (((Component_HealthGauge*)hm)->GetNow() <= 0.f) {
			((Stage*)holder_->FindObject("Stage"))->DeleteStageObject((StageObject*)_target);
		}
	}
}
