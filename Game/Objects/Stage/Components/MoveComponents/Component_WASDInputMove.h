#pragma once
#include "../Component.h"
#include <DirectXMath.h>
#include "../../../EffekseeLib/EffekseerVFX.h"/*ÅöÅöÅö*/
#include "../../../../../Engine/Global.h"

using namespace DirectX;

class Component_WASDInputMove : public Component
{

public:
	enum class DIRECTION
	{
		FORWARD = 0,
		BACK,
		LEFT ,
		RIGHT,

		AMOUNT
	};

private:
	bool isMove_ : 1;
	bool isRotate: 1;
	DIRECTION direcionType_;
	XMVECTOR dir_;
	XMFLOAT3 angles_;

	// effekseer: ïœå`çsóÒ
	std::shared_ptr<EFFEKSEERLIB::EFKTransform> mt;/*ÅöÅöÅö*/
	EffectData data_;

public:
	Component_WASDInputMove(string _name, StageObject* _holder, Component* _parent);
	void Initialize() override;
	void Update() override;
	void Release() override;
	void DrawData() override;
	void Save(json& _saveObj) override;
	void Load(json& _loadObj) override;
	XMVECTOR GetMoveDirection() const { return dir_; }
	bool IsMove() const { return isMove_; }
	XMFLOAT3 GetRotation() { return this->angles_; };

	void SetRotateLock(bool flag) { this->isRotate = flag; };
	bool GetRotateLock() { return this->isRotate; };
	DIRECTION GetDirectionType() { return this->direcionType_; };
private:
	void Move(XMVECTOR _dir, float _speed);
};

