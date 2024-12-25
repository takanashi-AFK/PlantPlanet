#pragma once
#include "UIObject.h"
#include "UIPanel.h"
#include "../Stage/Stage.h"


namespace UIInventory
{
	void Initialize();
	void Update();
	void SwitchInventory(bool isShow);
	void InventoryDataSet();
	void SetStage(Stage* pStage);
	void Release();
	bool Check();
	void Make();
	bool isEnd();
}