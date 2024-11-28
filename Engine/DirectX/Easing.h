#pragma once
#include<string>
#include<functional>

class Easing
{
public :

	enum class TYPE
	{
		STRAIGHT,
		IN_SINE,
		OUT_SINE,
		IN_OUT_SINE,
		IN_QUAD,
		OUT_QUAD,
		IN_OUT_QUAD,
		IN_CUBIC,
		OUT_CUBIC,
		IN_QUART,
		OUT_QUART,
		IN_OUT_QUART,
		IN_QUINT,
		OUT_QUINT,
		IN_OUT_QUINT,
		IN_EXPO,
		OUT_EXPO,
		IN_OUT_EXPO,
		IN_CIRC,
		OUT_CIRC,
		IN_OUT_CIRC,
		IN_BACK,
		OUT_BACK,
		IN_OUT_BACK,
		IN_ELASTIC,
		OUT_ELASTIC,
		IN_OUT_ELASTIC,
		IN_BOUNCE,
		OUT_BOUNCE,
		IN_OUT_BOUNCE,

		AMOUNT
	};

	Easing(float v0,float v1,float addRatio);

	static float GetValue(float v0, float v1, float pile , Easing::TYPE type);
	float GetValue(Easing::TYPE type);

	float pile_;
	float ratio_;
	float val0_;
	float val1_;
	bool isStop;

	static std::string GetEnumName(Easing::TYPE t);

	static double CalcStraight(double t);

private:

	static std::function<double(double)>(GetCalcFunction(Easing::TYPE type));
};

