#include "MakeSalad.h"

MakeSalad::MakeSalad() :salad_{}, pArray_{ nullptr }, recipe_{}
{
}

MakeSalad::~MakeSalad()
{
}

void MakeSalad::SetRecipeDatum(type p, int index)
{
	index %= NEED_PLANT_NUM;

	recipe_[index] = p;
}

MakeSalad::type MakeSalad::GetRecipeDatum(int index)
{
	index %= NEED_PLANT_NUM;

	return recipe_[index];
}

void MakeSalad::SetArray(typeArray* arr)
{
	pArray_ = arr;
}

void MakeSalad::Make()
{

}

Salad MakeSalad::GetSalad()
{
	return salad_;
}
