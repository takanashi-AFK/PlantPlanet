#include "EnemyGenerator.h"
#include<algorithm>
#include<random>

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
	std::random_device random_device{};
	std::mt19937 engine(random_device());

	auto frand = [&](float min , float max)->float 
		{
			std::uniform_real_distribution<float> unireal(min, max);
			return unireal(engine);
		};

	for (auto i = 0u; i < information_.EnemyNum; ++i) {
		
		XMFLOAT3 plantPos = { frand(-1.0f, 1.0f), frand(-1.0f, 1.0f), frand(-1.0f, 1.0f) };

		{
			auto temporaryVec = XMVector3Normalize(XMLoadFloat3(&plantPos));
			XMStoreFloat3(&plantPos, temporaryVec);//plantPos‚Ì³‹K‰»
		}

		plantPos.x *= information_.RadiusX;
		plantPos.y *= information_.RadiusY;
		plantPos.z *= information_.RadiusZ;
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
