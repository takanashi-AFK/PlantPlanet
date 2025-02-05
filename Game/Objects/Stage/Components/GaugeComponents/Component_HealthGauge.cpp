#include "Component_HealthGauge.h"

// インクルード
#include "../../../../../Engine/ImGui/imgui.h"
#include "../../StageObject.h"

namespace
{
	const int SHADER_CHANGE_TIME = 30;
}
Component_HealthGauge::Component_HealthGauge(string _name, StageObject* _holder, Component* _parent)
	:Component_Gauge(_holder, _name, HealthGauge,_parent), 
	prev_(now_), 
	shaderChangeTime_(60),
	isLock_(false),
	isTakeDamage_(false)
{

}

void Component_HealthGauge::Initialize()
{
}

void Component_HealthGauge::Update()
{
	Direct3D::SHADER_TYPE prevShaderType = holder_->GetOriginalShaderType();
	// ダメージをくらったらシェーダーを変更
	if (prev_ > now_) {
		holder_->SetShader(Direct3D::SHADER_DAMAGE);
		shaderChangeTime_ = SHADER_CHANGE_TIME; // シェーダー変更時間をリセット
	}
	// 一定時間が経過したらシェーダーを元に戻す
	if (shaderChangeTime_ > 0) {
		shaderChangeTime_--;
		if (shaderChangeTime_ == 0) {
			holder_->SetShader(prevShaderType);
		}
	}

	// 各値のリセット
	prev_ = now_;
	isTakeDamage_ = false;
}

void Component_HealthGauge::Release()
{
}

void Component_HealthGauge::TakeDamage(float _damageValue)
{
	// ダメージを受けたフラグを立てる
	isTakeDamage_ = true;

	if(isLock_)return;

	// 前回の体力を保存
	prev_ = now_;

	//HPを減らす
	now_ -= _damageValue;

	//HPが0以下にならないようにする
	if (now_ < 0)now_ = 0;
}

void Component_HealthGauge::Heal(float _healValue)
{
	if (isLock_)return;

	//HPが最大値を超えないようにする
	if (now_ + _healValue <= max_)now_ += _healValue;
}