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
#include "../Objects/UI/UIText.h"

using namespace Constants;

namespace {
	// UI名定数
	const string BUTTON_NAME_START		= "startButton";
	const string BUTTON_NAME_CONTINUE	= "continueButton";
	const string BUTTON_NAME_END		= "EndButton";

	const string BUTTON_NAME_OK			= "okButton";
	const string BUTTON_NAME_NO			= "noButton";
	const string IMAGE_POPUP			= "pop-upWindowBackground";

	const string IMAGE_TEXT0			= "text0";
	const string IMAGE_TEXT1			= "text1";
	const string IMAGE_TEXT2			= "text2";
	const string IMAGE_TEXT3			= "text3";
	const string IMAGE_TEXT4			= "text4";

	const string TEXT_USER_NAME			= "text-userName";
	const string TEXT_LIBRARY_STATUS	= "text-libraryStatus";
	const string TEXT_PLAY_TOTAL_TIME	= "text-playTotalTime";
}

Scene_Title::Scene_Title(GameObject* parent)
	: GameObject(parent, "Scene_Title"), isFirstSelectButton_(true), status_(0)
{
}

void Scene_Title::Initialize()
{
	UserManager::GetInstance().LoadUser("Datas/userData.json");

	// UIパネル & レイアウトの読込
	json loadData;
	if (JsonReader::Load("Datas/SceneLayout/title.json", loadData)) UIPanel::GetInstance()->Load(loadData);

	UIPanel* uiPanel = UIPanel::GetInstance(); {
		// ボタン配列の初期化
		uiPanel->ResetArrayOfButton();

		// 特定のボタンを配列に追加
		uiPanel->PushButtonToArray((UIButton*)uiPanel->GetUIObject(BUTTON_NAME_START));
		uiPanel->PushButtonToArray((UIButton*)uiPanel->GetUIObject(BUTTON_NAME_CONTINUE));
	}

	// 植物情報を読込
	json plantData;
	if (JsonReader::Load("Datas/PlantData/TentativeFlowers.json", plantData))PlantCollection::Load(plantData);
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

	// 現在、選択中 かつ レイヤーが一番上のボタンを取得
	UIButton* selectingButton = UIButton::GetTopSelectingUI(_uiPanel->GetUIObject(UIType::UI_BUTTON));

	// UIボタンが押下された場合...
	if (selectingButton != nullptr && selectingButton->OnClick())
		ProcessButtonAction(_uiPanel, selectingButton->GetObjectName(), uiInputString->GetInputString());
	
	// パッドのAボタンが押下された場合... ( レイヤーに関係なく )
	selectingButton = _uiPanel->GetSelectingButton();
	if (selectingButton == nullptr) return;
	
	// ボタンのシェーダーを変更
	selectingButton->SetShader(Direct3D::SHADER_BUTTON_SELECT);
	
	// パッドのAボタンが押下された場合...
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_A))
		ProcessButtonAction(_uiPanel, selectingButton->GetObjectName(), uiInputString->GetInputString());
	
}

void Scene_Title::ProcessButtonAction(UIPanel* _uiPanel,string _buttonName, string _inputUserName)
{
	// ユーザーマネージャーのインスタンスを取得
	UserManager& um = UserManager::GetInstance();

	//ボタンが選択できるかどうか設定する
	auto SetSelectableButton_ContinueAndStart = [&](bool flag) 
		{
			UIButton* startButton = static_cast<UIButton*>(_uiPanel->GetUIObject(BUTTON_NAME_START));
			UIButton* continueButton = static_cast<UIButton*>(_uiPanel->GetUIObject(BUTTON_NAME_CONTINUE));

			if (!(startButton && continueButton)) return;

			startButton->SetSelctable(flag);
			continueButton->SetSelctable(flag);
		};

	//ゲームパッドのボタンで失敗したときの配列場所
	int failerButtonX = 0;
	int failerButtonY = 1;

	//通常時の場所
	int defaultButtonX = 0;
	int defaultButtonY = 0;

	// `_status`の定義
	// 0: ユーザー名が入力されていない
	// 1: ユーザー名が既に登録されている
	// 2: 新規データを作成する
	// 3: 既に登録されているユーザー名でゲームを開始する
	// 4: 既存データが存在しない

	// ボタン名によって処理を分岐
	if (_buttonName == BUTTON_NAME_START) {

		// ユーザー名が入力されていない場合...
		if (_inputUserName.empty()) {

			// ユーザー名が入力されていない旨を表示
			SetUIVisible(_uiPanel, { IMAGE_TEXT0, IMAGE_POPUP, BUTTON_NAME_OK }, true);
			_uiPanel->PushButtonToArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_OK));
			_uiPanel->SetButtonArrayIndex(failerButtonX, failerButtonY);

			SetSelectableButton_ContinueAndStart(false);

			status_ = 0;
		}

		// ユーザー名が入力されている場合...
		else {

			// 既存ユーザー名として登録されている場合...
			if (um.isUserRegistered(_inputUserName) == true) {

				// ユーザー名が既に登録されている旨を表示
				SetUIVisible(_uiPanel, { IMAGE_TEXT1, IMAGE_POPUP, BUTTON_NAME_OK }, true);
				_uiPanel->PushButtonToArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_OK));
				_uiPanel->SetButtonArrayIndex(failerButtonX, failerButtonY);

				SetSelectableButton_ContinueAndStart(false);

				status_ = 1;
			}

			// 既存ユーザー名として登録されていない場合...
			else {

				// 新規データを作成する旨を表示
				SetUIVisible(_uiPanel, { IMAGE_TEXT2, IMAGE_POPUP, BUTTON_NAME_OK, BUTTON_NAME_NO }, true);
				_uiPanel->PushButtonToArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_OK));
				_uiPanel->PushButtonToArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_NO));
				_uiPanel->SetButtonArrayIndex(failerButtonX, failerButtonY);

				SetSelectableButton_ContinueAndStart(false);

				status_ = 2;
			}
		}
	}

	else if (_buttonName == BUTTON_NAME_CONTINUE) {

		// ユーザー名が入力されていない場合...
		if (_inputUserName.empty()) {

			// ユーザー名が入力されていない旨を表示
			SetUIVisible(_uiPanel, { IMAGE_TEXT0, IMAGE_POPUP, BUTTON_NAME_OK }, true);
			_uiPanel->PushButtonToArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_OK));
			_uiPanel->SetButtonArrayIndex(failerButtonX, failerButtonY);

			SetSelectableButton_ContinueAndStart(false);

			status_ = 0;
		}

		// ユーザー名が入力されている場合...
		else {

			// 既存ユーザー名として登録されている場合...
			if (um.isUserRegistered(_inputUserName) == true) {

				// 既に登録されているユーザー名でゲームを開始する旨を表示
				SetUIVisible(_uiPanel, { IMAGE_TEXT3, TEXT_USER_NAME, TEXT_LIBRARY_STATUS, TEXT_PLAY_TOTAL_TIME, IMAGE_POPUP, BUTTON_NAME_OK,BUTTON_NAME_NO }, true);
				_uiPanel->PushButtonToArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_OK));
				_uiPanel->PushButtonToArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_NO));
				SetSelectableButton_ContinueAndStart(false);
				_uiPanel->SetButtonArrayIndex(failerButtonX, failerButtonY);

				// ユーザー情報を適応
				{
					// ユーザー名を適応
					((UIText*)_uiPanel->GetUIObject(TEXT_USER_NAME))->SetText(_inputUserName);

					// 図鑑の完成率を適応
					string completenessRateStr = std::to_string(um.GetLibraryCompletenessRate(_inputUserName)) + "%";
					((UIText*)_uiPanel->GetUIObject(TEXT_LIBRARY_STATUS))->SetText(completenessRateStr);

					// プレイ時間を適応
					string playTotalTimeStr; {
						int totalSec = um.GetPlayTotalTime(_inputUserName) ;

						int hour = totalSec / 3600; totalSec %= 3600;
						int min = (totalSec) / 60; totalSec %= 60;
						int sec = totalSec;

						std::ostringstream oss;
						oss << std::setw(2) << std::setfill('0') << hour << ":"
							<< std::setw(2) << std::setfill('0') << min << ":"
							<< std::setw(2) << std::setfill('0') << sec;

						playTotalTimeStr = oss.str();
					}
					((UIText*)_uiPanel->GetUIObject(TEXT_PLAY_TOTAL_TIME))->SetText(playTotalTimeStr);
				}

				// OKボタン,NOボタンを配列に追加

				status_ = 3;
			}

			// 既存ユーザー名として登録されていない場合...
			else {

				// 既存データが存在しない旨を表示
				SetUIVisible(_uiPanel, { IMAGE_TEXT4, IMAGE_POPUP, BUTTON_NAME_OK }, true);
				_uiPanel->PushButtonToArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_OK));
				_uiPanel->SetButtonArrayIndex(failerButtonX, failerButtonY);
				SetSelectableButton_ContinueAndStart(false);
				status_ = 4;
			}
		}
	}

	else if (_buttonName == BUTTON_NAME_OK) {
		
		_uiPanel->SetButtonArrayIndex(defaultButtonX, defaultButtonY);

		// 状態によって処理を分岐
		switch (status_) 
		{
		case 0: ClosePopup(_uiPanel);					break;
		case 1: ClosePopup(_uiPanel);					break;
		case 2: GameStart(&um,_inputUserName,true);		break;
		case 3: GameStart(&um, _inputUserName, false);	break;
		case 4: ClosePopup(_uiPanel);					break;
		}

		_uiPanel->RemoveButtonFromArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_OK));
		_uiPanel->RemoveButtonFromArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_NO));
		SetSelectableButton_ContinueAndStart(true);
	}

	else if (_buttonName == BUTTON_NAME_NO) {

		_uiPanel->SetButtonArrayIndex(defaultButtonX, defaultButtonY);
		_uiPanel->RemoveButtonFromArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_OK));
		_uiPanel->RemoveButtonFromArray((UIButton*)_uiPanel->GetUIObject(BUTTON_NAME_NO));
		SetSelectableButton_ContinueAndStart(true);
		ClosePopup(_uiPanel);
	}

	else if (_buttonName == BUTTON_NAME_END) {

		// ゲームを終了する
		PostQuitMessage(0);
	}
}

void Scene_Title::GameStart(UserManager* _userManager, string _userName, bool _isNewUser)
{
	// 新規ユーザーの場合はユーザーを登録
	if(_isNewUser)_userManager->RegisterUser(_userName);

	// ユーザーをログイン
	_userManager->LoginUser(_userName);

	// シーンを切り替える
	SceneManager* sceneManager = static_cast<SceneManager*>(FindObject("SceneManager"));
	sceneManager->ChangeScene(SCENE_ID_MENU, TID_BLACKOUT);
}

void Scene_Title::SetUIVisible(UIPanel* _uiPanel, vector<string> _uiObjectNames, bool _visible)
{
	// 指定されたUIオブジェクトを表示/非表示にする
	for (auto& uiObjectName : _uiObjectNames) {
		_uiPanel->GetUIObject(uiObjectName)->SetVisible(_visible);
	}
}

void Scene_Title::ClosePopup(UIPanel* _uiPanel)
{
	SetUIVisible(_uiPanel, {
		BUTTON_NAME_OK,
		BUTTON_NAME_NO, 
		IMAGE_POPUP,
		IMAGE_TEXT0,
		IMAGE_TEXT1,
		IMAGE_TEXT2,
		IMAGE_TEXT3,
		IMAGE_TEXT4,
		TEXT_USER_NAME,
		TEXT_LIBRARY_STATUS,
		TEXT_PLAY_TOTAL_TIME
	}, false);
}
