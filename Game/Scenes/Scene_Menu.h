#pragma once
#include "../../Engine/GameObject/GameObject.h"
#include <vector>
#include "../Objects/UI/UIImage.h"
#include "../Plants/Plant.h"
class UIButton;
class UIPanel;
class UIObject;
class Scene_Menu : public GameObject
{
private:

	enum MenuType
	{
		PLAY = 0,
		INDEX,
		RANKING,
		OPTION,
		MAX,
	}currentMenuType;
	UIPanel* panel;
	UIButton* playButton;
	UIButton* playReturnButton;
	UIButton* gameEndButton_;
	UIImage* backGround;
	UIImage* playBackGround;
	UIImage* descriptionImage;

	std::vector<PlantData> countedPlantData_;
	std::vector<UIButton*> plantButtonList;
	std::unordered_map<std::string, PlantData> plantDataMap_;

	std::vector<UIObject*> uiObject_;

	std::vector<UIButton*>	tabButtonList;
	std::vector<UIObject*>  playUIList_;
	std::vector<UIObject*>  popUpUIList_;
	std::vector<UIObject*>	indexUIList_;
	std::vector<UIObject*>	rankingUIList_;
	std::vector<UIObject*>	optionUIList_;
	bool isFirstChange_;
	bool isFirstSelectButton_;
	bool isInputDPad_;
	bool isPopUpMode_;
	bool isPopUpModeFirst_;

	int frameCount_;
public:
	/// <summary> �R���X�g���N�^ </summary>
	Scene_Menu(GameObject* _parent);
	/// <summary> ������ </summary>
	void Initialize() override;
	/// <summary> �X�V </summary>
	void Update() override;
	/// <summary> �`�� </summary>
	void Draw() override;
	/// <summary> ��� </summary>
	void Release() override;

private:
	void SetMenuType(MenuType _type) {isFirstChange_ = true; currentMenuType = _type; }

	//�@�e���j���[�̏���
	void Play();

	void Index();

	void Ranking();

	void Option();

//==========================================//
	// �|�b�v�A�b�v���[�h���̏���
	void PopUpMode();

	// DPAD�ł̑���
	void DPadMove();

	// LB.RB�ł̃^�u����
	void GamePadTabMove();

	// �^�u�{�^���̕ύX(�}�E�X�ł̑���)
	void MouseTabMove();

	// �^�u�{�^���̉摜�ύX
	void UpdateTabButtonImages(MenuType _menuType);

	bool ConfirmButton(UIButton* _button);

	bool IsButtonMouseOver(UIButton* _button);

};

