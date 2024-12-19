#include<array>
#include<utility>
#include"../../../Game/Objects/Stage/Components/BehaviorComponents/Component_PlayerBehavior.h"
#include"Salad.h"

class MakeSalad
{
public:

	static constexpr int NEED_PLANT_NUM = 3;
	using typeArray = decltype(std::declval<Component_PlayerBehavior>().GetMyPlants());
	using type = typeArray::value_type;

public:

	MakeSalad();
	~MakeSalad();

	void SetRecipeDatum(type p, int index);
	type GetRecipeDatum(int index);
	void SetArray(typeArray* arr);
	void Make();
	Salad GetSalad();

private:

	typeArray* pArray_;
	std::array<type, NEED_PLANT_NUM> recipe_;
	Salad salad_;
};