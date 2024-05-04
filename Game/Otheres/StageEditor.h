#pragma once

// �C���N���[�h
#include "../../Engine/GameObject/GameObject.h"

// �O���錾
class Stage;

// �X�e�[�W�G�f�B�^�[���Ǘ�����N���X
class StageEditor : public GameObject
{
private:
	Stage* editTarget_;
public:
	StageEditor(GameObject* _parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void DrawWorldOutLiner();

	void SetEditTarget(Stage* _editTarget) { editTarget_ = _editTarget; }
	void AddStageObject();
	void SaveStage();
	void LoadStage();
};

