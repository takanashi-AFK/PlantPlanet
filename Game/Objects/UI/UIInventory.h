#pragma once
#include "UIObject.h"
#include "UIPanel.h"
#include "../Stage/Stage.h"


namespace UIInventory
{
	void Initialize();
	void Update();
	void Draw();
	void SwitchInventory(bool isShow);
	void InventoryDataSet();
	void SetStage(Stage* pStage);
}