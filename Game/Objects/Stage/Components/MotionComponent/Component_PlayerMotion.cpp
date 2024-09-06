#include "../../../../../Engine/ResourceManager/Model.h"
#include "Component_PlayerMotion.h"
#include "../../Stage.h"
#include "../../../../../Engine/ImGui/imgui.h"

Component_PlayerMotion::Component_PlayerMotion(string _name, StageObject* _holder, Component* _parent)
	:Component_Motion(_name, _holder, PlayerMotion, _parent), currentFrame_(0), animationEndFrame_(0), isAnimationEnd_(false)
{
}

void Component_PlayerMotion::Initialize()
{
    // ���f���̃n���h�����X�g�Ƀ��f����ǉ�
    modelHandleList_[PSTATE_WALK] = MotionData("Models/Player/Running.fbx", 0, 40, 1);
    modelHandleList_[PSTATE_SHOOT] = MotionData("Models/Player/Fireball.fbx", 0, 150, 1);
    modelHandleList_[PSTATE_IDLE] = MotionData("Models/Player/Silly Dancing.fbx", 0, 200, 1);
    modelHandleList_[PSTATE_DODGE] = MotionData("Models/Player/Female Action Pose.fbx", 0, 60, 1);
    modelHandleList_[PSATE_DEAD] = MotionData("Models/Player/Standing React Death Right.fbx", 0, 106, 1);
}

void Component_PlayerMotion::Update()
{
    // �v���C���[�̏�Ԃ��擾 FIX: playerBehavior�������������Ȃ��悤�ɂ���
    PlayerState state = PSTATE_MAX;
    for (auto playerBehavior : holder_->FindComponent(PlayerBehavior)) state = ((Component_PlayerBehavior*)playerBehavior)->GetState();

    switch (state)
    {
    case PSTATE_IDLE: IdleMotion(); break;
    case PSTATE_WALK: WalkMotion(); break;
    case PSTATE_SHOOT: ShootMotion(); break;
    case PSTATE_DODGE: DodgeMotion(); break;
    case PSATE_DEAD: DeadMotion(); break;
    }

    // ���݂̃t���[�����C���N�������g
    currentFrame_++;

	// �A�j���[�V�����̏I���t���[���ɒB������t���[�������Z�b�g
    if (currentFrame_ == animationEndFrame_) {
        currentFrame_ = 0;
        isAnimationEnd_ = true;
    }

    ImGui::Text("nowModelHandle : %d", ((StageObject*)holder_)->GetModelHandle());
    ImGui::Text("currentFrame_ : %d", currentFrame_);
}

void Component_PlayerMotion::Release()
{
}

void Component_PlayerMotion::DrawData()
{
}

int Component_PlayerMotion::GetNowFrame()
{
    // �v���C���[�̏�Ԃ��擾 FIX: playerBehavior�������������Ȃ��悤�ɂ���
    PlayerState state = PSTATE_MAX;
    for (auto playerBehavior : holder_->FindComponent(PlayerBehavior)) state = ((Component_PlayerBehavior*)playerBehavior)->GetState();

    return Model::GetAnimFrame(modelHandleList_[state].modelHandle);
}

void Component_PlayerMotion::IdleMotion()
{
    // �I���t���[����ݒ�
    animationEndFrame_ = 200;

    // ���݂̃��f���ԍ����ҋ@���[�V�����̃��f���ԍ��ƈႤ�ꍇ...
    if (holder_->GetModelHandle() != modelHandleList_[PSTATE_IDLE].modelHandle) {

        // ���f����ҋ@���[�V�����̃��f���ɕύX
        holder_->SetModelHandle(modelHandleList_[PSTATE_IDLE].modelHandle);
        
        // ���f�����ς�����Ƃ��Ƀt���[�������Z�b�g
        currentFrame_ = 0;

        // �A�j���[�V�����I���t���O������
        isAnimationEnd_ = false;
    }

    // ���݂̃t���[�����J�n�t���[���Ɠ����ꍇ
    if (currentFrame_ == 0) {

        // �A�j���[�V�������Đ�
        holder_->PlayAnimation(0, animationEndFrame_, 1);
    }
}

void Component_PlayerMotion::WalkMotion()
{
    // �I���t���[����ݒ�
    animationEndFrame_ = 40;

    // ���݂̃��f���ԍ������s���[�V�����̃��f���ԍ��ƈႤ�ꍇ...
    if (holder_->GetModelHandle() != modelHandleList_[PSTATE_WALK].modelHandle) {

        // ���f������s���[�V�����̃��f���ɕύX
        holder_->SetModelHandle(modelHandleList_[PSTATE_WALK].modelHandle);

        // ���f�����ς�����Ƃ��Ƀt���[�������Z�b�g
        currentFrame_ = 0;

        // �A�j���[�V�����I���t���O������
        isAnimationEnd_ = false;

    }

    // ���݂̃t���[�����P�t���[���O�̃t���[���Ɠ����ꍇ
    if (currentFrame_ == previousFrame_) {

        // �A�j���[�V�������Đ�
        holder_->PlayAnimation(previousFrame_, animationEndFrame_, 1);
    }

    // �P�t���[���O�̃t���[���Ɍ��݂̃t���[������
    previousFrame_ = currentFrame_;

    // �r���ŕ�State�Ɉڍs�����ꍇ�A
    // Model�N���X�ɑ��݂���GetAnimFrame�֐����g���ăA�j���[�V�����̃t���[�����擾���A
}

void Component_PlayerMotion::ShootMotion()
{
    // �I���t���[����ݒ�
    animationEndFrame_ = 150;

    // ���݂̃��f���ԍ����ˌ����[�V�����̃��f���ԍ��ƈႤ�ꍇ...
    if (holder_->GetModelHandle() != modelHandleList_[PSTATE_SHOOT].modelHandle) {

        // ���f�����ˌ����[�V�����̃��f���ɕύX
        holder_->SetModelHandle(modelHandleList_[PSTATE_SHOOT].modelHandle);

        // ���f�����ς�����Ƃ��Ƀt���[�������Z�b�g
        currentFrame_ = 0;

        // �A�j���[�V�����I���t���O������
        isAnimationEnd_ = false;
    }

    // ���݂̃t���[�����J�n�t���[���Ɠ����ꍇ
    if (currentFrame_ == 0) {

        // �A�j���[�V�������Đ�
        holder_->PlayAnimation(0, animationEndFrame_, 1);
    }
}

void Component_PlayerMotion::DodgeMotion()
{
    // �I���t���[����ݒ�
    animationEndFrame_ = 400;

    // ���݂̃��f���ԍ���������[�V�����̃��f���ԍ��ƈႤ�ꍇ...
    if (holder_->GetModelHandle() != modelHandleList_[PSTATE_DODGE].modelHandle) {

        // ���f����������[�V�����̃��f���ɕύX
        holder_->SetModelHandle(modelHandleList_[PSTATE_DODGE].modelHandle);
        
        // ���f�����ς�����Ƃ��Ƀt���[�������Z�b�g
        currentFrame_ = 0;

        // �A�j���[�V�����I���t���O������
        isAnimationEnd_ = false;
    }

    // ���݂̃t���[�����J�n�t���[���Ɠ����ꍇ
    if (currentFrame_ == 0) {

        // �A�j���[�V�������Đ�
        holder_->PlayAnimation(0, animationEndFrame_, 1);
    }
}

void Component_PlayerMotion::DeadMotion()
{
    // �I���t���[����ݒ�
    animationEndFrame_ = 60;

    // ���݂̃��f���ԍ���������[�V�����̃��f���ԍ��ƈႤ�ꍇ...
    if (holder_->GetModelHandle() != modelHandleList_[PSATE_DEAD].modelHandle) {

        // ���f����������[�V�����̃��f���ɕύX
        holder_->SetModelHandle(modelHandleList_[PSATE_DEAD].modelHandle);

        // ���f�����ς�����Ƃ��Ƀt���[�������Z�b�g
        currentFrame_ = 0;

        // �A�j���[�V�����I���t���O������
        isAnimationEnd_ = false;
    }

    // ���݂̃t���[�����J�n�t���[���Ɠ����ꍇ
    if (currentFrame_ == 0) {

        // �A�j���[�V�������Đ�
        holder_->PlayAnimation(0, animationEndFrame_, 1);
    }
}
