#include "AudioController.h"

// �C���N���[�h
#include "../../Engine/ResourceManager/Audio.h"
#include "../../Engine/SceneManager.h"
#include "../../Game/Objects/UI/UIPanel.h"
#include "../Objects/UI/UIButton.h"


void AudioController::Update(GameObject* _root)
{
	// �����𖢏p���A�����������ɉ��y���Đ�������A��~�����肷�鏈�����L�q

	// UI�p�l�����擾
	UIPanel* uiPanel = UIPanel::GetInstance();

	// �V�[���Ɉˑ������I�[�f�B�I�̍Đ�/��~����
	{
		// �V�[���}�l�[�W���[���擾
		SceneManager* sm = (SceneManager*)_root->FindObject("SceneManager");

		if(sm->IsCurrentScene(SCENE_ID_TITLE))
		{
			// �^�C�g���V�[���̏ꍇ
			// �^�C�g��BGM���Đ�
			Audio::Play(Audio::Load("Audios/Retro-Beat.wav",true));

			// �{�^���������ꂽ�Ƃ���SE���Đ�
			{
				// �{�^�����擾
				UIButton* button = (UIButton*)uiPanel->GetUIObject("title_startButton");
				
				// �{�^���������ꂽ��
				if(button!= nullptr)if(button->OnClick()){
					Audio::Play(Audio::Load("Audios/����{�^��������2.wav",false));
				}
			}
		}

		else
		{
			// �^�C�g���V�[���ȊO�̏ꍇ
			// �^�C�g��BGM���~
			Audio::Stop(Audio::Load("Audios/Retro-Beat.wav",true));
		}

	}

}
