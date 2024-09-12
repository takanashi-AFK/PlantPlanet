#include "Component_HealthGauge.h"

// �C���N���[�h
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../StageObject.h"


Component_HealthGauge::Component_HealthGauge(string _name, StageObject* _holder, Component* _parent)
	:Component_Gauge(_holder, _name, HealthGauge,_parent), prev_(now_), shaderChangeTime_(60),isLock_(false)
{

}

void Component_HealthGauge::Initialize()
{
}

void Component_HealthGauge::Update()
{
	static int prevShaderType = (int)holder_->GetShader();
	// �_���[�W�����������V�F�[�_�[��ύX
	if (prev_ > now_) {
		holder_->SetShader(Direct3D::SHADER_DAMAGE);
		shaderChangeTime_ = 30; // �V�F�[�_�[�ύX���Ԃ����Z�b�g
	}
	// ��莞�Ԃ��o�߂�����V�F�[�_�[�����ɖ߂�
	if (shaderChangeTime_ > 0) {
		shaderChangeTime_--;
		if (shaderChangeTime_ == 0) {
			holder_->SetShader((Direct3D::SHADER_TYPE)prevShaderType);
		}
	}

	// �e�l�̃��Z�b�g
	prev_ = now_;
	isTakeDamage_ = false;

	/* DEBUG:*/{
		/*ImGui::Text("%s -> healthData", holder_->GetObjectName().c_str());
		ImGui::Text("now : %f", now_);
		ImGui::Text("prev : %f", prev_);
		ImGui::Text("max : %f", max_);
		ImGui::Text("shaderChangeTime : %d", shaderChangeTime_);
		ImGui::Text("isLock : %d", isLock_);
		ImGui::Separator();*/
	}
}

void Component_HealthGauge::Release()
{
}

void Component_HealthGauge::TakeDamage(float _damageValue)
{
	// �_���[�W���󂯂��t���O�𗧂Ă�
	isTakeDamage_ = true;

	if(isLock_)return;

	// �O��̗̑͂�ۑ�
	prev_ = now_;

	//HP�����炷
	now_ -= _damageValue;

	//HP��0�ȉ��ɂȂ�Ȃ��悤�ɂ���
	if (now_ < 0)now_ = 0;
}

void Component_HealthGauge::Heal(float _healValue)
{
	if (isLock_)return;

	//HP���ő�l�𒴂��Ȃ��悤�ɂ���
	if (now_ + _healValue <= max_)now_ += _healValue;
}