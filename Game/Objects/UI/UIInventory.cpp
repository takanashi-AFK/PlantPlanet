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

		// UI情報のすべてを保持
		itemTable_ = itemPanel_->GetUIObjects();

		// アイテムテーブルのみを取得
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


			// そのアイテム画像をアイテムテーブルに追加


			std::unordered_map<int, PlantData>allPlantData = PlantCollection::GetPlants();

			// 同じ植物の数をカウント
			std::unordered_map<std::string, int> countedPlant;
			for (const auto& plant : g_playerPlantData) {
				countedPlant[plant.name_]++;
			}

			for (int i = 0; i < invTable_.size(); i++) {

				// カウントした植物の数を取得
				int plantSize = countedPlant.size();

				if (((UIObject*)invTable_[i])->GetObjectName() == "INV_InventoryBackGround") {
					continue;
				}

				// 植物の数がiより大きい場合(取得できる場合)
				if (i <= plantSize - 1) {
					// countedPlantの中からi番目の植物を取得
					auto it = countedPlant.begin();
					std::advance(it, i); // i番目の要素に移動

					const std::string& plantName = it->first; // 植物の名前
					int plantCount = it->second;             // 植物の数

					for (const auto& p : allPlantData) {
						if (plantName == p.second.name_) {
							PlantData plantData = p.second;
							((UIButton*)invTable_[i])->SetImage(plantData.imageFilePath_);
							break;
						}
					}
				}
				else {
					// 取得できない場合は空の画像とテキストを表示
					//((UIButton*)invTable_[i])->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
				}
			}
		}
}