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
	: GameObject(parent, "Scene_Title"), isFirstSelectButton_(true)
{
}

void Scene_Title::Initialize()
{
	// UIパネル & レイアウトの読み込み
	json loadData;
	if (JsonReader::Load("Datas/Test/Prottype_titleScene_Layout.json", loadData)) UIPanel::GetInstance()->Load(loadData);

	UIPanel* uiPanel = UIPanel::GetInstance();
	isFirstSelectButton_ = true;
}

void Scene_Title::Update()
{
	// シーン切替処理
	{
		UIPanel* uiPanel = UIPanel::GetInstance();

		if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)) {
			// 最初の入力だったら
			if (isFirstSelectButton_ == true) {
				uiPanel->SetSelectingButton(0, 0);
				isFirstSelectButton_ = false;
			}
			else
			uiPanel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::LEFT);
		}
		else if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) {
			if (isFirstSelectButton_ == true) {
				uiPanel->SetSelectingButton(0, 0);
				isFirstSelectButton_ = false;
			}
			else
			uiPanel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::RIGHT);
		}

		for (auto button : uiPanel->GetUIObjects()) {
			// 名前がstartButtonのボタンがあったらかつ、選択中のボタンがなにかあったら
			if (((UIButton*)button)->GetObjectName() == "startButton" && uiPanel->GetSelectingButton() != nullptr) {
				//selectingButton_がstartButtonで、かつAボタンが押されたら or ボタンがマウスによって押されたら
				if (uiPanel->GetSelectingButton()->GetObjectName() == ((UIButton*)button)->GetObjectName() && Input::IsPadButtonDown(XINPUT_GAMEPAD_A) ||
					((UIButton*)button)->OnClick()) {
					// シーンマネージャーを取得してシーンを変更
					SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
					sceneManager->ChangeScene(SCENE_ID_MENU, TID_BLACKOUT);
				}
			}
			else if (((UIButton*)button)->GetObjectName() == "EndButton" && uiPanel->GetSelectingButton() != nullptr) {
				//selectingButton_がEndButtonで、かつAボタンが押されたら or ボタンがマウスによって押されたら
				if (uiPanel->GetSelectingButton()->GetObjectName() == ((UIButton*)button)->GetObjectName() && Input::IsPadButtonDown(XINPUT_GAMEPAD_A) ||
					((UIButton*)button)->OnClick()) {
					PostQuitMessage(0);

				}
			}
		}

		// ボタンが押されたら
		if (((UIButton*)UIPanel::GetInstance()->GetUIObject("EndButton"))->OnClick()) {
			PostQuitMessage(0);
		}
	}

}

void Scene_Title::Draw()
{
}

void Scene_Title::Release()
{
}
