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
		// �A�C�e���e�[�u���݂̂��擾
		// �e��I�u�W�F�N�g��ł�����
		for (auto& item : itemPanel_->GetUIObjects()) {
			if (item->GetObjectName().starts_with("INV")) {
				invTable_.push_back(item);

				if (item->GetObjectName().starts_with("INV-Ingredients"))ingredientTable_.push_back(item);
				else if (item->GetObjectName().starts_with("INV-GetPlantText"))invTextTable_.push_back(item);
				else if (item->GetObjectName().starts_with("INV-GetPlant"))getPlantTable_.push_back(item);

				else if (item->GetObjectName() == "INV-MakeButton")makeButton_ = static_cast<UIButton*>(item);
			}
		}
		// ingredient(��̑I�𒆂̃{�^��)�����ׂ�blank�ɂ���
		for (auto ingredient : ingredientTable_) {
			((UIButton*)ingredient)->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
		}
		// �I���ł���{�^�������ׂă��Z�b�g
		itemPanel_->ResetArrayOfButton();

	}

	void Update()
	{
		itemPanel_ = UIPanel::GetInstance();
		int x, y;
		itemPanel_->GetButtonIndex(&x, &y);

		{
			if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_LEFT)) {
				// �ŏ��̓��͂��A�I���ł���{�^��������ꍇ
				if (isFirstSelectButton_ == true && itemPanel_->GetArrayList().size() != 0) {
					// inventory�̈�ԍ��̃{�^����I��
					itemPanel_->SetButtonArrayIndex(0, -1);
					isFirstSelectButton_ = false;
				}
				else
					// �I�𒆂̃{�^�������Ɉړ�
					itemPanel_->SelectorMove(UIPanel::SELECTOR_MOVE_TO::LEFT);
			}
			if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_RIGHT)) {
				if (isFirstSelectButton_ == true && itemPanel_->GetArrayList().size() != 0) {
					// inventory�̈�ԍ��̃{�^����I��

					itemPanel_->SetButtonArrayIndex(0, -1);
					isFirstSelectButton_ = false;
				}
				else
					itemPanel_->SelectorMove(UIPanel::SELECTOR_MOVE_TO::RIGHT);
			}

			if (Input::IsPadButtonDown(XINPUT_GAMEPAD_DPAD_UP)) {
				if (isFirstSelectButton_ == true && itemPanel_->GetArrayList().size() != 0) {
					// inventory�̈�ԍ��̃{�^����I��
					itemPanel_->SetButtonArrayIndex(0, -1);
					isFirstSelectButton_ = false;
				}
				else if (y == -1) { // �C���x���g���̈�ԏ�̃{�^����I�����Ă���ꍇ

					// inventoryButtonList�̂Ȃ��́Aselectablearray�ɓ����Ă銎���ID���Ⴂ���̂�I��
					// �ŏ���X���W
					int lowestX = INT_MAX;
					for (auto ingredient : ingredientTable_) {
						for (auto selectable : itemPanel_->GetArrayList()) {
							// inv���I���ł���ꍇ
							if (ingredient == selectable) {
								// inv�̍��W���擾
								int x, y;
								((UIButton*)ingredient)->GetArrayPlace(&x, &y);
								// array��x���W���ŏ��̂��̂�I��
								if (x <= lowestX)
									lowestX = x;
							}
						}
					}

					// ���array��X���Ⴂ�{�^����I��
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
					// getPlantTable_�̂Ȃ��́Aselectablearray�ɓ����Ă銎���ID���Ⴂ���̂�I��

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

					// ���array��X���Ⴂ�{�^����I��
					itemPanel_->SetButtonArrayIndex(arrayX, -1);
				}
				else
					itemPanel_->SelectorMove(UIPanel::SELECTOR_MOVE_TO::BOTTOM);
			}
			// �C���x���g����\������
			if (Input::IsPadButtonDown(XINPUT_GAMEPAD_B)) {
				ShowInventory(false);
				isFirstSelectButton_ = true;
			}

		}


		// �C���x���g���̃{�^���������ꂽ�ꍇ
		for (auto inv : getPlantTable_) {
			if (Confirm((UIButton*)inv)) {

				// �I�������A����3�ȏ�ɂȂ����甲����
				if (selectedPlant_.size() >= 3) continue;

				// �I�������A����selectedPlant_�ɒǉ�
				for (auto& a : allPlantData) {
					if (a.second.imageFilePath_ == ((UIButton*)inv)->GetImageFilePath()) {
						selectedPlant_.push_back(a.second.name_);
						// �}�b�`������I��
						break;
					}
				}

				// �N���b�N���ꂽ��AINV-Ingredients0�̉摜�������ւ�
				// INV-Ingredients0�����������Ă���1�ɁA1�������Ă���2��
				for (auto& ingredient : ingredientTable_) {
					if (((UIButton*)ingredient)->GetImageFilePath() == "Models/tentativeFlowers/BlankFlowerImage.png") {
						// ingredient�̉摜�������ւ�
						((UIButton*)ingredient)->SetImage(((UIButton*)inv)->GetImageFilePath());
						// �摜���}�����ꂽ�{�^����I���\���X�g�ɒǉ�
						itemPanel_->PushButtonToArray((UIButton*)ingredient);
						break;
					}
				}
			}
			// �X�V
			InventoryDataSet();
		}

		// ���V�s�̃{�^�����������ꍇ 
		for (auto ingre : ingredientTable_) {
			if (Confirm((UIButton*)ingre)) {
				if (((UIButton*)ingre)->GetObjectName().starts_with("INV-Ingredients")) {

					for (auto& a : allPlantData) {
						if (a.second.imageFilePath_ == ((UIButton*)ingre)->GetImageFilePath()) {
							// `selectedPlant_`����ŏ��Ɍ�������1�������폜
							auto it = std::find(selectedPlant_.begin(), selectedPlant_.end(), a.second.name_);
							if (it != selectedPlant_.end()) {
								// �I�������A����selectedPlant_����폜
								selectedPlant_.erase(it);
							}
							break;
						}
					}
					for (auto& inventory : getPlantTable_) {
						if (((UIButton*)inventory)->GetImageFilePath() == "Models/tentativeFlowers/BlankFlowerImage.png") {
							// inventory�̉摜�������ւ�
							((UIButton*)inventory)->SetImage(((UIButton*)ingre)->GetImageFilePath());
							// ingredient�̉摜�������ւ�
							((UIButton*)ingre)->SetImage("Models/tentativeFlowers/BlankFlowerImage.png");
							// ����̉摜�������Ă���{�^����I���\���X�g����r��
							itemPanel_->RemoveButtonFromArray((UIButton*)ingre);
							break;
						}
					}
				}
				// �X�V
					InventoryDataSet();
			}
		}

		// �T���_�����{�^���������ꂽ�ꍇ
		if (Confirm(makeButton_))
		{
			if (!Check()) return;

			Make();
			isMadeSalad_ = true;

			//�C���x���g������f�ނ�����
			//�C���x���g���𔲂���

			vector<PlantData>pPlant = playerBehavior_->GetMyPlants();


			for (const auto& selected : selectedPlant_) {
				for (auto it = pPlant.begin(); it != pPlant.end(); ++it) {
					if (it->name_ == selected) {
						pPlant.erase(it); // �ŏ��Ɍ�������1�������폜
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

		// �v���C���[�����擾
		{
			for (auto pb : pStage_->FindComponents(ComponentType::PlayerBehavior))playerBehavior_ = (Component_PlayerBehavior*)pb;
		}
		// ���̃A�C�e���摜���A�C�e���e�[�u���ɒǉ�
		countedPlant.clear();
		// �����A���̐����J�E���g
		for (const auto& plant : playerBehavior_->GetMyPlants()) {
			countedPlant[plant.name_]++;
		}

		for (auto dec : selectedPlant_) {
			if (dec != "") {
				countedPlant[dec]--;
			}
		}

		for (int i = 0; i < getPlantTable_.size(); i++) {

			// �J�E���g�����A���̐����擾
			int plantSize = countedPlant.size();

			// �A���̐���i���傫���ꍇ(�擾�ł���ꍇ)
			if (i < plantSize ) {
				// countedPlant�̒�����i�Ԗڂ̐A�����擾
				auto it = countedPlant.begin();
				std::advance(it, i); // i�Ԗڂ̗v�f�Ɉړ�

				const std::string& plantName = it->first; // �A���̖��O
				int plantCount = it->second;			  // �A���̐�

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
				// �擾�ł��Ȃ��ꍇ�͋�̉摜�ƃe�L�X�g��\��
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