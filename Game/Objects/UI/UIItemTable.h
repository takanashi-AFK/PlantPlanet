#pragma once
#include "UIObject.h"
#include "UIPanel.h"


class UIItemTable :
	public UIObject
{

private:

	vector<UIObject*> itemTable_;
	XMFLOAT2 itemInterval_;
	UIPanel* itemPanel_ = UIPanel::GetInstance(); 

	int itemNum_;
	int startLayer_;
	string fileName_;
	int lineBreakCount_;
public:

	UIItemTable(string _name, UIObject* parent, int _layerNum);
	~UIItemTable();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void DrawData() override;
	std::string RemoveExtensionAndLastNumber(const std::string& filePath);
};

