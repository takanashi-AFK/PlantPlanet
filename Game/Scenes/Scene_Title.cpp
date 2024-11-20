#include "Scene_Title.h"

// インクルード
#include"../../Engine/SceneManager.h"
#include "../Objects/UI/UIPanel.h"
#include "../Objects/UI/UIButton.h"
#include "../Constants.h"

#include "../../Engine/DirectX/Input.h"
#include "../../Engine/ImGui/imgui.h"
#include "../Objects/UI/UIInputString.h"
using namespace Constants;

Scene_Title::Scene_Title(GameObject* parent)
	: GameObject(parent, "Scene_Title")
{
}

void Scene_Title::Initialize()
{
	// UIパネル & レイアウトの読み込み
	json loadData;
	if (JsonReader::Load(TITLE_SCENE_UI_LAYOUT_JSON, loadData)) UIPanel::GetInstance()->Load(loadData);

	// ユーザー名の初期化
	/*UIInputString* input = (UIInputString*)UIPanel::GetInstance()->GetUIObject(TITLE_SCENE_USER_NAME_INPUT_NAME);
	input->SetInputString(ScoreManager::userName);*/
}

void Scene_Title::Update()
{

	// シーン切替処理
	{
		// ランキングシーンへの遷移
		UIButton* rankingButton = (UIButton*)UIPanel::GetInstance()->GetUIObject(TITLE_SCENE_RANKING_BUTTON_NAME);
		if (rankingButton->OnClick() == true || Input::IsPadButtonDown(XINPUT_GAMEPAD_X)) {
			SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
			sceneManager->ChangeScene(SCENE_ID_RANKING, TID_BLACKOUT);
		}

		// アドベンチャーへシーン遷移
		UIButton* adventureStartButton = (UIButton*)UIPanel::GetInstance()->GetUIObject("ADV_StartButton");
		if (adventureStartButton->OnClick() == true || Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) {

			// ここでユーザー名を取得する
			UIInputString* input = (UIInputString*)UIPanel::GetInstance()->GetUIObject(TITLE_SCENE_USER_NAME_INPUT_NAME);
			ScoreManager::userName = input->GetInputString();

			g_selectedGameMode = 1;

			SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
			sceneManager->ChangeScene(SCENE_ID_PLAY, TID_BLACKOUT);
		}

		// スコアアタックへシーン遷移
		UIButton* scoreAttackStartButton = (UIButton*)UIPanel::GetInstance()->GetUIObject("SCA_StartButton");
		if (scoreAttackStartButton->OnClick() == true || Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) {

			// ここでユーザー名を取得する
			UIInputString* input = (UIInputString*)UIPanel::GetInstance()->GetUIObject(TITLE_SCENE_USER_NAME_INPUT_NAME);
			ScoreManager::userName = input->GetInputString();

			g_selectedGameMode = 2;

			SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
			sceneManager->ChangeScene(SCENE_ID_PLAY, TID_BLACKOUT);
		}
	}
}

void Scene_Title::Draw()
{
}

void Scene_Title::Release()
{
}
