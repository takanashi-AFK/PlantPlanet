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
#ifdef _DEBUG
	previewImage_ = itemPanel_->CreateUIObject("previewImage", UIType::UI_IMAGE, itemPanel_, 100);
	((UIImage*)previewImage_)->SetImage("Images/DebugImage/ItemTablePreviewPos.png");
#endif // _DEBUG
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

	ImGui::InputInt("ItemNum", &itemNum_);

	ImGui::InputInt("StartLayer", &startLayer_);

	ImGui::InputInt("LineBreakCount", &lineBreakCount_);

	ImGui::InputFloat2("ItemInterval", &itemInterval_.x);

	ImGui::DragFloat3("PreviewPos", &previewTransform_.position_.x, 0.01f);
	((UIImage*)previewImage_)->SetTrasform(previewTransform_);

	ImGui::DragFloat3("ItemSize", &itemTransform_.scale_.x, 0.01f);

	if (ImGui::Button("Create")) {


		if (itemNum_ <= 0) {
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "ItemNum is invalid.");
			return;
		}
		if (lineBreakCount_ == 0) {
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "everybody can't divide by 0!");
			return;
		}

		// グリッド配置用の初期位置
		XMFLOAT3 currentPos = previewTransform_.position_;

		for (int i = 0; i < itemNum_; i++) {
			// UIObject を作成してテーブルに追加
			UIObject* newItem = CreateUIObject(nameBuffer + std::to_string(i), uitype, itemPanel_, startLayer_ + i + 1);
			itemTable_.push_back(newItem);

			// Transform を取得して位置を設定
			itemTransform_.position_ = { currentPos.x, currentPos.y, 0.0f };

			// 折り返しチェック
			if ((i + 1) % lineBreakCount_ == 0) {
				// 行が折り返されたら X をリセットし、Y を更新
				currentPos.x = previewTransform_.position_.x;
				currentPos.y -= itemInterval_.y;
			}
			else {
				// 次のアイテムの X 座標を更新
				currentPos.x += itemInterval_.x;
			}

			// UIImage 特有の設定
			if (uitype == UIType::UI_IMAGE) {
				((UIImage*)itemTable_[i])->SetImage("Images/TT.png");
				((UIImage*)itemTable_[i])->SetTrasform(itemTransform_);
			}
			else if (uitype == UIType::UI_BUTTON) {
				((UIButton*)itemTable_[i])->SetImage("Images/TT.png");
				((UIButton*)itemTable_[i])->SetTrasform(itemTransform_);
			}

			// Transform を適用
			newItem->SetTrasform(itemTransform_);
		}
	}


}
