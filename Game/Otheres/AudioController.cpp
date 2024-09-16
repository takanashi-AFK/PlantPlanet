#include "AudioController.h"

// �C���N���[�h
#include "../../Engine/ResourceManager/Audio.h"
#include "../../Engine/SceneManager.h"
#include "../../Game/Objects/UI/UIPanel.h"
#include "../Objects/UI/UIButton.h"
#include "../Objects/UI/CountDown.h"

namespace {
	const string TITLE_BGM = "Audios/�v���[���g�R�[�i�[.wav";
	const string PLAY_BGM = "Audios/Fried_Chicken_Scramble.wav";
	const string BUTTON_SE = "Audios/����{�^��������2.wav";
	const string COUNTDOWN_SE = "Audios/Countdown03-2.wav";
}

void AudioController::Update(GameObject* _root)
{
	// �����𖢏p���A�����������ɉ��y���Đ�������A��~�����肷�鏈�����L�q

	// UI�p�l�����擾
	UIPanel* uiPanel = UIPanel::GetInstance();

	// �{�^�����N���b�N���ꂽ����SE�Đ�����
	for (auto button : UIPanel::GetInstance()->GetUIObject(UI_BUTTON)) {
		if (((UIButton*)button)->OnClick()) Audio::Play(Audio::Load(BUTTON_SE, false));
	}

	// �V�[���؂�ւ�����BGM�Đ�����
	SceneManager* sceneManager = (SceneManager*)_root->FindObject("SceneManager");


	if (sceneManager->IsCurrentScene(SCENE_ID_PLAY)) {

		// �w�i���y�̍Đ�
		Audio::Play(Audio::Load(PLAY_BGM, true));
		Audio::Stop(Audio::Load(TITLE_BGM, true));

		// �J�E���g�_�E���̉��̍Đ�
		CountDown* countDown = (CountDown*)_root->FindObject("CountDown");
		if(countDown->IsActive())Audio::Play(Audio::Load(COUNTDOWN_SE, false));
	}
	else
	{
		if(!sceneManager->IsCurrentScene(SCENE_ID_SPLASH) && !sceneManager->IsCurrentScene(SCENE_ID_END))Audio::Play(Audio::Load(TITLE_BGM, true));
		Audio::Stop(Audio::Load(PLAY_BGM, true));
	}
}
