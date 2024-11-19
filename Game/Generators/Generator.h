#pragma once
#include<vector>
#include"../../Engine/Json/JsonReader.h"
#include"../../Engine/GameObject/Transform.h"
#include<cstdint>
#include"../../Engine/GameObject/GameObject.h"

class Generator
{
public:

	Generator(XMFLOAT3 pos);
	~Generator();
	virtual void Generate(string fileDir,GameObject* parent) = 0;
	virtual void* Pop() = 0;
	virtual bool isEmpty() = 0;

	enum class RANGE_TYPE
	{
		BOX,
		SPHERE,

		AMOUNT
	};

protected:

	XMFLOAT3 pos_;

	virtual void BoxGenerate() = 0;
	virtual void SphereGenerate() = 0;
	
	string fileDir_;

};

struct Information
{
	Generator::RANGE_TYPE RangeType;

	float RadiusX;
	float RadiusY;
	float RadiusZ;
};