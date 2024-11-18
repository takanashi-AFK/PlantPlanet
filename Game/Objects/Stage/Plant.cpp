#include "Plant.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../Otheres/PlantCollection.h"
Plant::Plant(GameObject* _parent)
	: StageObject("Plant", "Models/DebugCollision/SphereCollider.fbx", _parent), id_(), rarity_(), name_()
{
}

void Plant::Save(json& _saveObj)
{
	_saveObj["id_"] = id_;
	_saveObj["rarity_"] = rarity_;
	_saveObj["name_"] = name_;
	_saveObj["modelFilePath_"] = modelFilePath_;
	_saveObj["imageFilePath_"] = imageFilePath_;
}

void Plant::Load(json& _loadObj)
{
	if (_loadObj.find("id_") != _loadObj.end()) id_ = _loadObj["id_"];
	if (_loadObj.find("rarity_") != _loadObj.end()) rarity_ = _loadObj["rarity_"];
	if (_loadObj.find("name_") != _loadObj.end()) name_ = _loadObj["name_"];
	if (_loadObj.find("modelFilePath_") != _loadObj.end()) modelFilePath_ = _loadObj["modelFilePath_"];
	if (_loadObj.find("imageFilePath_") != _loadObj.end()) imageFilePath_ = _loadObj["imageFilePath_"];
}

void Plant::Initialize()
{
}

void Plant::Update()
{
}

void Plant::Draw()
{
}

void Plant::DrawData()
{
	if (ImGui::SmallButton("delete"))
		PlantCollection::RemovePlant(this->id_);
	ImGui::Separator();

	ImGui::Text("ID : %d", id_);
	ImGui::Text("Rarity : %d", rarity_);
	ImGui::Text("Name : %s", name_.c_str());
	ImGui::Text("ModelFilePath : %s", modelFilePath_.c_str());
	ImGui::Text("ImageFilePath : %s", imageFilePath_.c_str());
}
