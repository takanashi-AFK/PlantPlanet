
// �C���N���[�h
#include "../../../Engine/ImGui/imgui.h"
#include "../../Components/DetectorComponents/Component_CircleRangeDetector.h"
#include "../../Components/MoveComponents/Component_Chase.h"
#include "../../Stage.h"
#include "../../StageObject.h"
#include "../DetectorComponents/Component_FanRangeDetector.h"
#include "../GaugeComponents/Component_HealthGauge.h"
#include "../TimerComponent/Component_Timer.h"
#include "Component_MeleeEnemyBehavior.h"

Component_MeleeEnemyBehavior::Component_MeleeEnemyBehavior(string _name, StageObject* _holder, Component* _parent):
	Component(_holder, _name, MeleeEnemyBehavior, _parent),
	enemyState_(E_IDLE),
	playerObjects_(nullptr),
	deltaDistance_(1.f),
	power_(5.f),
	isAttacked_(false)
{
}

void Component_MeleeEnemyBehavior::Initialize()
{
	// �q�R���|�[�l���g��ǉ�
	if (FindChildComponent("CircleRangeDetector") == false)AddChildComponent(CreateComponent("CircleRangeDetector", CircleRangeDetector, holder_, this));
	if (FindChildComponent("Chase") == false)AddChildComponent(CreateComponent("Chase", Chase, holder_, this));
	if (FindChildComponent("FanRangeDetector") == false)AddChildComponent(CreateComponent("FanRangeDetector", FanRangeDetector, holder_, this));
	if (FindChildComponent("timer")==false)AddChildComponent(CreateComponent("timer", Timer, holder_, this));
}

void Component_MeleeEnemyBehavior::Update()
{
	//�eState���Ƃ̏���
	switch (enemyState_)
	{
	case E_IDLE:	Idle();		break; // �ҋ@��� �v���C���[���߂��ɂ��邩�ǂ��������m
	case E_MOVE:	Move();		break; // �ړ���� �v���C���[�ɋ߂Â�
	case E_ATTACK:	Attack();	break; // �U����� �v���C���[�ɍU��
	case E_DEAD:	Dead();		break; // ���S���
	}
}

void Component_MeleeEnemyBehavior::Release()
{
}

void Component_MeleeEnemyBehavior::Save(json& _saveObj)
{
	_saveObj["speed_"] = speed_;
	_saveObj["power_"] = power_;
	_saveObj["attackCoolTime_"] = attackCoolTime_;
}

void Component_MeleeEnemyBehavior::Load(json& _loadObj)
{
	if (_loadObj.contains("speed_"))speed_ = _loadObj["speed_"];
	if (_loadObj.contains("power_"))power_ = _loadObj["power_"];
	if (_loadObj.contains("attackCoolTime_"))attackCoolTime_ = _loadObj["attackCoolTime_"];
}

void Component_MeleeEnemyBehavior::DrawData()
{
	// �ړ����x
	ImGui::DragFloat("speed_", &speed_, 0.1, 0);
	// �^�_���[�W
	ImGui::DragFloat("power_", &power_, 0.1, 0);
	// �U���N�[���^�C��
	ImGui::DragFloat("attackCoolTime_",&attackCoolTime_, 0.1, 0);
}

void Component_MeleeEnemyBehavior::Idle()
{
	// �X�e�[�W�����擾
	Stage* pStage = (Stage*)(holder_->FindObject("Stage"));
	if (pStage == nullptr)return;

	// �e�R���|�[�l���g���擾
	Component_CircleRangeDetector* detector = (Component_CircleRangeDetector*)(GetChildComponent("CircleRangeDetector"));
	if (detector == nullptr)return;

	Component_Timer* timer = (Component_Timer*)(GetChildComponent("timer"));
	if (timer == nullptr)return;

	// �O��̃��[�v�ōU�����Ă�����N�[���^�C����ݒ�
	if (isAttacked_ == true) {

		timer->SetTime(attackCoolTime_);
		timer->Start();
		if (timer->GetIsEnd()) {
			isAttacked_ = false;
			timer->Reset();
		}
	}
	else if(isAttacked_ == false){
		// �v���C���[���߂��ɂ��邩�ǂ��������m
		if (detector->IsContains()) {
			for (StageObject* object : pStage->GetStageObjects()) {
				// Player�̑������������I�u�W�F�N�g���擾
				if (object->GetObjectType() == StageObject::TYPE_PLAYER) {
					playerObjects_ = object;
				}
			}
			if (playerObjects_ == nullptr) {
				SetState(E_IDLE);
				return;
			}
			else {
				SetState(E_MOVE);
				return;
			}
		}
	}
}

void Component_MeleeEnemyBehavior::Move()
{
	// �e�R���|�[�l���g���擾
	Component_Chase* chase = (Component_Chase*)(GetChildComponent("Chase"));
	if (chase == nullptr)return;

	Component_CircleRangeDetector* detector = (Component_CircleRangeDetector*)(GetChildComponent("CircleRangeDetector"));
	if (detector == nullptr)return;

	// �^�[�Q�b�g��ݒ�A�ǐՊJ�n
	chase->SetTarget(playerObjects_);
	chase->SetSpeed(speed_);
	chase->Execute();

	XMFLOAT3 playerPos = playerObjects_->GetPosition();
	XMFLOAT3 enemyPos = holder_->GetPosition();
	XMVECTOR deltaDist = XMLoadFloat3(&playerPos) - XMLoadFloat3(&enemyPos);

	// ���������ȉ��ɂȂ�����U����ԂɑJ��
	if (XMVectorGetX(XMVector3Length(deltaDist)) <= deltaDistance_) {
		chase->Stop();
		SetState(E_ATTACK);
	};

	// �T�m�͈͊O�ɏo����ҋ@��ԂɑJ��
	if (!detector->IsContains()) {
		chase ->Stop();
		SetState(E_IDLE);
	}

}

void Component_MeleeEnemyBehavior::Attack()
{
	// �e�R���|�[�l���g���擾
	Component_FanRangeDetector* attackRange = (Component_FanRangeDetector*)(GetChildComponent("FanRangeDetector"));
	if (attackRange == nullptr)return;

	// �v���C���[���U���͈͓��ɂ��邩�ǂ��������m
	attackRange->SetTarget(playerObjects_);
	if (attackRange->IsContains()) {
		// �v���C���[�Ƀ_���[�W��^����
		Component_HealthGauge* healthGauge = (Component_HealthGauge*)playerObjects_->FindComponent("PlayerHealthGauge");
		if (healthGauge == nullptr)return;
		healthGauge->TakeDamage(power_);
		// �t���O�𗧂Ă�
		isAttacked_ = true;
	}
	// �v���C���[���U���͈͊O�ɏo����ҋ@��ԂɑJ��
		SetState(E_IDLE);

}

void Component_MeleeEnemyBehavior::Dead()
{
	// ���̏�Ɏw�肵���Ԃ��o��
}
