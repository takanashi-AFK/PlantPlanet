#pragma once

// インクルード
#include <string>

// using宣言
using std::string;

// ファイルパスの定数を定義
namespace Constants {

// 各種Jsonファイルのパス //////////////////////////////////////////////
	// UIレイアウト
	const string TITLE_SCENE_UI_LAYOUT_JSON = "Datas/UILayouts/alpha_titleScene_layout.json";
	const string DIFFICULTY_SELECT_SCENE_UI_LAYOUT_JSON = "Datas/UILayouts/difficultySelectScene_layout.json";
	const string PLAY_SCENE_UI_LAYOUT_JSON = "Datas/無題.json";
	const string RESULT_SCENE_UI_LAYOUT_JSON = "Datas/UILayouts/resultScene_layout.json";
	const string END_SCENE_UI_LAYOUT_JSON = "Datas/UILayouts/endScene_layout.json";
	const string RANKING_SCENE_UI_LAYOUT_JSON = "Datas/UILayouts/rankingScene_layout.json";
	const string MENU_SCENE_UI_LAYOUT_JSON = "Datas/SceneLayout/MenuScene/MenuSceneLayout2.json";

	// ステージレイアウト
	const string STAGE_EASY_JSON = "Datas/StageLayouts/stage_easy.json";
	const string STAGE_NORMAL_JSON = "Datas/StageLayouts/stage_normal.json";
	const string STAGE_HARD_JSON = "Datas/StageLayouts/stage_hard.json";
	const string STAGE_EXPERT_JSON = "Datas/StageLayouts/stage_expert.json";
	const string STAGE_INSANE_JSON = "Datas/StageLayouts/stage_insane.json";
	const string STAGE_BACKGROUND_JSON = "Datas/StageLayouts/resultStage.json";

	// カメラレイアウト
	const string PLAY_SCENE_CAMERA_LAYOUT_JSON = "Datas/CameraLayouts/TPSCamera.json";
	const string TEST_SCENE_CAMERA_LAYOUT_JSON = "Datas/CameraLayouts/camera.json";

// 各種オブジェクトの名前 //////////////////////////////////////////////

	// タイトルシーンで使用するオブジェクト名
	const string TITLE_SCENE_START_BUTTON_NAME = "startButton";
	const string TITLE_SCENE_RANKING_BUTTON_NAME = "rankingButton";
	const string TITLE_SCENE_USER_NAME_INPUT_NAME = "userNameInput";

	// 難易度選択シーンで使用するオブジェクト名
	const string DIFFICULTY_SELECT_SCENE_EASY_BUTTON_NAME = "easyButton";
	const string DIFFICULTY_SELECT_SCENE_NORMAL_BUTTON_NAME = "normalButton";
	const string DIFFICULTY_SELECT_SCENE_HARD_BUTTON_NAME = "hardButton";
	const string DIFFICULTY_SELECT_SCENE_EXPERT_BUTTON_NAME = "expertButton";

	// プレイシーンで使用するオブジェクト名
	const string PLAY_SCENE_TIMER_NAME = "timer";
	const string PLAY_SCENE_PLAYER_HP_GAUGE_NAME = "player_hpGauge";
	const string PLAY_SCENE_BOSS_HP_GAUGE_NAME = "boss_hpGauge";
	const string PLAY_SCENE_STAMINA_GAUGE_NAME = "staminaGauge";

	// リザルトシーンで使用するオブジェクト名
	const string RESULT_SCENE_HP_TEXT_NAME = "hpNum";
	const string RESULT_SCENE_TIME_TEXT_NAME = "timeNum";
	const string RESULT_SCENE_SCORE_TEXT_NAME = "scoreNum";
	const string RESULT_SCENE_NEXT_BUTTON_NAME = "nextButton";

	// ランキングシーンで使用するオブジェクト名
	const string RANKING_SCENE_RETURN_BUTTON_NAME = "returnButton";

// 各種fbxファイルのパス ///////////////////////////////////////////////

	/// プレイヤーのモーション ///
	const string PLAYER_STATE_WALK_FBX = "Models/Player/Running.fbx";
	const string PLAYER_STATE_SHOOT_FBX = "Models/Player/Firing Rifle.fbx";
	const string PLAYER_STATE_IDLE_FBX = "Models/Player/Idle.fbx";
	const string PLAYER_STATE_DODGE_FBX = "Models/Player/Female Action Pose.fbx";
	const string PLAYER_STATE_DEAD_FBX = "Models/Player/Standing React Death Right.fbx";
	const string PLAYER_STATE_SHOOT_WALK_LEFT_FBX ="Models/Player/Run Right.fbx"; 
	const string PLAYER_STATE_SHOOT_WALK_RIGHT_FBX = "Models/Player/Run Left.fbx";
	const string PLAYER_STATE_SHOOT_WALK_BACK_FBX = "Models/Player/Run Backward.fbx";
	const string PLAYER_STATE_SHOOT_WALK_FORWARD_FBX = "Models/Player/Run Forward.fbx";
	const string PLAYER_STATE_SHOOT_IDLE_FBX = "Models/Player/Rifle Aiming Idle.fbx";
	const string PLAYER_STATE_INTRACT_FBX = "Models/Player/Gathering Objects.fbx";
	const string PLAYER_STATE_MELEE_FBX = "Models/Player/Sword And Shield Slash.fbx";
	const string PLAYER_STATE_MADESALAD_FBX = "Models/Player/Drinking.fbx";

// 各種データのパス ////////////////////////////////////////////////////

	// ランキングデータ
	const string RANKING_DATA_JSON = "Datas/ranking.json";

////////////////////////////////////////////////////////////////////////
}

// 2025/02/11 以降追加分ファイルパス定数
namespace Constants {
	const string SPLASH_SCENE_UI_LAYOUT_JSON = "00_datas/00_sceneLayouts/00_splashSceneLayouts/00_splashSceneUILayout.json";
}