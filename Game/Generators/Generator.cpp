#include "Generator.h"
#include "EnemyGenerator.h"

Generator::Generator(XMFLOAT3 pos):pos_(pos),generatorType_(GENERATOR_TYPE::ENEMY)
{
}

Generator::~Generator()
{
}

void Generator::Draw()
{
}

void Generator::RootSave(json& saveObj)
{
	for (auto i = 0u; i < Generators.size(); ++i) {
		saveObj[i]["Position_X"] = Generators[i]->GetPosition().x;
		saveObj[i]["Position_Y"] = Generators[i]->GetPosition().y;
		saveObj[i]["Position_Z"] = Generators[i]->GetPosition().z;
		saveObj[i]["InformationFilePath"] = Generators[i]->informationDir_;
		saveObj[i]["GeneratorType"] = Generators[i]->GetGeneratorType();
		saveObj[i]["GeneratorName"] = Generators[i]->GetName();

		Generators[i]->Save(saveObj,i);
	}
}

void Generator::RootLoad(json& loadObj)
{
	uint16_t count = 0u;
	for (auto itr = loadObj.begin(); itr != loadObj.end(); ++itr) {
		
		XMFLOAT3 position =
		{
			itr.value()["Position_X"].get<float>(),
			itr.value()["Position_Y"].get<float>(),
			itr.value()["Position_Z"].get<float>()
		};

		GENERATOR_TYPE generatorType = static_cast<GENERATOR_TYPE>(itr.value()["GeneratorType"].get<int>());
		Generator* generator = nullptr;

		switch (generatorType)
		{
		case Generator::GENERATOR_TYPE::ENEMY:	generator = new EnemyGenerator(position);
			break;
		}

		generator->informationDir_ = itr.value()["InformationFilePath"].get<string>();
		generator->Load(loadObj ,count);
		generator->SetName(itr.value()["GeneratorName"].get<string>());

		Generators.push_back(generator);
		++count;
	}
}

void Generator::Clear()
{
	for (auto pointer : Generators) {
		delete pointer;
	}

	Generators.clear();
}

void Generator::Add(Generator* ptr)
{
	Generators.push_back(ptr);
}

void Generator::Remove(Generator* ptr)
{
	for (auto itr = Generators.begin(); itr != Generators.end();) {

		if (*itr == ptr)
		{
			Generators.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}

void Generator::SetPosition(XMFLOAT3 position_)
{
}

void Generator::SetName(const string& name)
{
	name_ = name;
}

XMFLOAT3 Generator::GetPosition()
{
	return XMFLOAT3();
}

XMFLOAT3* Generator::GetPositionAddress()
{
	return &pos_;
}

const string& Generator::GetName() const
{
	return name_;
}

string Generator::GetTypeString(GENERATOR_TYPE type)
{
	switch (type)
	{
	case GENERATOR_TYPE::ENEMY: return "ENEMY";

	case GENERATOR_TYPE::AMOUNT: return "AMOUNT";

	default:return "ENEMY";
	}
}

Generator::GENERATOR_TYPE Generator::GetGeneratorType()
{
	return generatorType_;
}
