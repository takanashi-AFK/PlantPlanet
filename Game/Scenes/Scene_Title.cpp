#include "Scene_Title.h"

// インクルード
#include"../../Engine/SceneManager.h"
#include "../Objects/UI/UIPanel.h"
#include "../Objects/UI/UIButton.h"
#include "../Constants.h"

#include "../../Engine/DirectX/Input.h"
#include "../../Engine/ImGui/imgui.h"
#include "../Objects/UI/UIInputString.h"

#include "../Otheres/UserManager.h"

using namespace Constants;

Scene_Title::Scene_Title(GameObject* parent)
	: GameObject(parent, "Scene_Title"), isFirstSelectButton_(true)
{
}

void Scene_Title::Initialize()
{
	// UIパネル & レイアウトの読み込み
	json loadData;
	if (JsonReader::Load("Datas/SceneLayout/title.json", loadData)) UIPanel::GetInstance()->Load(loadData);

	UIPanel* uiPanel = UIPanel::GetInstance();
	isFirstSelectButton_ = true;
}

void Scene_Title::Update()
{
	// UIパネルの取得
	UIPanel* uiPanel = UIPanel::GetInstance();

	// UI入力処理
	HandleUIInput(uiPanel, isFirstSelectButton_);

	// ユーザー名入力フォームから文字列を取得
	UIInputString* uiInputString = (UIInputString*)uiPanel->GetUIObject(UIType::UI_INPUTSTRING)[0];
	if (uiInputString == nullptr) return;

	// debug code
	ImGui::Begin("userData"); {
		UserManager& um = UserManager::GetInstance();

		for (auto& userData : um.GetAllUsers()) {
			ImGui::Text(userData.first.c_str());
			ImGui::SameLine();
			ImGui::Text(std::to_string(userData.second->bestScore).c_str());
		}

		ImGui::Separator();
		ImGui::Text("Logged in user:");
		ImGui::Text(um.isUserLoggedIn() ? um.GetLoggedInUser()->userName.c_str() : "none");

		ImGui::Separator();
		if (ImGui::Button("+")) um.RegisterUser(uiInputString->GetInputString()); ImGui::SameLine();
		if (ImGui::Button("-")) um.DeleteUser(uiInputString->GetInputString());
		if (ImGui::Button("login")) um.LoginUser(uiInputString->GetInputString()); ImGui::SameLine();
		if (ImGui::Button("logout")) um.LogoutUser();
		if (ImGui::Button("save")) um.SaveUser("Datas/userData.json"); ImGui::SameLine();
		if (ImGui::Button("load")) um.LoadUser("Datas/userData.json");

	} ImGui::End();
}

void Scene_Title::Draw()
{
}

void Scene_Title::Release()
{
}

void Scene_Title::HandleUIInput(UIPanel* _uiPanel, bool& _isFirstSelectButton)
{
	// ユーザー名入力フォームから文字列を取得
	UIInputString* uiInputString = (UIInputString*)_uiPanel->GetUIObject(UIType::UI_INPUTSTRING)[0];
	if (uiInputString == nullptr) return;

	// コントローラー入力により押下するボタンを選択
	{
		if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)) {

			// シーン内最初の入力の場合...
			if (_isFirstSelectButton == true) {

				// ボタンの選択位置を初期化
				_uiPanel->SetButtonArrayIndex(0, 0);

				// シーン内最初の入力フラグをOFF
				_isFirstSelectButton = false;
			}

			// ボタンの選択位置を左に移動
			else _uiPanel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::LEFT);
			
		}
		else if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) {

			// シーン内最初の入力の場合...
			if (_isFirstSelectButton == true) {

				// ボタンの選択位置を初期化
				_uiPanel->SetButtonArrayIndex(0, 0);

				// シーン内最初の入力フラグをOFF
				_isFirstSelectButton = false;
			}

			// ボタンの選択位置を右に移動
			else _uiPanel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::RIGHT);
		}
	}

	// ボタンが押下されたかを判定し、ボタンのアクションを実行
	for (auto uiObject : _uiPanel->GetUIObject(UIType::UI_BUTTON)) {

		// ボタンが押下された場合...
		UIButton* uiButton = static_cast<UIButton*>(uiObject);
		if (uiButton->OnClick()) 
			ProcessButtonAction(uiButton->GetObjectName(), uiInputString->GetInputString());

		// パッドのAボタンが押下された場合...
		if(Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) 
			ProcessButtonAction(_uiPanel->GetSelectingButton()->GetObjectName(), uiInputString->GetInputString());
	}
}

void Scene_Title::ProcessButtonAction(string _buttonName, string _inputUserName)
{
	// `_status`の定義
	// 0: ユーザー名が入力されていない
	// 1: ユーザー名が既に登録されている
	// 2: 新規データを作成する
	// 3: 既に登録されているユーザー名でゲームを開始する
	// 4: 既存データが存在しない

	// ボタン名定数
	const string BUTTON_NAME_START = "startButton";
	const string BUTTON_NAME_CONTINUE = "continueButton";
	const string BUTTON_NAME_END = "EndButton";

	const string BUTTON_NAME_OK = "okButton";
	const string BUTTON_NAME_NO = "noButton";

	// ユーザーマネージャーのインスタンスを取得
	UserManager& um = UserManager::GetInstance();

	// ボタン名によって処理を分岐
	if (_buttonName == BUTTON_NAME_START) {

		// ユーザー名が入力されていない場合...
		if (_inputUserName.empty()) {

			// ユーザー名が入力されていない旨を表示
			status_ = 0;
		}

		// ユーザー名が入力されている場合...
		else {

			// 既存ユーザー名として登録されている場合...
			if (um.isUserRegistered(_inputUserName) == true) {

				// ユーザー名が既に登録されている旨を表示
				status_ = 1;
			}

			// 既存ユーザー名として登録されていない場合...
			else {

				// 新規データを作成する旨を表示
				status_ = 2;
			}
		}
	}

	else if (_buttonName == BUTTON_NAME_CONTINUE) {

		// ユーザー名が入力されていない場合...
		if (_inputUserName.empty()) {

			// ユーザー名が入力されていない旨を表示

			status_ = 0;
		}

		// ユーザー名が入力されている場合...
		else {

			// 既存ユーザー名として登録されている場合...
			if (um.isUserRegistered(_inputUserName) == true) {

				// 既に登録されているユーザー名でゲームを開始する旨を表示
				status_ = 3;
			}

			// 既存ユーザー名として登録されていない場合...
			else {

				// 既存データが存在しない旨を表示
				
				status_ = 4;
			}
		}
	}

	else if (_buttonName == BUTTON_NAME_OK) {
		
		switch (status_) 
		{
			case 0: /*ポップアップを閉じる*/ break;
			case 1: /*ポップアップを閉じる*/ break;
			case 2: /*新規データを作成 & シーンを遷移*/	
				SceneManager* sceneManager = static_cast<SceneManager*>(FindObject("SceneManager"));
				sceneManager->ChangeScene(SCENE_ID_MENU, TID_BLACKOUT);
				break;
		}
	}

	else if (_buttonName == BUTTON_NAME_NO) {

		// ポップアップを閉じる
	}

	else if (_buttonName == BUTTON_NAME_END) {

		// ゲームを終了する
		PostQuitMessage(0);
	}
}
