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

	// �i�`�������\�[�g�p�̔�r�֐�
	bool naturalSortComparator(const UIObject& a, const UIObject& b) {
		const std::string& nameA = a.objectName_;
		const std::string& nameB = b.objectName_;

		auto isDigit = [](char c) { return std::isdigit(static_cast<unsigned char>(c)); };

		size_t i = 0, j = 0;
		while (i < nameA.size() && j < nameB.size()) {
			if (isDigit(nameA[i]) && isDigit(nameB[j])) {
				// �����������r
				std::string numA, numB;
				while (i < nameA.size() && isDigit(nameA[i])) {
					numA += nameA[i++];
				}
				while (j < nameB.size() && isDigit(nameB[j])) {
					numB += nameB[j++];
				}

				int intA = std::stoi(numA);
				int intB = std::stoi(numB);

				if (intA != intB) return intA < intB;
			}
			else {
				// �����ȊO�̕������r
				if (nameA[i] != nameB[j]) return nameA[i] < nameB[j];
				i++;
				j++;
			}
		}
		return nameA.size() < nameB.size();
	}


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

		std::sort(invTable_.begin(), invTable_.end(), naturalSortComparator);
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

				inv->SetVisible(isShow);
			}
		}
	}

		void InventoryDataSet(Stage* pStage)
		{
			if (pStage == nullptr)return;


			// ���̃A�C�e���摜���A�C�e���e�[�u���ɒǉ�
			std::unordered_map<int, PlantData>allPlantData = PlantCollection::GetPlants();

			// �v���C���[�����擾
			Component_PlayerBehavior* playerBehavior = nullptr; {
				for (auto pb : pStage->FindComponents(ComponentType::PlayerBehavior))playerBehavior = (Component_PlayerBehavior*)pb;
			}

			// �v���C���[���擾�����A���̏����擾
			getPlant_ = playerBehavior->GetMyPlants();

			// �����A���̐����J�E���g
			std::unordered_map<std::string, int> countedPlant;
			for (const auto& plant : getPlant_) {
				countedPlant[plant.name_]++;
			}

			for (int i = 0; i < invTable_.size(); i++) {

				// �J�E���g�����A���̐����擾
				int plantSize = countedPlant.size();

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
					if (invTable_[i]->GetObjectName().find("INV-InventoryBack") != 0)
					((UIButton*)invTable_[i])->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
				}
			}
		}
}