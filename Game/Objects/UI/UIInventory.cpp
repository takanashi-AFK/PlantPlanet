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

	// ナチュラルソート用の比較関数
	bool naturalSortComparator(const UIObject& a, const UIObject& b) {
		const std::string& nameA = a.objectName_;
		const std::string& nameB = b.objectName_;

		auto isDigit = [](char c) { return std::isdigit(static_cast<unsigned char>(c)); };

		size_t i = 0, j = 0;
		while (i < nameA.size() && j < nameB.size()) {
			if (isDigit(nameA[i]) && isDigit(nameB[j])) {
				// 数字部分を比較
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
				// 数字以外の部分を比較
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

		// UI情報のすべてを保持
		itemTable_ = itemPanel_->GetUIObjects();

		// アイテムテーブルのみを取得
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


			// そのアイテム画像をアイテムテーブルに追加
			std::unordered_map<int, PlantData>allPlantData = PlantCollection::GetPlants();

			// プレイヤー情報を取得
			Component_PlayerBehavior* playerBehavior = nullptr; {
				for (auto pb : pStage->FindComponents(ComponentType::PlayerBehavior))playerBehavior = (Component_PlayerBehavior*)pb;
			}

			// プレイヤーが取得した植物の情報を取得
			getPlant_ = playerBehavior->GetMyPlants();

			// 同じ植物の数をカウント
			std::unordered_map<std::string, int> countedPlant;
			for (const auto& plant : getPlant_) {
				countedPlant[plant.name_]++;
			}

			for (int i = 0; i < invTable_.size(); i++) {

				// カウントした植物の数を取得
				int plantSize = countedPlant.size();

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
					if (invTable_[i]->GetObjectName().find("INV-InventoryBack") != 0)
					((UIButton*)invTable_[i])->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
				}
			}
		}
}