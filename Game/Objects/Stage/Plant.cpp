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

	// ÉTÉCÉYÇ™0ÇÊÇËëÂÇ´Ç¢èÍçá
	if (PlantCollection::GetPlantsKind().size() > 0) {
		// IDÇ™ë∂ç›Ç∑ÇÈèÍçá
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

