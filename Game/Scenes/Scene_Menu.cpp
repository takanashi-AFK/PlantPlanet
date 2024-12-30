#include "Scene_Menu.h"
#include "../Objects/UI/UIPanel.h"
#include "../Objects/UI/UIButton.h"
#include "../Constants.h"
#include "../../Engine/SceneManager.h"
#include "../Plants/PlantCollection.h"
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

	uiObject_ = panel->GetUIObjects();

	// ���ׂẴI�u�W�F�N�g���e���X�g�ɑł�������
	for (auto uiItem : uiObject_) {
		uiItem->SetVisible(true);
		if ((uiItem)->GetObjectName().starts_with("TAB"))		tabButtonList.push_back(((UIButton*)uiItem));
		else if (uiItem->GetObjectName().starts_with("PLAY"))	playUIList_.push_back(uiItem);
		else if (uiItem->GetObjectName().starts_with("INDEX"))	indexUIList_.push_back(uiItem);
		else if (uiItem->GetObjectName().starts_with("RANKING"))rankingUIList_.push_back(uiItem);
		else if (uiItem->GetObjectName().starts_with("OPTION"))	optionUIList_.push_back(uiItem);
		else if (uiItem->GetObjectName() == "BackGround")		backGround = (UIImage*)uiItem;
	}


}

void Scene_Menu::Update()
{
	// �^�u�{�^���̏���
	for (auto tabButton : tabButtonList) {
		if (tabButton->OnClick() || tabButton == panel->GetSelectingButton()) {
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

	// �e���j���[�̏���
	switch (currentMenuType)
	{
	case PLAY:Play();break;
	case INDEX:Index();break;
	case RANKING:Ranking();break;
	case OPTION:Option();break;
	default:
		break;
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

		for (auto playUI : playUIList_) {
			if (playUI->GetObjectName() == "PLAY-PlayButton")
				playButton = (UIButton*)playUI;
			else if (playUI->GetObjectName() == "PLAY-POPUP-BACKGROUND")
				playBackGround = (UIImage*)playUI;
			else if (playUI->GetObjectName() == "PLAY-POPUP-MODE-RETURNBUTTON")
				playReturnButton = (UIButton*)playUI;


			if (playUI->GetObjectName().starts_with("PLAY-POPUP"))
				popUpUIList_.push_back(playUI);

		}
		isFirstChange_ = false;

	}

	// �v���C�{�^���������ꂽ��|�b�v�A�b�v��\��

	if (playButton->OnClick()) {
		for (auto popUpUI : popUpUIList_) {
			popUpUI->SetVisible(true);
		}
	}


	for (auto popUpUI : popUpUIList_) {
		if (popUpUI->GetObjectName().starts_with("PLAY-POPUP-MODE"))
			if (((UIButton*)popUpUI)->OnClick()) {

				// �����ꂽ�{�^����return��������
				if (((UIButton*)popUpUI)->GetObjectName() == "PLAY-POPUP-MODE-RETURNBUTTON") {
					for (auto popUpUI : popUpUIList_) {
						popUpUI->SetVisible(false);
					}
				}
				else if (((UIButton*)popUpUI)->GetObjectName() == "PLAY-POPUP-MODE-ADV") {
					SceneManager* sceneManager = (SceneManager*)FindObject("SceneManager");
					sceneManager->ChangeScene(SCENE_ID_PLAY, TID_BLACKOUT);
				}
				else if (((UIButton*)popUpUI)->GetObjectName() == "PLAY-POPUP-MODE-SCA") {

				}
				else if (((UIButton*)popUpUI)->GetObjectName() == "PLAY-POPUP-MODE-TUT") {
				}

			}
	}
}

void Scene_Menu::Index()
{
	if (isFirstChange_ == true) {

		std::unordered_map<std::string, PlantData> plantDataMap;
		for (const auto plant : g_playerPlantData) {
			plantDataMap[plant.name_] = plant;
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
		std::vector<UIButton*> plantButtonList;

		for (auto item : panel->GetUIObjects()) {

			if (item->GetObjectName().starts_with("INDEX-Plant")) {
				plantButtonList.push_back((UIButton*)item);
			}
		}

		for (auto button : plantButtonList) {
			// ������Ԃ��\���ɐݒ�
			button->SetVisible(false);

			// �Ή�����PlantData�����邩�m�F
			for ( auto [key, plantData] : plantDataMap) {
				if (button->GetObjectName() == "INDEX-Plant" + std::to_string(plantData.id_)) {
					button->SetImage(plantData.imageFilePath_);
					button->SetVisible(true); // �����𖞂������ꍇ�̂ݕ\��
					break; // �}�b�`�����烋�[�v�I��
				}
			}
		}

		isFirstChange_ = false;
	}
}

void Scene_Menu::Ranking()
{
}

void Scene_Menu::Option()
{
}
