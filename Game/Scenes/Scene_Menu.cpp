#include "Scene_Menu.h"
#include "../Objects/UI/UIPanel.h"
#include "../Objects/UI/UIButton.h"
#include "../Constants.h"
#include "../../Engine/SceneManager.h"
#include "../Plants/PlantCollection.h"
#include "../../Engine/ImGui/ImGui.h"
#include "../../Engine/DirectX/Input.h"
#include "../Otheres/RankingManager.h"
#include "../Objects/UI/UIText.h"
#include "../Otheres/UserManager.h"

using namespace Constants;


Scene_Menu::Scene_Menu(GameObject* _parent)
	: GameObject(_parent, "Scene_Menu"),
	currentMenuType(PLAY), 
	isFirstChange_(true),
	panel(nullptr), 
	playButton(nullptr),
	uiObject_({}),
	tabButtonList({})
{
}

void Scene_Menu::Initialize()
{
	// UIパネルを取得
	panel = UIPanel::GetInstance();

	// UILayout_jsonファイルを読み込む
	json loadData;
	if (JsonReader::Load(MENU_SCENE_UI_LAYOUT_JSON, loadData)) 
		panel->Load(loadData);

	// UIObjectを取得
	uiObject_ = panel->GetUIObjects();

	// すべてのオブジェクトを各リストに打ち分ける
	for (auto uiItem : uiObject_) {
		uiItem->SetVisible(true);
		if ((uiItem)->GetObjectName().starts_with("TAB")) { tabButtonList.push_back(((UIButton*)uiItem));  }
		else if (uiItem->GetObjectName() == "INDEX-DescriptionImage") { descriptionImage = (UIImage*)uiItem; }
		else if (uiItem->GetObjectName().starts_with("PLAY")) { playUIList_.push_back(uiItem);  }
		else if (uiItem->GetObjectName().starts_with("INDEX")) { indexUIList_.push_back(uiItem);  }
		else if (uiItem->GetObjectName().starts_with("RANKING")) { rankingUIList_.push_back(uiItem);  }
		else if (uiItem->GetObjectName().starts_with("OPTION")) { optionUIList_.push_back(uiItem); }
		else if (uiItem->GetObjectName() == "BackGround") { backGround = (UIImage*)uiItem; }
	}

	panel->ResetSelectedButton();
	isFirstSelectButton_ = true;
}

void Scene_Menu::Update()
{

	// タブボタンの操作
	MouseTabMove();
	GamePadTabMove();

	// ボタンのDPADでの操作
	DPadMove();

	// 各メニューの処理
	switch (currentMenuType)
	{
	case PLAY:Play();break;
	case INDEX:Index();break;
	case RANKING:Ranking();break;
	case OPTION:Option();break;
	}

}

void Scene_Menu::Draw()
{
}

void Scene_Menu::Release()
{
}

void Scene_Menu::Play()
{
	// 初回のみ処理
	if (isFirstChange_ == true) {

		// PopUp系かそれ以外かで分ける
		for (auto playUI : playUIList_) {
			if (playUI->GetObjectName() == "PLAY-PlayButton")
				playButton = (UIButton*)playUI;
			else if (playUI->GetObjectName() == "PLAY-POPUP-BACKGROUND")
				playBackGround = (UIImage*)playUI;
			else if (playUI->GetObjectName().starts_with("PLAY-POPUP"))
				popUpUIList_.push_back(playUI);
		}

		// 背景とプレイボタン以外のUIを非表示にする
		for (auto item : panel->GetUIObjects()) {
			if (std::find(tabButtonList.begin(), tabButtonList.end(), item) == tabButtonList.end() &&
				std::find(playUIList_.begin(), playUIList_.end(), item) == playUIList_.end() &&
				item != backGround ||
				item->GetObjectName().starts_with("PLAY-POPUP")) {
				item->SetVisible(false);
			}
			else {
				item->SetVisible(true);
			}
		};

		// タブボタンの画像を変更
		UpdateTabButtonImages(PLAY);

		panel->ResetArrayOfButton();

		panel->PushButtonToArray(playButton);

		isFirstChange_ = false;

	}

	// playButtonにフォーカスする
	if (isPopUpMode_ == false && isInputDPad_ == true) {
		panel->SetButtonArrayIndex(1,1);
		isInputDPad_ = false;
	}

	// プレイボタンが押されたらポップアップを表示
	if (ConfirmButton(playButton)) {
		isPopUpMode_ = true;
		isPopUpModeFirst_ = true;
		frameCount_ = 0;
	}

	// ポップアップ中の処理
	// 少しだけ遅らせてポップアップを表示(連続入力防止)
	if (isPopUpMode_ == true) {
		frameCount_++;
		if(frameCount_ >= 10)
		PopUpMode();
	}

}

void Scene_Menu::Index()
{
	static string imageNameHead = "INDEX-FrameButton";
	// StateがIndexに変わった時の処理
	if (isFirstChange_ == true) {


		// 検証にあたり、コメントアウト
		// globalから今まで取得したことのある植物のデータを取得
		for (const auto plant : g_playerPlantData) {
			plantDataMap_[plant.name_] = plant;
		}

		// 検証用※　`plantDataMap_`に現在ログイン中のユーザーの植物データを格納
		{
			// ユーザーマネージャーのインスタンスを取得
			UserManager& um = UserManager::GetInstance();

			// ログイン中のユーザーの植物データを取得
			for (int i = 0; i < PlantCollection::GetPlants().size(); i++) {
				if (um.GetLoggedInUser()->libraryStatus[i] == true) {
					PlantData plantData = PlantCollection::GetPlants()[i];
					plantDataMap_[plantData.name_] = plantData;
				}
			} 
		}

		// tab,index,background以外のUIを非表示にする
		for (auto item : panel->GetUIObjects()) {
			if (std::find(tabButtonList.begin(), tabButtonList.end(), item) == tabButtonList.end() &&
				std::find(indexUIList_.begin(), indexUIList_.end(), item) == indexUIList_.end() &&
				item != backGround) {
				item->SetVisible(false);
			}
			else {
				item->SetVisible(true);
			}
		}


		// カウントした植物のサイズ(何種類知ってるか)を取得
		int plantDataSize = countedPlantData_.size();

		// UIObjectを各リストに打ち分ける	
		for (auto item : panel->GetUIObjects()) {

			if (item->GetObjectName().starts_with(imageNameHead)) {
				plantFrameButtonList.push_back((UIButton*)item);
			}
			else if (item->GetObjectName().starts_with("INDEX-Plant")) {
				plantImageList.push_back((UIImage*)item);
			}
		}

		// 所持している花の画像を適用
		for (auto button : plantFrameButtonList) {
			
			button->SetVisible(true);
			// 対応するPlantDataがあるか確認
			for (auto [key, plantData] : plantDataMap_) {
				for(auto plantImage : plantImageList)
				if (plantImage->GetObjectName() == "INDEX-Plant" + std::to_string(plantData.id_)) {
					plantImage->SetImage(plantData.imageFilePath_);
					break; // マッチしたらループ終了
				}
			}
		}

		// タブボタンの画像を変更
		UpdateTabButtonImages(INDEX);

		panel->ResetArrayOfButton();

		for(auto plantButton : plantFrameButtonList)
			panel->PushButtonToArray(plantButton);

		UserManager& um = UserManager::GetInstance();
		g_playerPlantData.clear();
		for (int i = 0; i < 10; i++) {
			if (um.GetLoggedInUser()->libraryStatus[i] == true) {
				g_playerPlantData.push_back(PlantCollection::GetPlants()[i]);
			}
		}


		descriptionImage->SetVisible(false);
		isFirstChange_ = false;
		isPopUpMode_ = false;

		panel->SetButtonArrayIndex(0, 0);
	}

	for (auto item : plantFrameButtonList) {
		if (((UIButton*)item)->GetIsMouseOverThisButton()) {
			panel->ResetSelectedButton();
			int x, y;
			item->GetArrayPlace(&x,&y);
			panel->SetButtonArrayIndex(x, y);
		}
	}

	vector<UIObject*>list(plantFrameButtonList.begin(), plantFrameButtonList.end());
	UIButton* plantButton = UIButton::GetTopSelectingUI(list);
	
	if (plantButton == nullptr)plantButton = (UIButton*)panel->GetSelectingButton();

	for (auto& btn : plantFrameButtonList) {
		btn->SetAutoShader(false);
		btn->UnselectShader();
	}

	if (plantButton != nullptr) {

		plantButton->SelectShader();

		if (ConfirmButton(plantButton)) {
			bool havePlantFlag = false;
			std::string buttonName = ((UIButton*)plantButton)->GetObjectName();

			std::string idString = buttonName.substr(imageNameHead.size());

			int id = std::stoi(idString);

			if (g_playerPlantData.empty())
				havePlantFlag = false;

			for (auto havePlant : g_playerPlantData) {
				if (havePlant.id_ == id) {
					havePlantFlag = true;
					break;
				}
				else {
					havePlantFlag = false;
				}
			}

			for (auto plants : PlantCollection::GetPlants()) {
				if (plants.second.id_ == id) {
					if (havePlantFlag == true) {
						descriptionImage->SetImage(plants.second.descriptionImageFilePath_Complete_);
						descriptionImage->SetVisible(true);
					}

					else if (havePlantFlag == false) {
						descriptionImage->SetImage(plants.second.descriptionImageFilePath_InComplete_);
						descriptionImage->SetVisible(true);
					}
				}
			}
		}
	}

	//// 折り返す処理 うまくいかなに
	//int x, y;
	//panel->GetButtonIndex(&x, &y);
	//if(panel->GetSelectingButton() != nullptr){
	//	if (x == 4 && y == 0 && panel->GetSelectingButton()->GetObjectName() == "INDEX-Plant4" && Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) {
	//		//panel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::LEFT);
	//		//panel->SetSelectingButton(0, -1);
	//	}
	//	else if (x == 0 && y == -1 && Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)) {
	//		panel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::RIGHT);
	//		panel->SetSelectingButton(4, 0);
	//	}
	//
	//ImGui::Text("x:%d y:%d", x, y);
	//ImGui::Text("%s", panel->GetSelectingButton()->GetObjectName().c_str());
	//}
}

void Scene_Menu::Ranking()
{
	if (isFirstChange_ == true) {
		// タブボタンの画像を変更
		UpdateTabButtonImages(RANKING);

		// tab,ranking,background以外のUIを非表示にする
		for (auto item : panel->GetUIObjects()) {
			if (std::find(tabButtonList.begin(), tabButtonList.end(), item) == tabButtonList.end() &&
				std::find(rankingUIList_.begin(), rankingUIList_.end(), item) == rankingUIList_.end() &&
				item != backGround) {
				item->SetVisible(false);
			}
			else {
				item->SetVisible(true);
			}
		}

		// ユーザーマネージャーのインスタンスを取得
		UserManager& um = UserManager::GetInstance();

		// １位から５位までのユーザーデータを取得 & 表示
		for (int i = 1; i <= 5; i++) {

			// ユーザー情報を取得
			UserInfo* user = um.GetUser(i);

			// ユーザー情報が存在しない場合は処理をスキップ
			if (user == nullptr) break;

			// ユーザーネームを取得
			{
				// UIテキストを取得
				std::string name = "RANKING-rank_userName" + std::to_string(i);
				UIText* text = (UIText*)panel->GetUIObject(name);

				// テキストにユーザーネームを設定
				if (text != nullptr) text->SetText(user->userName);
			}

			// スコアを取得
			{
				// UIテキスト名を取得
				std::string name = "RANKING-rank_scoreNum" + std::to_string(i);
				UIText* text = (UIText*)panel->GetUIObject(name);

				// テキストにスコアを設定
				if (text != nullptr) text->SetText(std::to_string(user->bestScore));
			}
		}

		// ログイン中のユーザーのベストデータを取得 & 表示
		{
			// ユーザー情報を取得
			UserInfo* user = um.GetLoggedInUser();

			// ユーザー情報が存在しない場合は処理をスキップ
			if (user == nullptr) return;

			// ユーザーネームを取得
			{
				// UIテキストを取得
				std::string name = "RANKING-rank_userNameYou";
				UIText* text = (UIText*)panel->GetUIObject(name);

				// テキストにユーザーネームを設定
				if (text != nullptr) text->SetText(user->userName);
			}

			// スコアを取得
			{
				// UIテキスト名を取得
				std::string name = "RANKING-rank_scoreNumYou";
				UIText* text = (UIText*)panel->GetUIObject(name);

				// テキストにスコアを設定
				if (text != nullptr) text->SetText(std::to_string(user->bestScore));
			}
		}

		isFirstChange_ = false;
	}
}

void Scene_Menu::Option()
{
	if (isFirstChange_ == true) {
		// タブボタンの画像を変更
		UpdateTabButtonImages(OPTION);

		// 背景とプレイボタン以外のUIを非表示にする
		for (auto item : panel->GetUIObjects()) {
			if (std::find(tabButtonList.begin(), tabButtonList.end(), item) == tabButtonList.end() &&
				item != backGround &&
				item->GetObjectName().starts_with("OPTION") == false) {
				item->SetVisible(false);
			}
			else {
				item->SetVisible(true);
				if(item->GetObjectName() == "OPTION-GameEndButton")
					gameEndButton_ = (UIButton*)item;

			}

		}

		panel->ResetArrayOfButton();
	
		if(gameEndButton_ != nullptr)
		panel->PushButtonToArray(gameEndButton_);

	}
	// endにフォーカスする
	if (isInputDPad_ == true) {
		panel->SetButtonArrayIndex(1, 1);
		isInputDPad_ = false;
	}

	if(ConfirmButton(gameEndButton_)){
		SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
		sceneManager->ChangeScene(SCENE_ID_END, TID_BLACKOUT);

	}

}

void Scene_Menu::PopUpMode()
{
	if (isPopUpModeFirst_ == true) {
		panel->ResetArrayOfButton();
		
		for(auto button	: popUpUIList_)
			panel->PushButtonToArray((UIButton*)button);

		panel->SetButtonArrayIndex(0,0);
		isPopUpModeFirst_ = false;
		for (auto popUpUI : popUpUIList_) {
			popUpUI->SetVisible(true);
		}
		playBackGround->SetVisible(true);

	}

	for (auto item : popUpUIList_) {
		if (((UIButton*)item)->GetIsMouseOverThisButton()) {
			panel->ResetSelectedButton();
			int x, y;
			((UIButton*)item)->GetArrayPlace(&x, &y);
			panel->SetButtonArrayIndex(x, y);

		}
	}
	for (auto popUpUI : popUpUIList_) {
		if (ConfirmButton((UIButton*)popUpUI)) {

			// 押されたボタンがreturnだったら
			if (((UIButton*)popUpUI)->GetObjectName() == "PLAY-POPUP-MODE-RETURNBUTTON") {
				for (auto pop : popUpUIList_) {
					// ボタン選択対象をリセット後、playButtonを挿入
					panel->ResetArrayOfButton();
					panel->PushButtonToArray(playButton);
					// ポップアップUIを非表示にする
					pop->SetVisible(false);
				}
				playBackGround->SetVisible(false);
				isPopUpMode_ = false;
			}
			else if (((UIButton*)popUpUI)->GetObjectName() == "PLAY-POPUP-MODE-ADV") {

				// モードをADVに変更
				g_gameMode = GameMode::MODE_ADVENTURE;

				// シーンを切り替える
				SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
				sceneManager->ChangeScene(SCENE_ID_PLAY, TID_BLACKOUT);

				// ポップアップUIを非表示にする
				for(auto pop : popUpUIList_)
					pop->SetVisible(false);
				playBackGround->SetVisible(false);

				isPopUpMode_ = false;
			}
			else if (((UIButton*)popUpUI)->GetObjectName() == "PLAY-POPUP-MODE-SCA") {

				// モードをSCAに変更
				g_gameMode = GameMode::MODE_SCOREATTACK;

				// シーンを切り替える
				SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
				sceneManager->ChangeScene(SCENE_ID_PLAY, TID_BLACKOUT);

				// ポップアップUIを非表示にする
				for (auto pop : popUpUIList_)
					pop->SetVisible(false);		
				playBackGround->SetVisible(false);

				isPopUpMode_ = false;
			}
			else if (((UIButton*)popUpUI)->GetObjectName() == "PLAY-POPUP-MODE-TUT") {

				// モードをTUTに変更
				g_gameMode = GameMode::MODE_TUTORIAL;

				// ポップアップUIを非表示にする
				for (auto pop : popUpUIList_)
					pop->SetVisible(false);
				playBackGround->SetVisible(false);

				isPopUpMode_ = false;
			}
		}
	}
}

void Scene_Menu::DPadMove()
{
	// DPADで通常ボタンを操作
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_UP)) { // 上
		panel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::UP);
		isInputDPad_ = true;
	}
	else if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_DOWN)) { // 下
		panel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::BOTTOM);
		isInputDPad_ = true;
	}
	else if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)) { // 左
		panel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::LEFT);
		isInputDPad_ = true;
	}
	else if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) { // 右
		panel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::RIGHT);
		isInputDPad_ = true;
	}
}

void Scene_Menu::GamePadTabMove()
{
	// タブボタンの変更(コントローラ操作)
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		// プレイ(一番左)の場合、左には移動しない
		if (currentMenuType != PLAY) {
			SetMenuType((MenuType)((currentMenuType - 1)));
		}
	}
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		// オプション(一番右)の場合、右には移動しない
		if (currentMenuType != OPTION) {
			SetMenuType((MenuType)((currentMenuType + 1)));
		}
	}
}

void Scene_Menu::MouseTabMove()
{
	// タブボタンの変更(マウスでの操作)
	for (auto tabButton : tabButtonList) {
		if (tabButton->OnClick()) {
			const std::string& objectName = tabButton->GetObjectName();


			if (objectName == "TAB-PlayButton" && currentMenuType != PLAY) {
				SetMenuType(PLAY);
			}
			else if (objectName == "TAB-IndexButton" && currentMenuType != INDEX) {
				SetMenuType(INDEX);
			}
			else if (objectName == "TAB-RankingButton" && currentMenuType != RANKING) {
				SetMenuType(RANKING);
			}
			else if (objectName == "TAB-OptionButton" && currentMenuType != OPTION) {
				SetMenuType(OPTION);
			}
		}
	}

}

void Scene_Menu::UpdateTabButtonImages(MenuType _menuType)
{

	for (auto tab : tabButtonList)
	{
		std::string imagePath;
		if (tab->GetObjectName() == "TAB-PlayButton")
			imagePath = (_menuType == PLAY) ? "Images/MenuScene/common/05_playTabSelected.png" : "Images/MenuScene/common/01_playTab.png";
		else if (tab->GetObjectName() == "TAB-IndexButton")
			imagePath = (_menuType == INDEX) ? "Images/MenuScene/common/06_libraryTabSelected.png" : "Images/MenuScene/common/02_libraryTab.png";
		else if (tab->GetObjectName() == "TAB-RankingButton")
			imagePath = (_menuType == RANKING) ? "Images/MenuScene/common/07_rankingTabSelected.png" : "Images/MenuScene/common/03_rankingTab.png";
		else if (tab->GetObjectName() == "TAB-OptionButton")
			imagePath = (_menuType == OPTION) ? "Images/MenuScene/common/08_optionTabSelected.png" : "Images/MenuScene/common/04_optionTab.png";

		tab->SetImage(imagePath);
	}

}

bool Scene_Menu::ConfirmButton(UIButton* _button)
{
	return _button->OnClick() || panel->GetSelectingButton() == _button && Input::IsPadButtonDown(XINPUT_GAMEPAD_A);
}
