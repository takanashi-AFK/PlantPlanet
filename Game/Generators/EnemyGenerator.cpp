#include "EnemyGenerator.h"
#include<algorithm>

EnemyGenerator::EnemyGenerator(XMFLOAT3 pos):Generator(pos)
{
}

EnemyGenerator::~EnemyGenerator()
{
}

void* EnemyGenerator::Pop()
{
	return plants_.size() <= 0 ?
		nullptr:&plants_[plants_.size() -1];
	
}

bool EnemyGenerator::isEmpty()
{
	return plants_.empty();
}

void EnemyGenerator::BoxGenerate()
{
	for (auto i = 0u; i < information_.EnemyNum; ++i) {
		XMFLOAT3 plantPos = {};
		plantPos.x = (information_.RadiusX <= .0f ? .0f : fmod(rand(), 2 * information_.RadiusX) - information_.RadiusX) + pos_.x;
		plantPos.y = (information_.RadiusY <= .0f ? .0f : fmod(rand(), 2 * information_.RadiusY) - information_.RadiusY) + pos_.y;
		plantPos.z = (information_.RadiusZ <= .0f ? .0f : fmod(rand(), 2 * information_.RadiusZ) - information_.RadiusZ) + pos_.z;

		///new Plant
		///vector.push_back
		///
		


	}
}

void EnemyGenerator::SphereGenerate()
{
	for (auto i = 0u; i < information_.EnemyNum; ++i) {
		
		float length = fmod(rand(), 1.0f);
		
		XMFLOAT3 plantLength = {length,0,0};

		const float normalRadius = sqrt(1.0f / 3.0f);

	}
}

void EnemyGenerator::Generate(string fileDir, GameObject* parent)
{
	srand(time(NULL));

	json file;
	JsonReader::Load(fileDir, file);

	fileDir_ = fileDir;

	information_.RangeType= file.contains("RANGE_TYPE") 
		? static_cast<Generator::RANGE_TYPE>(file["RANGE_TYPE"].get<int>()) : Generator::RANGE_TYPE::SPHERE ;

	information_.RadiusX = file.contains("RadiusX")
		? file["RadiusX"].get<float>() : 1.0f;

	information_.RadiusY = file.contains("RadiusY")
		? file["RadiusY"].get<float>() : 1.0f;

	information_.RadiusZ = file.contains("RadiusZ")
		? file["RadiusZ"].get<float>() : 1.0f;

	information_.EnemyNum = file.contains("EnemyNum")
		? file["EnemyNum"].get<uint32_t>() : 1u;

	switch (information_.RangeType)
	{
	case Generator::RANGE_TYPE::BOX:BoxGenerate(); break;

	case Generator::RANGE_TYPE::SPHERE:SphereGenerate(); break;

	default:SphereGenerate();
	}

	
}
