#pragma once

// �C���N���[�h
#include "../../Engine/GameObject/GameObject.h"

// �O���錾
class Stage;
class StageObject;

class UIPanel;
class UIObject;

enum EditType
{
	STAGE,
	UIPANEL,
	NONE
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

private:
	void DrawWorldOutLiner();
	void DrawStageOutLiner();
	void DrawUIPanelOutLiner();

	void DrawDatails();
	void DrawStageObjectDatails();
	void DrawUIObjectDatails();
};

