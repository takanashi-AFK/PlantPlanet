#include "EnemyGenerator.h"

EnemyGenerator::EnemyGenerator(XMFLOAT3 pos):Generator(pos)
{
}

EnemyGenerator::~EnemyGenerator()
{
}

void EnemyGenerator::Generate(void* ptrOfVector, json file)
{
	vector_ = reinterpret_cast<std::vector<StageObject*>*>(ptrOfVector);
	file_ = file;
	
	RangeType t = file.contains("RangeType") ? file["RangeType"] : RangeType::SPHERE;
}
