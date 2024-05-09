#include "StageEditor.h"
#include "../../Engine/ImGui/imgui.h"
#include "../../Engine/DirectX/Direct3D.h"
#include "../Objects/Stage/Stage.h"
#include "../Objects/Stage/StageObject.h"

namespace {
	// ��������̑S�Ă� "\\" �� "/" �ɒu������֐�
	void ReplaceBackslashes(string& str) {
		size_t found = str.find("\\");
		while (found != string::npos) {
			str.replace(found, 1, "/"); // "\\" �� "/" �ɒu������
			found = str.find("\\", found + 1); // ���� "\\" ����������
		}
	}

	// �J�����g�f�B���N�g������̑��΃p�X���擾����֐�
	std::string GetAssetsRelativePath(const std::string& absolutePath) {
		const std::string assetsFolder = "Assets\\";
		// "Assets\\" �̈ʒu������
		size_t assetsPos = absolutePath.find(assetsFolder);
		if (assetsPos != std::string::npos) {
			// "Assets\\" �̕������������A����ȍ~�̕������擾
			std::string relativePath = absolutePath.substr(assetsPos + assetsFolder.size());
			return relativePath;
		}
		else {
			// "Assets\\" ��������Ȃ��ꍇ�́A���̃t�@�C���p�X��Ԃ�
			return absolutePath;
		}
	}

}

StageEditor::StageEditor(GameObject* _parent)
	:GameObject(_parent,"StageEditor"),editTarget_(nullptr)
{
}

void StageEditor::Initialize()
{
}

void StageEditor::Update()
{
}

void StageEditor::Draw()
{
	DrawWorldOutLiner();
}

void StageEditor::Release()
{
}

void StageEditor::DrawWorldOutLiner()
{
	// ImGui�ŕ\������E�B���h�E�̐ݒ���s��
	ImGui::SetNextWindowPos(ImVec2(Direct3D::screenWidth_ * 0.7f, 0));
	ImGui::SetNextWindowSize(ImVec2(Direct3D::screenWidth_ * 0.3f, Direct3D::screenHeight_ * 0.5f));

	// �Œ�E�B���h�E��\��
	ImGui::Begin("World Outliner", NULL,
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse| ImGuiWindowFlags_AlwaysHorizontalScrollbar);
	{
		ImGui::Text("object menu");

		if (ImGui::Button("Add"))AddStageObject();
		ImGui::SameLine();

		if (ImGui::Button("Save"))SaveStage();
		ImGui::SameLine();

		if (ImGui::Button("Load"))LoadStage();
		ImGui::SameLine();

		if (ImGui::Button("Delete"))editTarget_->DeleteAllStageObject();

		ImGui::Separator();

		ImGui::BeginChild("list"); {
			editTarget_->DrawData();
		}
		ImGui::EndChild();
	}
	ImGui::End();
}

void StageEditor::AddStageObject()
{
	// �ǉ�����I�u�W�F�N�g�̏�������ݒ�
	string name = "object" + std::to_string(editTarget_->objects_.size());

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
	editTarget_->AddStageObject(CreateStageObject(name, filePath, editTarget_));
}

void StageEditor::SaveStage()
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
	editTarget_->Save(saveObj);
	if (JsonReader::Save(filePath, saveObj) == false) MessageBox(NULL, "�ۑ��Ɏ��s���܂����B", 0, 0);
	
}

void StageEditor::LoadStage()
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
	editTarget_->Load(loadObj);
}
