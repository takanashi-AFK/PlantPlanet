#pragma once

// インクルード
#include "../../Engine/GameObject/GameObject.h"
#include <string>
using std::string;

// 前方宣言
class Stage;
class StageObject;

class UIPanel;
class UIObject;

class TPSCamera;
class Plant;

enum EditType
{
	NONE,
	STAGE,
	UIPANEL,
	CAMERA,
	PLANT,
};

enum CameraType
{
	DEFAULT,
	TPS,
};

// ステージエディターを管理するクラス
class GameEditor : public GameObject
{
private:
	Stage* editStage_;					// 編集対象のステージ
	int selectEditStageObjectIndex_;	// 選択中のステージオブジェクトのインデックス
	UIPanel* editUIPanel_;				// 編集パネル
	int selectEditUIObjectIndex_;		// 選択中のUIオブジェクトのインデックス
	EditType editType_;					// 編集対象の種類
	bool isShowCreateUIObjectWindow_;	// UIオブジェクト作成ウィンドウを表示するか
	int layerNumberCount_;				// レイヤー番号のカウント
	CameraType cameraType_;				// カメラの種類
	TPSCamera* tpsCamera_;				// TPSカメラ
	XMFLOAT3 cameraPosition_;			// カメラの位置
	XMFLOAT3 cameraTarget_;				// カメラの回転
	XMFLOAT3 threeDCamMove_{};			// 3Dカメラの移動量
	bool isShowPlantWindow_;			// 植物ウィンドウを表示するか
	int selectEditPlantIndex_;			// 選択中の植物のインデックス

public:
	/// <summary> コンストラクタ </summary>
	GameEditor(GameObject* _parent);

	/// <summary> 初期化 </summary>
	void Initialize() override;

	/// <summary> 更新 </summary>
	void Update() override;

	/// <summary> 描画 </summary>
	void Draw() override;

	/// <summary> 解放 </summary>
	void Release() override;

/*
stage :*/

	/// <summary> 編集するステージを設定 </summary>
	void SetEditStage(Stage* _editStage) { editStage_ = _editStage; }
	
	/// <summary> ステージオブジェクトを追加 </summary>
	void AddStageObject();

	/// <summary> ステージを保存 </summary>
	void SaveStage();

	/// <summary> ステージを読込 </summary>
	void LoadStage();

/*
uiPanel :*/
	/// <summary> 編集するUIパネルを設定 </summary>
	void SetEditUIPanel(UIPanel* _editUIPanel) { editUIPanel_ = _editUIPanel; }
	
	/// <summary> UIオブジェクトを追加 </summary>
	void AddUIObject();

	/// <summary> UIパネルを保存 </summary>
	void SaveUIPanel();

	/// <summary> UIパネルを読込 </summary>
	void LoadUIPanel();
	
	/// <summary> UIオブジェクト作成ウィンドウを描画 </summary>
	void UIObjectCreateWindow();
/*
plants :*/
	/// <summary> 植物を保存 </summary>
	void SavePlant();

	/// <summary> 植物を読込 </summary>
	void LoadPlant();

	/// <summary> 植物情報追加ウィンドウを描画 </summary>
	void DrawAddPlantWindow();
/*
camera :*/
	/// <summary> 使用する3人称カメラを設定 </summary>
	void SetTPSCamera(TPSCamera* _tpsCamera) { tpsCamera_ = _tpsCamera; }

private:
	/// <summary> ワールドアウトライナーを描画 </summary>
	void DrawWorldOutLiner();

	/// <summary> 詳細情報を描画 </summary>
	void DrawDatails();
/*
outLiner :*/
	/// <summary> ステージ用アウトライナーを描画 </summary>
	void DrawStageOutLiner();

	/// <summary> UIパネル用アウトライナーを描画 </summary>
	void DrawUIPanelOutLiner();

	/// <summary> 植物用アウトライナーを描画 </summary>
	void DrawPlantOutLiner();

/*
datails :*/
	/// <summary> ステージオブジェクトの詳細情報を描画 </summary>
	void DrawStageObjectDatails();
	
	/// <summary> UIオブジェクトの詳細情報を描画 </summary>
	void DrawUIObjectDatails();

	/// <summary> 植物データの詳細情報を描画 </summary>
	void DrawPlantDatails();

	/// <summary> カメラの詳細情報を描画 </summary>
	void DrawDatalsCamera();

/*
otheres :*/
	string GetFBXFilePath();
	string GetPNGFilePath();
};

