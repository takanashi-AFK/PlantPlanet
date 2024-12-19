#include "UIItemTable.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/Global.h"
#include "UIImage.h"
#include "UIButton.h"

UIItemTable::UIItemTable(string _name, UIObject* parent, int _layerNum)
: UIObject(_name, UIType::UI_ITEMTABLE,parent, _layerNum)
,  itemInterval_(0.1,0.1), lineBreakCount_(0),fileName_("")
{
}

UIItemTable::~UIItemTable()
{
}

void UIItemTable::Initialize()
{
}

void UIItemTable::Update()
{
}

void UIItemTable::Draw()
{
}

void UIItemTable::Release()
{
}

void UIItemTable::DrawData()
{
	static char nameBuffer[256] = "";
	// 名前を入力
	ImGui::InputTextWithHint(":setting name", "Input object name...", nameBuffer, IM_ARRAYSIZE(nameBuffer));

	// タイプを選択
	static UIType uitype = UIType::UI_NONE;	// 初期選択項目
	static std::string type = "NONE";		// 初期選択項目

	if (ImGui::BeginCombo(":setting type", type.c_str())) {
		for (int i = 0; i < UIType::UI_MAX; i++) {
			std::string uiTypeString = UIObject::GetUITypeString((UIType)i);
			if (uiTypeString.empty()) continue; // 空文字列を無視

			bool isSelected = (type == UIObject::GetUITypeString((UIType)i));
			if (ImGui::Selectable(UIObject::GetUITypeString((UIType)i).c_str(), isSelected)) {
				type = UIObject::GetUITypeString((UIType)i);
				uitype = (UIType)i;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	static int itemNum = 0;
	ImGui::InputInt("ItemNum", &itemNum);

	static int layer = 0;
	ImGui::InputInt("StartLayer", &layer);

	static int lineBreakCount = 0;
	ImGui::InputInt("LineBreakCount", &lineBreakCount);

	if (ImGui::Button("Create")) {
		itemNum_ = itemNum;
		lineBreakCount_ = lineBreakCount;
		startLayer_ = layer;

		if (itemNum <= 0) {
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "ItemNum is invalid.");
			return;
		}
		if (lineBreakCount_ == 0) {
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "everybody cant 0 Div!");
			return;
		}

		for (int i = 0; i < itemNum; i++) {
			itemTable_.push_back(CreateUIObject(nameBuffer + std::to_string(i), uitype, this, layer + i + 1));
			Transform itemTransform = itemTable_[i]->GetTransform();
			itemTransform.position_ = {itemInterval_.x * i, itemTransform.position_.y, 0.0f};
			if(i % lineBreakCount_ == 0) {
				itemTransform.position_ = { itemTransform.position_.x, itemTransform.position_.y + (itemInterval_.y * i), 0.0f };
			}

			if(uitype == UIType::UI_IMAGE) {
				((UIImage*)itemTable_[i])->SetImage("Images/TT.png");
				itemTransform.scale_ = {0.1f, 0.1f, 0.0f};
				((UIImage*)itemTable_[i])->SetTrasform(itemTransform);
			}
			
			itemTable_[i]->SetTrasform(itemTransform);
		}
	}

}
