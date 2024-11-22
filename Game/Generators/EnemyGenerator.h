#pragma once
#include"Generator.h"

class Plant;
class Collider;

class EnemyGenerator:public Generator
{
public:
	EnemyGenerator(XMFLOAT3 pos);
	~EnemyGenerator();

	/// <summary>
	/// void* ‚É‚ÍStage*‚ğ“ü‚ê‚Ä‚ËB
	/// </summary>
	/// <param name="fileDir"></param>
	/// <param name="parent"></param>
	void Generate(void* parent) override;
	void* Pop() override;
	bool isEmpty() override;
	void Draw() override;


	virtual void Save(json& saveObj, int index);
	virtual void Load(json& loadObj, int index);

	struct Information
	{
		Generator::RANGE_TYPE RangeType;

		float radiusX;
		float radiusY;
		float radiusZ;
		uint16_t enemyNum;
		string enemyName;
		string modelPath;

		void Save(string& jsonName , bool isFinish);
		void Load(string& jsonName , bool isInit);

		Information(Generator* holder);
	}information_;

protected:

	void BoxGenerate();
	void SphereGenerate();

	std::vector<StageObject*> enemies_;
	Stage* parent_;
	Collider* collider_;
	
	Generator::RANGE_TYPE prevRangeType_;
};

