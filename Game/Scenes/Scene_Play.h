#pragma once

// �C���N���[�h�f�B���N�e�B�u
#include"../../Engine/GameObject/GameObject.h"

// using�f�B���N�e�B�u
using std::string;

// �O���錾
class Stage;
class CountDown;
class TPSCamera;
class Component_PlayerBehavior;
class Component_BossBehavior;

/// <summary> �Q�[���v���C�V�[���N���X </summary>
class Scene_Play:public GameObject
{
private:
	Stage* pStage_;					// �X�e�[�W
	TPSCamera* tpsCamera_;			// TPS�J����

	bool fixedCursorPos_;			// �J�[�\���Œ艻
	bool cursorVisible_;				// �J�[�\���\��
	bool isShowInventoryFirstTime_;	// �C���x���g��UI�����߂ĕ\���������̃t���O
	bool isOpenInventoryUI_;		// �C���x���g��UI���J���Ă��邩�̃t���O

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
	/// <summary> �C���x���g��UI���\�� </summary>
	void CloseInventoryUI();

	/// <summary> �C���x���g��UI��\�� </summary>
	void OpenInventoryUI();

	/// <summary> �J�[�\���̏�Ԃ�ݒ� </summary>
	void SetCursorMode();

/*
initialize:*/
	/// <summary> �A�h�x���`���[���[�h�̏����� </summary>
	void InitAdventureMode();
	
	/// <summary> �X�R�A�A�^�b�N���[�h�̏����� </summary>
	void InitScoreAttackMode();

	/// <summary> UI�p�l���̏����� </summary>
	void InitUIPanel(const string& _fileName);

	/// <summary> �X�e�[�W�̏����� </summary>
	void InitStage(const string& _fileName);

	/// <summary> �J�����̏����� </summary>
	void InitCamera();
/*
update :*/
	/// <summary> �X�R�A�A�^�b�N���[�h�̍X�V </summary>
	void UpdateScoreAttackMode(Component_PlayerBehavior* _playerBehavior,Component_BossBehavior* _bossBehavior);

	/// <summary> �A�h�x���`���[���[�h�̍X�V </summary>
	void UpdateAdventureMode(Component_PlayerBehavior* _playerBehavior, Component_BossBehavior* _bossBehavior);

	/// <summary> �C���x���g����\������UI�̍X�V </summary>
	void UpdateInventoryUI();

	/// <summary> �v���C���[�̏�Ԃ�\������UI�̍X�V </summary>
	void UpdateNormalUI(Component_PlayerBehavior* _playerBehavior, Component_BossBehavior* _bossBehavior);
/*
debug :*/
	/// <summary> �f�o�b�O�f�[�^�ҏW�E�B���h�E�̕`�� ���폜�\�� </summary>
	void DrawDebugDataEditWindow();
};

