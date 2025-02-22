#include "Stage.h"

// インクルード
#include "StageObject.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/GameObject/Camera.h"

Stage::Stage(GameObject* _parent)
	:GameObject(_parent,"Stage"),objects_()
{
}

void Stage::Initialize()
{
}

void Stage::Update()
{
}

void Stage::Draw()
{
}

void Stage::Release()
{
}

void Stage::Save(json& _saveObj)
{
	// オブジェクト群を保存
	for (auto obj : objects_)obj->Save(_saveObj[obj->GetObjectName()]);
}

void Stage::Load(json& _loadObj)
{
	// ステージオブジェクトをすべて削除
	DeleteAllStageObject();

	for (auto it = _loadObj.begin(); it != _loadObj.end();++it) {
		
		// オブジェクトのインスタンスを生成
		StageObject* obj = CreateStageObject(it.key(), it.value()["modelFilePath_"], this);

		// オブジェクト情報を読込
		obj->Load(it.value());

		// オブジェクトをリストに追加
		AddStageObject(obj);
	}
}

void Stage::DrawData()
{
	// オブジェクトを描画
	for (auto obj : objects_)obj->DrawData();
}

void Stage::AddStageObject(StageObject* _obj)
{
	// リストに追加
	if (_obj != nullptr)objects_.push_back(_obj);
}

void Stage::DeleteStageObject(StageObject* _obj)
{

	// オブジェクトのイテレータを取得する
	auto it = std::find(objects_.begin(), objects_.end(), _obj);

	// イテレータが見つかった場合、ベクターから削除する
	if (it != objects_.end()) objects_.erase(it);

	// オブジェクトを削除する
	_obj->KillMe();
}

void Stage::DeleteAllStageObject()
{
	// リスト内にある要素をすべて削除
	for (auto obj : objects_) obj->KillMe();
	objects_.clear();
}

StageObject* Stage::GetStageObject(string _name)
{
	// オブジェクトを検索
	for (auto obj : objects_) {
		if (obj->GetObjectName() == _name)return obj;
	}
	return nullptr;

}

Component* Stage::FindComponent(string _name)
{
	Component* comp = nullptr;
	for (auto obj : objects_) {
		comp = obj->FindComponent(_name);
		if (comp != nullptr)return comp;
	}
	return nullptr;
}

vector<Component*> Stage::FindComponents(ComponentType _type)
{
	vector<Component*> result;
	for (auto obj : objects_) {

		// コンポーネントを再帰的に検索
		auto comps = obj->FindComponent(_type);
		result.insert(result.end(), comps.begin(), comps.end());
	}
	return result;
}
