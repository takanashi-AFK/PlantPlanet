#include "UIPanel.h"
#include "../../Otheres/GameEditor.h"

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
