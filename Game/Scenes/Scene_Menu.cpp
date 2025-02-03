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
	// UI�p�l�����擾
	panel = UIPanel::GetInstance();

	// UILayout_json�t�@�C����ǂݍ���
	json loadData;
	if (JsonReader::Load(MENU_SCENE_UI_LAYOUT_JSON, loadData)) 
		panel->Load(loadData);

	// UIObject���擾
	uiObject_ = panel->GetUIObjects();

	// ���ׂẴI�u�W�F�N�g���e���X�g�ɑł�������
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

	// �^�u�{�^���̑���
	MouseTabMove();
	GamePadTabMove();

	// �{�^����DPAD�ł̑���
	DPadMove();

	// �e���j���[�̏���
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
	// ����̂ݏ���
	if (isFirstChange_ == true) {

		// PopUp�n������ȊO���ŕ�����
		for (auto playUI : playUIList_) {
			if (playUI->GetObjectName() == "PLAY-PlayButton")
				playButton = (UIButton*)playUI;
			else if (playUI->GetObjectName() == "PLAY-POPUP-BACKGROUND")
				playBackGround = (UIImage*)playUI;
			else if (playUI->GetObjectName().starts_with("PLAY-POPUP"))
				popUpUIList_.push_back(playUI);
		}

		// �w�i�ƃv���C�{�^���ȊO��UI���\���ɂ���
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

		// �^�u�{�^���̉摜��ύX
		UpdateTabButtonImages(PLAY);

		panel->ResetArrayOfButton();

		panel->PushButtonToArray(playButton);

		isFirstChange_ = false;

	}

	// playButton�Ƀt�H�[�J�X����
	if (isPopUpMode_ == false && isInputDPad_ == true) {
		panel->SetButtonArrayIndex(1,1);
		isInputDPad_ = false;
	}

	// �v���C�{�^���������ꂽ��|�b�v�A�b�v��\��
	if (ConfirmButton(playButton)) {
		isPopUpMode_ = true;
		isPopUpModeFirst_ = true;
		frameCount_ = 0;
	}

	// �|�b�v�A�b�v���̏���
	// ���������x�点�ă|�b�v�A�b�v��\��(�A�����͖h�~)
	if (isPopUpMode_ == true) {
		frameCount_++;
		if(frameCount_ >= 10)
		PopUpMode();
	}

}

void Scene_Menu::Index()
{
	static string imageNameHead = "INDEX-FrameButton";
	// State��Index�ɕς�������̏���
	if (isFirstChange_ == true) {


		// ���؂ɂ�����A�R�����g�A�E�g
		// global���獡�܂Ŏ擾�������Ƃ̂���A���̃f�[�^���擾
		for (const auto plant : g_playerPlantData) {
			plantDataMap_[plant.name_] = plant;
		}

		// ���ؗp���@`plantDataMap_`�Ɍ��݃��O�C�����̃��[�U�[�̐A���f�[�^���i�[
		{
			// ���[�U�[�}�l�[�W���[�̃C���X�^���X���擾
			UserManager& um = UserManager::GetInstance();

			// ���O�C�����̃��[�U�[�̐A���f�[�^���擾
			for (int i = 0; i < PlantCollection::GetPlants().size(); i++) {
				if (um.GetLoggedInUser()->libraryStatus[i] == true) {
					PlantData plantData = PlantCollection::GetPlants()[i];
					plantDataMap_[plantData.name_] = plantData;
				}
			} 
		}

		// tab,index,background�ȊO��UI���\���ɂ���
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


		// �J�E���g�����A���̃T�C�Y(����ޒm���Ă邩)���擾
		int plantDataSize = countedPlantData_.size();

		// UIObject���e���X�g�ɑł�������	
		for (auto item : panel->GetUIObjects()) {

			if (item->GetObjectName().starts_with(imageNameHead)) {
				plantFrameButtonList.push_back((UIButton*)item);
			}
			else if (item->GetObjectName().starts_with("INDEX-Plant")) {
				plantImageList.push_back((UIImage*)item);
			}
		}

		// �������Ă���Ԃ̉摜��K�p
		for (auto button : plantFrameButtonList) {
			
			button->SetVisible(true);
			// �Ή�����PlantData�����邩�m�F
			for (auto [key, plantData] : plantDataMap_) {
				for(auto plantImage : plantImageList)
				if (plantImage->GetObjectName() == "INDEX-Plant" + std::to_string(plantData.id_)) {
					plantImage->SetImage(plantData.imageFilePath_);
					break; // �}�b�`�����烋�[�v�I��
				}
			}
		}

		// �^�u�{�^���̉摜��ύX
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

	//// �܂�Ԃ����� ���܂������Ȃ�
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
		// �^�u�{�^���̉摜��ύX
		UpdateTabButtonImages(RANKING);

		// tab,ranking,background�ȊO��UI���\���ɂ���
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

		// ���[�U�[�}�l�[�W���[�̃C���X�^���X���擾
		UserManager& um = UserManager::GetInstance();

		// �P�ʂ���T�ʂ܂ł̃��[�U�[�f�[�^���擾 & �\��
		for (int i = 1; i <= 5; i++) {

			// ���[�U�[�����擾
			UserInfo* user = um.GetUser(i);

			// ���[�U�[��񂪑��݂��Ȃ��ꍇ�͏������X�L�b�v
			if (user == nullptr) break;

			// ���[�U�[�l�[�����擾
			{
				// UI�e�L�X�g���擾
				std::string name = "RANKING-rank_userName" + std::to_string(i);
				UIText* text = (UIText*)panel->GetUIObject(name);

				// �e�L�X�g�Ƀ��[�U�[�l�[����ݒ�
				if (text != nullptr) text->SetText(user->userName);
			}

			// �X�R�A���擾
			{
				// UI�e�L�X�g�����擾
				std::string name = "RANKING-rank_scoreNum" + std::to_string(i);
				UIText* text = (UIText*)panel->GetUIObject(name);

				// �e�L�X�g�ɃX�R�A��ݒ�
				if (text != nullptr) text->SetText(std::to_string(user->bestScore));
			}
		}

		// ���O�C�����̃��[�U�[�̃x�X�g�f�[�^���擾 & �\��
		{
			// ���[�U�[�����擾
			UserInfo* user = um.GetLoggedInUser();

			// ���[�U�[��񂪑��݂��Ȃ��ꍇ�͏������X�L�b�v
			if (user == nullptr) return;

			// ���[�U�[�l�[�����擾
			{
				// UI�e�L�X�g���擾
				std::string name = "RANKING-rank_userNameYou";
				UIText* text = (UIText*)panel->GetUIObject(name);

				// �e�L�X�g�Ƀ��[�U�[�l�[����ݒ�
				if (text != nullptr) text->SetText(user->userName);
			}

			// �X�R�A���擾
			{
				// UI�e�L�X�g�����擾
				std::string name = "RANKING-rank_scoreNumYou";
				UIText* text = (UIText*)panel->GetUIObject(name);

				// �e�L�X�g�ɃX�R�A��ݒ�
				if (text != nullptr) text->SetText(std::to_string(user->bestScore));
			}
		}

		isFirstChange_ = false;
	}
}

void Scene_Menu::Option()
{
	if (isFirstChange_ == true) {
		// �^�u�{�^���̉摜��ύX
		UpdateTabButtonImages(OPTION);

		// �w�i�ƃv���C�{�^���ȊO��UI���\���ɂ���
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
	// end�Ƀt�H�[�J�X����
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

			// �����ꂽ�{�^����return��������
			if (((UIButton*)popUpUI)->GetObjectName() == "PLAY-POPUP-MODE-RETURNBUTTON") {
				for (auto pop : popUpUIList_) {
					// �{�^���I��Ώۂ����Z�b�g��AplayButton��}��
					panel->ResetArrayOfButton();
					panel->PushButtonToArray(playButton);
					// �|�b�v�A�b�vUI���\���ɂ���
					pop->SetVisible(false);
				}
				playBackGround->SetVisible(false);
				isPopUpMode_ = false;
			}
			else if (((UIButton*)popUpUI)->GetObjectName() == "PLAY-POPUP-MODE-ADV") {

				// ���[�h��ADV�ɕύX
				g_gameMode = GameMode::MODE_ADVENTURE;

				// �V�[����؂�ւ���
				SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
				sceneManager->ChangeScene(SCENE_ID_PLAY, TID_BLACKOUT);

				// �|�b�v�A�b�vUI���\���ɂ���
				for(auto pop : popUpUIList_)
					pop->SetVisible(false);
				playBackGround->SetVisible(false);

				isPopUpMode_ = false;
			}
			else if (((UIButton*)popUpUI)->GetObjectName() == "PLAY-POPUP-MODE-SCA") {

				// ���[�h��SCA�ɕύX
				g_gameMode = GameMode::MODE_SCOREATTACK;

				// �V�[����؂�ւ���
				SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
				sceneManager->ChangeScene(SCENE_ID_PLAY, TID_BLACKOUT);

				// �|�b�v�A�b�vUI���\���ɂ���
				for (auto pop : popUpUIList_)
					pop->SetVisible(false);		
				playBackGround->SetVisible(false);

				isPopUpMode_ = false;
			}
			else if (((UIButton*)popUpUI)->GetObjectName() == "PLAY-POPUP-MODE-TUT") {

				// ���[�h��TUT�ɕύX
				g_gameMode = GameMode::MODE_TUTORIAL;

				// �|�b�v�A�b�vUI���\���ɂ���
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
	// DPAD�Œʏ�{�^���𑀍�
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_UP)) { // ��
		panel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::UP);
		isInputDPad_ = true;
	}
	else if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_DOWN)) { // ��
		panel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::BOTTOM);
		isInputDPad_ = true;
	}
	else if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)) { // ��
		panel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::LEFT);
		isInputDPad_ = true;
	}
	else if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) { // �E
		panel->SelectorMove(UIPanel::SELECTOR_MOVE_TO::RIGHT);
		isInputDPad_ = true;
	}
}

void Scene_Menu::GamePadTabMove()
{
	// �^�u�{�^���̕ύX(�R���g���[������)
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_LEFT_SHOULDER)) {
		// �v���C(��ԍ�)�̏ꍇ�A���ɂ͈ړ����Ȃ�
		if (currentMenuType != PLAY) {
			SetMenuType((MenuType)((currentMenuType - 1)));
		}
	}
	if (Input::IsPadButtonDown(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		// �I�v�V����(��ԉE)�̏ꍇ�A�E�ɂ͈ړ����Ȃ�
		if (currentMenuType != OPTION) {
			SetMenuType((MenuType)((currentMenuType + 1)));
		}
	}
}

void Scene_Menu::MouseTabMove()
{
	// �^�u�{�^���̕ύX(�}�E�X�ł̑���)
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
