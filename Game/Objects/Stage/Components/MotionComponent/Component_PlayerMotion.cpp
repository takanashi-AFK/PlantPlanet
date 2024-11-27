#include "../../../../../Engine/ResourceManager/Model.h"
#include "Component_PlayerMotion.h"
#include "../../Stage.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../../../Constants.h"
#include"../MoveComponents/Component_WASDInputMove.h"

using namespace Constants;

Component_PlayerMotion::Component_PlayerMotion(string _name, StageObject* _holder, Component* _parent)
	:Component_Motion(_name, _holder, PlayerMotion, _parent)
{
}

void Component_PlayerMotion::Initialize()
{
    // ���f���̃n���h�����X�g�Ƀ��f����ǉ�
    modelHandleList_[PLAYER_STATE_WALK] = MotionData(PLAYER_STATE_WALK_FBX, 0, 40, 1,true);
    modelHandleList_[PLAYER_STATE_SHOOT] = MotionData(PLAYER_STATE_SHOOT_FBX, 0, 150, 1,false);
    modelHandleList_[PLAYER_STATE_IDLE] = MotionData(PLAYER_STATE_IDLE_FBX, 0, 200, 1,true);
    modelHandleList_[PLAYER_STATE_DODGE] = MotionData(PLAYER_STATE_DODGE_FBX, 0, 60, 1,false);
    modelHandleList_[PLAYER_STATE_DEAD] = MotionData(PLAYER_STATE_DEAD_FBX, 0, 182, 1,false);
}

void Component_PlayerMotion::Update()
{
    // �v���C���[�̏�Ԃ��擾 FIX: playerBehavior�������������Ȃ��悤�ɂ���
    PlayerState state = PLAYER_STATE_MAX;
    Component_PlayerBehavior* p_playerBehavior = nullptr;

    for (auto playerBehavior : holder_->FindComponent(PlayerBehavior)) p_playerBehavior = (Component_PlayerBehavior*)(playerBehavior);
        
    state = (p_playerBehavior)->GetState();
    auto lockRotateTimeLeft = (p_playerBehavior)->GetLockRotateTime();
    //�U������������Ȃ畡���̃A�j���[�V�����ɕ��򂷂�
    if (lockRotateTimeLeft)
    {
       // auto direction = ((Component_WASDInputMove*)(holder_->FindComponent("InputMove")))->GetDirectionType();

       // switch (direction)
    //    {
     //   case Component_WASDInputMove::DIRECTION::LEFT:
     //   }
    }

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

