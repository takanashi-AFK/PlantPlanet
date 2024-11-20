#pragma once
#include<vector>
#include"../../Engine/Json/JsonReader.h"
#include"../../Engine/GameObject/Transform.h"
#include<cstdint>
#include"../Objects/Stage/Stage.h"

class Generator
{
public:

	inline static vector<Generator*> Generators ;

	Generator(XMFLOAT3 pos);
	~Generator();

	virtual void Generate(void* parent) = 0;
	virtual void* Pop() = 0;
	virtual bool isEmpty() = 0;
	virtual void Draw() = 0;

	static void RootSave(json& saveObj);
	static void RootLoad(json& loadObj);

	virtual void Save(json& saveObj,int index) = 0;
	virtual void Load(json& loadObj,int index) = 0;

	static void Clear();
	static void Add(Generator* ptr);
	static void Remove(Generator* ptr);

	void SetPosition(XMFLOAT3 position_);
	void SetName(const string& name);

	XMFLOAT3 GetPosition();
	XMFLOAT3* GetPositionAddress();
	const string& GetName()const;

	enum class RANGE_TYPE
	{
		BOX,
		SPHERE,

		AMOUNT
	};

	enum class GENERATOR_TYPE
	{
		ENEMY,

		AMOUNT
	};
	static string GetTypeString(GENERATOR_TYPE type);
	string informationDir_;

	GENERATOR_TYPE GetGeneratorType();

protected:

	XMFLOAT3 pos_;
	string name_;

	GENERATOR_TYPE generatorType_;

	virtual void BoxGenerate() = 0;
	virtual void SphereGenerate() = 0;
};

struct Information
{
	Generator::RANGE_TYPE RangeType;

	float RadiusX;
	float RadiusY;
	float RadiusZ;
};