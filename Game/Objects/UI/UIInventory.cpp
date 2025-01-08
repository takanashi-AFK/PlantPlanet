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
#include "../Stage/MakeSalad.h"
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
	Component_PlayerBehavior* playerBehavior_;

	std::vector<std::string> selectedPlant_;
	MakeSalad maker_;
	UIButton* makeButton_;
	bool showInventory_ = false;
	bool isMadeSalad_ = false;

	Salad prevSalad;
	std::array<PlantData, MakeSalad::NEED_PLANT_NUM> prevRecipe_;

	void Initialize()
	{
		itemPanel_ = UIPanel::GetInstance();
		allPlantData = PlantCollection::GetPlants();
		// アイテムテーブルのみを取得
		for (auto& item : itemPanel_->GetUIObjects()) {
			if (item->GetObjectName().starts_with("INV")) {
				invTable_.push_back(item);

				if (item->GetObjectName().starts_with("INV-Ingredients"))ingredientTable_.push_back(item);
				else if (item->GetObjectName().starts_with("INV-GetPlantText"))invTextTable_.push_back(item);
				else if (item->GetObjectName().starts_with("INV-GetPlant"))getPlantTable_.push_back(item);

				else if (item->GetObjectName() == "INV-MakeButton")makeButton_ = static_cast<UIButton*>(item);
			}
		}

		for (auto ingredient : ingredientTable_) {
			((UIButton*)ingredient)->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
		}

		prevSalad.effect_0 = PlantData::GetFunction(-1);
		prevSalad.effect_1 = PlantData::GetFunction(-1);
		prevSalad.effect_2 = PlantData::GetFunction(-1);

		prevRecipe_[0].id_ = -1;
		prevRecipe_[1].id_ = -1;
		prevRecipe_[2].id_ = -1;

		json load;
		//datasってなんだ？しかしこういう名前のディレクトリに誰かがしたので仕方ない
		JsonReader::Load("Datas/Test/SaladMenu.json", load);
		maker_.Load(load);
	}

	void Update()
	{
		
		for (auto inv : getPlantTable_) {
			if (((UIButton*)inv)->OnClick()) {
				if (((UIButton*)inv)->GetObjectName().starts_with("INV-GetPlant")) {

					if (selectedPlant_.size() >= 3) continue;

					// 選択した植物をselectedPlant_に追加
					for (auto& a : allPlantData) {
						if (a.second.imageFilePath_ == ((UIButton*)inv)->GetImageFilePath()) {
							selectedPlant_.push_back(a.second.name_);
							break;
						}
					}

					// クリックされたら、INV-Ingredients0の画像を差し替え
					// INV-Ingredients0がもう入ってたら1に、1が入ってたら2に
					for (auto& ingredient : ingredientTable_) {
						if (((UIButton*)ingredient)->GetImageFilePath() == "Models/tentativeFlowers/BlankFlowerImage.png") {
							((UIButton*)ingredient)->SetImage(((UIButton*)inv)->GetImageFilePath());
							break;
						}
					}
				}

				UIInventory:: InventoryDataSet();
			}
		}

		// レシピのボタンを押した場合 
		for (auto ingre : ingredientTable_) {
			if (((UIButton*)ingre)->OnClick()) {
				if (((UIButton*)ingre)->GetObjectName().starts_with("INV-Ingredients")) {

					// 選択した植物をdecPlant_に追加
					for (auto& a : allPlantData) {
						if (a.second.imageFilePath_ == ((UIButton*)ingre)->GetImageFilePath()) {
							// `selectedPlant_`から最初に見つかった1つだけを削除
							auto it = std::find(selectedPlant_.begin(), selectedPlant_.end(), a.second.name_);
							if (it != selectedPlant_.end()) {
								selectedPlant_.erase(it);
							}
							break;
						}
					}
					for (auto& inventory : getPlantTable_) {
						if (((UIButton*)inventory)->GetImageFilePath() == "Models/tentativeFlowers/BlankFlowerImage.png") {
							((UIButton*)inventory)->SetImage(((UIButton*)ingre)->GetImageFilePath());
							((UIButton*)ingre)->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
							break;
						}
					}


				}
					InventoryDataSet();
			}
		}

		if (makeButton_->OnClick())
		{
			if (!Check()) return;

			Make();
			isMadeSalad_ = true;

			//インベントリから素材を消す
			//インベントリを抜ける

			vector<PlantData>pPlant = playerBehavior_->GetMyPlants();


			for (const auto& selected : selectedPlant_) {
				for (auto it = pPlant.begin(); it != pPlant.end(); ++it) {
					if (it->name_ == selected) {
						pPlant.erase(it); // 最初に見つかった1つだけを削除
						break;
					}
				}
			}

			playerBehavior_->SetMyPlants(pPlant);
			selectedPlant_.clear();

			for (auto& ingredient : ingredientTable_) {
				((UIButton*)ingredient)->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
			}
			ShowInventory(false);
		}
	}

	void ShowInventory(bool isShow)
	{
		for (auto inv : invTable_) {

			inv->SetVisible(isShow);
		}
		showInventory_ = isShow;
		makeButton_->SetVisible(isShow);
	}

	void InventoryDataSet()
	{
		if (pStage_ == nullptr)return;

		// プレイヤー情報を取得
		{
			for (auto pb : pStage_->FindComponents(ComponentType::PlayerBehavior))playerBehavior_ = (Component_PlayerBehavior*)pb;
		}
		// そのアイテム画像をアイテムテーブルに追加
		countedPlant.clear();
		// 同じ植物の数をカウント
		for (const auto& plant : playerBehavior_->GetMyPlants()) {
			countedPlant[plant.name_]++;
		}

		for (auto dec : selectedPlant_) {
			if (dec != "") {
				countedPlant[dec]--;
			}
		}

		for (int i = 0; i < getPlantTable_.size(); i++) {

			// カウントした植物の数を取得
			int plantSize = countedPlant.size();

			// 植物の数がiより大きい場合(取得できる場合)
			if (i < plantSize ) {
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
		selectedPlant_.clear();

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
	bool Check()
	{
		if (selectedPlant_.size() < 3) return false;

		return selectedPlant_[0] != "" && selectedPlant_[1] != "" && selectedPlant_[2] != "";
	}

	void Make()
	{
		for (const auto& plant : allPlantData) {
		
			bool isMake = false;
			int index = -1;

			if (selectedPlant_[0] == plant.second.name_)	maker_.SetRecipeDatum(plant.second, 0);
			if (selectedPlant_[1] == plant.second.name_)	maker_.SetRecipeDatum(plant.second, 1);
			if (selectedPlant_[2] == plant.second.name_)	maker_.SetRecipeDatum(plant.second, 2);
		}

		maker_.Make();
		
		auto salad = maker_.GetSalad();

		if (!playerBehavior_)	return;

		playerBehavior_->EatSalad(salad);

		prevSalad = salad;
		for (auto i = 0u; i < MakeSalad::NEED_PLANT_NUM; ++i) {
			prevRecipe_[i] = maker_.GetRecipeDatum(i);
		}
	}

	bool IsShowInventory()
	{
		return showInventory_;
	}

	bool IsMadeSalad()
	{
		return isMadeSalad_;
	}
}