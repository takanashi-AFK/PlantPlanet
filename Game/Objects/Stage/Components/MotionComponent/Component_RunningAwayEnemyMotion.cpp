#include "Component_RunningAwayEnemyMotion.h"
#include"Component_Motion.h"
#include"../../../Engine/ResourceManager/Model.h"

Component_RunningAwayEnemyMotion::Component_RunningAwayEnemyMotion(string _name, StageObject* _holder, Component* _parent)
	:Component(_holder, _name, RunningawayEnemy, _parent)
{
}

void Component_RunningAwayEnemyMotion::Initialize()
{
	modelHandleList_[Component_RunningAwayEnemyBehavior::STATE::RUNNING]
		= MotionData("Models/Enemies/Jab Cross Running.fbx" , 0, endRunningFrame_, 1, true);
	modelHandleList_[Component_RunningAwayEnemyBehavior::STATE::DEATH]
		= MotionData("Models/Enemies/Jab Cross Running.fbx", 0, endRunningFrame_, 1, false);
}

void Component_RunningAwayEnemyMotion::Update()
{

    Component_RunningAwayEnemyBehavior* p_ruuningEnemyBehavior = nullptr;

    for (auto enemyBehavior : holder_->FindComponent(RunningawayEnemy)) p_ruuningEnemyBehavior = (Component_RunningAwayEnemyBehavior*)(enemyBehavior);

    Component_RunningAwayEnemyBehavior::STATE state = (p_ruuningEnemyBehavior)->GetState();

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

void Component_RunningAwayEnemyMotion::Release()
{
}

void Component_RunningAwayEnemyMotion::DrawData()
{
}

int Component_RunningAwayEnemyMotion::GetNowFrame()
{
    Component_RunningAwayEnemyBehavior::STATE state = Component_RunningAwayEnemyBehavior::STATE::AMOUNT;
    for (auto enemyBehavior : holder_->FindComponent(RunningawayEnemy))
        state = ((Component_RunningAwayEnemyBehavior*)enemyBehavior)->GetState();

    return Model::GetAnimFrame(modelHandleList_[state].modelHandle);
}

bool Component_RunningAwayEnemyMotion::IsEnd()
{
    Component_RunningAwayEnemyBehavior::STATE state = Component_RunningAwayEnemyBehavior::STATE::AMOUNT;
    for (auto enemyBehavior : holder_->FindComponent(RunningawayEnemy))
        state = ((Component_RunningAwayEnemyBehavior*)enemyBehavior)->GetState();

    return (Model::GetAnimFrame(modelHandleList_[state].modelHandle) >= modelHandleList_[state].endFrame);
}
