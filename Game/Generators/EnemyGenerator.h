#pragma once
#include"Generator.h"

class Plant;

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

	struct Information
	{
		Generator::RANGE_TYPE RangeType;

		float radiusX;
		float radiusY;
		float radiusZ;
		uint16_t enemyNum;
		string enemyName;
		string modelPath;
		string fileName;

		void Save();
		void Load();

	}information_;

protected:

	void BoxGenerate();
	void SphereGenerate();

	std::vector<StageObject*> enemies_;
	Stage* parent_;
};