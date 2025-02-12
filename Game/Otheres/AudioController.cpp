#include "AudioController.h"

// �C���N���[�h

#include "../../Engine/DirectX/Input.h"
#include "../../Engine/ImGui/imgui.h"
#include "../../Engine/ResourceManager/Audio.h"
#include "../../Engine/SceneManager.h"
#include "../../Game/Objects/UI/UIPanel.h"
#include "../Objects/Stage/Components/AttackComponents/Component_ShootAttack.h"
#include "../Objects/Stage/Components/BehaviorComponents/Component_BossBehavior.h"
#include "../Objects/Stage/Components/BehaviorComponents/Component_PlayerBehavior.h"
#include "../Objects/Stage/Components/GaugeComponents/Component_HealthGauge.h"
#include "../Objects/Stage/Stage.h"
#include "../Objects/UI/CountDown.h"
#include "../Objects/UI/UIButton.h"
#include "../Objects/UI/UIInputString.h"
#include "../Objects/UI/UITimer.h"
#include "../Scenes/Scene_Splash.h"
namespace {
	const string PLAY_BGM = "Audios/Fried_Chicken_Scramble.wav";

	const string BUTTON_SE = "Audios/����{�^��������2.wav";
	const string COUNTDOWN_SE = "Audios/Countdown03-2.wav";
	const string DAMAGE_SE = "Audios/�Ō�1.wav";
	const string JUST_DODGE_SE = "Audios/����[��2.wav";
	const string DODGE_SE = "Audios/�X���C�f�B���O1.wav";
	const string SHOOT_SE = "Audios/�Ή����@1.wav";
	const string WALK_SE = "Audios/���̏�𑖂�.wav";

	const float VOLUME = 0.2f;
}

namespace {
	// �K�v�Ȓ萔���`

	// �X�v���b�V���V�[��SE
	const string SPLASH_SE = "03_audios/01_SE/00_splashSceneSE/����{�^��������29.wav";
	const float SPLASH_SE_TIME = 2.5f;
	const float SPLASH_SE_VOLUME = 1.f;

	// �^�C�g���V�[��BGM
	const string TITLE_BGM = "03_audios/00_BGM/00_titleSceneBGM/�v���[���g�R�[�i�[.wav";
	const float TITLE_BGM_VOLUME = 1.f;

	// ���j���[�V�[��BGM
	const string MENU_BGM = "03_audios/00_BGM/01_menuSceneBGM/�j���g���_���X.wav";
	const float MENU_BGM_VOLUME = 0.5f;
}

void AudioController::Update(GameObject* _root)
{
	// �������L�q���A�����������ɉ��y���Đ�������A��~�����肷�鏈�����L�q
	UIPanel* uiPanel = UIPanel::GetInstance();
	SceneManager* sceneManager = (SceneManager*)_root->FindObject("SceneManager");

// �e�X�g�V�[�� //////////////////////////////////////////////
	if (sceneManager->IsCurrentScene(SCENE_ID_TEST)) return;

// �X�v���b�V���V�[�� //////////////////////////////////////////////
	else if (sceneManager->IsCurrentScene(SCENE_ID_SPLASH))
	{
		// �X�v���b�V���V�[���̃C���X�^���X���擾
		Scene_Splash* splash = (Scene_Splash*)_root->FindObject("Scene_Splash");

		// �X�v���b�V���V�[���̃J�E���g��2.5�b�o�߂�������SE���Đ�
		if (splash->IsCountNow(SPLASH_SE_TIME) == true)
			Audio::Play(Audio::Load(SPLASH_SE, false), SPLASH_SE_VOLUME);
	}
	
// �^�C�g���V�[�� //////////////////////////////////////////////
	else if (sceneManager->IsCurrentScene(SCENE_ID_TITLE)) {

		// �w�i���y�̍Đ�
			Audio::Play(Audio::Load(TITLE_BGM, true), TITLE_BGM_VOLUME);
		//

		// �R���g���[���[�ɂ���ă{�^�����ړ���������SE�Đ�����
			// �� �^�C�g���V�[�� 133,158�s�ڂ����� �ɋL�q�@�C���\��
		// 

		// �I���{�^�����ړ���������SE�Đ�����
			// �� �^�C�g���V�[�� 133,158�s�ڂ����� �ɋL�q�@�C���\��
		//

		// �{�^�����N���b�N���ꂽ����SE�Đ�����
			// �� �^�C�g���V�[�� 237, 260, 281, 305, 
			// 357, 376, 459�s�ڂ����� �ɋL�q�@�C���\��
		//

		// �V�[����؂�ւ��鎞��SE�Đ�����
			// �� �^�C�g���V�[�� 427�s�ڂ����� �ɋL�q�@�C���\��
		//
	}

// ���j���[�V�[�� //////////////////////////////////////////////
	else if (sceneManager->IsCurrentScene(SCENE_ID_MENU)) {

		// �^�C�g��BGM�̒�~
		Audio::Stop(Audio::Load(TITLE_BGM, true));

		// ���j���[�V�[����BGM�Đ�
		Audio::Play(Audio::Load(MENU_BGM, true), MENU_BGM_VOLUME);

		// �{�^�����N���b�N���ꂽ����SE�Đ�����
			// �� ���j���[�V�[��575�s�ڂ����� �ɋL�q�@�C���\��
		//
	}

	if (sceneManager->IsCurrentScene(SCENE_ID_PLAY)) {

		// �w�i���y�̍Đ�
		//Audio::Play(Audio::Load(ScoreManager::userName != "insane" ? PLAY_BGM : "Audios/�����͒ʂ��˂����I.wav", true), VOLUME);
		Audio::Play(Audio::Load(TITLE_BGM, true));

		// �J�E���g�_�E���̉��̍Đ�
		CountDown* countDown = (CountDown*)_root->FindObject("CountDown");
		if(countDown != nullptr)if (countDown->IsActive())Audio::Play(Audio::Load(COUNTDOWN_SE, false));

		// �X�e�[�W���擾
		Stage* stage = (Stage*)_root->FindObject("Stage");

		// �_���[�W���󂯂����̉��̍Đ�
		for (auto hg : stage->FindComponents(HealthGauge)) {
			if (((Component_HealthGauge*)hg)->IsUnlockAndReduce()) Audio::Play(Audio::Load(DAMAGE_SE, false));
			if (((Component_HealthGauge*)hg)->IsLockAndReduce())
			{
				Audio::Play(Audio::Load(JUST_DODGE_SE, false));
			}
		}

		for (auto pb : stage->FindComponents(PlayerBehavior)) {
			
			// �v���C���[���V���[�g���������̉��̍Đ�
			if(((Component_PlayerBehavior*)pb)->IsShootStart()) Audio::Play(Audio::Load(SHOOT_SE, false));

			// �v���C���[��������������̉��̍Đ�
			if(((Component_PlayerBehavior*)pb)->IsDodgeStart()) Audio::Play(Audio::Load(DODGE_SE, false));

			// �v���C���[�������Ă��鎞�̉��̍Đ�
			if(((Component_PlayerBehavior*)pb)->IsState(PLAYER_STATE_WALK)) Audio::Play(Audio::Load(WALK_SE, true));
			else Audio::Stop(Audio::Load(WALK_SE, true));
		}
	}

	//else 
	//{
		// �^�C�g���V�[���ȊO�̃V�[���ł���΁A�^�C�g��BGM���Đ�
		if(!sceneManager->IsCurrentScene(SCENE_ID_SPLASH) && !sceneManager->IsCurrentScene(SCENE_ID_END))Audio::Play(Audio::Load(TITLE_BGM, true), VOLUME);
		Audio::Stop(Audio::Load(PLAY_BGM, true));

	//	if(Input::IsPadButtonDown()) Audio::Play(Audio::Load(BUTTON_SE, false));
	//}
	
	// �{�^�����N���b�N���ꂽ����SE�Đ�����
	/*for (auto button : UIPanel::GetInstance()->GetUIObject(UI_BUTTON)) {
		if (((UIButton*)button)->OnClick()) Audio::Play(Audio::Load(BUTTON_SE, false));
	}*/

	for (auto timer : uiPanel->GetUIObject(UI_TIMER)) {
		if (((UITimer*)timer)->GetSeconds() <= 15.f) Audio::SetPlaybackSpeed(Audio::Load(PLAY_BGM, true), 1.2f);
	}
}
