#pragma once
#include <vector>
#include "../../Components/BehaviorComponents/Component_RunningAwayEnemyBehavior.h"
#include <map>
#include"Component_Motion.h"
struct MotioData;

class Component_RunningAwayEnemyMotion :public Component
{
public:
	/// <summary> コンストラクタ </summary>
	Component_RunningAwayEnemyMotion(string _name, StageObject* _holder, Component* _parent);

	/// <summary> 初期化 </summary>
	void Initialize() override;

	/// <summary> 更新 </summary>
	void Update() override;

	/// <summary> 解放 </summary>
	void Release() override;

	/// <summary> ImGuiパネル表示 </summary>
	void DrawData() override;
	/*
	getter :*/
	/// <summary> 現在のフレームを取得 </summary>
	int GetNowFrame();

	/*
	predicate :*/
	/// <summary> アニメーションが終了したか </summary>
	bool IsEnd();

	//撃った後のアニメーション時間
	static constexpr int endRunningFrame_ = 60;
	static constexpr int endDyingFrame = 20;
private:

	std::map<Component_RunningAwayEnemyBehavior::STATE, MotionData>modelHandleList_;
};