#include "Scene_Test.h"

// �C���N���[�h
#include "../../Engine/ImGui/imgui.h"
#include "../Objects/Stage/SkySphere.h"
#include "../../TestObject_Box.h"
#include "../../Engine/GameObject/Camera.h"

Scene_Test::Scene_Test(GameObject * parent)
	: GameObject(parent, "Scene_Test"),pStage_(nullptr)
{
}

void Scene_Test::Initialize()
{
	// �X�J�C�X�t�B�A��W���Ő���
	Instantiate<SkySphere>(this);

	// �e�X�g�I�u�W�F�N�g�𐶐�
	Instantiate<TestObject_Box>(this);

}

void Scene_Test::Update()
{
	static XMFLOAT3 camPos = Camera::GetPosition();
	static XMFLOAT3 camTgt = Camera::GetTarget();

	ImGui::Text("cam");
	ImGui::DragFloat3("camPos", &camPos.x, 0.1f);
	ImGui::DragFloat3("camTgt", &camTgt.x, 0.1f);

	Camera::SetPosition(camPos);
	Camera::SetTarget(camTgt);

}

void Scene_Test::Draw()
{
}

void Scene_Test::Release()
{
}
