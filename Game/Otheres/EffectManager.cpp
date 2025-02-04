#include "EffectManager.h"
#include "../../Engine/ImGui/imgui.h"
EffectManager& EffectManager::GetInstance()
{
	static EffectManager instance;
	return instance;
}

void EffectManager::AddEffect(EData _data)
{
	if (_data.name.empty() || _data.path.empty()) return;

	// すでにロード済みなら削除して再ロード
	auto it = std::find_if(effects_.begin(), effects_.end(),
		[&](EData& effect) { return effect.name == _data.name; });

	if (it != effects_.end()) {
		// 既存のエフェクトを削除
		effects_.erase(it);
	}

	// EFKTransform を新たに設定
	EData data = {
		_data.name,
		_data.path,
		_data.isLoop,
		_data.maxFrame,
		_data.speed,
		_data.holder,
		_data.position
	};

	// 新しいエフェクトを追加
	effects_.push_back(data);
	// Effekseer にエフェクトを追加
	EFFEKSEERLIB::gEfk->AddEffect(_data.name, _data.path);
}

void EffectManager::PlayEffect(const string _name)
{
	for (auto effect : effects_){
		if (effect.name == _name){
			effect.holder != nullptr ? 
				PlayEffectWithHolder(effect): PlayEffecWithoutHolder(effect);
		}
	}
}

void EffectManager::Load(json& _loadObj)
{
	effects_.clear();
	for (auto& effect : _loadObj["Effects"]) {
		EData data;

		if (_loadObj.contains("holder_"))
			data.holder = (StageObject*)stage_->FindObject(effect["holder_"]);
		if (_loadObj.contains("name"))
			data.name = effect["name"];
		if (_loadObj.contains("path"))
			data.path = effect["path"];
		if (_loadObj.contains("maxFrame"))
			data.maxFrame = effect["maxFrame"];
		if (_loadObj.contains("speed"))
			data.speed = effect["speed"];

		// EFKTransform の読み込み
		if (effect.contains("transform")) {
			EFFEKSEERLIB::EFKTransform transform;
			if (effect["transform"].contains("matrix")) {
				auto matrix = effect["transform"]["matrix"];
				transform.matrix = {
					matrix[0], matrix[1], matrix[2], matrix[3],
					matrix[4], matrix[5], matrix[6], matrix[7],
					matrix[8], matrix[9], matrix[10], matrix[11],
					matrix[12], matrix[13], matrix[14], matrix[15]
				};
			}
			if (effect["transform"].contains("isLoop"))
				transform.isLoop = effect["transform"]["isLoop"];
			if (effect["transform"].contains("speed"))
				transform.speed = effect["transform"]["speed"];
			if (effect["transform"].contains("maxFrame"))
				transform.maxFrame = effect["transform"]["maxFrame"];

			data.effectModelTransform = std::make_shared<EFFEKSEERLIB::EFKTransform>(transform);
		}

		AddEffect(data);
	}
}


void EffectManager::Save(json& _saveObj)
{
	for (auto& effect : effects_) {
		if (effect.holder != nullptr)
			_saveObj["Effects"][effect.name]["holder_"] = effect.holder->GetObjectName();

		_saveObj["Effects"][effect.name]["name"] = effect.name;
		_saveObj["Effects"][effect.name]["path"] = effect.path;
		_saveObj["Effects"][effect.name]["maxFrame"] = effect.maxFrame;
		_saveObj["Effects"][effect.name]["speed"] = effect.speed;

		// EFKTransform の保存
		if (effect.effectModelTransform) {
			_saveObj["Effects"][effect.name]["transform"]["isLoop"] = transform.isLoop;
			_saveObj["Effects"][effect.name]["transform"]["speed"] = transform.speed;
			_saveObj["Effects"][effect.name]["transform"]["maxFrame"] = transform.maxFrame;
		}
	}
}


void EffectManager::DrawData(int _selectIndex)
{
	ImGui::Text("%d",_selectIndex);

	ImGui::Text("name", effects_[_selectIndex].name.c_str());
	ImGui::Text("path", effects_[_selectIndex].path.c_str());
	ImGui::Text("maxFrame", "%d", effects_[_selectIndex].maxFrame);
	ImGui::Text("speed", "%f", effects_[_selectIndex].speed);

	if (effects_[_selectIndex].holder != nullptr)
		ImGui::Text("holder", effects_[_selectIndex].holder->GetObjectName().c_str());
	else 
		ImGui::Text("holder", "null");

	if (ImGui::Button("Play"));
		

	// ここにエフェクトのデータを表示する処理を書く
	// 情報の編集できるようにする
	// 再生もここからできるといいね
	// これをエディタのDrawDetailで表示するようにする
}

void EffectManager::PlayEffectWithHolder(EData _data)
{
	EFFEKSEERLIB::EFKTransform t;
	DirectX::XMStoreFloat4x4(&(t.matrix), _data.holder->GetWorldMatrix());
	t.isLoop = false;
	t.maxFrame = _data.maxFrame;
	t.speed = _data.speed;
	// _data.effectModelTransform = EFFEKSEERLIB::gEfk->Play(_data.name, t);	
}

void EffectManager::PlayEffecWithoutHolder(EData _data)
{
	EFFEKSEERLIB::EFKTransform t;
	XMMATRIX m = XMMatrixTranslation(_data.position.x, _data.position.y, _data.position.z);
	DirectX::XMStoreFloat4x4(&(t.matrix), m);
	t.isLoop = false;
	t.maxFrame = _data.maxFrame;
	t.speed = _data.speed;
	// _data.effectModelTransform = EFFEKSEERLIB::gEfk->Play(_data.name, t);
}
