#pragma once
#include"../../Engine/GameObject/GameObject.h"

class Scene_Result :public GameObject
{
private:
	int scoreNum_;	// スコア
	int layerNumberCount_;
public:
	/// <summary> コンストラクタ </summary>
	Scene_Result(GameObject* parent_);

	/// <summary> 初期化 </summary>
	void Initialize() override;

	/// <summary> 更新 </summary>
	void Update() override;

	/// <summary> 描画 </summary>
	void Draw() override;

	/// <summary> 解放 </summary>
	void Release() override;
	
private:
	/// <summary> スコアの計算 </summary>
	int CalculateScore(bool isCleared, int remainingTime, int remainingHP);
};

	