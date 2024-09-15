#pragma once

// �C���N���[�h
#include <string>

// using�錾
using std::string;

// �t�@�C���p�X�̒萔���`
namespace Constants{

	/// �X�v���b�V���V�[�� ///
	const string SPLASH_SCENE_LAYOUT_JSON = "release/layouts/ui/splashScene_layout.json";

	/// �^�C�g���V�[�� ///
	const string TITLE_SCENE_LAYOUT_JSON = "release/layouts/ui/titleScene_layout.json";
	const string TITLE_SCENE_START_BUTTON_NAME = "startButton";
	const string TITLE_SCENE_RANKING_BUTTON_NAME = "rankingButton";

	/// �v���C�V�[�� ///
	const string PLAY_SCENE_LAYOUT_JSON = "release/layouts/ui/playScene_layout.json";
	const string PLAY_SCENE_STAGE_LAYOUT_JSON = "Stage02.json";
	const string PLAY_SCENE_CAMERA_LAYOUT_JSON = "release/layouts/camera/tpsCamera_layout.json";
	const string PLAY_SCENE_TIMER_NAME = "timer";
}