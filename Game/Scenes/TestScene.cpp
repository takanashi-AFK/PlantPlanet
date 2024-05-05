#include "TestScene.h"

// �C���N���[�h
#include "../Objects/Stage/Stage.h"
#include "../../Engine/ImGui/imgui.h"
#include "../Objects/Stage/SkySphere.h"
#include "../Otheres/StageEditor.h"

TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene"),pStage_(nullptr)
{
}

void TestScene::Initialize()
{
	// �X�J�C�X�t�B�A��W���Ő���
	Instantiate<SkySphere>(this);

	// �X�e�[�W���쐬
	pStage_ = Instantiate<Stage>(this);

	// �X�e�[�W����Ǎ�
	json loadObj;
	JsonReader::Load("Datas/testData.json", loadObj);
	pStage_->Load(loadObj);

#ifdef _DEBUG
	// �X�e�[�W�G�f�B�^�[�����s
	StageEditor* pSe = Instantiate<StageEditor>(this); {

		// �ҏW�Ώۂ�ݒ�
		pSe->SetEditTarget(pStage_);
	}
#endif // _DEBUG
}

void TestScene::Update()
{
}

void TestScene::Draw()
{
}

void TestScene::Release()
{
}
