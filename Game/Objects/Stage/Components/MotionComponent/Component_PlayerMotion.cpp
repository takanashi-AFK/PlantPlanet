#include "../../../../../Engine/ResourceManager/Model.h"
#include "Component_PlayerMotion.h"
#include "../../Stage.h"
#include "../../../../../Engine/ImGui/imgui.h"

Component_PlayerMotion::Component_PlayerMotion(string _name, StageObject* _holder, Component* _parent)
	:Component_Motion(_name, _holder, PlayerMotion, _parent)
{
}

void Component_PlayerMotion::Initialize()
{
    // ���f���̃n���h�����X�g�Ƀ��f����ǉ�
    modelHandleList_[PLAYER_STATE_WALK] = MotionData("Models/Player/Running.fbx", 0, 40, 1,true);
    modelHandleList_[PLAYER_STATE_SHOOT] = MotionData("Models/Player/Fireball.fbx", 0, 150, 1,false);
    modelHandleList_[PLAYER_STATE_IDLE] = MotionData("Models/Player/Silly Dancing.fbx", 0, 200, 1,true);
    modelHandleList_[PLAYER_STATE_DODGE] = MotionData("Models/Player/Female Action Pose.fbx", 0, 60, 1,false);
    modelHandleList_[PLAYER_STATE_DEAD] = MotionData("Models/Player/Standing React Death Right.fbx", 0, 182, 1,false);
}

void Component_PlayerMotion::Update()
{
    // �v���C���[�̏�Ԃ��擾 FIX: playerBehavior�������������Ȃ��悤�ɂ���
    PlayerState state = PLAYER_STATE_MAX;
    for (auto playerBehavior : holder_->FindComponent(PlayerBehavior)) state = ((Component_PlayerBehavior*)playerBehavior)->GetState();

    // ���݂̃��f���ԍ����A���݂̏�Ԃ̃��f���ԍ��ƈ�v���Ă��Ȃ���.
    if (holder_->GetModelHandle() != modelHandleList_[state].modelHandle) {

        // ���f����ύX�ヂ�[�V�����̃��f���ɕύX
        holder_->SetModelHandle(modelHandleList_[state].modelHandle);

        // �Đ�
        Model::SetAnimFrame(
            modelHandleList_[state].modelHandle, 
            modelHandleList_[state].startFrame, 
            modelHandleList_[state].endFrame, 
            modelHandleList_[state].speed
        );
    }

	// �A�j���[�V�����̏I���t���[���ɒB������
    if (Model::GetAnimFrame(modelHandleList_[state].modelHandle) == modelHandleList_[state].endFrame) {

        // �Đ� �������� ��~
        Model::SetAnimFrame(
            modelHandleList_[state].modelHandle,
            modelHandleList_[state].isLoop ? modelHandleList_[state].startFrame : modelHandleList_[state].endFrame,
            modelHandleList_[state].endFrame,

            // ���[�v����ꍇ�̓A�j���[�V�������x��ݒ�
            // HACK:�A�j���[�V�����̑��x��0�ɐݒ肷�邱�ƂŒ�~�ł���
            modelHandleList_[state].isLoop ? modelHandleList_[state].speed : 0
        );
    }
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
    PlayerState state = PLAYER_STATE_MAX;
    for (auto playerBehavior : holder_->FindComponent(PlayerBehavior)) state = ((Component_PlayerBehavior*)playerBehavior)->GetState();

    return Model::GetAnimFrame(modelHandleList_[state].modelHandle);
}

bool Component_PlayerMotion::IsEnd()
{
    // �v���C���[�̏�Ԃ��擾 FIX: playerBehavior�������������Ȃ��悤�ɂ���
    PlayerState state = PLAYER_STATE_MAX;
    for (auto playerBehavior : holder_->FindComponent(PlayerBehavior)) state = ((Component_PlayerBehavior*)playerBehavior)->GetState();

    // �A�j���[�V�����̏I���t���[���ɒB������
    return (Model::GetAnimFrame(modelHandleList_[state].modelHandle) >= modelHandleList_[state].endFrame);
}

