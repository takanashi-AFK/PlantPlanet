#include "SkySphere.h"
#include"../../../Engine/Global.h"

// インクルード
#include "../../../Engine/DirectX/Direct3D.h"
#include "../../../Engine/ResourceManager/Model.h"

SkySphere::SkySphere(GameObject* _parent)
	:StageObject("SkySphere", "Models/SkySphere/SkySphere.fbx", _parent), timeZone_{ TIME_ZONE::MORNING}
{
}

void SkySphere::Initialize()
{
	// モデルの読み込み
	transform_.pParent_ = nullptr;

	string modelNames[static_cast<int>(TIME_ZONE::AMOUNT)] =
	{
		"Models/SkySphere/SkySphere.fbx",
		"Models/SkySphere/SkySphere.fbx",
	};

	for (auto i = 0u; i < static_cast<int>(TIME_ZONE::AMOUNT); ++i) {

		modelHandles_[i] = Model::Load(modelNames[i]);
		assert(modelHandles_[i] >= 0);
	}

	// 保有するコンポーネントの初期化処理
	for (auto comp : myComponents_)comp->ChildIntialize();

	SetLights();
}

void SkySphere::Draw()
{
	// スカイスフィア用のシェーダーを設定
	SetLights();
	Direct3D::SetShader(Direct3D::SHADER_SKY);

	modelHandle_ = modelHandles_[static_cast<int>(timeZone_)];

	// モデルの描画

	Model::SetTransform(modelHandle_, transform_);
	Model::Draw(modelHandle_);

	// シェーダーを元に戻す
	Direct3D::SetShader(Direct3D::SHADER_3D);
}

void SkySphere::SetTimeZone(TIME_ZONE tz)
{
	timeZone_ = tz;

	SetLights();
}

SkySphere::TIME_ZONE SkySphere::GetTimeZone()
{
	return timeZone_;
}

void SkySphere::SetLights()
{
	int index = static_cast<int>(timeZone_);

	Light::sunLight = globalLightColors_[index];
	Light::ambientLight = AmbientLightColors_[index];
}
