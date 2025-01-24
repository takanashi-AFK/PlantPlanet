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

	void InitializeScoreAttackResult();
	void InitializeAdventureResult();
	void UpdateScoreAttackResult();
	void UpdateAdventureResult();

//スコアアタック用--------------
	//スコアアタックの演出がスキップされたか確認
	void CheckSkipScoreAttackResult();

	//テキスト群のイージング
	void UpdateTexts();

	//スコア数値のイージング
	void UpdateValues();

	//トータルスコアのイージング
	void UpdateTotalScore();

	//ボタンのイージング
	void UpdateButton();

	//最終的なupdate状態。ボタンを押されるのを待っている。
	void UpdateWaitingForReturn();

	void UpdateEmptyWork();
	//最終的な数値。正規の情報
	void ApplyFinalData();
private:

	void (Scene_Result::*scoreAttackUpdateFunction)();
	float easingValue_;
	int totalScore_;

//-----------------------------------------
};

	