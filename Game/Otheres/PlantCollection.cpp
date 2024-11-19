#include "PlantCollection.h"
#include "../Objects/Stage/Plant.h"

namespace PlantCollection
{
	vector<Plant*> plantsKind_;
}

void PlantCollection::AddPlant(Plant* _plant)
{
	if (&_plant != nullptr) {
		plantsKind_.push_back(_plant);
	}
}

void PlantCollection::RemovePlant(int _index)
{
	if (_index >= 0 && _index < plantsKind_.size()) {
		delete plantsKind_[_index];
		plantsKind_.erase(plantsKind_.begin() + _index);
		
	}
}

void PlantCollection::ClearPlantKind()
{
	plantsKind_.clear();
}

vector<Plant*> PlantCollection::GetPlantsKind()
{
	return plantsKind_; 
}

void PlantCollection::Save(json& _saveObj)
{
	// オブジェクト群を保存
	//for (Plant& obj : plantsKind_)obj.Save(_saveObj[obj.GetID()]);
	
}


