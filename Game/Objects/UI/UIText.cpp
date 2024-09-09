#include "UIText.h"
#include "../../../Engine/ResourceManager/Text.h"
#include "../../../Engine/ImGui/imgui.h"
#include "../../../Engine/DirectX/Direct3D.h"
#include "../../../Engine/Global.h"

UIText::UIText(string _name, UIObject* parent,int _layerNum)
	: UIObject(_name, UI_TEXT, parent, _layerNum), pText_(nullptr), drawText_(""), size_(1.f), intNum_(nullptr)
{
}

void UIText::Initialize()
{
	// �e�L�X�g�I�u�W�F�N�g�𐶐���������
	pText_ = new Text;
	pText_->Initialize();

	LockScale();
	LockRotate();

}

void UIText::Update()
{
	OpenChangeFontDialog();
}

void UIText::Draw()
{
	// �\���T�C�Y���v�Z
	pText_->SetScale(size_);

	// �\���ʒu���v�Z
	float drawX = (transform_.position_.x * (Direct3D::screenWidth_ / 2)) + (Direct3D::screenWidth_ / 2);		
	float drawY = (transform_.position_.y * (Direct3D::screenHeight_ / 2)) + (Direct3D::screenHeight_ / 2);
	
	// �e�L�X�g��`��
	if (intNum_ == nullptr)pText_->Draw(drawX, drawY, drawText_.c_str());
	else pText_->Draw(drawX, drawY,*intNum_);
}

void UIText::Release()
{
	// �e�L�X�g�I�u�W�F�N�g�̉��
	pText_->Release();
}

void UIText::Save(json& saveObj)
{
	saveObj["text"] = drawText_;
	saveObj["size"] = size_;
	saveObj["fontFilePath"] = fontFilePath_;
	saveObj["charWidth"] = charWidth;
	saveObj["charHeight"] = charHeight;
	saveObj["rowLength"] = rowLength;
}

void UIText::Load(json& loadObj)
{
	if (loadObj.contains("text"))drawText_ = loadObj["text"].get<string>();
	if (loadObj.contains("size"))size_ = loadObj["size"].get<float>();
	if (loadObj.contains("fontFilePath"))fontFilePath_ = loadObj["fontFilePath"].get<string>();
	if (loadObj.contains("charWidth"))charWidth = loadObj["charWidth"].get<int>();
	if (loadObj.contains("charHeight"))charHeight = loadObj["charHeight"].get<int>();
	if (loadObj.contains("rowLength"))rowLength = loadObj["rowLength"].get<int>();

	if (fontFilePath_ != "") {
		pText_ = new Text;
		pText_->Initialize(fontFilePath_.c_str(), charWidth, charHeight, rowLength);
	}
}

void UIText::DrawData()
{
	// ��������������������������������������������
	// �\������e�L�X�g��ύX
	// ��������������������������������������������
	if(intNum_ == nullptr)
	if (ImGui::TreeNode("TextString")) {
		ImGui::Text("Current Text : %s", drawText_.c_str());
		char buffer[256] = "";
		if (ImGui::InputTextWithHint("##Input", "Input New Text...", buffer, IM_ARRAYSIZE(buffer)))
			this->drawText_ = buffer;
		ImGui::TreePop();
	}

	// ��������������������������������������������
	// �t�H���g�T�C�Y��ύX
	// ��������������������������������������������
	ImGui::DragFloat("size", &size_, 0.01f);


	if (ImGui::Button("ChangeFont")) {
		openChangeFontDialog_ = true;
	}
}

void UIText::OpenChangeFontDialog()
{
	if(openChangeFontDialog_){
		ImGui::Begin("ChangeFont");
	

		if (ImGui::Button("Select fontPath")) {
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
					ofn.lpstrFilter = TEXT("PNG�t�@�C��(*.png)\0*.png\0���ׂẴt�@�C��(*.*)\0*.*\0"); // �t�B���^�[�iFBX�t�@�C���̂ݕ\���j
					ofn.nFilterIndex = 1; // �����I������t�B���^�[
					ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // �t���O�i�t�@�C�������݂��邱�ƁA�p�X�����݂��邱�Ƃ��m�F�j
					ofn.lpstrInitialDir = TEXT("."); // �J�����g�f�B���N�g���������I���ʒu�Ƃ��Đݒ�
				}

				// �t�@�C����I������_�C�A���O�̕\��
				if (GetOpenFileName(&ofn) == TRUE) {
					// �t�@�C���p�X���擾
					fontFilePath_ = ofn.lpstrFile;

					// �J�����g�f�B���N�g������̑��΃p�X���擾
					fontFilePath_ = FileManager::GetAssetsRelativePath(fontFilePath_);

					// ���������"\\"��"/"�ɒu��
					FileManager::ReplaceBackslashes(fontFilePath_);

					// �f�B���N�g����߂�
					SetCurrentDirectory(defaultCurrentDir);


				}
				else {
					return;
				}
			}
		}
			ImGui::Text("%s", fontFilePath_.c_str());
			ImGui::DragInt("charWidth", &charWidth);
			ImGui::DragInt("charHeight", &charHeight);
			ImGui::DragInt("rowLength", &rowLength);

			if (ImGui::Button("confirm")) {

				if (fontFilePath_ == "") {
					ImGui::TextColored(ImVec4(1, 0, 0, 1), "Please Select FilePath");
				}
				else {
					pText_ = new Text;
					pText_->Initialize(fontFilePath_.c_str(), charWidth, charHeight, rowLength);
				}
			}


		ImGui::End();
	}

}
