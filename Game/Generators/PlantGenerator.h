#pragma once
#include"../../Game/Otheres/PlantCollection.h"
#include "Generator.h"

class Collider;

class PlantGenerator : public Generator
{
public:
	PlantGenerator(XMFLOAT3 pos);
	~PlantGenerator();

	/// <summary>
	/// void* Ç…ÇÕStage*Çì¸ÇÍÇƒÇÀÅB
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

		float rare1probability;
		float rare2probability;
		float rare3probability;

		uint16_t plantNum;
		uint8_t areaID;

		void Save(string& jsonName, bool isFinish);
		void Load(string& jsonName, bool isInit);

		Information();
	}information_;

protected:

	void BoxGenerate();
	void SphereGenerate();

	std::vector<Plant> plants_ ;
	Stage* parent_;

	Generator::RANGE_TYPE prevRangeType_;
};

