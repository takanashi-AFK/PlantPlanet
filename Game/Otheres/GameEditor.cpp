#include "GameEditor.h"
#include "../../Engine/ImGui/imgui.h"
#include "../../Engine/DirectX/Direct3D.h"
#include "../Objects/Stage/Stage.h"
#include "../Objects/Stage/StageObject.h"
#include "../Objects/UI/UIPanel.h"
#include "../Objects/UI/UIObject.h"
#include "../../Engine/Global.h"
#include "../../Engine/DirectX/Texture.h"
#include "../../Engine/GameObject/Camera.h"
#include "../Objects/Camera/TPSCamera.h"
#include "../Plants/PlantCollection.h"
#include "../../Engine/SceneManager.h"
#include "../../Engine/magic_enum/magic_enum.hpp"
#include "WinUser.h"
#include<format>
#include "../Objects/UI/UIButton.h"

using namespace FileManager;

namespace {
	Texture* pTexture_camera = nullptr;
}

GameEditor::GameEditor(GameObject* _parent)
	:GameObject(_parent, "GameEditor"), editStage_(nullptr), selectEditStageObjectIndex_(-1), editUIPanel_(nullptr), selectEditUIObjectIndex_(-1), editType_(NONE),layerNumberCount_(1)
{
}

void GameEditor::Initialize()
{
	// カメラ画像のテクスチャを読み込む
	pTexture_camera = new Texture();
	pTexture_camera->Load("Images/defaults/GUI_Camera.png");

	// カメラの位置を取得
	cameraPosition_ = Camera::GetPosition();
	cameraTarget_ = Camera::GetTarget();
}

void GameEditor::Update()
{
}

void GameEditor::Draw()
{
	// ワールドアウトライナーを描画
	DrawWorldOutLiner();

	// 詳細ウィンドウを描画
	DrawDatails();

	// UIオブジェクト作成ウィンドウを描画
	if(isShowCreateUIObjectWindow_)UIObjectCreateWindow();
}

void GameEditor::Release()
{
}

void GameEditor::SaveRecipe()
{
	//現在のカレントディレクトリを覚えておく
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// 保存先のファイルパスを取得
	string filePath{}; {
		// 「ファイルを保存」ダイアログの設定用構造体を設定
		OPENFILENAME ofn; {
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFilter = TEXT("objectData(*.json)\0*.json\0すべてのファイル(*.*)\0*.*\0\0");
			char fileName[MAX_PATH] = "無題.json";
			ofn.lpstrFile = fileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_OVERWRITEPROMPT;
			ofn.lpstrDefExt = "json";
			ofn.nFilterIndex = 1; // 初期選択するフィルター
			ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
		}

		// ファイルを保存するダイアログの表示
		if (GetSaveFileName(&ofn) == TRUE) {
			// ファイルパスを取得
			filePath = ofn.lpstrFile;

			// カレントディレクトリからの相対パスを取得
			filePath = GetAssetsRelativePath(filePath);

			// 文字列内の"\\"を"/"に置換
			ReplaceBackslashes(filePath);

			// ディレクトリを戻す
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return;
		}
	}

	// ファイルにステージ情報を保存
	json saveObj;
	maker_.Save(saveObj);
	JsonReader::Save(filePath, saveObj);
}

void GameEditor::LoadRecipe()
{
	//現在のカレントディレクトリを覚えておく
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// 読み込むファイルのパスを取得
	string filePath{}; {
		// 「ファイルを開く」ダイアログの設定用構造体を設定
		OPENFILENAME ofn; {
			TCHAR szFile[MAX_PATH] = {}; // ファイル名を格納するバッファ
			ZeroMemory(&ofn, sizeof(ofn)); // 構造体の初期化
			ofn.lStructSize = sizeof(ofn); // 構造体のサイズ
			ofn.lpstrFile = szFile; // ファイル名を格納するバッファ
			ofn.lpstrFile[0] = '\0'; // 初期化
			ofn.nMaxFile = sizeof(szFile); // ファイル名バッファのサイズ
			ofn.lpstrFilter = TEXT("JSONファイル(*.json)\0*.json\0すべてのファイル(*.*)\0*.*\0"); // フィルター（FBXファイルのみ表示）
			ofn.nFilterIndex = 1; // 初期選択するフィルター
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // フラグ（ファイルが存在すること、パスが存在することを確認）
			ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
		}

		// ファイルを選択するダイアログの表示
		if (GetOpenFileName(&ofn) == TRUE) {
			// ファイルパスを取得
			filePath = ofn.lpstrFile;

			// カレントディレクトリからの相対パスを取得
			filePath = GetAssetsRelativePath(filePath);

			// 文字列内の"\\"を"/"に置換
			ReplaceBackslashes(filePath);

			// ディレクトリを戻す
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return;
		}
	}

	// ファイルを読み込みステージを生成
	json loadObj;
	JsonReader::Load(filePath, loadObj);
	maker_.Load(loadObj);
}

void GameEditor::DrawWorldOutLiner()
{
	// ImGuiで表示するウィンドウの設定を行う
	ImGui::SetNextWindowPos(ImVec2(Direct3D::screenWidth_ * 0.7f, 0));
	ImGui::SetNextWindowSize(ImVec2(Direct3D::screenWidth_ * 0.3f, Direct3D::screenHeight_ * 0.5f));

	// 固定ウィンドウを表示
	ImGui::Begin("World Outliner", NULL,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_AlwaysHorizontalScrollbar);
	{
		ImGui::BeginTabBar("tab Ber");{

			// ステージオブジェクトのタブを表示
			if(editStage_ != nullptr)
				if (ImGui::BeginTabItem("StageObject")) {
					DrawStageOutLiner();
					editType_ = STAGE;
					ImGui::EndTabItem();
				}
			
			// UIオブジェクトのタブを表示
			if(editUIPanel_ != nullptr)
				if (ImGui::BeginTabItem("UIPanel")) {
					DrawUIPanelOutLiner();
					editType_ = UIPANEL;
					ImGui::EndTabItem();
				}

			// カメラのタブを表示
			if (ImGui::BeginTabItem("Camera")) {
				ImGui::Image(pTexture_camera->GetSRV(), ImVec2(16*23, 9*23));

				ImGui::Dummy(ImVec2(0, 10));
				// ラジオボタン
				ImGui::Text("Camera type");
				if (ImGui::RadioButton("default camera", cameraType_ == DEFAULT))cameraType_ = DEFAULT;
				if (ImGui::RadioButton("tps camera", cameraType_ == TPS))cameraType_ = TPS;

				editType_ = CAMERA;
				ImGui::EndTabItem();
			}

			// 植物用のタブを表示
			if (ImGui::BeginTabItem("Plants")) {
				DrawPlantOutLiner();
				editType_ = PLANT;
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Salad")) {
				DrawSaladRecipeOutLiner();
				editType_ = SALADRECIPE;
				ImGui::EndTabItem();
			}

		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

void GameEditor::DrawStageOutLiner()
{
	ImGui::Text("StageObject menu");

	if (ImGui::Button("Add"))AddStageObject();
	ImGui::SameLine();

	if (ImGui::Button("Save"))SaveStage();
	ImGui::SameLine();

	if (ImGui::Button("Load"))LoadStage();
	ImGui::SameLine();

	if (ImGui::Button("Delete"))editStage_->DeleteAllStageObject();

	ImGui::Separator();

	ImGui::BeginChild("ObjectList"); {
		// リストを表示
		for (int i = 0; i < editStage_->GetStageObjects().size(); ++i)
			if (ImGui::Selectable(editStage_->GetStageObjects()[i]->GetObjectName().c_str(), selectEditStageObjectIndex_ == i)) {
				selectEditStageObjectIndex_ = i;
			}
	}
	ImGui::EndChild();
}

void GameEditor::DrawUIPanelOutLiner()
{
	ImGui::Text("UIObject menu");

	if (ImGui::Button("Add"))AddUIObject();
	ImGui::SameLine();

	if (ImGui::Button("Save"))SaveUIPanel();
	ImGui::SameLine();

	if (ImGui::Button("Load"))LoadUIPanel();
	ImGui::SameLine();

	if (ImGui::Button("Delete"))editUIPanel_->DeleteAllUIObject();
	ImGui::SameLine();

	if (ImGui::Button("Hide")) for (auto obj : editUIPanel_->GetUIObjects())obj->SetVisible(false);
	ImGui::SameLine();

	if (ImGui::Button("Show")) for (auto obj : editUIPanel_->GetUIObjects())obj->SetVisible(true);

	{
		auto ins = UIPanel::GetInstance();
		ins->CheckSelectingButton();
		int x, y;
		ins->GetButtonIndex(&x, &y);

		ImGui::Text(std::format("<{},{}>",x,y ).c_str());

		if (ImGui::Button("->")) ins->SelectorMove(UIPanel::SELECTOR_MOVE_TO::RIGHT);
		if (ImGui::Button("<-"))ins->SelectorMove(UIPanel::SELECTOR_MOVE_TO::LEFT);
		if (ImGui::Button("up"))ins->SelectorMove(UIPanel::SELECTOR_MOVE_TO::UP);
		if (ImGui::Button("btm")) ins->SelectorMove(UIPanel::SELECTOR_MOVE_TO::BOTTOM);

		auto ptr = ins->GetSelectingButton();
		if (ptr)
		{
			ImGui::SameLine(); ImGui::Text(ptr->GetObjectName().c_str());

			ptr->SetShader(Direct3D::SHADER_BUTTON_SELECT);
		}
	}

	ImGui::Separator();

	ImGui::BeginChild("ObjectList"); {
		// リストを表示
		for (int i = 0; i < editUIPanel_->GetUIObjects().size(); ++i)
			if (ImGui::Selectable(editUIPanel_->GetUIObjects()[i]->GetObjectName().c_str(), selectEditUIObjectIndex_ == i)) {
				selectEditUIObjectIndex_ = i;
			}
	}
	ImGui::EndChild();
}

void GameEditor::DrawDatails()
{
	// ImGuiで表示するウィンドウの設定を行う
	ImGui::SetNextWindowPos(ImVec2(Direct3D::screenWidth_ * 0.7f, Direct3D::screenHeight_ * 0.5f));
	ImGui::SetNextWindowSize(ImVec2(Direct3D::screenWidth_ * 0.3f, Direct3D::screenHeight_ * 0.5f));

	// 固定ウィンドウを表示
	ImGui::Begin("Details", NULL,
				ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysHorizontalScrollbar);
	{
		switch (editType_)
		{
		case STAGE:DrawStageObjectDatails(); break;
		case UIPANEL:DrawUIObjectDatails();break;
		case CAMERA:DrawDatalsCamera(); break;
		case PLANT:DrawPlantDatails(); break;
		case SALADRECIPE:DrawSaladRecipeDatails(); break;
		default:ImGui::Text("No information to display");break;
		}
	}
	ImGui::End();
}

void GameEditor::DrawStageObjectDatails()
{
	if (editStage_ == nullptr)return;

	if (selectEditStageObjectIndex_ >= 0 && selectEditStageObjectIndex_ < editStage_->GetStageObjects().size()) {

		editStage_->GetStageObjects()[selectEditStageObjectIndex_]->DrawData();
	}
	else ImGui::Text("No object selected");
}

void GameEditor::DrawUIObjectDatails()
{
	if (editUIPanel_ == nullptr)return;

	if (selectEditUIObjectIndex_ >= 0 && selectEditUIObjectIndex_ < editUIPanel_->GetUIObjects().size()) {

		editUIPanel_->GetUIObjects()[selectEditUIObjectIndex_]->ChildDrawData();
	}
	else ImGui::Text("No object selected");
}

void GameEditor::DrawPlantDatails()
{
	if (selectEditPlantIndex_ >= 0 && selectEditPlantIndex_ < PlantCollection::GetPlants().size()) {

		ImGui::Text("id:%d",PlantCollection::GetPlants()[selectEditPlantIndex_].id_);
		ImGui::Text("rarity:%d",PlantCollection::GetPlants()[selectEditPlantIndex_].rarity_);
		ImGui::InputInt("rarity", &PlantCollection::GetPlants()[selectEditPlantIndex_].rarity_);
		ImGui::Text("name:%s",PlantCollection::GetPlants()[selectEditPlantIndex_].name_.c_str());
		ImGui::InputText("name", &PlantCollection::GetPlants()[selectEditPlantIndex_].name_[0], PlantCollection::GetPlants()[selectEditPlantIndex_].name_.size());
		ImGui::Text("isSpawn", PlantCollection::GetPlants()[selectEditPlantIndex_].isSpawn_);
		ImGui::Checkbox("isSpawn", &PlantCollection::GetPlants()[selectEditPlantIndex_].isSpawn_);
		ImGui::Text("modelFilePath:%s",PlantCollection::GetPlants()[selectEditPlantIndex_].modelFilePath_.c_str());
		ImGui::Text("imageFilePath:%s",PlantCollection::GetPlants()[selectEditPlantIndex_].imageFilePath_.c_str());
		ImGui::Text("descriptionImageFilePath_Complete:%s",PlantCollection::GetPlants()[selectEditPlantIndex_].textImageFilePath_.c_str());
	}
	else ImGui::Text("No object selected");
}

void GameEditor::DrawDatalsCamera()
{
	ImGui::Text("Camera menu");
	switch (cameraType_)
	{
	case DEFAULT:
		tpsCamera_->SetActive(false);
		// カメラの位置を設定
		ImGui::DragFloat3("Camera position", &cameraPosition_.x);
		// カメラの焦点を設定
		ImGui::DragFloat3("Camera target", &cameraTarget_.x);

		ImGui::DragFloat3("3D CamMove", &threeDCamMove_.x);
		// 初期化ボタン
		if (ImGui::Button("Default")){
			cameraPosition_ = { 0,30,-20 };
			cameraTarget_   = { 0,-20,20 };
			threeDCamMove_  = { 0,0,0 };
		}
		
		
		// カメラの位置を設定
		Camera::SetPosition(cameraPosition_ + threeDCamMove_);
		// カメラの焦点を設定
		Camera::SetTarget(cameraTarget_ + threeDCamMove_);

		break;

	case TPS:
		// 保存ボタン
		if (ImGui::Button("save")) {
			json saveObj;
			tpsCamera_->Save(saveObj);
			JsonReader::Save("Datas/CameraLayouts/TPSCamera.json", saveObj);
		}
		ImGui::SameLine();

		// 読み込みボタン
		if (ImGui::Button("load")) {
			json loadObj;
			JsonReader::Load("Datas/CameraLayouts/TPSCamera.json", loadObj);
			tpsCamera_->Load(loadObj);
		}

		ImGui::Separator();

		// 設定用のウィンドウを表示
		tpsCamera_->DrawData();

		break;
	}
}

void GameEditor::DrawSaladRecipeDatails()
{

}

void GameEditor::UIObjectCreateWindow()
{
	static char nameBuffer[256] = "";
	if (isShowCreateUIObjectWindow_) {
		ImGui::Begin("Create UIObject", &isShowCreateUIObjectWindow_); {

			ImGui::NewLine();
			ImGui::Text("Set the details of the object to be generated!!");
			ImGui::Separator();

			// 名前を入力
			ImGui::InputTextWithHint(":setting name", "Input object name...", nameBuffer, IM_ARRAYSIZE(nameBuffer));

			// タイプを選択
			static UIType uitype = UIType::UI_NONE;	// 初期選択項目
			static std::string type = "NONE";		// 初期選択項目

			if (ImGui::BeginCombo(":setting type", type.c_str())) {
				for (int i = 0; i < UIType::UI_MAX; i++) {
					std::string uiTypeString = UIObject::GetUITypeString((UIType)i);
					if (uiTypeString.empty()) continue; // 空文字列を無視

					bool isSelected = (type == UIObject::GetUITypeString((UIType)i));
					if (ImGui::Selectable(UIObject::GetUITypeString((UIType)i).c_str(), isSelected)) {
						type = UIObject::GetUITypeString((UIType)i);
						uitype = (UIType)i;
					}
					if (isSelected) {
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			static bool isEasing = false;
			ImGui::Checkbox(":Is using Easing?",&isEasing);

			//レイヤー番号を入力
			static int beforeLayerNumber = -1; //直前のレイヤー番号
			bool isLayerNumberDuplicate = false; //レイヤー番号が重複しているか
			ImGui::InputInt("LayerNumber", &layerNumberCount_);
			//重複チェック
			for (const auto& uiObject : editUIPanel_->GetUIObjects()) {
				if (uiObject->GetLayerNumber() == layerNumberCount_) {
					isLayerNumberDuplicate = true;
					break;
				}
			}

			if (layerNumberCount_ <= 0) {
				layerNumberCount_ = 1;
			}

			//警告表示
			if (isLayerNumberDuplicate) {
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "LayerNumber is duplicated");
			}

			// 生成ボタン
			if (ImGui::Button("Create") && !isLayerNumberDuplicate) {
				// UIオブジェクトを作成・追加
				UIObject* obj = UIObject::CreateUIObject(nameBuffer, uitype, editUIPanel_,layerNumberCount_);

				//イージングの生成
				if (isEasing)	obj->CreateEasing();

			
				if (obj != nullptr) {
					isShowCreateUIObjectWindow_ = false;
					//レイヤー番号の更新
					layerNumberCount_++;
				}
			}
		}
		ImGui::End();
	}
}

void GameEditor::AddStageObject()
{
	// 追加するオブジェクトの初期名を設定
	string name = "object" + std::to_string(editStage_->objects_.size());

	//現在のカレントディレクトリを覚えておく
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// 追加するオブジェクトのモデルファイルパスを設定
	string filePath{}; {
		// 「ファイルを開く」ダイアログの設定用構造体を設定
		OPENFILENAME ofn; {
			TCHAR szFile[MAX_PATH] = {}; // ファイル名を格納するバッファ
			ZeroMemory(&ofn, sizeof(ofn)); // 構造体の初期化
			ofn.lStructSize = sizeof(ofn); // 構造体のサイズ
			ofn.lpstrFile = szFile; // ファイル名を格納するバッファ
			ofn.lpstrFile[0] = '\0'; // 初期化
			ofn.nMaxFile = sizeof(szFile); // ファイル名バッファのサイズ
			ofn.lpstrFilter = TEXT("FBXファイル(*.fbx)\0*.fbx\0すべてのファイル(*.*)\0*.*\0"); // フィルター（FBXファイルのみ表示）
			ofn.nFilterIndex = 1; // 初期選択するフィルター
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // フラグ（ファイルが存在すること、パスが存在することを確認）
			ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
		}

		// ファイルを選択するダイアログの表示
		if (GetOpenFileName(&ofn) == TRUE) {
			// ファイルパスを取得
			filePath = ofn.lpstrFile;

			// カレントディレクトリからの相対パスを取得
			filePath = GetAssetsRelativePath(filePath);

			// 文字列内の"\\"を"/"に置換
			ReplaceBackslashes(filePath);

			// ディレクトリを戻す
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return;
		}
	}

	// オブジェクトを作成・追加
	editStage_->AddStageObject(CreateStageObject(name, filePath, editStage_));
}

void GameEditor::SaveStage()
{
	//現在のカレントディレクトリを覚えておく
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// 保存先のファイルパスを取得
	string filePath{}; {
		// 「ファイルを保存」ダイアログの設定用構造体を設定
		OPENFILENAME ofn; {
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFilter = TEXT("objectData(*.json)\0*.json\0すべてのファイル(*.*)\0*.*\0\0");
			char fileName[MAX_PATH] = "無題.json";
			ofn.lpstrFile = fileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_OVERWRITEPROMPT;
			ofn.lpstrDefExt = "json";
			ofn.nFilterIndex = 1; // 初期選択するフィルター
			ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
		}

		// ファイルを保存するダイアログの表示
		if (GetSaveFileName(&ofn) == TRUE) {
			// ファイルパスを取得
			filePath = ofn.lpstrFile;

			// カレントディレクトリからの相対パスを取得
			filePath = GetAssetsRelativePath(filePath);

			// 文字列内の"\\"を"/"に置換
			ReplaceBackslashes(filePath);

			// ディレクトリを戻す
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return;
		}
	}

	// ファイルにステージ情報を保存
	json saveObj;
	editStage_->Save(saveObj);
	JsonReader::Save(filePath, saveObj);
	
}

void GameEditor::LoadStage()
{
	//現在のカレントディレクトリを覚えておく
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// 読み込むファイルのパスを取得
	string filePath{}; {
		// 「ファイルを開く」ダイアログの設定用構造体を設定
		OPENFILENAME ofn; {
			TCHAR szFile[MAX_PATH] = {}; // ファイル名を格納するバッファ
			ZeroMemory(&ofn, sizeof(ofn)); // 構造体の初期化
			ofn.lStructSize = sizeof(ofn); // 構造体のサイズ
			ofn.lpstrFile = szFile; // ファイル名を格納するバッファ
			ofn.lpstrFile[0] = '\0'; // 初期化
			ofn.nMaxFile = sizeof(szFile); // ファイル名バッファのサイズ
			ofn.lpstrFilter = TEXT("JSONファイル(*.json)\0*.json\0すべてのファイル(*.*)\0*.*\0"); // フィルター（FBXファイルのみ表示）
			ofn.nFilterIndex = 1; // 初期選択するフィルター
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // フラグ（ファイルが存在すること、パスが存在することを確認）
			ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
		}

		// ファイルを選択するダイアログの表示
		if (GetOpenFileName(&ofn) == TRUE) {
			// ファイルパスを取得
			filePath = ofn.lpstrFile;

			// カレントディレクトリからの相対パスを取得
			filePath = GetAssetsRelativePath(filePath);

			// 文字列内の"\\"を"/"に置換
			ReplaceBackslashes(filePath);

			// ディレクトリを戻す
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return;
		}
	}

	// ファイルを読み込みステージを生成
	json loadObj;
	JsonReader::Load(filePath, loadObj);
	editStage_->Load(loadObj);
}

void GameEditor::AddUIObject()
{
	isShowCreateUIObjectWindow_ = true;
}

void GameEditor::SaveUIPanel()
{
	//現在のカレントディレクトリを覚えておく
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// 保存先のファイルパスを取得
	string filePath{}; {
		// 「ファイルを保存」ダイアログの設定用構造体を設定
		OPENFILENAME ofn; {
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFilter = TEXT("objectData(*.json)\0*.json\0すべてのファイル(*.*)\0*.*\0\0");
			char fileName[MAX_PATH] = "無題.json";
			ofn.lpstrFile = fileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_OVERWRITEPROMPT;
			ofn.lpstrDefExt = "json";
			ofn.nFilterIndex = 1; // 初期選択するフィルター
			ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
		}

		// ファイルを保存するダイアログの表示
		if (GetSaveFileName(&ofn) == TRUE) {
			// ファイルパスを取得
			filePath = ofn.lpstrFile;

			// カレントディレクトリからの相対パスを取得
			filePath = GetAssetsRelativePath(filePath);

			// 文字列内の"\\"を"/"に置換
			ReplaceBackslashes(filePath);

			// ディレクトリを戻す
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return;
		}
	}

	// ファイルにステージ情報を保存
	json saveObj;
	editUIPanel_->Save(saveObj);
	JsonReader::Save(filePath, saveObj);
}

void GameEditor::LoadUIPanel()
{
	//現在のカレントディレクトリを覚えておく
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// 読み込むファイルのパスを取得
	string filePath{}; {
		// 「ファイルを開く」ダイアログの設定用構造体を設定
		OPENFILENAME ofn; {
			TCHAR szFile[MAX_PATH] = {}; // ファイル名を格納するバッファ
			ZeroMemory(&ofn, sizeof(ofn)); // 構造体の初期化
			ofn.lStructSize = sizeof(ofn); // 構造体のサイズ
			ofn.lpstrFile = szFile; // ファイル名を格納するバッファ
			ofn.lpstrFile[0] = '\0'; // 初期化
			ofn.nMaxFile = sizeof(szFile); // ファイル名バッファのサイズ
			ofn.lpstrFilter = TEXT("JSONファイル(*.json)\0*.json\0すべてのファイル(*.*)\0*.*\0"); // フィルター（FBXファイルのみ表示）
			ofn.nFilterIndex = 1; // 初期選択するフィルター
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // フラグ（ファイルが存在すること、パスが存在することを確認）
			ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
		}

		// ファイルを選択するダイアログの表示
		if (GetOpenFileName(&ofn) == TRUE) {
			// ファイルパスを取得
			filePath = ofn.lpstrFile;

			// カレントディレクトリからの相対パスを取得
			filePath = GetAssetsRelativePath(filePath);

			// 文字列内の"\\"を"/"に置換
			ReplaceBackslashes(filePath);

			// ディレクトリを戻す
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return;
		}
	}

	// ファイルを読み込みステージを生成
	json loadObj;
	JsonReader::Load(filePath, loadObj);
	editUIPanel_->Load(loadObj);
}

void GameEditor::DrawPlantOutLiner()
{
	// ファイル操作用のボタンを表示
	ImGui::Text("file menu"); {
		// ファイルの保存・読み込みボタン
		if (ImGui::Button("Save")) SavePlant(); ImGui::SameLine();
		if (ImGui::Button("Load")) LoadPlant(); ImGui::SameLine();
		if (ImGui::Button("Clear")) PlantCollection::ClearPlants();
		ImGui::Separator();
	}

	// 植物の追加用のボタンを表示
	ImGui::Text("create menu"); {
		// 植物名入力ボックス
		static char plantNameBuffer[256] = "";
		ImGui::InputTextWithHint("##:setting_name", "Input plant name...", plantNameBuffer, IM_ARRAYSIZE(plantNameBuffer));
		ImGui::SameLine();

		// 追加ボタン
		if (ImGui::Button("+")) {
			if (strlen(plantNameBuffer) > 0) {
				PlantCollection::AddPlant({
					((int)PlantCollection::GetPlants().size()),
					1,
					plantNameBuffer,
					3,
					true,
					"defaultPlant.fbx",
					"dafaultPlant.png",
					"defaultPlantDescription.png",
					"defaultPlantDescription.png"
					});
			}

			strcpy_s(plantNameBuffer, sizeof(plantNameBuffer), "");
		}
		ImGui::SameLine();

		// 詳細設定追加ボタン
		if (ImGui::Button(":")) isShowPlantWindow_ = true;

		// 名前が未入力の場合、赤文字で警告
		if (strlen(plantNameBuffer) == 0) {
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Name is required!");
		}

	} 
	ImGui::Separator();

	// 植物のリストを表示
	ImGui::BeginChild("ObjectList"); {
		for (int i = 0; i < PlantCollection::GetPlants().size(); ++i)
			if (ImGui::Selectable(PlantCollection::GetPlants()[i].name_.c_str(), selectEditPlantIndex_ == i))
				selectEditPlantIndex_ = i;
	}
	ImGui::EndChild();

	// 植物データ詳細設定追加ウィンドウを表示
	if (isShowPlantWindow_) DrawAddPlantWindow();
}

void GameEditor::DrawSaladRecipeOutLiner()
{
	ImGui::Text("Special Salad Recipe");

	if (ImGui::Button("Add")) maker_.Add();
	ImGui::SameLine();

	if (ImGui::Button("Save")) SaveRecipe();
	ImGui::SameLine();

	if (ImGui::Button("Load")) LoadRecipe();
	ImGui::SameLine();

	if (ImGui::Button("Delete")) maker_.Delete();

	maker_.DrawData();
}

void GameEditor::SavePlant()
{
	//現在のカレントディレクトリを覚えておく
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// 保存先のファイルパスを取得
	string filePath{}; {
		// 「ファイルを保存」ダイアログの設定用構造体を設定
		OPENFILENAME ofn; {
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFilter = TEXT("objectData(*.json)\0*.json\0すべてのファイル(*.*)\0*.*\0\0");
			char fileName[MAX_PATH] = "無題.json";
			ofn.lpstrFile = fileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_OVERWRITEPROMPT;
			ofn.lpstrDefExt = "json";
			ofn.nFilterIndex = 1; // 初期選択するフィルター
			ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
		}

		// ファイルを保存するダイアログの表示
		if (GetSaveFileName(&ofn) == TRUE) {
			// ファイルパスを取得
			filePath = ofn.lpstrFile;

			// カレントディレクトリからの相対パスを取得
			filePath = GetAssetsRelativePath(filePath);

			// 文字列内の"\\"を"/"に置換
			ReplaceBackslashes(filePath);

			// ディレクトリを戻す
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return;
		}
	}

	// ファイルにステージ情報を保存
	json saveObj;
	PlantCollection::Save(saveObj);
	JsonReader::Save(filePath, saveObj);
}

void GameEditor::LoadPlant()
{
	//現在のカレントディレクトリを覚えておく
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// 読み込むファイルのパスを取得
	string filePath{}; {
		// 「ファイルを開く」ダイアログの設定用構造体を設定
		OPENFILENAME ofn; {
			TCHAR szFile[MAX_PATH] = {}; // ファイル名を格納するバッファ
			ZeroMemory(&ofn, sizeof(ofn)); // 構造体の初期化
			ofn.lStructSize = sizeof(ofn); // 構造体のサイズ
			ofn.lpstrFile = szFile; // ファイル名を格納するバッファ
			ofn.lpstrFile[0] = '\0'; // 初期化
			ofn.nMaxFile = sizeof(szFile); // ファイル名バッファのサイズ
			ofn.lpstrFilter = TEXT("JSONファイル(*.json)\0*.json\0すべてのファイル(*.*)\0*.*\0"); // フィルター（FBXファイルのみ表示）
			ofn.nFilterIndex = 1; // 初期選択するフィルター
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // フラグ（ファイルが存在すること、パスが存在することを確認）
			ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
		}

		// ファイルを選択するダイアログの表示
		if (GetOpenFileName(&ofn) == TRUE) {
			// ファイルパスを取得
			filePath = ofn.lpstrFile;

			// カレントディレクトリからの相対パスを取得
			filePath = GetAssetsRelativePath(filePath);

			// 文字列内の"\\"を"/"に置換
			ReplaceBackslashes(filePath);

			// ディレクトリを戻す
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return;
		}
	}

	// ファイルを読み込みステージを生成
	json loadObj;
	JsonReader::Load(filePath, loadObj);
	PlantCollection::Load(loadObj);
}

void GameEditor::DrawAddPlantWindow()
{
	bool isValid = true; // バリデーションが通ったかどうかを示すフラグ

	ImGui::Begin("Add Plant!!", &isShowPlantWindow_);
	ImGui::Text("Please set the information of the plant to be added.");
	ImGui::Separator();

	// 植物名入力ボックス
	static char plantNameBuffer[256] = "";
	ImGui::InputTextWithHint(":setting name", "Input plant name...", plantNameBuffer, IM_ARRAYSIZE(plantNameBuffer));

	// 必須チェック: 植物名が空でないかをチェック
	if (strlen(plantNameBuffer) == 0) {
		isValid = false;
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Name is required!");
	}

	// レアリティ入力ボックス
	static int rarity = 1;
	ImGui::InputInt(":setting rarity", &rarity);

	// 必須チェック: レアリティが1以上であること
	if (rarity <= 0) {
		isValid = false;
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Rarity must be greater than 0!");
	}

	// エリア番号入力ボックス
	static int areaNumber = 1;
	ImGui::InputInt(":setting areaNumber", &areaNumber);

	// 必須チェック: エリア番号が1以上であること
	if (areaNumber <= 0) {
		isValid = false;
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Area number must be greater than 0!");
	}

	static bool isSpawn = true;
	ImGui::Checkbox(":setting isSpawn", &isSpawn);

	// モデルファイルパス入力ボックス
	static string modelFilePath = "none.fbx"; {
		ImGui::InputText(":setting modelFilePath", &modelFilePath[0], modelFilePath.size());
		ImGui::SameLine();
		if (ImGui::Button(":set Model")) modelFilePath = GetFBXFilePath();
	}
	
	// 画像ファイルパス入力ボックス
	static string imageFilePath = "none.png";
	static string imageFilePath_seclet = "none.png";

	if (ImGui::TreeNode("ImageFilePath")) {
		 
		// 通常時の画像ファイルパス入力ボックス
		ImGui::InputText(":default", &imageFilePath[0], imageFilePath.size());
		ImGui::SameLine();
		if (ImGui::Button(":set default")) imageFilePath = GetPNGFilePath();
		
		// 隠ぺい時の画像ファイルパス入力ボックス
		ImGui::InputText(":seclet", &imageFilePath_seclet[0], imageFilePath_seclet.size());
		ImGui::SameLine();
		if (ImGui::Button(":set seclet")) imageFilePath_seclet = GetPNGFilePath();
		
		ImGui::TreePop();
	}

	// 説明画像ファイルパス入力ボックス
	static string textImageFilePath = "none.png";
	static string textImageFilePath_seclet = "none.png";

	if (ImGui::TreeNode("TextImageFilePath")) {
		
		// 通常時の説明画像ファイルパス入力ボックス
		ImGui::InputText(":default", &textImageFilePath[0], textImageFilePath.size());
		ImGui::SameLine();
		if (ImGui::Button(":set default")) textImageFilePath = GetPNGFilePath();

		// 隠ぺい時の説明画像ファイルパス入力ボックス
		ImGui::InputText(":seclet", &textImageFilePath_seclet[0], textImageFilePath_seclet.size());
		ImGui::SameLine();
		if (ImGui::Button(":set seclet")) textImageFilePath_seclet = GetPNGFilePath();
		
		ImGui::TreePop();
	}
	ImGui::Separator();

	// 「Add」ボタンを無効化する
	if (isValid && ImGui::Button("Add")) {
		PlantCollection::AddPlant({
			(int)PlantCollection::GetPlants().size(),
			rarity,
			plantNameBuffer,
			areaNumber,
			isSpawn,
			modelFilePath,
			imageFilePath,
			imageFilePath_seclet,
			textImageFilePath,
			textImageFilePath_seclet
			});
	}
	else if (!isValid) {
		// 入力エラーがある場合に、ボタンを無効化する
		ImGui::BeginDisabled();
		ImGui::Button("Add");
		ImGui::EndDisabled();
	}
	ImGui::End();
}

string GameEditor::GetFBXFilePath()
{
	//現在のカレントディレクトリを覚えておく
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// 追加するオブジェクトのモデルファイルパスを設定
	string filePath{}; {
		// 「ファイルを開く」ダイアログの設定用構造体を設定
		OPENFILENAME ofn; {
			TCHAR szFile[MAX_PATH] = {}; // ファイル名を格納するバッファ
			ZeroMemory(&ofn, sizeof(ofn)); // 構造体の初期化
			ofn.lStructSize = sizeof(ofn); // 構造体のサイズ
			ofn.lpstrFile = szFile; // ファイル名を格納するバッファ
			ofn.lpstrFile[0] = '\0'; // 初期化
			ofn.nMaxFile = sizeof(szFile); // ファイル名バッファのサイズ
			ofn.lpstrFilter = TEXT("FBXファイル(*.fbx)\0*.fbx\0すべてのファイル(*.*)\0*.*\0"); // フィルター（FBXファイルのみ表示）			ofn.nFilterIndex = 1; // 初期選択するフィルター
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // フラグ（ファイルが存在すること、パスが存在することを確認）
			ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
		}

		// ファイルを選択するダイアログの表示
		if (GetOpenFileName(&ofn) == TRUE) {
			// ファイルパスを取得
			filePath = ofn.lpstrFile;

			// カレントディレクトリからの相対パスを取得
			filePath = GetAssetsRelativePath(filePath);

			// 文字列内の"\\"を"/"に置換
			ReplaceBackslashes(filePath);

			// ディレクトリを戻す
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return "";
		}
	}

	// ファイルパスを返す
	return filePath;
}

string GameEditor::GetPNGFilePath()
{
	//現在のカレントディレクトリを覚えておく
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// 追加するオブジェクトのモデルファイルパスを設定
	string filePath{}; {
		// 「ファイルを開く」ダイアログの設定用構造体を設定
		OPENFILENAME ofn; {
			TCHAR szFile[MAX_PATH] = {}; // ファイル名を格納するバッファ
			ZeroMemory(&ofn, sizeof(ofn)); // 構造体の初期化
			ofn.lStructSize = sizeof(ofn); // 構造体のサイズ
			ofn.lpstrFile = szFile; // ファイル名を格納するバッファ
			ofn.lpstrFile[0] = '\0'; // 初期化
			ofn.nMaxFile = sizeof(szFile); // ファイル名バッファのサイズ
			ofn.lpstrFilter = TEXT("PNGファイル(*.fbx)\0*.png\0すべてのファイル(*.*)\0*.*\0"); // フィルター（FBXファイルのみ表示）
			ofn.nFilterIndex = 1; // 初期選択するフィルター
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // フラグ（ファイルが存在すること、パスが存在することを確認）
			ofn.lpstrInitialDir = TEXT("."); // カレントディレクトリを初期選択位置として設定
		}

		// ファイルを選択するダイアログの表示
		if (GetOpenFileName(&ofn) == TRUE) {
			// ファイルパスを取得
			filePath = ofn.lpstrFile;

			// カレントディレクトリからの相対パスを取得
			filePath = GetAssetsRelativePath(filePath);

			// 文字列内の"\\"を"/"に置換
			ReplaceBackslashes(filePath);

			// ディレクトリを戻す
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return "";
		}
	}

	// ファイルパスを返す
	return filePath;
}