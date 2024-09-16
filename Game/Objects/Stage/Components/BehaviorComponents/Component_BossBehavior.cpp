#include "Component_BossBehavior.h"

// �C���N���[�h
#include "../../StageObject.h"
#include "../TimerComponent/Component_Timer.h"
#include "../AttackComponents/Component_ShootAttack.h"
#include "../DetectorComponents/Component_CircleRangeDetector.h"
#include "../MoveComponents/Component_TackleMove.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../Stage.h"
#include "../../../../../Engine/Global.h"
#include <random>
#include "../../../UI/CountDown.h"
#include "../GaugeComponents/Component_HealthGauge.h"
#include "../../../UI/UIPanel.h"
#include "../../../UI/UIProgressBar.h"
#include "../../../../Constants.h"

using namespace Constants;

namespace
{
    const float SHOT_RATE = 0.2f;
    const float SHOT_ANGLE = 15;
    const int SHOT_TIME = 5;
	const float SMALL_VEROSITY = 0.02f;
    EFFEKSEERLIB::EFKTransform t;/*������*/
}

Component_BossBehavior::Component_BossBehavior(string _name, StageObject* _holder, Component* _parent)
    : Component(_holder, _name, BossBehavior, _parent)
{
#ifdef _DEBUG
    isActive_ = false;
#else
    isActive_ = true;
#endif // DEBUG

}

void Component_BossBehavior::Initialize()
{
    // �K�v�ȃR���|�[�l���g��ǉ�
    if (!FindChildComponent("ShootAttack")) AddChildComponent(CreateComponent("ShootAttack", ShootAttack, holder_, this));
    if (!FindChildComponent("Timer")) AddChildComponent(CreateComponent("Timer", Timer, holder_, this));
    if (!FindChildComponent("TackleMove")) AddChildComponent(CreateComponent("TackleMove", TackleMove, holder_, this));
    if (!FindChildComponent("HealthGauge")) AddChildComponent(CreateComponent("HealthGauge", HealthGauge, holder_, this));

    // effekseer: :Effect�̓ǂݍ���
    EFFEKSEERLIB::gEfk->AddEffect("sword", "Effects/Salamander12.efk");/*������*/
	EFFEKSEERLIB::gEfk->AddEffect("fire", "Effects/Fire3.efk");/*������*/

    // �R���C�_�[�̒ǉ�
    // fix: �R���C�_�[�̃T�C�Y������f�[�^����ǂݍ��ނ悤�ɕύX
    holder_->AddCollider(new BoxCollider({}, { 5.0f, 5.0f, 5.0f }));
}

void Component_BossBehavior::Update()
{
    // �^�[�Q�b�g�̎擾
    if (target_ == nullptr) target_ = (StageObject*)holder_->FindObject(targetName_);

    // �J�E���g���䂳��Ă���ꍇ�̏���
    CountDown* countDown = (CountDown*)(holder_->FindObject("CountDown"));
    if (countDown != nullptr && isGameStart_ == false) {

        // �J�E���g�_�E�����I�������ꍇ
        if (countDown->IsFinished()) {

            //�ړ����\�ɂ���
            isActive_ = true;

            // �Q�[���X�^�[�g�t���O�𗧂Ă�
            isGameStart_ = true;
        }
        else {
            // �ړ���s�\�ɂ���
            isActive_ = false;
            return;
        }
    }

    // �Ώۂ����݂��Ȃ� �܂��� �A�N�e�B�u�łȂ��ꍇ�͏������s��Ȃ�
    if (target_ == nullptr || !isActive_) return;


    // HP�֘A����
    {
        // �{�X��HP�Q�[�W�R���|�[�l���g���擾
        Component_HealthGauge* hg = (Component_HealthGauge*)(GetChildComponent("HealthGauge"));

        // UIProgressBar���擾
        UIProgressBar* hpBar = (UIProgressBar*)UIPanel::GetInstance()->FindObject(PLAY_SCENE_BOSS_HP_GAUGE_NAME);

        // HP�o�[�̒l��ݒ�
        if (hpBar != nullptr && hg != nullptr)hpBar->SetProgress(&hg->now_, &hg->max_);

        // HP��0�ȉ��ɂȂ�����... DEAD��ԂɑJ��
        if (hg != nullptr)if (hg->IsDead() == true)SetState(BOSS_STATE_DEAD);
    }

    // ��Ԃɂ���ď����𕪊�
    switch (nowState_)
    {
    case BOSS_STATE_IDLE:Idle(); break;
    case BOSS_STATE_SHOT:Shot(); break;
    case BOSS_STATE_TACKLE:Tackle(); break;
    case BOSS_STATE_DEAD:Dead(); break;
    }

}

void Component_BossBehavior::Release()
{
}

void Component_BossBehavior::OnCollision(GameObject* _target, Collider* _collider)
{
    // �ːi�U���ɂ��_���[�W����
    {
        for (auto tackleMove : GetChildComponent(ComponentType::TackleMove)){

            // �ːi�U�����A�N�e�B�u�łȂ� �܂��� �Ώۂ����݂��Ȃ��ꍇ�͏������s��Ȃ�
            if (!((Component_TackleMove*)tackleMove)->IsActived() || _target == nullptr) continue;

            // �Ώۂ�HP������
            for (auto hg : ((StageObject*)_target)->FindComponent(HealthGauge))((Component_HealthGauge*)hg)->TakeDamage(30);
        }
    }
}

void Component_BossBehavior::Save(json& _saveObj)
{
    // �Ώۂ̖��O��ۑ�
    if (target_ != nullptr) _saveObj["target_"] = target_->GetObjectName();
    
    // ���ˊԘg��ۑ�
    _saveObj["shotInterval_"] = shotInterval_;
    
    // �ˌ��̍�����ۑ�
    _saveObj["shootHeight_"] = shootHeight_;
    
    // ��ԕύX�x����ۑ�
    _saveObj["stateChangeDelay_"] = stateChangeDelay_;
}

void Component_BossBehavior::Load(json& _loadObj)
{
    // �Ώۂ̖��O��ǂݍ���
    if (_loadObj.contains("target_")) targetName_ = _loadObj["target_"];
    
    // ���ˊԊu��ǂݍ���
    if (_loadObj.contains("shotInterval_")) shotInterval_ = _loadObj["shotInterval_"];

    // �ˌ��̍�����ǂݍ���
    if(_loadObj.contains("shootHeight_")) shootHeight_ = _loadObj["shootHeight_"];
    
    // ��ԕύX�x����ǂݍ���
    if (_loadObj.contains("stateChangeDelay_")) stateChangeDelay_ = _loadObj["stateChangeDelay_"];
}

void Component_BossBehavior::DrawData()
{
    // �A�N�e�B�u�t���O��\��
    ImGui::Checkbox("isActive_", &isActive_);
    // �Ώۂ�ݒ�
    {
        // �X�e�[�W��ɑ��݂���I�u�W�F�N�g�̖��O��S�Ď擾
        vector<string> nameList{};nameList.push_back("None");
        for (auto obj : ((Stage*)holder_->FindObject("Stage"))->GetStageObjects()) nameList.push_back(obj->GetObjectName());

        // �R���{�{�b�N�X�őI��
        static int select = 0;
        if (ImGui::BeginCombo("target_", nameList[select].c_str())) {
            for (int i = 0; i < nameList.size(); i++) {
                bool is_selected = (select == i);
                if (ImGui::Selectable(nameList[i].c_str(), is_selected)) select = i;
                if (is_selected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        // �I�����ꂽ���O����Ώۂ�ݒ�
        if (select != 0) target_ = (StageObject*)holder_->FindObject(nameList[select]);
    }

    // ���ˊԊu��\��
    ImGui::DragFloat("shotInterval_", &shotInterval_);
    
    // �ˌ��̍�����\��
    ImGui::DragFloat("shootHeight_", &shootHeight_);
    
    // ��ԕύX�x����\��
    ImGui::DragFloat("stateChangeDelay_", &stateChangeDelay_);
}

bool Component_BossBehavior::IsDead()
{
    // �{�X�̏�Ԃ�DEAD�ł���A�傫����0�ȉ��̏ꍇ
    return (nowState_ == BOSS_STATE_DEAD && holder_->GetScale().x <= 0);
}

BossState Component_BossBehavior::RandomStatePick()
{
    // ���I���ʂ��i�[����ϐ���p��
    BossState res = BOSS_STATE_MAX;

    // ��Ԃ𒊑I�@�������AIDLE��DEAD�͏��O
    do res = (BossState)(rand() % BOSS_STATE_MAX);
    while (res == BOSS_STATE_IDLE || res == BOSS_STATE_DEAD);

    // ���I���ʂ�Ԃ�
    return res;
}

void Component_BossBehavior::Shot()
{
    // �ˌ��U���R���|�[�l���g�̎擾 & null�`�F�b�N
    Component_ShootAttack* shoot = (Component_ShootAttack*)GetChildComponent("ShootAttack");

    // �^�C�}�[�̎擾 & null�`�F�b�N
    Component_Timer* timer = (Component_Timer*)GetChildComponent("Timer");
    if (timer == nullptr || shoot == nullptr) return;

    // �^�C�}�[�̐ݒ� & �J�n
    timer->SetTime(SHOT_TIME);
    timer->Start();

    // �p�x���i�[����ϐ���p��
    static float angle = 0;
    static float rotateSpeed = 1.f + (rand() % 15);

    // ���L�҂̊p�x���X�V
    angle += rotateSpeed;
    holder_->SetRotateY(angle);

    // �^�C�}�[����莞�Ԍo�߂����ꍇ
    if (timer->IsIntervalTime(shotInterval_)) {

        // ���˕�����ݒ�
        XMVECTOR dir = { 0,0,1,0 };
        shoot->SetShootingDirection(XMVector3TransformCoord(dir, XMMatrixRotationY(XMConvertToRadians(holder_->GetRotate().y))));

        // ���ˈʒu��ݒ�
        XMFLOAT3 shootPosition = holder_->GetPosition();
        shootPosition.y += shootHeight_;
        shoot->SetShootingPosition(shootPosition);

        // ����
        shoot->Execute();
    }

    // �^�C�}�[���I�������ꍇ
    if (timer->GetIsEnd()) {
        
        // �ҋ@��ԂɑJ��
        SetState(BOSS_STATE_IDLE);

        // �^�C�}�[�����Z�b�g
        timer->Reset();

        // �p�x�����Z�b�g
        angle = 0;

        // ��]���x�� 1.f����15.f�͈̔͂Ń����_���ɐݒ�
        rotateSpeed = 1.f + (rand() % 15);
    }
}

void Component_BossBehavior::Tackle()
{
    // ����t���O��p��
    static bool isFirst = true;

    // �ːi�s���R���|�[�l���g�̎擾 & null�`�F�b�N
    Component_TackleMove* tackleMove = (Component_TackleMove*)GetChildComponent("TackleMove");
    if (tackleMove == nullptr) return;

    // ����̂ݏ������s��
    if (isFirst) {
		// �ːi�U���̏���t���O��false�ɐݒ�
		isFirst = false;

        // �v���C���[�̕���������
		{
			// ���L�҂̈ʒu���擾
            XMFLOAT3 holderPos = holder_->GetPosition();

			// �Ώۂ̈ʒu���擾
			XMFLOAT3 targetPos = target_->GetPosition();

			// ���L�҂̈ʒu��Ώۂ̈ʒu�Ɍ�����
			holder_->SetRotateY(atan2f(targetPos.z - holderPos.z, targetPos.x - holderPos.x) * 180 / XM_PI);
		}
        
        // �ːi�U�������s
        {
			// ���̎擾
			// ���L�҂̈ʒu���擾
			XMFLOAT3 holderPos = holder_->GetPosition();

			// �Ώۂ̈ʒu���擾
			XMFLOAT3 targetPos = target_->GetPosition();

			// �ːi������ݒ� 
			tackleMove->SetDirection(XMVectorSetY(XMVector3Normalize(XMLoadFloat3(&targetPos) - XMLoadFloat3(&holderPos)), 0));

			// ������ݒ�
			tackleMove->SetDistance(XMVectorGetX(XMVector3Length(XMLoadFloat3(&targetPos) - XMLoadFloat3(&holderPos))));

			// �ːi�����s
			tackleMove->Execute();
        }

        // �G�t�F�N�g���Đ�
        {
            // ���̐ݒ�
            DirectX::XMStoreFloat4x4(&(t.matrix), holder_->GetWorldMatrix());
            t.isLoop = false;
            t.maxFrame = 60;
            t.speed = 1.0f;

            // �Đ�
            mt = EFFEKSEERLIB::gEfk->Play("sword", t);
        }
	}

    // �ːi�U�����I�������ꍇ
    if (tackleMove->IsActived() == false) { SetState(BOSS_STATE_IDLE); isFirst = true; }
}

void Component_BossBehavior::Idle()
{
    // �^�C�}�[�̎擾 & null�`�F�b�N
    Component_Timer* timer = (Component_Timer*)GetChildComponent("Timer");
    if (timer == nullptr) return;

    // �^�C�}�[�̐ݒ�
    {
        // ��Ԃ�J�ڂ���܂ł̎��Ԃ�ݒ�
        timer->SetTime(stateChangeDelay_);

        // �^�C�}�[���J�n
        timer->Start();
    }

    // �^�C�}�[���I�������ꍇ
    if (timer->GetIsEnd()) {

        // �����_���ɏ�Ԃ�J��
        SetState(RandomStatePick());

        // �^�C�}�[�����Z�b�g
        timer->Reset();
    }
}

void Component_BossBehavior::Dead()
{

    Transform effectTransform;
	effectTransform.position_ = holder_->GetPosition();

    // �傫�����O�łȂ��ꍇ
    if (holder_->GetScale().x >= 0) {
    // �L�����N�^�[�̑傫�������X�ɏ���������
    holder_->SetScale(holder_->GetScale().x - SMALL_VEROSITY);
    }
    else {
        // ���̐ݒ�
        DirectX::XMStoreFloat4x4(&(t.matrix), effectTransform.GetWorldMatrix());
        t.isLoop = false;
        t.maxFrame = 60;
        t.speed = 1.0f;

        // �Đ�
        mt = EFFEKSEERLIB::gEfk->Play("fire", t);
    }
}