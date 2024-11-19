#include "Plant.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../Otheres/PlantCollection.h"

Plant::Plant()
{
}

void Plant::DrawData()
{
	if (ImGui::Button("delete")) {
		PlantCollection::RemovePlant(id_);
		return;
	}

	// �T�C�Y��0���傫���ꍇ
	if (PlantCollection::GetPlantsKind().size() > 0) {
		// ID�����݂���ꍇ
		if (PlantCollection::GetPlantsKind()[id_] != nullptr) {
			ImGui::Text("ID:%d", id_);
			ImGui::Text("Rarity:%d", rarity_);
			ImGui::Text("Name:%s", name_.c_str());
			ImGui::Text("ModelFilePath:%s", modelFilePath_.c_str());
			ImGui::Text("ImageFilePath:%s", imageFilePath_.c_str());
		}
	}
	else {
		return;
	}
}

