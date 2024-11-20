#pragma once

// �C���N���[�h
#include "../../Engine/GameObject/GameObject.h"
#include <string>
using std::string;

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
	Stage* editStage_;					// �ҏW�Ώۂ̃X�e�[�W
	int selectEditStageObjectIndex_;	// �I�𒆂̃X�e�[�W�I�u�W�F�N�g�̃C���f�b�N�X
	UIPanel* editUIPanel_;				// �ҏW�p�l��
	int selectEditUIObjectIndex_;		// �I�𒆂�UI�I�u�W�F�N�g�̃C���f�b�N�X
	EditType editType_;					// �ҏW�Ώۂ̎��
	bool isShowCreateUIObjectWindow_;	// UI�I�u�W�F�N�g�쐬�E�B���h�E��\�����邩
	int layerNumberCount_;				// ���C���[�ԍ��̃J�E���g
	CameraType cameraType_;				// �J�����̎��
	TPSCamera* tpsCamera_;				// TPS�J����
	XMFLOAT3 cameraPosition_;			// �J�����̈ʒu
	XMFLOAT3 cameraTarget_;				// �J�����̉�]
	XMFLOAT3 threeDCamMove_{};			// 3D�J�����̈ړ���
	bool isShowPlantWindow_;			// �A���E�B���h�E��\�����邩
	int selectEditPlantIndex_;			// �I�𒆂̐A���̃C���f�b�N�X

public:
	/// <summary> �R���X�g���N�^ </summary>
	GameEditor(GameObject* _parent);

	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> �`�� </summary>
	void Draw() override;

	/// <summary> ��� </summary>
	void Release() override;

/*
stage :*/

	/// <summary> �ҏW����X�e�[�W��ݒ� </summary>
	void SetEditStage(Stage* _editStage) { editStage_ = _editStage; }
	
	/// <summary> �X�e�[�W�I�u�W�F�N�g��ǉ� </summary>
	void AddStageObject();

	/// <summary> �X�e�[�W��ۑ� </summary>
	void SaveStage();

	/// <summary> �X�e�[�W��Ǎ� </summary>
	void LoadStage();

/*
uiPanel :*/
	/// <summary> �ҏW����UI�p�l����ݒ� </summary>
	void SetEditUIPanel(UIPanel* _editUIPanel) { editUIPanel_ = _editUIPanel; }
	
	/// <summary> UI�I�u�W�F�N�g��ǉ� </summary>
	void AddUIObject();

	/// <summary> UI�p�l����ۑ� </summary>
	void SaveUIPanel();

	/// <summary> UI�p�l����Ǎ� </summary>
	void LoadUIPanel();
	
	/// <summary> UI�I�u�W�F�N�g�쐬�E�B���h�E��`�� </summary>
	void UIObjectCreateWindow();
/*
plants :*/
	/// <summary> �A����ۑ� </summary>
	void SavePlant();

	/// <summary> �A����Ǎ� </summary>
	void LoadPlant();

	/// <summary> �A�����ǉ��E�B���h�E��`�� </summary>
	void DrawAddPlantWindow();
/*
camera :*/
	/// <summary> �g�p����3�l�̃J������ݒ� </summary>
	void SetTPSCamera(TPSCamera* _tpsCamera) { tpsCamera_ = _tpsCamera; }

private:
	/// <summary> ���[���h�A�E�g���C�i�[��`�� </summary>
	void DrawWorldOutLiner();

	/// <summary> �ڍ׏���`�� </summary>
	void DrawDatails();
/*
outLiner :*/
	/// <summary> �X�e�[�W�p�A�E�g���C�i�[��`�� </summary>
	void DrawStageOutLiner();

	/// <summary> UI�p�l���p�A�E�g���C�i�[��`�� </summary>
	void DrawUIPanelOutLiner();

	/// <summary> �A���p�A�E�g���C�i�[��`�� </summary>
	void DrawPlantOutLiner();

/*
datails :*/
	/// <summary> �X�e�[�W�I�u�W�F�N�g�̏ڍ׏���`�� </summary>
	void DrawStageObjectDatails();
	
	/// <summary> UI�I�u�W�F�N�g�̏ڍ׏���`�� </summary>
	void DrawUIObjectDatails();

	/// <summary> �A���f�[�^�̏ڍ׏���`�� </summary>
	void DrawPlantDatails();

	/// <summary> �J�����̏ڍ׏���`�� </summary>
	void DrawDatalsCamera();

/*
otheres :*/
	string GetFBXFilePath();
	string GetPNGFilePath();
};

