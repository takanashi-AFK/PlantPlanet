#include "MenuCharacter.h"
#include "../../../Engine/ResourceManager/Model.h"

MenuCharacter::MenuCharacter(GameObject* _parent)
	: StageObject("MenuCharacter", "04_models/00_player/Silly Dancing.fbx", _parent)
{
}

void MenuCharacter::Initialize()
{
	// ƒ‚ƒfƒ‹‚Ì“Ç‚Ýž‚Ý
	modelHandle_ = Model::Load(modelFilePath_);
	assert(modelHandle_ >= 0);

	Model::SetAnimFrame(modelHandle_, 0, 230, 1.f);
}

void MenuCharacter::Update()
{
}

void MenuCharacter::Draw()
{
	Direct3D::SetShader(Direct3D::SHADER_SKY);
	Model::SetTransform(modelHandle_, transform_);
	Model::Draw(modelHandle_);
}
