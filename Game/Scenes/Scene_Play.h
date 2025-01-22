#pragma once

// インクルード
#include"../../Engine/GameObject/GameObject.h"
#include<chrono>

// 前方宣言
class Stage;
class CountDown;
class TPSCamera;

class Scene_Play:public GameObject
{
private:
	Stage* pStage_;			// ステージ
	CountDown* countDown_;	// カウントダウン
	TPSCamera* tpsCamera_;	// TPSカメラ

	std::chrono::system_clock::time_point start_;

	bool isGameStart_ :1;		// ゲーム開始フラグ
	bool fixedCursorPos :1;	// カーソル固定化
	bool cursorVisible :1;		// カーソル表示
	bool isBossSpawn_:1;
	bool isDebugDataEditWindowOpen_:1;
	bool isShowInventoryFirstTime_:1;

	enum PlaySceneState {
		PlaySceneState_None = 0,
		PlaySceneState_Play,
		PlaySceneState_Inventory,
		PlaySceneState_Max
	}currentState_;

public:
	/// <summary> コンストラクタ </summary>
	Scene_Play(GameObject* parent);

	/// <summary> 初期化 </summary>
	void Initialize() override;

	/// <summary> 更新 </summary>
	void Update() override;

	/// <summary> 描画 </summary>
	void Draw() override;

	/// <summary> 解放 </summary>
	void Release() override;

private:
	/// <summary> UIパネルの初期化 </summary>
	void InitUIPanel();

	/// <summary> ステージの初期化 </summary>
	void InitStage();

	/// <summary> カメラの初期化 </summary>
	void InitCamera();

	/// <summary> カーソルの状態を設定 </summary>
	void SetCursorMode();

	/// <summary> プレイ情報の表示設定 </summary>
	void SetPlayInfo();

	/// <summary> ボス敵の生成 </summary>
	void SpawnBossEnemy();

	void DrawDebugDataEditWindow();

	void SetState(PlaySceneState state) { currentState_ = state; };

};

