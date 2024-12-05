#include "Component_Accessory.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/Global.h"

Component_Accessory::Component_Accessory(string _name, StageObject* _holder, Component* _parent)
	:Component_Motion(_name, _holder, PlayerMotion, _parent), originalPosition_{}, originalRotate_{}, originalScale_{1,1,1},
	gun_{}, gunModelHandle_{},bone_{ "mixamorig:LeftHand" }
{
}

void Component_Accessory::Initialize()
{
	char defaultCurrentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, defaultCurrentDir);
	{
		OPENFILENAME ofn; {
			TCHAR szFile[MAX_PATH] = {}; // �t�@�C�������i�[����o�b�t�@
			ZeroMemory(&ofn, sizeof(ofn)); // �\���̂̏�����
			ofn.lStructSize = sizeof(ofn); // �\���̂̃T�C�Y
			ofn.lpstrFile = szFile; // �t�@�C�������i�[����o�b�t�@
			ofn.lpstrFile[0] = '\0'; // ������
			ofn.nMaxFile = sizeof(szFile); // �t�@�C�����o�b�t�@�̃T�C�Y
			ofn.lpstrFilter = TEXT("FBX�t�@�C��(*.fbx)\0*.fbx\0���ׂẴt�@�C��(*.*)\0*.*\0");
			ofn.nFilterIndex = 1; // �����I������t�B���^�[
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
			ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
		}

		// �t�@�C����I������_�C�A���O�̕\��
		if (GetOpenFileName(&ofn) == TRUE) {
			// �t�@�C���p�X���擾
			string path = ofn.lpstrFile;

			// �J�����g�f�B���N�g������̑��΃p�X���擾
			path = FileManager::GetAssetsRelativePath(path);

			// ���������"\\"��"/"�ɒu��
			FileManager::ReplaceBackslashes(path);

			// �f�B���N�g����߂�
			SetCurrentDirectory(defaultCurrentDir);

			gun_ = CreateStageObject("Player's gun", path, holder_);

		}
	}
}

void Component_Accessory::Update()
{
	auto playerHandle = holder_->GetModelHandle();
	auto transform = holder_->GetTransform();
	Model::SetTransform(playerHandle, transform);

	auto bonePos = Model::GetBonePosition(playerHandle, bone_);
	auto boneRot = Model::GetBoneRotation(playerHandle, bone_);
	auto boneScale = Model::GetBoneScale(playerHandle, bone_);

	gun_->SetPosition(bonePos + originalPosition_);
	gun_->SetRotate(boneRot + originalRotate_);
	gun_->SetScale({ boneScale.x * originalScale_.x ,boneScale.y * originalScale_.y,boneScale.z * originalScale_.z });
}

void Component_Accessory::Release()
{
	
}

void Component_Accessory::DrawData()
{
	ImGui::DragFloat3("Original Position", &originalPosition_.x, .01f);
	ImGui::DragFloat3("Original Rotation", &originalRotate_.x, 1.f);
	ImGui::DragFloat3("Original Scale", &originalScale_.x, .01f);
	
	if (ImGui::Button("Load Model")) {
		//���݂̃J�����g�f�B���N�g�����o���Ă���
		char defaultCurrentDir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, defaultCurrentDir);
		{
			OPENFILENAME ofn; {
				TCHAR szFile[MAX_PATH] = {}; // �t�@�C�������i�[����o�b�t�@
				ZeroMemory(&ofn, sizeof(ofn)); // �\���̂̏�����
				ofn.lStructSize = sizeof(ofn); // �\���̂̃T�C�Y
				ofn.lpstrFile = szFile; // �t�@�C�������i�[����o�b�t�@
				ofn.lpstrFile[0] = '\0'; // ������
				ofn.nMaxFile = sizeof(szFile); // �t�@�C�����o�b�t�@�̃T�C�Y
				ofn.lpstrFilter = TEXT("FBX�t�@�C��(*.fbx)\0*.fbx\0���ׂẴt�@�C��(*.*)\0*.*\0"); 
				ofn.nFilterIndex = 1; // �����I������t�B���^�[
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
				ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
			}

			// �t�@�C����I������_�C�A���O�̕\��
			if (GetOpenFileName(&ofn) == TRUE) {
				// �t�@�C���p�X���擾
				string path = ofn.lpstrFile;

				// �J�����g�f�B���N�g������̑��΃p�X���擾
				path = FileManager::GetAssetsRelativePath(path);

				// ���������"\\"��"/"�ɒu��
				FileManager::ReplaceBackslashes(path);

				// �f�B���N�g����߂�
				SetCurrentDirectory(defaultCurrentDir);

				ExchangeModel(path);
			}
			else {
				return;
			}
		}
	}

	ImGui::SameLine();
	ImGui::Text(Model::GetModelName(gunModelHandle_).c_str());
	ImGui::InputText(": Bone Name", bone_, sizeof(bone_));
}

void Component_Accessory::ExchangeModel(std::string filePath)
{
	gunModelHandle_ = Model::Load(filePath);

	gun_->SetModelFilePath(filePath);
	gun_->SetModelHandle(gunModelHandle_);
}


void Component_Accessory::Load(json& _loadObj)
{
	if (_loadObj.contains("Accessory ModelPath")) gunModelHandle_ = Model::Load(_loadObj["ModelPath"]);

	if (_loadObj.contains("Accessory Original Position")) originalPosition_
		={ _loadObj["Accessory Original Position"][0],_loadObj["Original Position"][1],_loadObj["Original Position"][2] };
	if (_loadObj.contains("Accessory Original Rotate")) originalRotate_
		= { _loadObj["Accessory Original Rotate"][0],_loadObj["Original Rotate"][1],_loadObj["Original Rotate"][2] };
	if (_loadObj.contains("Accessory Original Scale"))originalScale_
		= { _loadObj["Accessory Original Scale"][0],_loadObj["Original Scale"][1],_loadObj["Original Scale"][2] };

	ExchangeModel(_loadObj["ModelPath"]);
}

void Component_Accessory::Save(json& _saveObj)
{
	_saveObj["Accessory ModelPath"] = Model::GetModelName(gunModelHandle_);

	_saveObj["Accessory Original Position"] = { originalPosition_.x,originalPosition_.y,originalPosition_.z };
	_saveObj["Accessory Original Rotate"] = { originalRotate_.x,originalRotate_.y,originalRotate_.z };
	_saveObj["Accessory Original Scale"] = { originalScale_.x,originalScale_.y,originalScale_.z };
}
