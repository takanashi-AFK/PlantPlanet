#pragma once
#include <vector>
#include <string>
#include "../../../EffekseeLib/EffekseerVFX.h"
#include "../../../../Engine/Json/JsonReader.h"
#include "../../../../../Engine/Global.h"
#include "../Objects/Stage/StageObject.h"
#include "../Objects/Stage/Stage.h"

using std::string;
using std::vector;

struct EData
{
	string name;
	string path; 
	int maxFrame;
	float speed;
	StageObject* holder;
	std::shared_ptr<EFFEKSEERLIB::EFKTransform> effectModelTransform;
	XMFLOAT3 position;
};

class EffectManager
{
private:
	std::vector<EData> effects_;

	Stage* stage_;
public:
	/// <summary> シングルトンインスタンスを取得 </summary>
	static EffectManager& GetInstance();
	void Initialize();
	void Update();

	void AddEffect(EData _data);
	void PlayEffect(const string _name);

	void Load(json& _loadObj);
	void Save(json& _saveObj);
	void DrawData(int _selectIndex);

	void SetStage(Stage* _stage) { stage_ = _stage; }

	vector<EData>	& GetEffects() { return effects_; }

private:
	EffectManager() {};
	EffectManager(const EffectManager&) = delete;
	EffectManager& operator=(const EffectManager&) = delete;
	void PlayEffectWithHolder(EData _data);
	void PlayEffecWithoutHolder(EData _data);

};

