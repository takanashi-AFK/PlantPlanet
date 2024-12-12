#include "UIPanel.h"
#include "../../Otheres/GameEditor.h"
#include"UIButton.h"

UIPanel* UIPanel::instance_ = nullptr;

UIPanel::UIPanel()
	: UIObject()
{
}

UIPanel* UIPanel::GetInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new UIPanel();
	}
		
	return instance_;
}

void UIPanel::Initialize()
{
}

void UIPanel::Update()
{
	
}

void UIPanel::Draw()
{
	//レイヤー番号でオブジェクトをソート
	SortUIObject();
}

void UIPanel::Release()
{
}

void UIPanel::Save(json& _saveObj)
{
	// 各オブジェクトの保存処理
	for (auto obj : childList_)obj->ChildSave(_saveObj[obj->GetObjectName()]);
}

void UIPanel::Load(json& _loadObj)
{
	// 全てのオブジェクトを削除
	DeleteAllUIObject();

	for (auto it = _loadObj.begin(); it != _loadObj.end(); ++it) {

		// レイヤー番号を読み取る
		int layerNumber = it.value().value("layerNumber", 0);  // デフォルト値0

		// オブジェクトのインスタンスを生成
		UIObject* obj = CreateUIObject(it.key(), it.value()["type_"], this,layerNumber);

		// オブジェクト情報を読込
		obj->ChildLoad(it.value());
	}
}

void UIPanel::DrawData()
{
	// 各オブジェクトのツリーの描画
	for (auto obj : childList_)
	{
		obj->DrawData();
	}
}

void UIPanel::DeleteUIObject(UIObject* _object)
{
	// オブジェクトを削除する
	_object->KillMe();

	// オブジェクトのイテレータを取得する
	auto it = std::find(childList_.begin(), childList_.end(), _object);
	// イテレータが見つかった場合、ベクターから削除する
	if (it != childList_.end()) childList_.erase(it);

	delete _object;
	_object = nullptr;
}

void UIPanel::DeleteAllUIObject()
{
	//ボタンのポインタを保存してあるコンテナのリセット
	ResetArrayOfButton();
	// 全てのオブジェクトを削除
	for (auto obj : childList_) {
		obj->KillMe();
		delete obj;
		obj = nullptr;
	}
	childList_.clear();
}

void UIPanel::SortUIObject()
{
	// レイヤー番号でソート
	std::sort(childList_.begin(), childList_.end(), UIObject::CompareLayerNumber);
}

UIObject* UIPanel::GetUIObject(std::string _name)
{
	// 名前でオブジェクトを検索
	for (auto obj : childList_)
	{
		if (obj->GetObjectName() == _name)return obj;
	}
	return nullptr;
}

vector<UIObject*> UIPanel::GetUIObject(UIType _type)
{
	// タイプでオブジェクトを検索
	vector<UIObject*> result;
	for (auto obj : childList_)
	{
		if (obj->GetType() == _type)result.push_back(obj);
	}
	return result;
}

void UIPanel::SetVisible(string _name, bool _visible)
{
	// 名前でオブジェクトを検索
	for (auto obj : childList_)
	{
		if (obj->GetObjectName() == _name)obj->SetVisible(_visible);
	}
}
//---------------------
bool UIPanel::SetButtonArrayIndex(int16_t x, int16_t y)
{
	for (auto& itr : arrayButton_)
	{
		int16_t tempX;
		int16_t tempY;

		itr->GetArrayPlace(&tempX, &tempY);

		if(tempX==x && tempY ==y)
		{
			buttonIndexX_ = x;
			buttonIndexY_ = y;
			return true;
		}
	}

	return false;
}

void UIPanel::GetButtonIndex(int16_t* x, int16_t* y)
{
	*x = buttonIndexX_;
	*y = buttonIndexY_;
}

UIButton* UIPanel::GetSelectingButton()
{
	return selectingButton_;
}

void UIPanel::SetCursorToSelectingButton()
{
	auto tempPos = selectingButton_->GetTransform().position_;

	SetCursorPos((int)(tempPos.x), (int)(tempPos.y));
}

void UIPanel::SelectorMove(SELECTOR_MOVE_TO way)
{
	auto sameLineButtons = GetSelectorMovable(way);

	if (!sameLineButtons.size())	return;

	auto asc = [](const UIButton* l,const UIButton* r)->bool
		{
			int16_t x, y; l->GetArrayPlace(&x, &y);
			auto left = x + y;

			r->GetArrayPlace(&x, &y);
			auto right = x + y;

			return left < right;
		};

	auto des = [](const UIButton* l, const UIButton* r)->bool
		{
			int16_t x, y; l->GetArrayPlace(&x, &y);
			auto left = x + y;

			r->GetArrayPlace(&x, &y);
			auto right = x + y;

			return left > right;
		};

	std::sort
	(sameLineButtons.begin(), sameLineButtons.end(),
		(way == SELECTOR_MOVE_TO::RIGHT) || (way == SELECTOR_MOVE_TO::UP) ? asc:des
	);

	selectingButton_ = sameLineButtons[0];

	if(selectingButton_) selectingButton_->GetArrayPlace(&buttonIndexX_, &buttonIndexY_);
}

void UIPanel::PushButtonToArray(UIButton* b)
{
	arrayButton_.push_back(b);
}

void UIPanel::RemoveButtonFromArray(UIButton* b)
{
	for (auto itr = arrayButton_.begin(); itr != arrayButton_.end();) {

		if (*itr == b)	itr = arrayButton_.erase(itr);
		
		else ++itr;
	}
}

void UIPanel::ResetArrayOfButton()
{
	arrayButton_.clear();
}

void UIPanel::CheckSelectingButton()
{
	for (auto& itr : arrayButton_)
	{
		int16_t tempX;
		int16_t tempY;

		itr->GetArrayPlace(&tempX, &tempY);

		if (tempX == buttonIndexX_ && tempY == buttonIndexY_)
		{
			selectingButton_ = itr;
			return;
		}
	}

	selectingButton_ = nullptr;
}

std::vector<UIButton*> UIPanel::GetSelectorMovable(SELECTOR_MOVE_TO way)
{
	std::vector<UIButton*> sameLinebuttons{};
	
	auto sameVert = [&](UIButton* btn)->void
		{
			int16_t tempX = NULL;
			int16_t tempY = NULL;
			btn->GetArrayPlace(&tempX, &tempY);

			if (tempX != buttonIndexX_)	return;

			switch (way)
			{
			case SELECTOR_MOVE_TO::UP :
				if (tempY > buttonIndexY_)	sameLinebuttons.push_back(btn);
				return;

			case  SELECTOR_MOVE_TO::BOTTOM:
				if (tempY < buttonIndexY_)	sameLinebuttons.push_back(btn);
				return;
			}
		};

	auto sameHori = [&](UIButton* btn)->void
		{
			int16_t tempX = NULL;
			int16_t tempY = NULL;
			btn->GetArrayPlace(&tempX, &tempY);

			if (tempY != buttonIndexY_)	return;

			switch (way)
			{
			case SELECTOR_MOVE_TO::RIGHT: 
				if (tempX > buttonIndexX_)	sameLinebuttons.push_back(btn);
				return;

			case  SELECTOR_MOVE_TO::LEFT:
				if (tempX < buttonIndexX_)	sameLinebuttons.push_back(btn);
				return;
			}
		};

	std::function<void(UIButton*)>func =(way == SELECTOR_MOVE_TO::UP || way == SELECTOR_MOVE_TO::BOTTOM) ?
		static_cast<std::function<void(UIButton*)>>(sameVert) : 
		static_cast<std::function<void(UIButton*)>>(sameHori);

	for (auto& itr : arrayButton_) {

		func(itr);
	}

	return sameLinebuttons;
}
