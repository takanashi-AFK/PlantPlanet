#pragma once

// �C���N���[�h
#include"../../Engine/GameObject/GameObject.h"

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

	bool isGameStart_;		// �Q�[���J�n�t���O
	bool fixedCursorPos;	// �J�[�\���Œ艻
	bool cursorVisible;		// �J�[�\���\��
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
};

