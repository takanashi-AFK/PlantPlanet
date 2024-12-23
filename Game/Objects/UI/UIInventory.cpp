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

	std::vector<std::string> decPlant_;
	std::vector<std::string> incPlant_;

	void Initialize()
	{
		itemPanel_ = UIPanel::GetInstance();
		allPlantData = PlantCollection::GetPlants();
		// アイテムテーブルのみを取得
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

					if (decPlant_.size() >= 3)continue;


					//　クリックされたら、INV-Ingredients0の画像を差し替え
					// INV-Ingredients0がもう入ってたら1に、1が入ってたら2に
					for (auto& ingredient : ingredientTable_) {
						if (((UIButton*)ingredient)->GetImageFilePath() == "Models/tentativeFlowers/BlankFlowerImage.png") {
							((UIButton*)ingredient)->SetImage(((UIButton*)inv)->GetImageFilePath());
							break;
						}
					}

					for (auto& a : allPlantData) {
						if (a.second.imageFilePath_ == ((UIButton*)inv)->GetImageFilePath()) {
							decPlant_.push_back(a.second.name_);
							break;
						}
					}
				}
				
					InventoryDataSet();
			}
		}
		// レシピのボタンを押した場合
		for (auto ingre : ingredientTable_) {
			if (((UIButton*)ingre)->OnClick()) {
				if (((UIButton*)ingre)->GetObjectName().starts_with("INV-Ingredients")) {
					// 空にする
					std::string removedPlant = "";
					for (auto& a : allPlantData) {
						if (a.second.imageFilePath_ == ((UIButton*)ingre)->GetImageFilePath()) {
							removedPlant = a.second.name_;
							break;
						}
					}
					((UIButton*)ingre)->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");

					// インベントリに戻す
					if (!removedPlant.empty()) {
						incPlant_.push_back(removedPlant);

						for (auto d : decPlant_) {
							if (d == removedPlant) {
								decPlant_.erase(std::remove(decPlant_.begin(), decPlant_.end(), d), decPlant_.end());
								break;
							}
						}
					}

					InventoryDataSet();
				}
			}
		}
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

		// プレイヤー情報を取得
		Component_PlayerBehavior* playerBehavior = nullptr; {
			for (auto pb : pStage_->FindComponents(ComponentType::PlayerBehavior))playerBehavior = (Component_PlayerBehavior*)pb;
		}
		// そのアイテム画像をアイテムテーブルに追加
		countedPlant.clear();
		// 同じ植物の数をカウント
		for (const auto& plant : playerBehavior->GetMyPlants()) {
			countedPlant[plant.name_]++;
		}

		for (auto dec : decPlant_) {
			if (dec != "")countedPlant[dec]--;
		}

		for (auto inc : incPlant_) {
			if (inc != "")countedPlant[inc]++;
		}

		for (int i = 0; i < getPlantTable_.size(); i++) {

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
						((UIButton*)getPlantTable_[i])->SetImage(plantData.imageFilePath_);
						((UIText*)invTextTable_[i])->SetText("x" + std::to_string(plantCount));
						break;
					}
				}
				if (plantCount == 0) {
					((UIButton*)getPlantTable_[i])->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
					((UIText*)invTextTable_[i])->SetText("");

				}
			}
			else {
				// 取得できない場合は空の画像とテキストを表示
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

	void Release()
	{
		getPlantTable_.clear();
		invTable_.clear();
		invTextTable_.clear();
		ingredientTable_.clear();
		countedPlant.clear();
		allPlantData.clear();
		decPlant_.clear();
		incPlant_.clear();

		for (auto deleteobj : getPlantTable_) {
			itemPanel_->DeleteUIObject(deleteobj);
		}
		for (auto deleteobj : invTable_) {
			itemPanel_->DeleteUIObject(deleteobj);
		}
		for (auto deleteobj : invTextTable_) {
			itemPanel_->DeleteUIObject(deleteobj);
		}
		for (auto deleteobj : ingredientTable_) {
			itemPanel_->DeleteUIObject(deleteobj);
		}
		
	}	
}