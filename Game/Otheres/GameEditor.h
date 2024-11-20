#pragma once

// インクルード
#include "../../Engine/GameObject/GameObject.h"

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
	GENERATOR
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
	Stage* editStage_;				// 編集対象のステージ
	int selectEditStageObjectIndex_;		// 選択中のステージオブジェクトのインデックス

	UIPanel* editUIPanel_;			// 編集パネル
	int selectEditUIObjectIndex_;		// 選択中のUIオブジェクトのインデックス
	
	EditType editType_;				// 編集対象の種類

	bool isShowCreateUIObjectWindow_;	// UIオブジェクト作成ウィンドウを表示するか
	int layerNumberCount_;			// レイヤー番号のカウント

	CameraType cameraType_;			// カメラの種類

	TPSCamera* tpsCamera_;			// TPSカメラ

	XMFLOAT3 cameraPosition_;			// カメラの位置
	XMFLOAT3 cameraTarget_;				// カメラの回転

	XMFLOAT3 threeDCamMove_{};

	bool isShowPlantWindow_;
	int selectEditPlantIndex_;

	bool isShowGeneratorWindow_;
	int selectEditGeneratorIndex_;

public:
	GameEditor(GameObject* _parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetEditStage(Stage* _editStage) { editStage_ = _editStage; }
	void AddStageObject();
	void SaveStage();
	void LoadStage();

	void SetEditUIPanel(UIPanel* _editUIPanel) { editUIPanel_ = _editUIPanel; }
	void AddUIObject();
	void SaveUIPanel();
	void LoadUIPanel();

	void SetTPSCamera(TPSCamera* _tpsCamera) { tpsCamera_ = _tpsCamera; }
private:
	void DrawWorldOutLiner();
	void DrawStageOutLiner();
	void DrawUIPanelOutLiner();
	void DrawPlantOutLiner();

	void DrawDatails();
	void DrawStageObjectDatails();
	void DrawUIObjectDatails();
	void DrawPlantDatails();
	void DrawGeneratorDetails();

	void DrawDatalsCamera();

	void UIObjectCreateWindow();
	void CreatePlantWindow();

	void AddPlant();

	void SavePlant();
	void LoadPlant();

	void ShowGenerator();

	void SaveGenerator();
	void LoadGenerator();
};

