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
#include <regex>

namespace UIInventory {
	UIPanel* itemPanel_; // 変更するUIパネル

	std::vector<UIObject*> invTable_; // インベントリすべてのUIObject

	std::vector<UIObject*> hasPlantTable_;// 取得した植物の画像リスト
	std::vector<UIObject*> hasPlantButtons_;// 取得した植物のボタンリスト

	std::vector<UIObject*> invTextTable_; // 取得した植物の数を表示するテキストリスト
	std::vector<UIObject*> cuttingBoardButtons_; // カッティングボードのボタンリスト
	std::vector<UIObject*> cuttingBoardPlants_; // カッティングボードのボタンリスト
	

	std::unordered_map<std::string, int> countedPlant;
	std::unordered_map<int, PlantData>allPlantData;

	StageObject* playerObjects_;
	Stage* pStage_;
	Component_PlayerBehavior* playerBehavior_;

	std::vector<std::string> selectedPlant_;
	MakeSalad maker_;
	UIButton* makeButton_;
	UIButton* makeFromHistoryButton_;

	bool showInventory_ = false;
	bool isMadeSalad_ = false;
	bool isFirstSelectButton_ = true;

	Salad prevSalad;
	std::array<PlantData, MakeSalad::NEED_PLANT_NUM> prevRecipe_;

	float fade_;

	void Initialize()
	{
		itemPanel_ = UIPanel::GetInstance();
		allPlantData = PlantCollection::GetPlants();
		// アイテムテーブルのみを取得
		// 各種オブジェクトを打ち分け
		for (auto& item : itemPanel_->GetUIObjects()) {
			if (item->GetObjectName().starts_with("INV")) {
				invTable_.push_back(item);

				if (item->GetObjectName().starts_with("INV-CuttingBoardButton"))cuttingBoardButtons_.push_back(item);
				else if (item->GetObjectName().starts_with("INV-CuttingBoardPlant"))cuttingBoardPlants_.push_back(item);

				else if (item->GetObjectName().starts_with("INV-InventoryPlantText"))invTextTable_.push_back(item);
				else if (item->GetObjectName().starts_with("INV-InventoryPlantImage"))hasPlantTable_.push_back(item);
				else if (item->GetObjectName().starts_with("INV-InventoryFrameButton"))hasPlantButtons_.push_back(item);

				else if (item->GetObjectName() == "INV-CreateButton")makeButton_ = static_cast<UIButton*>(item);
				else if (item->GetObjectName() == "INV-History-Button")makeFromHistoryButton_ = static_cast<UIButton*>(item);
			}
		}
		// ingredient(上の選択中のボタン)をすべてblankにする
		for (auto ingredient : cuttingBoardPlants_) {
			((UIImage*)ingredient)->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
		}

		prevSalad.effect_0 = PlantData::GetFunction(-1);
		prevSalad.effect_1 = PlantData::GetFunction(-1);
		prevSalad.effect_2 = PlantData::GetFunction(-1);

		prevRecipe_[0].id_ = -1;
		prevRecipe_[1].id_ = -1;
		prevRecipe_[2].id_ = -1;

		json load;
		JsonReader::Load("Datas/Test/SaladMenu.json", load);
		maker_.Load(load);

		// 選択できるボタンをすべてリセット
		itemPanel_->ResetArrayOfButton();

	}

	void Update()
	{
		itemPanel_ = UIPanel::GetInstance();
		int x, y;
		itemPanel_->GetButtonIndex(&x, &y);

		{

			//ポップアップの非表示
			(itemPanel_->FindObject("PopUp-Image-BackGround"))->SetVisible(false);
			(itemPanel_->FindObject("PopUp-Text-Title"))->SetVisible(false);
			for (auto i = 0u; i < Component_PlayerBehavior::NEED_PLANT_NUM; ++i) {

				(itemPanel_->FindObject(std::format("PopUp-Image-Effect-Icon{}", i)))->SetVisible(false);
				(UIPanel::GetInstance()->FindObject(std::format("PopUp-Text-Effect{}", i)))->SetVisible(false);
			}

			//拾った植物アイコンの非表示
			(itemPanel_->FindObject("PickUp-Plant-Image"))->SetVisible(false);
			(UIPanel::GetInstance()->FindObject("PickUp-Plant-BackGround"))->SetVisible(false);

			itemPanel_->GetUIObject("CheckLogo-IsBreakableWall")->SetVisible(false);
		}

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
					for (auto ingredient : cuttingBoardButtons_) {
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
					// hasPlantTable_のなかの、selectablearrayに入ってる且つ一番IDが若いものを選択

					int arrayX = INT_MAX;
					for (auto inv : hasPlantButtons_) {
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
		for (auto inv : hasPlantButtons_) {
			if (Confirm((UIButton*)inv)) {

				// 選択した植物が3つ以上になったら抜ける
				if (selectedPlant_.size() >= 3) continue;

				std::string input = inv->GetObjectName();
				std::regex re("(\\d+-\\d+)");  // "数字-数字" をキャプチャ
				std::smatch match;

				// マッチしなかったらスキップ
				if (!std::regex_search(input, match, re)) {
					continue;
				}

				std::string result = match[1].str();
				UIImage* image = nullptr;

				// hasPlantTable_ 内から result に一致するオブジェクトを探す
				for (auto plant : hasPlantTable_) {
					if (plant->GetObjectName().find(result) != std::string::npos) {
						image = (UIImage*)plant;
						break;
					}
				}

				// plant が見つからなかったらスキップ
				if (image == nullptr) continue;

				// 選択した植物を selectedPlant_ に追加
				for (auto& a : allPlantData) {
					if (a.second.imageFilePath_ == image->GetImageFilePath()) {
						selectedPlant_.push_back(a.second.name_);
						break;  // マッチしたら終了
					}
				}

				// クリックされたら、INV-Ingredients0の画像を差し替え
				for (auto& ingredient : cuttingBoardPlants_) {
					if (((UIImage*)ingredient)->GetImageFilePath() == "Models/tentativeFlowers/BlankFlowerImage.png") {
						((UIImage*)ingredient)->SetImage(image->GetImageFilePath());
						break;
					}
				}

				// 更新
				InventoryDataSet();
			}
		}


		// カッティングボードのボタンを押した場合 
		for (auto ingre : cuttingBoardButtons_) {
			if (Confirm((UIButton*)ingre)) {

				std::string str = ingre->GetObjectName();
				std::string number;

				// 末尾から数字を取得
				for (int i = str.size() - 1; i >= 0; --i) {
					if (std::isdigit(str[i])) {
						number = str[i] + number;  // 数字なら追加
					}
					else {
						break;  // 数字が終わったらループ終了
					}
				}

				UIImage* image = nullptr;

				// hasPlantTable_ 内から result に一致するオブジェクトを探す
				for (auto plant : cuttingBoardPlants_) {
					if (plant->GetObjectName().find(number) != std::string::npos) {
						image = (UIImage*)plant;
						break;
					}
				}

				for (auto& a : allPlantData) {
					if (a.second.imageFilePath_ == image->GetImageFilePath()) {
						// `selectedPlant_`から最初に見つかった1つだけを削除
						auto it = std::find(selectedPlant_.begin(), selectedPlant_.end(), a.second.name_);
						if (it != selectedPlant_.end()) {
							// 選択した植物をselectedPlant_から削除
							selectedPlant_.erase(it);
						}
						break;
					}
				}
				for (auto& inventory : hasPlantTable_) {
					if (((UIImage*)inventory)->GetImageFilePath() == "Models/tentativeFlowers/BlankFlowerImage.png") {
						// inventoryの画像を差し替え
						((UIImage*)inventory)->SetImage(image->GetImageFilePath());
						// ingredientの画像を差し替え
						image->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
						// からの画像が入っているボタンを選択可能リストから排除
						itemPanel_->RemoveButtonFromArray((UIButton*)ingre);
						break;
					}
				}
				// 更新
				InventoryDataSet();
			}
		}

		for (auto a : selectedPlant_) {
		
			ImGui::Text("%s", a.c_str());
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

			for (auto& ingredient : cuttingBoardPlants_) {
				((UIButton*)ingredient)->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
			}
			ShowInventory(false);
		}

		if (makeFromHistoryButton_->OnClick())
		{
			MakeFromHistory();
		}

		for (auto& ui : invTable_) {
			ui->SetFade(fade_);
		}
	}

	void ShowInventory(bool isShow)
	{
		for (auto inv : invTable_) {
			inv->SetVisible(isShow);
		}
		showInventory_ = isShow;
		makeButton_->SetVisible(isShow);

		// 履歴が揃っているか確認
		for (auto i = 0; i < MakeSalad::NEED_PLANT_NUM; ++i)
		{
			if (prevRecipe_[i].id_ == -1)	isShow = false;
		}
		makeFromHistoryButton_->SetVisible(isShow);
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

		for (int i = 0; i < hasPlantTable_.size(); i++) {

			// カウントした植物の数を取得
			int plantSize = countedPlant.size();

			// 植物の数がiより大きい場合(取得できる場合)
			if (i < plantSize) {
				// countedPlantの中からi番目の植物を取得
				auto it = countedPlant.begin();
				std::advance(it, i); // i番目の要素に移動

				const std::string& plantName = it->first; // 植物の名前
				int plantCount = it->second;			  // 植物の数

				for (const auto& p : allPlantData) {
					if (plantName == p.second.name_) {
						PlantData plantData = p.second;
						((UIImage*)hasPlantTable_[i])->SetImage(plantData.imageFilePath_);
						((UIText*)invTextTable_[i])->SetText("x" + std::to_string(plantCount));
						//itemPanel_->PushButtonToArray((UIImage*)hasPlantTable_[i]);
						break;
					}
				}
				if (plantCount <= 0) {
					((UIImage*)hasPlantTable_[i])->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
					((UIText*)invTextTable_[i])->SetText("");
					//itemPanel_->RemoveButtonFromArray((UIImage*)hasPlantTable_[i]);
				}
			}
			else {
				// 取得できない場合は空の画像とテキストを表示
				if (hasPlantTable_[i]->GetObjectName().find("INV-BackGround") != 0) {
					((UIImage*)hasPlantTable_[i])->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");

					((UIText*)invTextTable_[i])->SetText(" ");
					//itemPanel_->RemoveButtonFromArray((UIImage*)hasPlantTable_[i]);
				}
			}
		}

		for (auto ing : cuttingBoardPlants_) {
			if (((UIImage*)ing)->GetImageFilePath() != "Models/tentativeFlowers/BlankFlowerImage.png") {
			}
		}
	}
	void SetStage(Stage* pStage)
	{
		pStage_ = pStage;
	}

	void Release()
	{
		hasPlantTable_.clear();
		invTable_.clear();
		invTextTable_.clear();
		cuttingBoardButtons_.clear();
		cuttingBoardPlants_.clear();
		countedPlant.clear();
		allPlantData.clear();
		selectedPlant_.clear();
		hasPlantButtons_.clear();
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

		prevSalad = salad;
		for (auto i = 0u; i < MakeSalad::NEED_PLANT_NUM; ++i) {
			prevRecipe_[i] = maker_.GetRecipeDatum(i);
		}

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

	void SetsAlpha(uint8_t alpha)
	{
		//set all ui's alpha
		for (auto& ui : invTable_) {
			ui->SetAlpha(alpha);
		}
	}

	void SetFade(float fade)
	{
		fade_ = fade;
	}

	float GetFade()
	{
		return fade_;
	}

	void MakeFromHistory()
	{

		if (selectedPlant_.size() > 0)	return;


		for (auto i = 0; i < MakeSalad::NEED_PLANT_NUM; ++i)
		{
			if (prevRecipe_[i].id_ == -1)	return;
		}

		for (auto& itr : prevRecipe_) {

			if (countedPlant[itr.name_] > 0)
			{
				--countedPlant[itr.name_];
				selectedPlant_.push_back(itr.name_);
				for (auto& ingredient : cuttingBoardPlants_) {
					if (((UIImage*)ingredient)->GetImageFilePath() == "Models/tentativeFlowers/BlankFlowerImage.png")
					{
						((UIImage*)ingredient)->SetImage(itr.imageFilePath_);
						break;
					}
				}
			}
		}
		InventoryDataSet();
	}

	bool Confirm(UIButton* _button)
	{
		itemPanel_ = UIPanel::GetInstance();

		return _button->OnClick() || itemPanel_->GetSelectingButton() == _button && Input::IsPadButtonDown(XINPUT_GAMEPAD_A);
	}
}