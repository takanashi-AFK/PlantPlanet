#pragma once
#include "UIObject.h"
#include "UIPanel.h"


class UIItemTable :
	public UIObject
{

private:

	vector<UIObject*> itemTable_;
	UIPanel* itemPanel_ = UIPanel::GetInstance(); 

	XMFLOAT2 itemInterval_;
	int itemNum_;
	int startLayer_;
	string fileName_;
	int lineBreakCount_;
	UIObject* previewImage_;
	Transform previewTransform_;
public:

	UIItemTable(string _name, UIObject* parent, int _layerNum);
	~UIItemTable();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void DrawData() override;

	vector<UIObject*> GetItemTable() { return itemTable_; }

};

