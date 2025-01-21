#pragma once
#include "UIObject.h"
#include "UIPanel.h"
#include "../Stage/Stage.h"

class UIButton;
namespace UIInventory
{
	void Initialize();
	void Update();
	void ShowInventory(bool isShow);
	void InventoryDataSet();
	void SetStage(Stage* pStage);
	void Release();
	bool Check();
	void Make();
	bool IsShowInventory();
	bool IsMadeSalad();

	// A functions that make salad from history
	void MakeFromHistory();

	bool Confirm(UIButton* _button);

	/// <summary>
	/// 選択可能なボタンのXが一番若いものを取得
	/// </summary>
	/// <param name="_targetVector">どの配列から探すか</param>
	int GetLowestButtonArrayX(std::vector<UIObject*> _targetVector);

	bool IsFirstSelectableButton();

	void InventorySelectorMove(UIPanel::SELECTOR_MOVE_TO way);
}