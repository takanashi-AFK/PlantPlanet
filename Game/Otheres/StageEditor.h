#pragma once

// �C���N���[�h
#include "../../Engine/GameObject/GameObject.h"

// �O���錾
class Stage;
class StageObject;

// �X�e�[�W�G�f�B�^�[���Ǘ�����N���X
class StageEditor : public GameObject
{
private:
	Stage* editStage_;				// �ҏW�Ώۂ̃X�e�[�W
	int selectEditObjectIndex_;		// �I�𒆂̃I�u�W�F�N�g�̃C���f�b�N�X

public:
	StageEditor(GameObject* _parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetEditStage(Stage* _editStage) { editStage_ = _editStage; }
	void AddStageObject();
	void SaveStage();
	void LoadStage();

private:
	void DrawWorldOutLiner();
	void DrawDatails();
};

