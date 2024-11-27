#pragma once

// インクルード
#include"../../Engine/GameObject/GameObject.h"

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

	bool isGameStart_;		// ゲーム開始フラグ
	bool fixedCursorPos;	// カーソル固定化
	bool cursorVisible;		// カーソル表示
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
};

