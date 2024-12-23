#include "UIInventory.h"
#include "UIItemTable.h"
#include "UIButton.h"
#include "../../Plants/Plant.h"
#include "../Stage/StageObject.h"
#include "../Stage/Components/BehaviorComponents/Component_PlayerBehavior.h"
#include<unordered_map>
#include "../../Plants/PlantCollection.h"
#include "../../../Engine/ImGui/imgui.h"
#include "UIText.h"

namespace UIInventory {
	UIPanel* itemPanel_;
	std::vector<UIObject*> getPlantTable_;
	std::vector<UIObject*> invTable_;
	std::vector<UIObject*> invTextTable_;
	std::vector<UIObject*> ingredientTable_;

	std::unordered_map<std::string, int> countedPlant;
	std::unordered_map<int, PlantData>allPlantData;

	StageObject* playerObjects_;
	Stage* pStage_;

	void Initialize()
	{
		itemPanel_ = UIPanel::GetInstance();
		allPlantData = PlantCollection::GetPlants();
		// �A�C�e���e�[�u���݂̂��擾
		for (auto item : itemPanel_->GetUIObjects()) {
			if (item->GetObjectName().starts_with("INV")) {
				invTable_.push_back(item);

				if(item->GetObjectName().starts_with("INV-Ingredients"))ingredientTable_.push_back(item);
				else if (item->GetObjectName().starts_with("INV-GetPlantText"))invTextTable_.push_back(item);
				else if(item->GetObjectName().starts_with("INV-GetPlant"))getPlantTable_.push_back(item);
			}
		}

		for (auto ingredient : ingredientTable_) {
			((UIButton*)ingredient)->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
		}
	}

	void Update()
	{
		for (auto inv : getPlantTable_) {
			if (((UIButton*)inv)->OnClick()) {
				if (((UIButton*)inv)->GetObjectName().starts_with("INV-GetPlant")) {
					//�@�N���b�N���ꂽ��AINV-Ingredients0�̉摜�������ւ�
					// INV-Ingredients0�����������Ă���1�ɁA1�������Ă���2��
					if (((UIButton*)ingredientTable_[0])->GetImageFilePath() == "Models/tentativeFlowers/BlankFlowerImage.png") {
						((UIButton*)ingredientTable_[0])->SetImage(((UIButton*)inv)->GetImageFilePath());
					}
					else if (((UIButton*)ingredientTable_[1])->GetImageFilePath() == "Models/tentativeFlowers/BlankFlowerImage.png") {
						((UIButton*)ingredientTable_[1])->SetImage(((UIButton*)inv)->GetImageFilePath());
					}
					else if (((UIButton*)ingredientTable_[2])->GetImageFilePath() == "Models/tentativeFlowers/BlankFlowerImage.png") {
						((UIButton*)ingredientTable_[2])->SetImage(((UIButton*)inv)->GetImageFilePath());
					}
					else return;

					for (auto a : allPlantData) {
						if (a.second.imageFilePath_ == ((UIButton*)inv)->GetImageFilePath()) {
							countedPlant[a.second.name_]--;
							break;
						}
					}
				}
				else if (((UIButton*)inv)->GetObjectName().starts_with("INV-Ingredients")) {
					((UIButton*)inv)->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
				}
					InventoryDataSet();
			}
		}
	}

	void Draw()
	{
	}


	void SwitchInventory(bool isShow)
	{
		for (auto inv : invTable_) {

			inv->SetVisible(isShow);
		}
	}

	void InventoryDataSet()
	{
		if (pStage_ == nullptr)return;

		// �v���C���[�����擾
		Component_PlayerBehavior* playerBehavior = nullptr; {
			for (auto pb : pStage_->FindComponents(ComponentType::PlayerBehavior))playerBehavior = (Component_PlayerBehavior*)pb;
		}
		// ���̃A�C�e���摜���A�C�e���e�[�u���ɒǉ�
		countedPlant.clear();
		// �����A���̐����J�E���g
		for (const auto& plant : playerBehavior->GetMyPlants()) {
			countedPlant[plant.name_]++;
		}

		for (int i = 0; i < getPlantTable_.size(); i++) {

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
						((UIButton*)getPlantTable_[i])->SetImage(plantData.imageFilePath_);
						((UIText*)invTextTable_[i])->SetText("x" + std::to_string(plantCount));
						break;
					}
				}
			}
			else {
				// �擾�ł��Ȃ��ꍇ�͋�̉摜�ƃe�L�X�g��\��
				if (getPlantTable_[i]->GetObjectName().find("INV-InventoryBack") != 0) {
					((UIButton*)getPlantTable_[i])->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
					((UIText*)invTextTable_[i])->SetText(" ");
				}

			}
		}

	}
	void SetStage(Stage* pStage)
	{
		pStage_ = pStage;
	}
}