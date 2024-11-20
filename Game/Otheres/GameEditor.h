#pragma once

// �C���N���[�h
#include "../../Engine/GameObject/GameObject.h"

// �O���錾
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

// �X�e�[�W�G�f�B�^�[���Ǘ�����N���X
class GameEditor : public GameObject
{
private:
	Stage* editStage_;				// �ҏW�Ώۂ̃X�e�[�W
	int selectEditStageObjectIndex_;		// �I�𒆂̃X�e�[�W�I�u�W�F�N�g�̃C���f�b�N�X

	UIPanel* editUIPanel_;			// �ҏW�p�l��
	int selectEditUIObjectIndex_;		// �I�𒆂�UI�I�u�W�F�N�g�̃C���f�b�N�X
	
	EditType editType_;				// �ҏW�Ώۂ̎��

	bool isShowCreateUIObjectWindow_;	// UI�I�u�W�F�N�g�쐬�E�B���h�E��\�����邩
	int layerNumberCount_;			// ���C���[�ԍ��̃J�E���g

	CameraType cameraType_;			// �J�����̎��

	TPSCamera* tpsCamera_;			// TPS�J����

	XMFLOAT3 cameraPosition_;			// �J�����̈ʒu
	XMFLOAT3 cameraTarget_;				// �J�����̉�]

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

