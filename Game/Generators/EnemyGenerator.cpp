#include "EnemyGenerator.h"
#include<algorithm>
#include<random>
#include"../Objects/Stage/StageObject.h"
#include<format>

EnemyGenerator::EnemyGenerator(XMFLOAT3 pos):Generator(pos)
{
}

EnemyGenerator::~EnemyGenerator()
{
}

void* EnemyGenerator::Pop()
{
	return enemies_.size() <= 0 ?
		nullptr:&enemies_[enemies_.size() -1];
	
}

bool EnemyGenerator::isEmpty()
{
	return enemies_.empty();
}

void EnemyGenerator::BoxGenerate()
{
	std::random_device random_device{};
	std::mt19937 engine(random_device());

	auto frand = [&](float min, float max)->float
		{
			std::uniform_real_distribution<float> unireal(min, max);
			return unireal(engine);
		};

	for (auto i = 0u; i < information_.enemyNum; ++i) {
		XMFLOAT3 plantPos = {};
		plantPos.x = frand(-information_.radiusX, information_.radiusX);
		plantPos.y = frand(-information_.radiusY, information_.radiusY);
		plantPos.z = frand(-information_.radiusZ, information_.radiusZ);

		auto enemy = CreateStageObject(information_.enemyName + std::to_string(i), information_.modelPath, parent_);
		
		enemy->SetPosition(plantPos);
		enemy->SetRotate({0,frand(0,360),0 });
		enemy->AddComponent(CreateComponent("bossComp", BossBehavior, enemy));

		enemies_.push_back(enemy);
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

	for (auto i = 0u; i < information_.enemyNum; ++i) {
		
		XMFLOAT3 plantPos = { frand(-1.0f, 1.0f), frand(-1.0f, 1.0f), frand(-1.0f, 1.0f) };
		float length = frand(.0f, 1.0f);
		{
			auto temporaryVec = XMVector3Normalize(XMLoadFloat3(&plantPos));
			XMStoreFloat3(&plantPos, temporaryVec);//plantPos‚Ì³‹K‰»

			plantPos.x *= length;
			plantPos.y *= length;
			plantPos.z *= length;//length‚Ì’·‚³‚É‚·‚éB
		}

		plantPos.x *= information_.radiusX;
		plantPos.y *= information_.radiusY;
		plantPos.z *= information_.radiusZ;

		auto enemy = CreateStageObject(information_.enemyName + std::to_string(i), information_.modelPath, parent_);

		enemy->SetPosition(plantPos);
		enemy->SetRotate({ 0,frand(0,360),0 });
		enemy->AddComponent(CreateComponent("bossComp", BossBehavior, enemy));

		enemies_.push_back(enemy);
	}
}

void EnemyGenerator::Generate(void* parent)
{
	srand(time(NULL));
	parent_ = reinterpret_cast<Stage*>(parent);

	information_.enemyNum = information_.enemyNum > 0
		? information_.enemyNum: 1u;

	switch (information_.RangeType)
	{
	case Generator::RANGE_TYPE::BOX:BoxGenerate(); break;

	case Generator::RANGE_TYPE::SPHERE:SphereGenerate(); break;

	default:SphereGenerate();
	}
}

void EnemyGenerator::Information::Save()
{
	json saveObj = {};
	saveObj["RANGE_TYPE"] = this->RangeType;
	saveObj["RadiusX"] = this->radiusX;
	saveObj["RadiusY"] = this->radiusY;
	saveObj["RadiusZ"] = this->radiusZ;

	saveObj["EnemyNum"] = this->enemyNum;
	saveObj["EnemyName"] = this->enemyName;
	saveObj["ModelPath"] = this->enemyName;
	saveObj["FileName"] = this->fileName;

	JsonReader::Save(fileName, saveObj);
}

void EnemyGenerator::Information::Load()
{
	json file;
	json fileDir;
	if (!JsonReader::Load(fileDir, file)) return;

	RangeType = file.contains("RANGE_TYPE")
		? static_cast<Generator::RANGE_TYPE>(file["RANGE_TYPE"].get<int>()) : Generator::RANGE_TYPE::SPHERE;

	radiusX = file.contains("RadiusX")
		? file["RadiusX"].get<float>() : 1.0f;

	radiusY = file.contains("RadiusY")
		? file["RadiusY"].get<float>() : 1.0f;

	radiusZ = file.contains("RadiusZ")
		? file["RadiusZ"].get<float>() : 1.0f;

	enemyNum = file.contains("EnemyNum")
		? file["EnemyNum"].get<uint32_t>() : 1u;

	enemyName = file.contains("EnemyName")
		? file["EnemyName"].get<string>() : "";

	modelPath = file.contains("ModelPath")
		? file["ModelPath"].get<string>() : "";

	modelPath = file.contains("FileName")
		? file["FileName"].get<string>() : "";
}
