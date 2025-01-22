#pragma once

// �C���N���[�h
#include"../../Engine/GameObject/GameObject.h"
#include<chrono>

// �O���錾
class Stage;
class CountDown;
class TPSCamera;

class Scene_Play:public GameObject
{
private:
	Stage* pStage_;			// �X�e�[�W
	CountDown* countDown_;	// �J�E���g�_�E��
	TPSCamera* tpsCamera_;	// TPS�J����

	std::chrono::system_clock::time_point start_;

	bool isGameStart_ :1;		// �Q�[���J�n�t���O
	bool fixedCursorPos :1;	// �J�[�\���Œ艻
	bool cursorVisible :1;		// �J�[�\���\��
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
	/// <summary> �R���X�g���N�^ </summary>
	Scene_Play(GameObject* parent);

	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> �`�� </summary>
	void Draw() override;

	/// <summary> ��� </summary>
	void Release() override;

private:
	/// <summary> UI�p�l���̏����� </summary>
	void InitUIPanel();

	/// <summary> �X�e�[�W�̏����� </summary>
	void InitStage();

	/// <summary> �J�����̏����� </summary>
	void InitCamera();

	/// <summary> �J�[�\���̏�Ԃ�ݒ� </summary>
	void SetCursorMode();

	/// <summary> �v���C���̕\���ݒ� </summary>
	void SetPlayInfo();

	/// <summary> �{�X�G�̐��� </summary>
	void SpawnBossEnemy();

	void DrawDebugDataEditWindow();

	void SetState(PlaySceneState state) { currentState_ = state; };

};

