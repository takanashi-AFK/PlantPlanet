#pragma once
#include "UIObject.h"
#include "UIPanel.h"
#include "../Stage/Stage.h"


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

}