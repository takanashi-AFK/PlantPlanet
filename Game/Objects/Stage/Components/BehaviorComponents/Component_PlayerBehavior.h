#pragma once
#include "../Component.h"
#include <DirectXMath.h>
#include "../../StageObject.h"
#include <vector>
#include "../../../../../Engine/ResourceManager/Model.h"
class Component_PlayerBehavior : public Component
{
private:
	DirectX::XMVECTOR frontVec_;
	StageObject* target_;	// �^�[�Q�b�g
	string targetName_;
	std::vector<RayCastData> rayHitObjectList_;
	float shootHeight_;		// ���˂��鍂��
	bool isAnimationNow_;

	enum PlayerState {
		PSTATE_IDLE = 0,
		PSTATE_WALK,
		PSTATE_WALKANDSHOOT,
		PSTATE_MELEE,
		PSTATE_SHOOT,
		PSTATE_DIE,
		PSTATE_MAX
	}nowState,prevState;


public:
	Component_PlayerBehavior(string _name,StageObject* _holder,Component* _parent);

	void Initialize() override;
	void Update() override;
	void Release() override;
	void DrawData() override;
	
	void OnCollision(GameObject* _target, Collider* _collider) override;

	void Idle();
	void Walk();
	void WalkAndShoot();
	void Melee();
	void Shoot();

	void SetState(PlayerState _state) { prevState = nowState; nowState = _state; }
	
	void ShootExe();

	//bool IsEnemyInRect(StageObject* _target);

	//XMFLOAT3 ConvertTo2DPos(StageObject* _target);
};

