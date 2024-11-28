#include "Easing.h"
#include <cstdlib>
#include<algorithm>
#include<numbers>
#include<cmath>
#include<utility>
#include"Direct3D.h"

float Easing::GetValue(float v0, float v1, float pile, Easing::TYPE type)
{
	auto func = Easing::GetCalcFunction(type);

	double ratio = func(pile);

	return (v0*ratio) + ((1-ratio) * v1);
}

Easing::Easing(float v0, float v1, float addRatio) :val0_(v0), val1_(v1), ratio_(addRatio), pile_(.0f), isStop(false)
{
}

auto Check = [](float v0, float v1, float& ratio)
	{
		ratio = std::clamp(ratio, .0f, 1.0f);
	};

float Easing::GetValue(Easing::TYPE type)
{
	auto func = GetCalcFunction(type);

	if(!isStop)	pile_ += ratio_;

	Check(.0f, 1.0f, pile_);

	double ratio = func(pile_);

	return (ratio * val0_) + ((1 - ratio) * val1_);
}

double Easing::CalcStraight(double t)
{
	return t;
}

std::string Easing::GetEnumName(Easing::TYPE t)
{

	switch (t)
	{
    case TYPE::STRAIGHT: return "Straight";
    case TYPE::IN_SINE: return "InSine";
    case TYPE::OUT_SINE: return "OutSine";
    case TYPE::IN_OUT_SINE: return "InOutSine";
    case TYPE::IN_QUAD: return "InQuad";
    case TYPE::OUT_QUAD: return "OutQuad";
    case TYPE::IN_OUT_QUAD: return "InOutQuad";
    case TYPE::IN_CUBIC: return "InCubic";
    case TYPE::OUT_CUBIC: return "OutCubic";
    case TYPE::IN_QUART: return "InQuart";
    case TYPE::OUT_QUART: return "OutQuart";
    case TYPE::IN_OUT_QUART: return "InOutQuart";
    case TYPE::IN_QUINT: return "InQuint";
    case TYPE::OUT_QUINT: return "OutQuint";
    case TYPE::IN_OUT_QUINT: return "InOutQuint";
    case TYPE::IN_EXPO: return "InExpo";
    case TYPE::OUT_EXPO: return "OutExpo";
    case TYPE::IN_OUT_EXPO: return "InOutExpo";
    case TYPE::IN_CIRC: return "InCirc";
    case TYPE::OUT_CIRC: return "OutCirc";
    case TYPE::IN_OUT_CIRC: return "InOutCirc";
    case TYPE::IN_BACK: return "InBack";
    case TYPE::OUT_BACK: return "OutBack";
    case TYPE::IN_OUT_BACK: return "InOutBack";
    case TYPE::IN_ELASTIC: return "InElastic";
    case TYPE::OUT_ELASTIC: return "OutElastic";
    case TYPE::IN_OUT_ELASTIC: return "InOutElastic";
    case TYPE::IN_BOUNCE: return "InBounce";
    case TYPE::OUT_BOUNCE: return "OutBounce";
    case TYPE::IN_OUT_BOUNCE: return "InOutBounce";

	default: return "Error";
	}
}

std::function<double(double)>(Easing::GetCalcFunction(Easing::TYPE type))
{
	switch (type)
	{
	case Easing::TYPE::STRAIGHT:	
		return std::function<double(double)>(Easing::CalcStraight);
		
    default:
        return (Direct3D::EaseFunc.at(GetEnumName(type)));

	}
}
