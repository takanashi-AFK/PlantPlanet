#pragma once
#include"Generator.h"
#include "../Objects//Stage/StageObject.h"

class Plant;

class EnemyGenerator:public Generator
{
public:
	EnemyGenerator(XMFLOAT3 pos);
	~EnemyGenerator();
	void Generate(string fileDir, GameObject* parent) override;
	void* Pop() override;
	bool isEmpty() override;

	struct Information
	{
		Generator::RANGE_TYPE RangeType;

		float RadiusX;
		float RadiusY;
		float RadiusZ;
		uint16_t EnemyNum;
	}information_;

protected:

	void BoxGenerate();
	void SphereGenerate();

	string file_;
	std::vector<StageObject*>* vector_;

	std::vector<Plant*> plants_;
};