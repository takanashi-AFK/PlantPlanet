#pragma once
#include<vector>
#include"../../Engine/Json/JsonReader.h"
#include"../../Engine/GameObject/Transform.h"

class Generator
{
public:

	Generator(XMFLOAT3 pos);
	~Generator();
	virtual void Generate(void* ptrOfVector,json file) = 0;

	enum class RangeType
	{
		BOX,
		SPHERE,

		AMOUNT
	};

protected:

	XMFLOAT3 pos_;

	virtual void BoxGenerate() = 0;
	virtual void SphereGenerate() = 0;
	
};

