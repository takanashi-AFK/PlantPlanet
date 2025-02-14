#pragma once

// インクルードディレクティブ
#include"../../Engine/GameObject/GameObject.h"
#include<chrono>

// usingディレクティブ
using std::string;

// 前方宣言
class Stage;
class CountDown;
class TPSCamera;
class Component_PlayerBehavior;
class Component_BossBehavior;

/// <summary> ゲームプレイシーンクラス </summary>
class Scene_Play:public GameObject
{
private:
	Stage* pStage_;									// ステージ
	CountDown* countDown_;							// カウントダウン
	TPSCamera* tpsCamera_;							// TPSカメラ
	std::chrono::system_clock::time_point start_;	// ゲーム開始時間

	bool isGameStart_ :1;				// ゲーム開始フラグ
	bool fixedCursorPos_ :1;			// カーソル固定化
	bool cursorVisible_ :1;				// カーソル表示
	bool isShowInventoryFirstTime_:1;	// インベントリを初めて表示するかのフラグ
 	bool isOpenInventoryUI_ :1;			// インベントリUIを開いているかのフラグ

	bool isRocketSpawned_ : 1;			// ロケットが生成されたかのフラグ

	float fade_;

	static constexpr float FADESPEED = 0.1f;

	int notifyShowTimer = 0;
	bool hasShownNotification_;

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
	/// <summary> インベントリUIを非表示 </summary>
	void CloseInventoryUI();

	/// <summary> インベントリUIを表示 </summary>
	void OpenInventoryUI();

	/// <summary> カーソルの状態を設定 </summary>
	void SetCursorMode();

/*
initialize:*/
	/// <summary> アドベンチャーモードの初期化 </summary>
	void InitAdventureMode();
	
	/// <summary> スコアアタックモードの初期化 </summary>
	void InitScoreAttackMode();

	/// <summary> UIパネルの初期化 </summary>
	void InitUIPanel(const string& _fileName);

	/// <summary> ステージの初期化 </summary>
	void InitStage(const string& _fileName);

	/// <summary> カメラの初期化 </summary>
	void InitCamera();
/*
update :*/
	/// <summary> スコアアタックモードの更新 </summary>
	void UpdateScoreAttackMode(Component_PlayerBehavior* _playerBehavior,Component_BossBehavior* _bossBehavior);

	/// <summary> アドベンチャーモードの更新 </summary>
	void UpdateAdventureMode(Component_PlayerBehavior* _playerBehavior, Component_BossBehavior* _bossBehavior);

	/// <summary> インベントリを表示するUIの更新 </summary>
	void UpdateInventoryUI();

	/// <summary> プレイヤーの状態を表示するUIの更新 </summary>
	void UpdateNormalUI(Component_PlayerBehavior* _playerBehavior, Component_BossBehavior* _bossBehavior);
/*
debug :*/
	/// <summary> デバッグデータ編集ウィンドウの描画 ※削除予定 </summary>
	void DrawDebugDataEditWindow();
};

