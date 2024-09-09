#include "AudioController.h"

// �C���N���[�h
#include "../../Engine/ResourceManager/Audio.h"
#include "../../Engine/SceneManager.h"

void AudioController::Update(GameObject* _root)
{
	// �����𖢏p���A�����������ɉ��y���Đ�������A��~�����肷�鏈�����L�q

	// �V�[���Ɉˑ������I�[�f�B�I�̍Đ�/��~����
	{
		// �V�[���}�l�[�W���[���擾
		SceneManager* sm = (SceneManager*)_root->FindObject("SceneManager");

		if(sm->IsCurrentScene(SCENE_ID_TITLE))
		{
			// �^�C�g���V�[���̏ꍇ
			// �^�C�g��BGM���Đ�
			Audio::Play(Audio::Load("Audios/Retro-Beat.wav",true));
		}
		else
		{
			// �^�C�g���V�[���ȊO�̏ꍇ
			// �^�C�g��BGM���~
			Audio::Stop(Audio::Load("Audios/Retro-Beat.wav",true));
		}

	}

}
