#pragma once
#include"../../Engine/GameObject/GameObject.h"

class Scene_Result :public GameObject
{
private:
	int scoreNum_;	// �X�R�A
	int layerNumberCount_;
public:
	/// <summary> �R���X�g���N�^ </summary>
	Scene_Result(GameObject* parent_);

	/// <summary> ������ </summary>
	void Initialize() override;

	/// <summary> �X�V </summary>
	void Update() override;

	/// <summary> �`�� </summary>
	void Draw() override;

	/// <summary> ��� </summary>
	void Release() override;

private:
	/// <summary> �X�R�A�̌v�Z </summary>
	int CalculateScore(bool isCleared, int remainingTime, int remainingHP);

	void InitializeScoreAttackResult();
	void InitializeAdventureResult();
	void UpdateScoreAttackResult();
	void UpdateAdventureResult();

//�X�R�A�A�^�b�N�p--------------
	//�X�R�A�A�^�b�N�̉��o���X�L�b�v���ꂽ���m�F
	void CheckSkipScoreAttackResult();

	//�e�L�X�g�Q�̃C�[�W���O
	void UpdateTexts();

	//�X�R�A���l�̃C�[�W���O
	void UpdateValues();

	//�g�[�^���X�R�A�̃C�[�W���O
	void UpdateTotalScore();

	//�{�^���̃C�[�W���O
	void UpdateButton();

	//�ŏI�I��update��ԁB�{�^�����������̂�҂��Ă���B
	void UpdateWaitingForReturn();

	void UpdateEmptyWork();
	//�ŏI�I�Ȑ��l�B���K�̏��
	void ApplyFinalData();
private:

	void (Scene_Result::*scoreAttackUpdateFunction)();
	float easingValue_;
	int totalScore_;

//-----------------------------------------
};

	