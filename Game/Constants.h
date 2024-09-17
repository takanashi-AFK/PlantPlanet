#pragma once

// �C���N���[�h
#include <string>

// using�錾
using std::string;

// �t�@�C���p�X�̒萔���`
namespace Constants {

	/// �X�v���b�V���V�[�� ///
	const string SPLASH_SCENE_LAYOUT_JSON = "Datas/UILayouts/splashScene_layout.json";

	/// �^�C�g���V�[�� ///
	const string TITLE_SCENE_LAYOUT_JSON = "Datas/UILayouts/titleScene_layout.json";
	const string TITLE_SCENE_START_BUTTON_NAME = "startButton";
	const string TITLE_SCENE_RANKING_BUTTON_NAME = "rankingButton";

	/// ��Փx�I���V�[�� ///
	const string DIFFICULTY_SELECT_SCENE_LAYOUT_JSON = "Datas/UILayouts/difficultySelectScene _layout.json";
	const string DIFFICULTY_SELECT_SCENE_EASY_BUTTON_NAME = "easyButton";
	const string DIFFICULTY_SELECT_SCENE_NORMAL_BUTTON_NAME = "normalButton";
	const string DIFFICULTY_SELECT_SCENE_HARD_BUTTON_NAME = "hardButton";
	const string DIFFICULTY_SELECT_SCENE_EXPERT_BUTTON_NAME = "expertButton";

	/// �v���C�V�[�� ///
	const string PLAY_SCENE_LAYOUT_JSON = "Datas/UILayouts/playScene_layout.json";
	const string PLAY_SCENE_STAGE_LAYOUT_JSON = "Datas/StageLayouts/stage.json";
	const string PLAY_SCENE_CAMERA_LAYOUT_JSON = "Datas/CameraLayouts/TPSCamera.json";
	const string PLAY_SCENE_TIMER_NAME = "timer";
	const string PLAY_SCENE_PLAYER_HP_GAUGE_NAME = "player_hpGauge";
	const string PLAY_SCENE_BOSS_HP_GAUGE_NAME = "boss_hpGauge";

	/// ���U���g�V�[�� ///
	const string RESULT_SCENE_LAYOUT_JSON = "Datas/UILayouts/resultScene_layout.json";
	const string RESULT_SCENE_STAGE_LAYOUT_JSON = "Datas/StageLayouts/resultStage.json";
	const string RESULT_SCENE_HP_TEXT_NAME = "hpNum";
	const string RESULT_SCENE_TIME_TEXT_NAME = "timeNum";
	const string RESULT_SCENE_SCORE_TEXT_NAME = "scoreNum";
	const string RESULT_SCENE_NEXT_BUTTON_NAME = "nextButton";

	/// �G���h�V�[�� ///
	const string END_SCENE_LAYOUT_JSON = "Datas/UILayouts/endScene_layout.json";

	/// �����L���O�V�[�� ///
	const string RANKING_SCENE_LAYOUT_JSON = "Datas/UILayouts/rankingScene_layout.json";
	const string RANKING_SCENE_RETURN_BUTTON_NAME = "returnButton";

	/// �e�X�g�V�[�� ///
	const string TEST_SCENE_CAMERA_LAYOUT_JSON = "Datas/CameraLayouts/camera.json";

	// �f�[�^
	const string RANKING_DATA_CSV = "Datas/ranking.csv";

	/// �v���C���[�̃��[�V���� ///
	const string PLAYER_STATE_WALK_FBX = "Models/Player/Running.fbx";
	const string PLAYER_STATE_SHOOT_FBX = "Models/Player/Fireball.fbx";
	const string PLAYER_STATE_IDLE_FBX = "Models/Player/Silly Dancing.fbx";
	const string PLAYER_STATE_DODGE_FBX = "Models/Player/Female Action Pose.fbx";
	const string PLAYER_STATE_DEAD_FBX = "Models/Player/Standing React Death Right.fbx";

	// ��Փx���̃X�e�[�W��
	const string STAGE_EASY = "stage_easy.json";
	const string STAGE_NORMAL = "stage_normal.json";
	const string STAGE_HARD = "stage_hard.json";
	const string STAGE_EXPERT = "stage_expert.json";
}