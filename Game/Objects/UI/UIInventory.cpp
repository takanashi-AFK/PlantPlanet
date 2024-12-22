#include "UIInventory.h"
#include "UIItemTable.h"
#include "UIButton.h"
#include "../../Plants/Plant.h"
#include "../Stage/StageObject.h"
#include "../Stage/Components/BehaviorComponents/Component_PlayerBehavior.h"
#include<unordered_map>
#include "../../Plants/PlantCollection.h"
#include "../../../Engine/ImGui/imgui.h"

namespace UIInventory {
	UIPanel* itemPanel_;
	std::vector<UIObject*> itemTable_;
	std::vector<UIObject*> invTable_;
	std::vector<PlantData> getPlant_;
	StageObject* playerObjects_;


	void Initialize()
	{
		itemPanel_ = UIPanel::GetInstance();

		// UI���̂��ׂĂ�ێ�
		itemTable_ = itemPanel_->GetUIObjects();

		// �A�C�e���e�[�u���݂̂��擾
		for (auto item : itemTable_) {
			if (item->GetObjectName().starts_with("INV"))
				invTable_.push_back(item);
		}
	}

	void Update()
	{
		ImGui::Begin("Inventory");
		for (auto item : itemTable_) {
			ImGui::Text(item->GetObjectName().c_str());
			ImGui::SameLine();
			ImGui::Text("%d",item->isVisible_);
		}
		ImGui::End();	
	}

	void Draw()
	{
	}


	void SwitchInventory(bool isShow)
	{
		for (auto item : itemTable_) {
			for (auto inv : invTable_) {
				inv->SetVisible((item == inv) == isShow);
			}
		}
	}

		void InventoryDataSet(Stage* pStage)
		{
			if (pStage == nullptr)return;


			// ���̃A�C�e���摜���A�C�e���e�[�u���ɒǉ�


			std::unordered_map<int, PlantData>allPlantData = PlantCollection::GetPlants();

			// �����A���̐����J�E���g
			std::unordered_map<std::string, int> countedPlant;
			for (const auto& plant : g_playerPlantData) {
				countedPlant[plant.name_]++;
			}

			for (int i = 0; i < invTable_.size(); i++) {

				// �J�E���g�����A���̐����擾
				int plantSize = countedPlant.size();

				if (((UIObject*)invTable_[i])->GetObjectName() == "INV_InventoryBackGround") {
					continue;
				}

				// �A���̐���i���傫���ꍇ(�擾�ł���ꍇ)
				if (i <= plantSize - 1) {
					// countedPlant�̒�����i�Ԗڂ̐A�����擾
					auto it = countedPlant.begin();
					std::advance(it, i); // i�Ԗڂ̗v�f�Ɉړ�

					const std::string& plantName = it->first; // �A���̖��O
					int plantCount = it->second;             // �A���̐�

					for (const auto& p : allPlantData) {
						if (plantName == p.second.name_) {
							PlantData plantData = p.second;
							((UIButton*)invTable_[i])->SetImage(plantData.imageFilePath_);
							break;
						}
					}
				}
				else {
					// �擾�ł��Ȃ��ꍇ�͋�̉摜�ƃe�L�X�g��\��
					//((UIButton*)invTable_[i])->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
				}
			}
		}
}