#pragma once
#include"Generator.h"
#include "../Objects//Stage/StageObject.h"

class EnemyGenerator:public Generator
{
public:
	EnemyGenerator(XMFLOAT3 pos);
	~EnemyGenerator();
	void Generate(void* ptrOfVector, json file) override;

protected:

	void BoxGenerate();
	void SphereGenerate();

	json file_;
	std::vector<StageObject*>* vector_;
};