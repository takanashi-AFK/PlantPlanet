#include "AudioController.h"

// インクルード

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

	const string BUTTON_SE = "Audios/決定ボタンを押す2.wav";
	const string COUNTDOWN_SE = "Audios/Countdown03-2.wav";
	const string DAMAGE_SE = "Audios/打撃1.wav";
	const string JUST_DODGE_SE = "Audios/きらーん2.wav";
	const string DODGE_SE = "Audios/スライディング1.wav";
	const string SHOOT_SE = "Audios/火炎魔法1.wav";
	const string WALK_SE = "Audios/砂の上を走る.wav";

	const float VOLUME = 0.2f;
}

namespace {
	// 必要な定数を定義

	// スプラッシュシーンSE
	const string SPLASH_SE = "03_audios/01_SE/00_splashSceneSE/決定ボタンを押す29.wav";
	const float SPLASH_SE_TIME = 2.5f;
	const float SPLASH_SE_VOLUME = 1.f;

	// タイトルシーンBGM
	const string TITLE_BGM = "03_audios/00_BGM/00_titleSceneBGM/プレゼントコーナー.wav";
	const float TITLE_BGM_VOLUME = 1.f;

	// メニューシーンBGM
	const string MENU_BGM = "03_audios/00_BGM/01_menuSceneBGM/ニワトリダンス.wav";
	const float MENU_BGM_VOLUME = 0.5f;
}

void AudioController::Update(GameObject* _root)
{
	// 条件を記述し、満たした時に音楽を再生したり、停止したりする処理を記述
	UIPanel* uiPanel = UIPanel::GetInstance();
	SceneManager* sceneManager = (SceneManager*)_root->FindObject("SceneManager");

// テストシーン //////////////////////////////////////////////
	if (sceneManager->IsCurrentScene(SCENE_ID_TEST)) return;

// スプラッシュシーン //////////////////////////////////////////////
	else if (sceneManager->IsCurrentScene(SCENE_ID_SPLASH))
	{
		// スプラッシュシーンのインスタンスを取得
		Scene_Splash* splash = (Scene_Splash*)_root->FindObject("Scene_Splash");

		// スプラッシュシーンのカウントが2.5秒経過した時にSEを再生
		if (splash->IsCountNow(SPLASH_SE_TIME) == true)
			Audio::Play(Audio::Load(SPLASH_SE, false), SPLASH_SE_VOLUME);
	}
	
// タイトルシーン //////////////////////////////////////////////
	else if (sceneManager->IsCurrentScene(SCENE_ID_TITLE)) {

		// 背景音楽の再生
			Audio::Play(Audio::Load(TITLE_BGM, true), TITLE_BGM_VOLUME);
		//

		// コントローラーによってボタンが移動した時のSE再生処理
			// ※ タイトルシーン 133,158行目あたり に記述　修正予定
		// 

		// 選択ボタンが移動した時のSE再生処理
			// ※ タイトルシーン 133,158行目あたり に記述　修正予定
		//

		// ボタンがクリックされた時のSE再生処理
			// ※ タイトルシーン 237, 260, 281, 305, 
			// 357, 376, 459行目あたり に記述　修正予定
		//

		// シーンを切り替える時のSE再生処理
			// ※ タイトルシーン 427行目あたり に記述　修正予定
		//
	}

// メニューシーン //////////////////////////////////////////////
	else if (sceneManager->IsCurrentScene(SCENE_ID_MENU)) {

		// タイトルBGMの停止
		Audio::Stop(Audio::Load(TITLE_BGM, true));

		// メニューシーンのBGM再生
		Audio::Play(Audio::Load(MENU_BGM, true), MENU_BGM_VOLUME);

		// ボタンがクリックされた時のSE再生処理
			// ※ メニューシーン575行目あたり に記述　修正予定
		//
	}

	if (sceneManager->IsCurrentScene(SCENE_ID_PLAY)) {

		// 背景音楽の再生
		//Audio::Play(Audio::Load(ScoreManager::userName != "insane" ? PLAY_BGM : "Audios/ここは通さねえぜ！.wav", true), VOLUME);
		Audio::Play(Audio::Load(TITLE_BGM, true));

		// カウントダウンの音の再生
		CountDown* countDown = (CountDown*)_root->FindObject("CountDown");
		if(countDown != nullptr)if (countDown->IsActive())Audio::Play(Audio::Load(COUNTDOWN_SE, false));

		// ステージを取得
		Stage* stage = (Stage*)_root->FindObject("Stage");

		// ダメージを受けた時の音の再生
		for (auto hg : stage->FindComponents(HealthGauge)) {
			if (((Component_HealthGauge*)hg)->IsUnlockAndReduce()) Audio::Play(Audio::Load(DAMAGE_SE, false));
			if (((Component_HealthGauge*)hg)->IsLockAndReduce())
			{
				Audio::Play(Audio::Load(JUST_DODGE_SE, false));
			}
		}

		for (auto pb : stage->FindComponents(PlayerBehavior)) {
			
			// プレイヤーがシュートをした時の音の再生
			if(((Component_PlayerBehavior*)pb)->IsShootStart()) Audio::Play(Audio::Load(SHOOT_SE, false));

			// プレイヤーが回避をした時の音の再生
			if(((Component_PlayerBehavior*)pb)->IsDodgeStart()) Audio::Play(Audio::Load(DODGE_SE, false));

			// プレイヤーが歩いている時の音の再生
			if(((Component_PlayerBehavior*)pb)->IsState(PLAYER_STATE_WALK)) Audio::Play(Audio::Load(WALK_SE, true));
			else Audio::Stop(Audio::Load(WALK_SE, true));
		}
	}

	//else 
	//{
		// タイトルシーン以外のシーンであれば、タイトルBGMを再生
		if(!sceneManager->IsCurrentScene(SCENE_ID_SPLASH) && !sceneManager->IsCurrentScene(SCENE_ID_END))Audio::Play(Audio::Load(TITLE_BGM, true), VOLUME);
		Audio::Stop(Audio::Load(PLAY_BGM, true));

	//	if(Input::IsPadButtonDown()) Audio::Play(Audio::Load(BUTTON_SE, false));
	//}
	
	// ボタンがクリックされた時のSE再生処理
	/*for (auto button : UIPanel::GetInstance()->GetUIObject(UI_BUTTON)) {
		if (((UIButton*)button)->OnClick()) Audio::Play(Audio::Load(BUTTON_SE, false));
	}*/

	for (auto timer : uiPanel->GetUIObject(UI_TIMER)) {
		if (((UITimer*)timer)->GetSeconds() <= 15.f) Audio::SetPlaybackSpeed(Audio::Load(PLAY_BGM, true), 1.2f);
	}
}
