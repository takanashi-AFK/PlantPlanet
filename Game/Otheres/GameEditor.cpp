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

using namespace FileManager;

namespace {
	Texture* pTexture_camera = nullptr;
}

GameEditor::GameEditor(GameObject* _parent)
	:GameObject(_parent, "StageEditor"), editStage_(nullptr), selectEditStageObjectIndex_(-1), editUIPanel_(nullptr), selectEditUIObjectIndex_(-1), editType_(NONE),layerNumberCount_(1)
{
}

void GameEditor::Initialize()
{
	// �J�����摜�̃e�N�X�`����ǂݍ���
	pTexture_camera = new Texture();
	pTexture_camera->Load("Images/defaults/GUI_Camera.png");

	// �J�����̈ʒu���擾
	cameraPosition_ = Camera::GetPosition();
	cameraTarget_ = Camera::GetTarget();
}

void GameEditor::Update()
{
}

void GameEditor::Draw()
{
	// ���[���h�A�E�g���C�i�[��`��
	DrawWorldOutLiner();

	// �ڍ׃E�B���h�E��`��
	DrawDatails();

	// UI�I�u�W�F�N�g�쐬�E�B���h�E��`��
	if(isShowCreateUIObjectWindow_)UIObjectCreateWindow();
}

void GameEditor::Release()
{
}

void GameEditor::DrawWorldOutLiner()
{
	// ImGui�ŕ\������E�B���h�E�̐ݒ���s��
	ImGui::SetNextWindowPos(ImVec2(Direct3D::screenWidth_ * 0.7f, 0));
	ImGui::SetNextWindowSize(ImVec2(Direct3D::screenWidth_ * 0.3f, Direct3D::screenHeight_ * 0.5f));

	// �Œ�E�B���h�E��\��
	ImGui::Begin("World Outliner", NULL,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_AlwaysHorizontalScrollbar);
	{
		ImGui::BeginTabBar("tab Ber");{

			// �X�e�[�W�I�u�W�F�N�g�̃^�u��\��
			if(editStage_ != nullptr)
				if (ImGui::BeginTabItem("StageObject")) {
					DrawStageOutLiner();
					editType_ = STAGE;
					ImGui::EndTabItem();
				}

			// UI�I�u�W�F�N�g�̃^�u��\��
			if(editUIPanel_ != nullptr)
				if (ImGui::BeginTabItem("UIPanel")) {
					DrawUIPanelOutLiner();
					editType_ = UIPANEL;
					ImGui::EndTabItem();
				}

			// �J�����̃^�u��\��
			if (ImGui::BeginTabItem("Camera")) {
				ImGui::Image(pTexture_camera->GetSRV(), ImVec2(16*23, 9*23));

				ImGui::Dummy(ImVec2(0, 10));
				// ���W�I�{�^��
				ImGui::Text("Camera type");
				if (ImGui::RadioButton("default camera", cameraType_ == DEFAULT))cameraType_ = DEFAULT;
				if (ImGui::RadioButton("tps camera", cameraType_ == TPS))cameraType_ = TPS;

				editType_ = CAMERA;
				ImGui::EndTabItem();
			}

			// �A���p�̃^�u��\��
			if (ImGui::BeginTabItem("Plants")) {
				DrawPlantOutLiner();
				editType_ = PLANT;
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
		// ���X�g��\��
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

	ImGui::Separator();

	ImGui::BeginChild("ObjectList"); {
		// ���X�g��\��
		for (int i = 0; i < editUIPanel_->GetUIObjects().size(); ++i)
			if (ImGui::Selectable(editUIPanel_->GetUIObjects()[i]->GetObjectName().c_str(), selectEditUIObjectIndex_ == i)) {
				selectEditUIObjectIndex_ = i;
			}
	}
	ImGui::EndChild();
}

void GameEditor::DrawDatails()
{
	// ImGui�ŕ\������E�B���h�E�̐ݒ���s��
	ImGui::SetNextWindowPos(ImVec2(Direct3D::screenWidth_ * 0.7f, Direct3D::screenHeight_ * 0.5f));
	ImGui::SetNextWindowSize(ImVec2(Direct3D::screenWidth_ * 0.3f, Direct3D::screenHeight_ * 0.5f));

	// �Œ�E�B���h�E��\��
	ImGui::Begin("Details", NULL,
				ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysHorizontalScrollbar);
	{
		switch (editType_)
		{
		case STAGE:DrawStageObjectDatails(); break;
		case UIPANEL:DrawUIObjectDatails();break;
		case CAMERA:DrawDatalsCamera(); break;
		case PLANT:DrawPlantDatails(); break;
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
		ImGui::Text("name:%s",PlantCollection::GetPlants()[selectEditPlantIndex_].name_.c_str());
		ImGui::Text("modelFilePath:%s",PlantCollection::GetPlants()[selectEditPlantIndex_].modelFilePath_.c_str());
		ImGui::Text("imageFilePath:%s",PlantCollection::GetPlants()[selectEditPlantIndex_].imageFilePath_.c_str());
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
		// �J�����̈ʒu��ݒ�
		ImGui::DragFloat3("Camera position", &cameraPosition_.x);
		// �J�����̏œ_��ݒ�
		ImGui::DragFloat3("Camera target", &cameraTarget_.x);

		ImGui::DragFloat3("3D CamMove", &threeDCamMove_.x);
		// �������{�^��
		if (ImGui::Button("Default")){
			cameraPosition_ = { 0,30,-20 };
			cameraTarget_   = { 0,-20,20 };
			threeDCamMove_  = { 0,0,0 };
		}
		
		
		// �J�����̈ʒu��ݒ�
		Camera::SetPosition(cameraPosition_ + threeDCamMove_);
		// �J�����̏œ_��ݒ�
		Camera::SetTarget(cameraTarget_ + threeDCamMove_);

		break;

	case TPS:
		// �ۑ��{�^��
		if (ImGui::Button("save")) {
			json saveObj;
			tpsCamera_->Save(saveObj);
			JsonReader::Save("Datas/CameraLayouts/TPSCamera.json", saveObj);
		}
		ImGui::SameLine();

		// �ǂݍ��݃{�^��
		if (ImGui::Button("load")) {
			json loadObj;
			JsonReader::Load("Datas/CameraLayouts/TPSCamera.json", loadObj);
			tpsCamera_->Load(loadObj);
		}

		ImGui::Separator();

		// �ݒ�p�̃E�B���h�E��\��
		tpsCamera_->DrawData();

		break;
	}
}

void GameEditor::UIObjectCreateWindow()
{
	static char nameBuffer[256] = "";
	if (isShowCreateUIObjectWindow_) {
		ImGui::Begin("Create UIObject", &isShowCreateUIObjectWindow_); {

			ImGui::NewLine();
			ImGui::Text("Set the details of the object to be generated!!");
			ImGui::Separator();

			// ���O�����
			ImGui::InputTextWithHint(":seting name", "Input object name...", nameBuffer, IM_ARRAYSIZE(nameBuffer));

			// �^�C�v��I��
			static UIType uitype = UIType::UI_NONE;	// �����I������
			static std::string type = "NONE";		// �����I������

			if (ImGui::BeginCombo(":seting type", type.c_str())) {
				for (int i = 0; i < UIType::UI_MAX; i++) {
					std::string uiTypeString = UIObject::GetUITypeString((UIType)i);
					if (uiTypeString.empty()) continue; // �󕶎���𖳎�

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

			//���C���[�ԍ������
			static int beforeLayerNumber = -1; //���O�̃��C���[�ԍ�
			bool isLayerNumberDuplicate = false; //���C���[�ԍ����d�����Ă��邩
			ImGui::InputInt("LayerNumber", &layerNumberCount_);
			//�d���`�F�b�N
			for (const auto& uiObject : editUIPanel_->GetUIObjects()) {
				if (uiObject->GetLayerNumber() == layerNumberCount_) {
					isLayerNumberDuplicate = true;
					break;
				}
			}

			if (layerNumberCount_ <= 0) {
				layerNumberCount_ = 1;
			}

			//�x���\��
			if (isLayerNumberDuplicate) {
				ImGui::TextColored(ImVec4(1, 0, 0, 1), "LayerNumber is duplicated");
			}

			// �����{�^��
			if (ImGui::Button("Create") && !isLayerNumberDuplicate) {
				// UI�I�u�W�F�N�g���쐬�E�ǉ�
				UIObject* obj = UIObject::CreateUIObject(nameBuffer, uitype, editUIPanel_,layerNumberCount_);

				//�C�[�W���O�̐���
				if (isEasing)	obj->CreateEasing();

				// ���������s��
				obj->Initialize();

				if (obj != nullptr) {
					isShowCreateUIObjectWindow_ = false;
					//���C���[�ԍ��̍X�V
					layerNumberCount_++;
				}
			}
		}
		ImGui::End();
	}
}

void GameEditor::AddStageObject()
{
	// �ǉ�����I�u�W�F�N�g�̏�������ݒ�
	string name = "object" + std::to_string(editStage_->objects_.size());

	//���݂̃J�����g�f�B���N�g�����o���Ă���
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// �ǉ�����I�u�W�F�N�g�̃��f���t�@�C���p�X��ݒ�
	string filePath{}; {
		// �u�t�@�C�����J���v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
		OPENFILENAME ofn; {
			TCHAR szFile[MAX_PATH] = {}; // �t�@�C�������i�[����o�b�t�@
			ZeroMemory(&ofn, sizeof(ofn)); // �\���̂̏�����
			ofn.lStructSize = sizeof(ofn); // �\���̂̃T�C�Y
			ofn.lpstrFile = szFile; // �t�@�C�������i�[����o�b�t�@
			ofn.lpstrFile[0] = '\0'; // ������
			ofn.nMaxFile = sizeof(szFile); // �t�@�C�����o�b�t�@�̃T�C�Y
			ofn.lpstrFilter = TEXT("FBX�t�@�C��(*.fbx)\0*.fbx\0���ׂẴt�@�C��(*.*)\0*.*\0"); // �t�B���^�[�iFBX�t�@�C���̂ݕ\���j
			ofn.nFilterIndex = 1; // �����I������t�B���^�[
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
			ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
		}

		// �t�@�C����I������_�C�A���O�̕\��
		if (GetOpenFileName(&ofn) == TRUE) {
			// �t�@�C���p�X���擾
			filePath = ofn.lpstrFile;

			// �J�����g�f�B���N�g������̑��΃p�X���擾
			filePath = GetAssetsRelativePath(filePath);

			// ���������"\\"��"/"�ɒu��
			ReplaceBackslashes(filePath);

			// �f�B���N�g����߂�
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return;
		}
	}

	// �I�u�W�F�N�g���쐬�E�ǉ�
	editStage_->AddStageObject(CreateStageObject(name, filePath, editStage_));
}

void GameEditor::SaveStage()
{
	//���݂̃J�����g�f�B���N�g�����o���Ă���
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// �ۑ���̃t�@�C���p�X���擾
	string filePath{}; {
		// �u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
		OPENFILENAME ofn; {
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFilter = TEXT("objectData(*.json)\0*.json\0���ׂẴt�@�C��(*.*)\0*.*\0\0");
			char fileName[MAX_PATH] = "����.json";
			ofn.lpstrFile = fileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_OVERWRITEPROMPT;
			ofn.lpstrDefExt = "json";
			ofn.nFilterIndex = 1; // �����I������t�B���^�[
			ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
		}

		// �t�@�C����ۑ�����_�C�A���O�̕\��
		if (GetSaveFileName(&ofn) == TRUE) {
			// �t�@�C���p�X���擾
			filePath = ofn.lpstrFile;

			// �J�����g�f�B���N�g������̑��΃p�X���擾
			filePath = GetAssetsRelativePath(filePath);

			// ���������"\\"��"/"�ɒu��
			ReplaceBackslashes(filePath);

			// �f�B���N�g����߂�
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return;
		}
	}

	// �t�@�C���ɃX�e�[�W����ۑ�
	json saveObj;
	editStage_->Save(saveObj);
	if (JsonReader::Save(filePath, saveObj) == false) MessageBox(NULL, "�ۑ��Ɏ��s���܂����B", 0, 0);
	
}

void GameEditor::LoadStage()
{
	//���݂̃J�����g�f�B���N�g�����o���Ă���
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// �ǂݍ��ރt�@�C���̃p�X���擾
	string filePath{}; {
		// �u�t�@�C�����J���v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
		OPENFILENAME ofn; {
			TCHAR szFile[MAX_PATH] = {}; // �t�@�C�������i�[����o�b�t�@
			ZeroMemory(&ofn, sizeof(ofn)); // �\���̂̏�����
			ofn.lStructSize = sizeof(ofn); // �\���̂̃T�C�Y
			ofn.lpstrFile = szFile; // �t�@�C�������i�[����o�b�t�@
			ofn.lpstrFile[0] = '\0'; // ������
			ofn.nMaxFile = sizeof(szFile); // �t�@�C�����o�b�t�@�̃T�C�Y
			ofn.lpstrFilter = TEXT("JSON�t�@�C��(*.json)\0*.json\0���ׂẴt�@�C��(*.*)\0*.*\0"); // �t�B���^�[�iFBX�t�@�C���̂ݕ\���j
			ofn.nFilterIndex = 1; // �����I������t�B���^�[
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
			ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
		}

		// �t�@�C����I������_�C�A���O�̕\��
		if (GetOpenFileName(&ofn) == TRUE) {
			// �t�@�C���p�X���擾
			filePath = ofn.lpstrFile;

			// �J�����g�f�B���N�g������̑��΃p�X���擾
			filePath = GetAssetsRelativePath(filePath);

			// ���������"\\"��"/"�ɒu��
			ReplaceBackslashes(filePath);

			// �f�B���N�g����߂�
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return;
		}
	}

	// �t�@�C����ǂݍ��݃X�e�[�W�𐶐�
	json loadObj;
	if (JsonReader::Load(filePath, loadObj) == false) MessageBox(NULL, "�Ǎ��Ɏ��s���܂����B", 0, 0);
	editStage_->Load(loadObj);
}

void GameEditor::AddUIObject()
{
	isShowCreateUIObjectWindow_ = true;
}

void GameEditor::SaveUIPanel()
{
	//���݂̃J�����g�f�B���N�g�����o���Ă���
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// �ۑ���̃t�@�C���p�X���擾
	string filePath{}; {
		// �u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
		OPENFILENAME ofn; {
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFilter = TEXT("objectData(*.json)\0*.json\0���ׂẴt�@�C��(*.*)\0*.*\0\0");
			char fileName[MAX_PATH] = "����.json";
			ofn.lpstrFile = fileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_OVERWRITEPROMPT;
			ofn.lpstrDefExt = "json";
			ofn.nFilterIndex = 1; // �����I������t�B���^�[
			ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
		}

		// �t�@�C����ۑ�����_�C�A���O�̕\��
		if (GetSaveFileName(&ofn) == TRUE) {
			// �t�@�C���p�X���擾
			filePath = ofn.lpstrFile;

			// �J�����g�f�B���N�g������̑��΃p�X���擾
			filePath = GetAssetsRelativePath(filePath);

			// ���������"\\"��"/"�ɒu��
			ReplaceBackslashes(filePath);

			// �f�B���N�g����߂�
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return;
		}
	}

	// �t�@�C���ɃX�e�[�W����ۑ�
	json saveObj;
	editUIPanel_->Save(saveObj);
	if (JsonReader::Save(filePath, saveObj) == false) MessageBox(NULL, "�ۑ��Ɏ��s���܂����B", 0, 0);
}

void GameEditor::LoadUIPanel()
{
	//���݂̃J�����g�f�B���N�g�����o���Ă���
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// �ǂݍ��ރt�@�C���̃p�X���擾
	string filePath{}; {
		// �u�t�@�C�����J���v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
		OPENFILENAME ofn; {
			TCHAR szFile[MAX_PATH] = {}; // �t�@�C�������i�[����o�b�t�@
			ZeroMemory(&ofn, sizeof(ofn)); // �\���̂̏�����
			ofn.lStructSize = sizeof(ofn); // �\���̂̃T�C�Y
			ofn.lpstrFile = szFile; // �t�@�C�������i�[����o�b�t�@
			ofn.lpstrFile[0] = '\0'; // ������
			ofn.nMaxFile = sizeof(szFile); // �t�@�C�����o�b�t�@�̃T�C�Y
			ofn.lpstrFilter = TEXT("JSON�t�@�C��(*.json)\0*.json\0���ׂẴt�@�C��(*.*)\0*.*\0"); // �t�B���^�[�iFBX�t�@�C���̂ݕ\���j
			ofn.nFilterIndex = 1; // �����I������t�B���^�[
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
			ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
		}

		// �t�@�C����I������_�C�A���O�̕\��
		if (GetOpenFileName(&ofn) == TRUE) {
			// �t�@�C���p�X���擾
			filePath = ofn.lpstrFile;

			// �J�����g�f�B���N�g������̑��΃p�X���擾
			filePath = GetAssetsRelativePath(filePath);

			// ���������"\\"��"/"�ɒu��
			ReplaceBackslashes(filePath);

			// �f�B���N�g����߂�
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return;
		}
	}

	// �t�@�C����ǂݍ��݃X�e�[�W�𐶐�
	json loadObj;
	if (JsonReader::Load(filePath, loadObj) == false) MessageBox(NULL, "�Ǎ��Ɏ��s���܂����B", 0, 0);
	editUIPanel_->Load(loadObj);
}

void GameEditor::DrawPlantOutLiner()
{
	// �t�@�C������p�̃{�^����\��
	ImGui::Text("file menu"); {
		// �t�@�C���̕ۑ��E�ǂݍ��݃{�^��
		if (ImGui::Button("Save")) SavePlant(); ImGui::SameLine();
		if (ImGui::Button("Load")) LoadPlant(); ImGui::SameLine();
		if (ImGui::Button("Clear")) PlantCollection::ClearPlants();
		ImGui::Separator();
	}

	// �A���̒ǉ��p�̃{�^����\��
	ImGui::Text("create menu"); {
		// �A�������̓{�b�N�X
		static char plantNameBuffer[256] = "";
		ImGui::InputTextWithHint("##:setting_name", "Input plant name...", plantNameBuffer, IM_ARRAYSIZE(plantNameBuffer));
		ImGui::SameLine();

		// �ǉ��{�^��
		if (ImGui::Button("+")) {
			if (strlen(plantNameBuffer) > 0) {
				PlantCollection::AddPlant({
					((int)PlantCollection::GetPlants().size()),
					1,
					plantNameBuffer,
					3,
					"defaultPlant.fbx",
					"dafaultPlant.png"
					});
			}

			strcpy_s(plantNameBuffer, sizeof(plantNameBuffer), "");
		}
		ImGui::SameLine();

		// �ڍאݒ�ǉ��{�^��
		if (ImGui::Button(":")) isShowPlantWindow_ = true;

		// ���O�������͂̏ꍇ�A�ԕ����Ōx��
		if (strlen(plantNameBuffer) == 0) {
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Name is required!");
		}

	} 
	ImGui::Separator();

	// �A���̃��X�g��\��
	ImGui::BeginChild("ObjectList"); {
		for (int i = 0; i < PlantCollection::GetPlants().size(); ++i)
			if (ImGui::Selectable(PlantCollection::GetPlants()[i].name_.c_str(), selectEditPlantIndex_ == i))
				selectEditPlantIndex_ = i;
	}
	ImGui::EndChild();

	// �A���f�[�^�ڍאݒ�ǉ��E�B���h�E��\��
	if (isShowPlantWindow_) DrawAddPlantWindow();
}

void GameEditor::SavePlant()
{
	//���݂̃J�����g�f�B���N�g�����o���Ă���
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// �ۑ���̃t�@�C���p�X���擾
	string filePath{}; {
		// �u�t�@�C����ۑ��v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
		OPENFILENAME ofn; {
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.lpstrFilter = TEXT("objectData(*.json)\0*.json\0���ׂẴt�@�C��(*.*)\0*.*\0\0");
			char fileName[MAX_PATH] = "����.json";
			ofn.lpstrFile = fileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_OVERWRITEPROMPT;
			ofn.lpstrDefExt = "json";
			ofn.nFilterIndex = 1; // �����I������t�B���^�[
			ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
		}

		// �t�@�C����ۑ�����_�C�A���O�̕\��
		if (GetSaveFileName(&ofn) == TRUE) {
			// �t�@�C���p�X���擾
			filePath = ofn.lpstrFile;

			// �J�����g�f�B���N�g������̑��΃p�X���擾
			filePath = GetAssetsRelativePath(filePath);

			// ���������"\\"��"/"�ɒu��
			ReplaceBackslashes(filePath);

			// �f�B���N�g����߂�
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return;
		}
	}

	// �t�@�C���ɃX�e�[�W����ۑ�
	json saveObj;
	PlantCollection::Save(saveObj);
	if (JsonReader::Save(filePath, saveObj) == false) MessageBox(NULL, "�ۑ��Ɏ��s���܂����B", 0, 0);
}

void GameEditor::LoadPlant()
{
	//���݂̃J�����g�f�B���N�g�����o���Ă���
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// �ǂݍ��ރt�@�C���̃p�X���擾
	string filePath{}; {
		// �u�t�@�C�����J���v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
		OPENFILENAME ofn; {
			TCHAR szFile[MAX_PATH] = {}; // �t�@�C�������i�[����o�b�t�@
			ZeroMemory(&ofn, sizeof(ofn)); // �\���̂̏�����
			ofn.lStructSize = sizeof(ofn); // �\���̂̃T�C�Y
			ofn.lpstrFile = szFile; // �t�@�C�������i�[����o�b�t�@
			ofn.lpstrFile[0] = '\0'; // ������
			ofn.nMaxFile = sizeof(szFile); // �t�@�C�����o�b�t�@�̃T�C�Y
			ofn.lpstrFilter = TEXT("JSON�t�@�C��(*.json)\0*.json\0���ׂẴt�@�C��(*.*)\0*.*\0"); // �t�B���^�[�iFBX�t�@�C���̂ݕ\���j
			ofn.nFilterIndex = 1; // �����I������t�B���^�[
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
			ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
		}

		// �t�@�C����I������_�C�A���O�̕\��
		if (GetOpenFileName(&ofn) == TRUE) {
			// �t�@�C���p�X���擾
			filePath = ofn.lpstrFile;

			// �J�����g�f�B���N�g������̑��΃p�X���擾
			filePath = GetAssetsRelativePath(filePath);

			// ���������"\\"��"/"�ɒu��
			ReplaceBackslashes(filePath);

			// �f�B���N�g����߂�
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return;
		}
	}

	// �t�@�C����ǂݍ��݃X�e�[�W�𐶐�
	json loadObj;
	if (JsonReader::Load(filePath, loadObj) == false) MessageBox(NULL, "�Ǎ��Ɏ��s���܂����B", 0, 0);
	PlantCollection::Load(loadObj);
}

void GameEditor::DrawAddPlantWindow()
{
	bool isValid = true; // �o���f�[�V�������ʂ������ǂ����������t���O

	ImGui::Begin("Add Plant!!", &isShowPlantWindow_);
	ImGui::Text("Please set the information of the plant to be added.");
	ImGui::Separator();

	// �A�������̓{�b�N�X
	static char plantNameBuffer[256] = "";
	ImGui::InputTextWithHint(":setting name", "Input plant name...", plantNameBuffer, IM_ARRAYSIZE(plantNameBuffer));

	// �K�{�`�F�b�N: �A��������łȂ������`�F�b�N
	if (strlen(plantNameBuffer) == 0) {
		isValid = false;
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Name is required!");
	}

	// ���A���e�B���̓{�b�N�X
	static int rarity = 1;
	ImGui::InputInt(":setting rarity", &rarity);

	// �K�{�`�F�b�N: ���A���e�B��1�ȏ�ł��邱��
	if (rarity <= 0) {
		isValid = false;
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Rarity must be greater than 0!");
	}

	// �G���A�ԍ����̓{�b�N�X
	static int areaNumber = 1;
	ImGui::InputInt(":setting areaNumber", &areaNumber);

	// �K�{�`�F�b�N: �G���A�ԍ���1�ȏ�ł��邱��
	if (areaNumber <= 0) {
		isValid = false;
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Area number must be greater than 0!");
	}

	// ���f���t�@�C���p�X���̓{�b�N�X
	static string modelFilePath = "defaultPlant.fbx";
	ImGui::InputText(":setting modelFilePath", &modelFilePath[0], modelFilePath.size());
	ImGui::SameLine();
	if (ImGui::Button(":set Model")) modelFilePath = GetFBXFilePath();

	// �摜�t�@�C���p�X���̓{�b�N�X
	static string imageFilePath = "defaultPlant.png";
	ImGui::InputText(":setting imageFilePath", &imageFilePath[0], imageFilePath.size());
	ImGui::SameLine();
	if (ImGui::Button(":set Image")) imageFilePath = GetPNGFilePath();

	ImGui::Separator();

	// �uAdd�v�{�^���𖳌�������
	if (isValid && ImGui::Button("Add")) {
		PlantCollection::AddPlant({
			(int)PlantCollection::GetPlants().size(),
			rarity,
			plantNameBuffer,
			areaNumber,
			modelFilePath,
			imageFilePath
			});
	}
	else if (!isValid) {
		// ���̓G���[������ꍇ�ɁA�{�^���𖳌�������
		ImGui::BeginDisabled();
		ImGui::Button("Add");
		ImGui::EndDisabled();
	}
	ImGui::End();
}

string GameEditor::GetFBXFilePath()
{
	//���݂̃J�����g�f�B���N�g�����o���Ă���
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// �ǉ�����I�u�W�F�N�g�̃��f���t�@�C���p�X��ݒ�
	string filePath{}; {
		// �u�t�@�C�����J���v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
		OPENFILENAME ofn; {
			TCHAR szFile[MAX_PATH] = {}; // �t�@�C�������i�[����o�b�t�@
			ZeroMemory(&ofn, sizeof(ofn)); // �\���̂̏�����
			ofn.lStructSize = sizeof(ofn); // �\���̂̃T�C�Y
			ofn.lpstrFile = szFile; // �t�@�C�������i�[����o�b�t�@
			ofn.lpstrFile[0] = '\0'; // ������
			ofn.nMaxFile = sizeof(szFile); // �t�@�C�����o�b�t�@�̃T�C�Y
			ofn.lpstrFilter = TEXT("FBX�t�@�C��(*.fbx)\0*.fbx\0���ׂẴt�@�C��(*.*)\0*.*\0"); // �t�B���^�[�iFBX�t�@�C���̂ݕ\���j			ofn.nFilterIndex = 1; // �����I������t�B���^�[
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
			ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
		}

		// �t�@�C����I������_�C�A���O�̕\��
		if (GetOpenFileName(&ofn) == TRUE) {
			// �t�@�C���p�X���擾
			filePath = ofn.lpstrFile;

			// �J�����g�f�B���N�g������̑��΃p�X���擾
			filePath = GetAssetsRelativePath(filePath);

			// ���������"\\"��"/"�ɒu��
			ReplaceBackslashes(filePath);

			// �f�B���N�g����߂�
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return "";
		}
	}

	// �t�@�C���p�X��Ԃ�
	return filePath;
}

string GameEditor::GetPNGFilePath()
{
	//���݂̃J�����g�f�B���N�g�����o���Ă���
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

	// �ǉ�����I�u�W�F�N�g�̃��f���t�@�C���p�X��ݒ�
	string filePath{}; {
		// �u�t�@�C�����J���v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
		OPENFILENAME ofn; {
			TCHAR szFile[MAX_PATH] = {}; // �t�@�C�������i�[����o�b�t�@
			ZeroMemory(&ofn, sizeof(ofn)); // �\���̂̏�����
			ofn.lStructSize = sizeof(ofn); // �\���̂̃T�C�Y
			ofn.lpstrFile = szFile; // �t�@�C�������i�[����o�b�t�@
			ofn.lpstrFile[0] = '\0'; // ������
			ofn.nMaxFile = sizeof(szFile); // �t�@�C�����o�b�t�@�̃T�C�Y
			ofn.lpstrFilter = TEXT("PNG�t�@�C��(*.fbx)\0*.png\0���ׂẴt�@�C��(*.*)\0*.*\0"); // �t�B���^�[�iFBX�t�@�C���̂ݕ\���j
			ofn.nFilterIndex = 1; // �����I������t�B���^�[
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
			ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
		}

		// �t�@�C����I������_�C�A���O�̕\��
		if (GetOpenFileName(&ofn) == TRUE) {
			// �t�@�C���p�X���擾
			filePath = ofn.lpstrFile;

			// �J�����g�f�B���N�g������̑��΃p�X���擾
			filePath = GetAssetsRelativePath(filePath);

			// ���������"\\"��"/"�ɒu��
			ReplaceBackslashes(filePath);

			// �f�B���N�g����߂�
			SetCurrentDirectory(defaultCurrentDir);
		}
		else {
			return "";
		}
	}

	// �t�@�C���p�X��Ԃ�
	return filePath;
}