#include "UIItemTable.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/Global.h"
#include "UIImage.h"
#include "UIButton.h"
using namespace FileManager;

UIItemTable::UIItemTable(string _name, UIObject* parent, int _layerNum)
: UIObject(_name, UIType::UI_ITEMTABLE,parent, _layerNum)
,  itemInterval_(0.1,0.1), lineBreakCount_(0),filePath_("")
{
}

UIItemTable::~UIItemTable()
{
}

void UIItemTable::Initialize()
{
#ifdef _DEBUG
	previewImage_ = itemPanel_->CreateUIObject("previewImage", UIType::UI_IMAGE, itemPanel_, 100);
	((UIImage*)previewImage_)->SetImage("Images/DebugImage/ItemTablePreviewPos.png");
#endif // _DEBUG
}

void UIItemTable::Update()
{
}

void UIItemTable::Draw()
{
}

void UIItemTable::Release()
{
}

void UIItemTable::DrawData()
{
	static char nameBuffer[256] = "";
	// ���O�����
	ImGui::InputTextWithHint(":setting name", "Input object name...", nameBuffer, IM_ARRAYSIZE(nameBuffer));

	// �^�C�v��I��
	static UIType uitype = UIType::UI_NONE;	// �����I������
	static std::string type = "NONE";		// �����I������
	if (ImGui::BeginCombo(":setting type", type.c_str())) {
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

	ImGui::InputInt("ItemNum", &itemNum_);

	ImGui::InputInt("StartLayer", &startLayer_);

	ImGui::InputInt("LineBreakCount", &lineBreakCount_);

	ImGui::InputFloat2("ItemInterval", &itemInterval_.x);

	ImGui::DragFloat3("PreviewPos", &previewTransform_.position_.x, 0.01f);
	((UIImage*)previewImage_)->SetTrasform(previewTransform_);

	ImGui::DragFloat3("ItemSize", &itemTransform_.scale_.x, 0.01f);


	ImGui::Text("File Name : %s", filePath_.c_str());
	ImGui::SameLine();
	if (ImGui::Button("...")) {
		// �t�@�C�������擾
		//���݂̃J�����g�f�B���N�g�����o���Ă���
		char defaultCurrentDir[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, defaultCurrentDir);

		// �ǉ�����I�u�W�F�N�g�̃��f���t�@�C���p�X��ݒ�
		{
			// �u�t�@�C�����J���v�_�C�A���O�̐ݒ�p�\���̂�ݒ�
			OPENFILENAME ofn; {
				TCHAR szFile[MAX_PATH] = {}; // �t�@�C�������i�[����o�b�t�@
				ZeroMemory(&ofn, sizeof(ofn)); // �\���̂̏�����
				ofn.lStructSize = sizeof(ofn); // �\���̂̃T�C�Y
				ofn.lpstrFile = szFile; // �t�@�C�������i�[����o�b�t�@
				ofn.lpstrFile[0] = '\0'; // ������
				ofn.nMaxFile = sizeof(szFile); // �t�@�C�����o�b�t�@�̃T�C�Y
				ofn.lpstrFilter = TEXT("PNG�t�@�C��(*.png)\0*.png\0���ׂẴt�@�C��(*.*)\0*.*\0"); // �t�B���^�[�iPNG�t�@�C���̂ݕ\���j
				ofn.nFilterIndex = 1; // �����I������t�B���^�[
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
				ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
			}

			// �t�@�C����I������_�C�A���O�̕\��
			if (GetOpenFileName(&ofn) == TRUE) {
				// �t�@�C���p�X���擾
				filePath_ = ofn.lpstrFile;

				// �J�����g�f�B���N�g������̑��΃p�X���擾
				filePath_ = GetAssetsRelativePath(filePath_);

				// ���������"\\"��"/"�ɒu��
				ReplaceBackslashes(filePath_);

				// �f�B���N�g����߂�
				SetCurrentDirectory(defaultCurrentDir);
			}
			else {
				return;
			}
		}
	}


	if (ImGui::Button("Create")) {

		if (filePath_ == "") {
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "FilePath is invalid.");
			return;
		}
		if (itemNum_ <= 0) {
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "ItemNum is invalid.");
			return;
		}
		if (lineBreakCount_ == 0) {
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "everybody can't divide by 0!");
			return;
		}

		// �O���b�h�z�u�p�̏����ʒu
		XMFLOAT3 currentPos = previewTransform_.position_;

		for (int i = 0; i < itemNum_; i++) {
			// UIObject ���쐬���ăe�[�u���ɒǉ�
			UIObject* newItem = CreateUIObject(nameBuffer + std::to_string(i), uitype, itemPanel_, startLayer_ + i + 1);
			itemTable_.push_back(newItem);

			// Transform ���擾���Ĉʒu��ݒ�
			itemTransform_.position_ = { currentPos.x, currentPos.y, 0.0f };

			// �܂�Ԃ��`�F�b�N
			if ((i + 1) % lineBreakCount_ == 0) {
				// �s���܂�Ԃ��ꂽ�� X �����Z�b�g���AY ���X�V
				currentPos.x = previewTransform_.position_.x;
				currentPos.y -= itemInterval_.y;
			}
			else {
				// ���̃A�C�e���� X ���W���X�V
				currentPos.x += itemInterval_.x;
			}

			// UIImage ���L�̐ݒ�
			if (uitype == UIType::UI_IMAGE) {
				((UIImage*)itemTable_[i])->SetImage(filePath_);
				((UIImage*)itemTable_[i])->SetTrasform(itemTransform_);
			}
			else if (uitype == UIType::UI_BUTTON) {
				((UIButton*)itemTable_[i])->SetImage(filePath_);
				((UIButton*)itemTable_[i])->SetTrasform(itemTransform_);
			}

			// Transform ��K�p
			newItem->SetTrasform(itemTransform_);
		}
	}


}
