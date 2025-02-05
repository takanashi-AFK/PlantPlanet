#include "Component_MeleeAttack.h"
#include "../../StageObject.h"
#include "../../../../../Engine/ImGui/imgui.h"
#include "../TimerComponent/Component_Timer.h"
#include "../GaugeComponents/Component_HealthGauge.h"
#include "../../../../Engine/GameObject/Camera.h"
#include "../../../../../Engine/DirectX/Direct3D.h"
#include "../DetectorComponents/Component_FanRangeDetector.h"
#include "../../Stage.h"
#include "../MoveComponents/Component_TackleMove.h"
#include "../MoveComponents/Component_WASDInputMove.h"
#include "../../../../../Engine/ResourceManager/Model.h"

namespace
{
	const float MOVE_SPEED = 0.1f;
	const float MOVE_DISTANCE = 2.5f;
	const float MOVE_RAY_OFFSET = 0.5f;
	const float MOVE_DISTANCE_LIMIT = 0.7f;

	const int EFFECT_FRAME = 60;
	const float EFFECT_SPEED = 1.0f;

}

Component_MeleeAttack::Component_MeleeAttack(string _name, StageObject* _holder, Component* _parent)
	:Component_Attack(_holder, _name, MeleeAttack, _parent), 
	isHit_(false),
	isFirstMove_(false),
	startPosition_(XMFLOAT3(0, 0, 0)),
	easingType_("Linear"), 
	rate_(0),
	speed_(0.1),
	distance_(5.f),
	sequence_(E_STEPFORWORD)
{
}

void Component_MeleeAttack::Initialize()
{

	holder_->AddCollider(new BoxCollider(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1)));
	EFFEKSEERLIB::gEfk->AddEffect("slash", "Effects/Slash.efk");
	EFFEKSEERLIB::gEfk->AddEffect("hit", "Effects/Attack_Impact.efk");


	if (FindChildComponent("Timer") == false) AddChildComponent(CreateComponent("Timer", Timer, holder_, this));
	if (FindChildComponent("attackRange") == false)AddChildComponent(CreateComponent("attackRange", FanRangeDetector, holder_, this));
	if (FindChildComponent("tackle") == false)AddChildComponent(CreateComponent("tackle", TackleMove, holder_, this));
}

void Component_MeleeAttack::Update()
{
	attackRange_ = (Component_FanRangeDetector*)(GetChildComponent("attackRange"));
	if (attackRange_ == nullptr)return;

	if (!isActive_) return;

	switch (sequence_)
	{ 
		case E_STEPFORWORD: StepForward();  break;
		case E_SLASH:		Slash();		break;
	}
}

void Component_MeleeAttack::Release()
{
}

void Component_MeleeAttack::DrawData()
{
	ImGui::Separator();

	ImGui::Text("Slash");
	ImGui::Separator();

	ImGui::DragInt("power_", &power_);
	if (ImGui::Button("Execute")) Execute();


	ImGui::Separator();

}

void Component_MeleeAttack::Save(json& _saveObj)
{
	_saveObj["power_"] = power_;
	_saveObj["angle_"] = angle_;
	_saveObj["range_"] = range_;
}

void Component_MeleeAttack::Load(json& _loadObj)
{
	if (_loadObj.contains("power_")) power_ = _loadObj["power_"];
	if (_loadObj.contains("angle_")) angle_ = _loadObj["angle_"];
	if (_loadObj.contains("range_")) range_ = _loadObj["range_"];
}

void Component_MeleeAttack::StepForward()
{
	Component_TackleMove* tackleMove = (Component_TackleMove*)holder_->FindComponent("TackleMove");
	if (tackleMove == nullptr)return;


	if (isFirstMove_ == false) {
		// �ːi������ݒ�
		forward_ = Camera::GetSightLine();
		forward_ = XMVector3Normalize(XMVectorSet(XMVectorGetX(forward_), 0, XMVectorGetZ(forward_), 0));

		// �ːi������ݒ�
		tackleMove->SetDirection(forward_);

		// �X�e�[�W�����擾
		Stage* pStage = (Stage*)(holder_->FindObject("Stage"));
		if (pStage == nullptr) return;
		std::vector<StageObject*> stageObj = pStage->GetStageObjects();

		// �X�e�[�W�I�u�W�F�N�g���ׂĂɃ��C������
		for (auto obj : stageObj) {
			// �������g�̃I�u�W�F�N�g��������X�L�b�v
			if (obj->GetObjectName() == holder_->GetObjectName() || !obj->GetIsColliding())
				continue;

			// ���f���n���h�����擾
			int hGroundModel = obj->GetModelHandle();
			if (hGroundModel < 0) continue;

			// ���ʕ����Ƀ��C�𔭎�
			RayCastData tackleRayData; {
				tackleRayData.start = holder_->GetPosition(); // ���C�̔��ˈʒu
				tackleRayData.start.y += MOVE_RAY_OFFSET;
				XMStoreFloat3(&tackleRayData.dir, forward_); // ���C�̕���
				Model::RayCast(hGroundModel, &tackleRayData); // ���C�𔭎�
			}

			// ���C�������ɓ��������犎�A���̋������ːi�����ȉ���������ːi�����̍Đݒ�
			if (tackleRayData.hit && tackleRayData.dist <= distance_) {

				// �߂�������0�ɂ���
				if (tackleRayData.dist <= MOVE_DISTANCE_LIMIT) {
					distance_ = 0;
					break;
				}
				else {
					distance_ = tackleRayData.dist;
					break;

				}
			}
			else
				distance_ = MOVE_DISTANCE;
		}

		// �ːi������ݒ�
		tackleMove->SetDistance(distance_);

		// �ːi���������s
		tackleMove->Execute();

		// �_�b�V���t���O�𗧂Ă�
		isFirstMove_ = true;
	}
	if (tackleMove->IsActived() == false) {
		isFirstMove_ = false;

		distance_ = MOVE_DISTANCE;
		SetSequence(E_SLASH);
	}
}

void Component_MeleeAttack::Slash()
{
	// �S�G�I�u�W�F�N�g���擾
	vector<StageObject*> enemyObjects; {

		// �G�t�F�N�g�Đ�
		EFFEKSEERLIB::EFKTransform t;
		DirectX::XMStoreFloat4x4(&(t.matrix), holder_->GetWorldMatrix());
		t.isLoop = false;
		t.maxFrame = 120;
		t.speed = EFFECT_SPEED;
		
		effectModelTransform = EFFEKSEERLIB::gEfk->Play("slash", t);

		// �X�e�[�W���̎擾
		Stage* pStage = (Stage*)(holder_->FindObject("Stage"));
		if (pStage == nullptr)return;

		// �X�e�[�W�I�u�W�F�N�g�̎擾
		for (StageObject* object : pStage->GetStageObjects()) {

			// �G�I�u�W�F�N�g�������烊�X�g�ɒǉ�
			if (object->GetObjectType() == StageObject::TYPE_ENEMY)
				enemyObjects.push_back(object);
		}
		if (enemyObjects.size() == 0) {
			Stop();
			SetSequence(E_STEPFORWORD);
			return;
		}
	}


	for (auto enemy : enemyObjects) {

		attackRange_->SetTarget(enemy);
		if (attackRange_->IsContains()) {

			Component_HealthGauge* healthGauge = (Component_HealthGauge*)enemy->FindComponent("HealthGauge");
			if (healthGauge == nullptr)continue;
			healthGauge->TakeDamage(power_);
			
			// �G�t�F�N�g�Đ�
			EFFEKSEERLIB::EFKTransform t;
			DirectX::XMStoreFloat4x4(&(t.matrix), enemy->GetWorldMatrix());
			t.isLoop = false;
			t.maxFrame = EFFECT_FRAME;
			t.speed = EFFECT_SPEED;

			effectModelTransform = EFFEKSEERLIB::gEfk->Play("hit", t);


		}

	}
	SetSequence(E_STEPFORWORD);
	Stop();
}

