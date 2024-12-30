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
		PLAY,
		INDEX,
		RANKING,
		OPTION,
		MAX,
	}currentMenuType;
	UIPanel* panel;
	UIButton* playButton;
	UIButton* playReturnButton;
	UIImage* backGround;
	UIImage* playBackGround;
	std::vector<PlantData> countedPlantData_;

	std::vector<UIObject*> uiObject_;

	std::vector<UIButton*>	tabButtonList;
	std::vector<UIObject*>  playUIList_;
	std::vector<UIObject*>  popUpUIList_;
	std::vector<UIObject*>	indexUIList_;
	std::vector<UIObject*>	rankingUIList_;
	std::vector<UIObject*>	optionUIList_;
	bool isFirstChange_;
public:
	/// <summary> コンストラクタ </summary>
	Scene_Menu(GameObject* _parent);
	/// <summary> 初期化 </summary>
	void Initialize() override;
	/// <summary> 更新 </summary>
	void Update() override;
	/// <summary> 描画 </summary>
	void Draw() override;
	/// <summary> 解放 </summary>
	void Release() override;

private:
	void SetMenuType(MenuType _type) { isFirstChange_ = true; currentMenuType = _type; }

	void Play();

	void Index();

	void Ranking();

	void Option();
};

