#pragma once
#include "UIObject.h"
#include "UIPanel.h"

class UIItemTable;

class UIInventory : public UIObject
{
	private:
	vector<UIObject*> itemTable_;
	UIPanel* itemPanel_ = UIPanel::GetInstance();

public:	
	
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void DrawData() override;
};

