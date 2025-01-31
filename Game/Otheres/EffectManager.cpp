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

	// ���łɃ��[�h�ς݂Ȃ�폜���čă��[�h
	auto it = std::find_if(effects_.begin(), effects_.end(),
		[&](EData& effect) { return effect.name == _data.name; });

	if (it != effects_.end()) {
		// �����̃G�t�F�N�g���폜
		effects_.erase(it);
	}

	// EFKTransform ��V���ɐݒ�
	EData data = {
		_data.name,
		_data.path,
		_data.maxFrame,
		_data.speed,
		_data.holder,
		_data.effectModelTransform,
		_data.position
	};

	// �V�����G�t�F�N�g��ǉ�
	effects_.push_back(data);
	// Effekseer �ɃG�t�F�N�g��ǉ�
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

		// EFKTransform �̓ǂݍ���
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

			data.effectModelTransform = std::make_shared<EFKTransform>(transform);
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

		// EFKTransform �̕ۑ�
		if (effect.effectModelTransform) {
			auto& transform = *effect.effectModelTransform;
			_saveObj["Effects"][effect.name]["transform"]["matrix"] = {
				transform.matrix._11, transform.matrix._12, transform.matrix._13, transform.matrix._14,
				transform.matrix._21, transform.matrix._22, transform.matrix._23, transform.matrix._24,
				transform.matrix._31, transform.matrix._32, transform.matrix._33, transform.matrix._34,
				transform.matrix._41, transform.matrix._42, transform.matrix._43, transform.matrix._44
			};
			_saveObj["Effects"][effect.name]["transform"]["isLoop"] = transform.isLoop;
			_saveObj["Effects"][effect.name]["transform"]["speed"] = transform.speed;
			_saveObj["Effects"][effect.name]["transform"]["maxFrame"] = transform.maxFrame;
		}
	}
}


void EffectManager::DrawData(int _selectIndex)
{
	ImGui::Text("%d",_selectIndex);
	// �����ɃG�t�F�N�g�̃f�[�^��\�����鏈��������
	// ���̕ҏW�ł���悤�ɂ���
	// �Đ�����������ł���Ƃ�����
	// ������G�f�B�^��DrawDetail�ŕ\������悤�ɂ���
}

void EffectManager::PlayEffectWithHolder(EData _data)
{
	EFFEKSEERLIB::EFKTransform t;
	DirectX::XMStoreFloat4x4(&(t.matrix), _data.holder->GetWorldMatrix());
	t.isLoop = false;
	t.maxFrame = _data.maxFrame;
	t.speed = _data.speed;
	_data.effectModelTransform = EFFEKSEERLIB::gEfk->Play(_data.name, t);	
}

void EffectManager::PlayEffecWithoutHolder(EData _data)
{
	EFFEKSEERLIB::EFKTransform t;
	XMMATRIX m = XMMatrixTranslation(_data.position.x, _data.position.y, _data.position.z);
	DirectX::XMStoreFloat4x4(&(t.matrix), m);
	t.isLoop = false;
	t.maxFrame = _data.maxFrame;
	t.speed = _data.speed;
	_data.effectModelTransform = EFFEKSEERLIB::gEfk->Play(_data.name, t);
}
