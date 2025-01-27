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
#include "../../../Engine/DirectX/Input.h"
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

	bool isFirstSelectButton_ = true;

	void Initialize()
	{
		itemPanel_ = UIPanel::GetInstance();
		allPlantData = PlantCollection::GetPlants();
		// アイテムテーブルのみを取得
		// 各種オブジェクトを打ち分け
		for (auto& item : itemPanel_->GetUIObjects()) {
			if (item->GetObjectName().starts_with("INV")) {
				invTable_.push_back(item);

				if (item->GetObjectName().starts_with("INV-Ingredients"))ingredientTable_.push_back(item);
				else if (item->GetObjectName().starts_with("INV-GetPlantText"))invTextTable_.push_back(item);
				else if (item->GetObjectName().starts_with("INV-GetPlant"))getPlantTable_.push_back(item);

				else if (item->GetObjectName() == "INV-MakeButton")makeButton_ = static_cast<UIButton*>(item);
			}
		}
		// ingredient(上の選択中のボタン)をすべてblankにする
		for (auto ingredient : ingredientTable_) {
			((UIButton*)ingredient)->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
		}
		// 選択できるボタンをすべてリセット
		itemPanel_->ResetArrayOfButton();

	}

	void Update()
	{
		itemPanel_ = UIPanel::GetInstance();
		int x, y;
		itemPanel_->GetButtonIndex(&x, &y);

		{
			if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)) {
				// 最初の入力かつ、選択できるボタンがある場合
				if (isFirstSelectButton_ == true && itemPanel_->GetArrayList().size() != 0) {
					// inventoryの一番左のボタンを選択
					itemPanel_->SetButtonArrayIndex(0, -1);
					isFirstSelectButton_ = false;
				}
				else
					// 選択中のボタンを左に移動
					itemPanel_->SelectorMove(UIPanel::SELECTOR_MOVE_TO::LEFT);
			}
			if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) {
				if (isFirstSelectButton_ == true && itemPanel_->GetArrayList().size() != 0) {
					// inventoryの一番左のボタンを選択

					itemPanel_->SetButtonArrayIndex(0, -1);
					isFirstSelectButton_ = false;
				}
				else
					itemPanel_->SelectorMove(UIPanel::SELECTOR_MOVE_TO::RIGHT);
			}

			if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_UP)) {
				if (isFirstSelectButton_ == true && itemPanel_->GetArrayList().size() != 0) {
					// inventoryの一番左のボタンを選択
					itemPanel_->SetButtonArrayIndex(0, -1);
					isFirstSelectButton_ = false;
				}
				else if (y == -1) { // インベントリの一番上のボタンを選択している場合

					// inventoryButtonListのなかの、selectablearrayに入ってる且つ一番IDが若いものを選択
					// 最小のX座標
					int lowestX = INT_MAX;
					for (auto ingredient : ingredientTable_) {
						for (auto selectable : itemPanel_->GetArrayList()) {
							// invが選択できる場合
							if (ingredient == selectable) {
								// invの座標を取得
								int x, y;
								((UIButton*)ingredient)->GetArrayPlace(&x, &y);
								// arrayのx座標が最小のものを選択
								if (x <= lowestX)
									lowestX = x;
							}
						}
					}

					// 一番arrayのXが若いボタンを選択
					itemPanel_->SetButtonArrayIndex(lowestX, 0);
				}
				else
					itemPanel_->SelectorMove(UIPanel::SELECTOR_MOVE_TO::UP);
			}
			if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_DOWN)) {
				if (isFirstSelectButton_ == true && itemPanel_->GetArrayList().size() != 0) {
					itemPanel_->SetButtonArrayIndex(3, 0);
					isFirstSelectButton_ = false;
				}
				else if (y == 0) {
					// getPlantTable_のなかの、selectablearrayに入ってる且つ一番IDが若いものを選択

					int arrayX = INT_MAX;
					for (auto inv : getPlantTable_) {
						for (auto selectable : itemPanel_->GetArrayList()) {
							if (inv == selectable) {
								int x, y;
								((UIButton*)inv)->GetArrayPlace(&x, &y);
								if (x <= arrayX)
									arrayX = x;
							}
						}
					}

					// 一番arrayのXが若いボタンを選択
					itemPanel_->SetButtonArrayIndex(arrayX, -1);
				}
				else
					itemPanel_->SelectorMove(UIPanel::SELECTOR_MOVE_TO::BOTTOM);
			}
			// インベントリを表示する
			if (Input::IsPadButtonDown(XINPUT_GAMEPAD_B)) {
				ShowInventory(false);
				isFirstSelectButton_ = true;
			}

		}


		// インベントリのボタンが押された場合
		for (auto inv : getPlantTable_) {
			if (Confirm((UIButton*)inv)) {

				// 選択した植物が3つ以上になったら抜ける
				if (selectedPlant_.size() >= 3) continue;

				// 選択した植物をselectedPlant_に追加
				for (auto& a : allPlantData) {
					if (a.second.imageFilePath_ == ((UIButton*)inv)->GetImageFilePath()) {
						selectedPlant_.push_back(a.second.name_);
						// マッチしたら終了
						break;
					}
				}

				// クリックされたら、INV-Ingredients0の画像を差し替え
				// INV-Ingredients0がもう入ってたら1に、1が入ってたら2に
				for (auto& ingredient : ingredientTable_) {
					if (((UIButton*)ingredient)->GetImageFilePath() == "Models/tentativeFlowers/BlankFlowerImage.png") {
						// ingredientの画像を差し替え
						((UIButton*)ingredient)->SetImage(((UIButton*)inv)->GetImageFilePath());
						// 画像が挿入されたボタンを選択可能リストに追加
						itemPanel_->PushButtonToArray((UIButton*)ingredient);
						break;
					}
				}
			}
			// 更新
			InventoryDataSet();
		}

		// レシピのボタンを押した場合 
		for (auto ingre : ingredientTable_) {
			if (Confirm((UIButton*)ingre)) {
				if (((UIButton*)ingre)->GetObjectName().starts_with("INV-Ingredients")) {

					for (auto& a : allPlantData) {
						if (a.second.imageFilePath_ == ((UIButton*)ingre)->GetImageFilePath()) {
							// `selectedPlant_`から最初に見つかった1つだけを削除
							auto it = std::find(selectedPlant_.begin(), selectedPlant_.end(), a.second.name_);
							if (it != selectedPlant_.end()) {
								// 選択した植物をselectedPlant_から削除
								selectedPlant_.erase(it);
							}
							break;
						}
					}
					for (auto& inventory : getPlantTable_) {
						if (((UIButton*)inventory)->GetImageFilePath() == "Models/tentativeFlowers/BlankFlowerImage.png") {
							// inventoryの画像を差し替え
							((UIButton*)inventory)->SetImage(((UIButton*)ingre)->GetImageFilePath());
							// ingredientの画像を差し替え
							((UIButton*)ingre)->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
							// からの画像が入っているボタンを選択可能リストから排除
							itemPanel_->RemoveButtonFromArray((UIButton*)ingre);
							break;
						}
					}
				}
				// 更新
					InventoryDataSet();
			}
		}

		// サラダを作るボタンが押された場合
		if (Confirm(makeButton_))
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

		itemPanel_->ResetArrayOfButton();

		itemPanel_->PushButtonToArray(makeButton_);

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
				int plantCount = it->second;			  // 植物の数

				for (const auto& p : allPlantData) {
					if (plantName == p.second.name_) {
						PlantData plantData = p.second;
						((UIButton*)getPlantTable_[i])->SetImage(plantData.imageFilePath_);
						((UIText*)invTextTable_[i])->SetText("x" + std::to_string(plantCount));
						itemPanel_->PushButtonToArray((UIButton*)getPlantTable_[i]);
						break;
					}
				}
				if (plantCount <= 0) {
					((UIButton*)getPlantTable_[i])->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
					((UIText*)invTextTable_[i])->SetText("");
					itemPanel_->RemoveButtonFromArray((UIButton*)getPlantTable_[i]);
				}
			}
			else {
				// 取得できない場合は空の画像とテキストを表示
				if (getPlantTable_[i]->GetObjectName().find("INV-InventoryBack") != 0) {
					((UIButton*)getPlantTable_[i])->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
					((UIText*)invTextTable_[i])->SetText(" ");
					itemPanel_->RemoveButtonFromArray((UIButton*)getPlantTable_[i]);

				}
			}
		}

		for (auto ing : ingredientTable_) {
			if (((UIButton*)ing)->GetImageFilePath() != "Models/tentativeFlowers/BlankFlowerImage.png") {
				itemPanel_->PushButtonToArray((UIButton*)ing);
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
	}

	bool IsShowInventory()
	{
		return showInventory_;
	}

	bool IsMadeSalad()
	{
		return isMadeSalad_;
	}
	bool Confirm(UIButton* _button)
	{
		itemPanel_ = UIPanel::GetInstance();

		return _button->OnClick() || itemPanel_->GetSelectingButton() == _button && Input::IsPadButtonDown(XINPUT_GAMEPAD_A);
	}
}