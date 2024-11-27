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
    // モデルのハンドルリストにモデルを追加
    modelHandleList_[PLAYER_STATE_WALK] = MotionData(PLAYER_STATE_WALK_FBX, 0, 40, 1,true);
    modelHandleList_[PLAYER_STATE_SHOOT] = MotionData(PLAYER_STATE_SHOOT_FBX, 0, 150, 1,false);
    modelHandleList_[PLAYER_STATE_IDLE] = MotionData(PLAYER_STATE_IDLE_FBX, 0, 200, 1,true);
    modelHandleList_[PLAYER_STATE_DODGE] = MotionData(PLAYER_STATE_DODGE_FBX, 0, 60, 1,false);
    modelHandleList_[PLAYER_STATE_DEAD] = MotionData(PLAYER_STATE_DEAD_FBX, 0, 182, 1,false);
}

void Component_PlayerMotion::Update()
{
    // プレイヤーの状態を取得 FIX: playerBehaviorが複数持たせないようにする
    PlayerState state = PLAYER_STATE_MAX;
    Component_PlayerBehavior* p_playerBehavior = nullptr;

    for (auto playerBehavior : holder_->FindComponent(PlayerBehavior)) p_playerBehavior = (Component_PlayerBehavior*)(playerBehavior);
        
    state = (p_playerBehavior)->GetState();
    auto lockRotateTimeLeft = (p_playerBehavior)->GetLockRotateTime();
    //攻撃を撃った後なら複数のアニメーションに分岐する
    if (lockRotateTimeLeft)
    {
       // auto direction = ((Component_WASDInputMove*)(holder_->FindComponent("InputMove")))->GetDirectionType();

       // switch (direction)
    //    {
     //   case Component_WASDInputMove::DIRECTION::LEFT:
     //   }
    }

    // 現在のモデル番号が、現在の状態のモデル番号と一致していない時.
    if (holder_->GetModelHandle() != modelHandleList_[state].modelHandle) {

        // モデルを変更後モーションのモデルに変更
        holder_->SetModelHandle(modelHandleList_[state].modelHandle);

        // 再生
        Model::SetAnimFrame(
            modelHandleList_[state].modelHandle, 
            modelHandleList_[state].startFrame, 
            modelHandleList_[state].endFrame, 
            modelHandleList_[state].speed
        );
    }

	// アニメーションの終了フレームに達したら
    if (Model::GetAnimFrame(modelHandleList_[state].modelHandle) == modelHandleList_[state].endFrame) {

        // 再生 もしくは 停止
        Model::SetAnimFrame(
            modelHandleList_[state].modelHandle,
            modelHandleList_[state].isLoop ? modelHandleList_[state].startFrame : modelHandleList_[state].endFrame,
            modelHandleList_[state].endFrame,

            // ループする場合はアニメーション速度を設定
            // HACK:アニメーションの速度を0に設定することで停止できる
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
    // プレイヤーの状態を取得 FIX: playerBehaviorが複数持たせないようにする
    PlayerState state = PLAYER_STATE_MAX;
    for (auto playerBehavior : holder_->FindComponent(PlayerBehavior)) state = ((Component_PlayerBehavior*)playerBehavior)->GetState();

    return Model::GetAnimFrame(modelHandleList_[state].modelHandle);
}

bool Component_PlayerMotion::IsEnd()
{
    // プレイヤーの状態を取得 FIX: playerBehaviorが複数持たせないようにする
    PlayerState state = PLAYER_STATE_MAX;
    for (auto playerBehavior : holder_->FindComponent(PlayerBehavior)) state = ((Component_PlayerBehavior*)playerBehavior)->GetState();

    // アニメーションの終了フレームに達したら
    return (Model::GetAnimFrame(modelHandleList_[state].modelHandle) >= modelHandleList_[state].endFrame);
}

