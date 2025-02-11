#pragma once

// インクルード
#include "../../Engine/GameObject/GameObject.h"

class Scene_Splash : public GameObject
{
private:
	float count_;		// 経過時間

public:
	/// <summary> コンストラクタ </summary>
	Scene_Splash(GameObject* _parent);

	/// <summary> 初期化 </summary>
	void Initialize() override;

	/// <summary> 更新 </summary>
	void Update() override;

	/// <summary> 描画 </summary>
	void Draw() override;

	/// <summary> 解放 </summary>
	void Release() override;

	/*
	getter :*/
	/// <summary> 経過時間の取得 </summary>
	float GetCount() const { return count_; }

	/*
	predicate :*/
	/// <summary> シーン切替可能か </summary>
	bool IsChangeScene() const;

	/// <summary> 指定時間経過したか(秒) </summary>
	bool IsCountNow(float _sec) const;
};
