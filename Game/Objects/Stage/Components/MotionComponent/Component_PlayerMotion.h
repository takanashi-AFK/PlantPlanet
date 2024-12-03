#pragma once
#include "Component_Motion.h"
#include <vector>
#include "../../Components/BehaviorComponents/Component_PlayerBehavior.h"

class Component_PlayerMotion : public Component_Motion
{
public:
	/// <summary> コンストラクタ </summary>
	Component_PlayerMotion(string _name, StageObject* _holder, Component* _parent);
	
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
	static const int shotWalkTime_ = 90;
};

