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
	// PopUp系かそれ以外かで分ける
	for (auto playUI : playUIList_) {
		if (playUI->GetObjectName() == "PLAY-PlayButton")
			playButton = (UIButton*)playUI;
		else if (playUI->GetObjectName() == "PLAY-POPUP-BACKGROUND")
			playBackGround = (UIImage*)playUI;

		else if (playUI->GetObjectName().starts_with("PLAY-POPUP"))
			popUpUIList_.push_back(playUI);

	}
	isFirstSelectButton_ = true;
}

void Scene_Menu::Update()
{
	// タブボタンの処理
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

	
	// LB,RBでタブを切り替える
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

	// DPADで通常ボタンを操作
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_UP)) {
		panel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::UP);
		isInputDPad_ = true;
	}
	else if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_DOWN)) {
		panel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::BOTTOM);
		isInputDPad_ = true;
	}
	else if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)) {
		panel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::LEFT);
		isInputDPad_ = true;
	}
	else if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) {
		panel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::RIGHT);
		isInputDPad_ = true;
	}


	// 各メニューの処理
	switch (currentMenuType)
	{
	case PLAY:Play();break;
	case INDEX:Index();break;
	case RANKING:Ranking();break;
	case OPTION:Option();break;
	default:
		break;
	}

	int x, y;
	panel->GetButtonIndex(&x, &y);
	ImGui::Text("x:%d y:%d", x, y);

	if (panel->GetSelectingButton() != nullptr) {
		ImGui::Text("SelectingButton:%s", panel->GetSelectingButton()->GetObjectName().c_str());
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
		for (auto tab : tabButtonList) {
			if (tab->GetObjectName() == "TAB-PlayButton")tab->SetImage("Images/MenuScene/PlayButton_Selected.png");
			else if (tab->GetObjectName() == "TAB-IndexButton")tab->SetImage("Images/MenuScene/IndexButton_UnSelected.png");
			else if (tab->GetObjectName() == "TAB-RankingButton")tab->SetImage("Images/MenuScene/RankingButton_UnSelected.png");
			else if (tab->GetObjectName() == "TAB-OptionButton")tab->SetImage("Images/MenuScene/OptionButton_UnSelected.png");
		}

		panel->ResetArrayOfButton();

		panel->PushButtonToArray(playButton);

		isFirstChange_ = false;

	}

	// playButtooにフォーカスする
	if (isPopUpMode_ == false && isInputDPad_ == true) {
		panel->SetButtonArrayIndex(1,1);
		isInputDPad_ = false;
	}

	// プレイボタンが押されたらポップアップを表示
	if (playButton->OnClick() || panel->GetSelectingButton() == playButton && Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) {

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
	if (isFirstChange_ == true) {

		for (const auto plant : g_playerPlantData) {
			plantDataMap_[plant.name_] = plant;
		}
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

		int plantDataSize = countedPlantData_.size();

		for (auto item : panel->GetUIObjects()) {

			if (item->GetObjectName().starts_with("INDEX-Plant")) {
				plantButtonList.push_back((UIButton*)item);
			}
		}

		for (auto button : plantButtonList) {

			// ここには本来、ゲットしていない花のシルエットが表示される
			button->SetImage("Models/tentativeFlowers/R1_flower_1.png");
			button->SetVisible(true); // 条件を満たした場合のみ表示

			// 対応するPlantDataがあるか確認
			for (auto [key, plantData] : plantDataMap_) {
				if (button->GetObjectName() == "INDEX-Plant" + std::to_string(plantData.id_)) {
					button->SetImage(plantData.imageFilePath_);
					break; // マッチしたらループ終了
				}
			}
		}

		for (auto tab : tabButtonList) {
			if (tab->GetObjectName() == "TAB-PlayButton")tab->SetImage("Images/MenuScene/PlayButton_UnSelected.png");
			else if (tab->GetObjectName() == "TAB-IndexButton")tab->SetImage("Images/MenuScene/IndexButton_Selected.png");
			else if (tab->GetObjectName() == "TAB-RankingButton")tab->SetImage("Images/MenuScene/RankingButton_UnSelected.png");
			else if (tab->GetObjectName() == "TAB-OptionButton")tab->SetImage("Images/MenuScene/OptionButton_UnSelected.png");
		}

		panel->ResetArrayOfButton();

		for(auto plantButton : plantButtonList)
		panel->PushButtonToArray(plantButton);


		descriptionImage->SetVisible(false);
		isFirstChange_ = false;
		isPopUpMode_ = false;

		panel->SetButtonArrayIndex(0, 0);
	}
	const string buttonObjectHeadName = "INDEX-Plant";

	for (auto plantButton : plantButtonList) {
		if (((UIButton*)plantButton)->OnClick() || panel->GetSelectingButton() == plantButton && Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) {

			// 対応するPlantDataがあるか確認
			for (auto [key, plantData] : plantDataMap_) {
				// その植物を持っていたら完璧な説明を表示
				if (plantButton->GetObjectName() == buttonObjectHeadName + std::to_string(plantData.id_)) {
					std::string buttonName = ((UIButton*)plantButton)->GetObjectName();

					std::string idString = buttonName.substr(buttonObjectHeadName.size());

					int id = std::stoi(idString);

					for (auto plant : g_playerPlantData) {
						if (plant.id_ == id) {
							std::string imagePath = plant.descriptionImageFilePath_;
							imagePath.erase(0, imagePath.find_first_not_of(' ')); // 先頭のスペースを削除
							imagePath.erase(imagePath.find_last_not_of(' ') + 1); // 末尾のスペースを削除

							descriptionImage->SetImage(imagePath.c_str());
							descriptionImage->SetVisible(true);
							break;
						}
					}
					break;
				}
				else { // その植物を持っていなかったら不完全な説明を表示
					

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
	//if (isFirstChange_ == true) {

	//	// UIパネルを取得
	//	UIPanel* panel = UIPanel::GetInstance();

	//	// ランキングデータを読み込む
	//	RankingManager::GetInstance().Load(RANKING_DATA_JSON);

	//	// ランキングデータを表示
	//	{
	//		// ランキングデータを取得 ※5位まで
	//		for (int i = 1; i <= 5; i++) {

	//			// ユーザーネームを取得
	//			{
	//				// UIテキスト名を取得
	//				std::string name = "rank" + std::to_string(i) + "_userName";
	//				UIText* text = (UIText*)panel->GetUIObject(name);

	//				// ユーザーネームを取得
	//				string userName = RankingManager::GetInstance().GetUser(i - 1);

	//				// テキストにユーザーネームを設定
	//				if (text != nullptr) text->SetText(userName);
	//			}

	//			// スコアを取得
	//			{
	//				// UIテキスト名を取得
	//				std::string name = "rank" + std::to_string(i) + "_scoreNum";
	//				UIText* text = (UIText*)panel->GetUIObject(name);

	//				// スコアを取得
	//				string score = std::to_string(RankingManager::GetInstance().GetScore(i - 1));

	//				// テキストにスコアを設定
	//				if (text != nullptr) text->SetText(score);
	//			}
	//		}
	//	}
	//}


}

void Scene_Menu::Option()
{
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

	for (auto popUpUI : popUpUIList_) {
		if (popUpUI->GetObjectName().starts_with("PLAY-POPUP-MODE")) {
			if (((UIButton*)popUpUI)->OnClick() ||
				((UIButton*)popUpUI) == panel->GetSelectingButton() && Input::IsPadButtonDown(XINPUT_GAMEPAD_A)) {

				// 押されたボタンがreturnだったら
				if (((UIButton*)popUpUI)->GetObjectName() == "PLAY-POPUP-MODE-RETURNBUTTON") {
					for (auto popUpUI : popUpUIList_) {

						panel->ResetArrayOfButton();
						panel->PushButtonToArray(playButton);
						popUpUI->SetVisible(false);
					}
					playBackGround->SetVisible(false);
					isPopUpMode_ = false;
				}
				else if (((UIButton*)popUpUI)->GetObjectName() == "PLAY-POPUP-MODE-ADV") {
					SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
					sceneManager->ChangeScene(SCENE_ID_PLAY, TID_BLACKOUT);
					isPopUpMode_ = false;
				}
				else if (((UIButton*)popUpUI)->GetObjectName() == "PLAY-POPUP-MODE-SCA") {
					isPopUpMode_ = false;
				}
				else if (((UIButton*)popUpUI)->GetObjectName() == "PLAY-POPUP-MODE-TUT") {
					isPopUpMode_ = false;
				}

			}
		}
	}
}
