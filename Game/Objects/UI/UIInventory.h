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
	void SetsAlpha(uint8_t alpha);

	void SetFade(float fade);
	float GetFade();

	// A functions that make salad from history
	void MakeFromHistory();

	bool Confirm(UIButton* _button);
}