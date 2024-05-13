#include "Component_FanRangeDetector.h"
#include "../../StageObject.h"

Component_FanRangeDetector::Component_FanRangeDetector(StageObject* _holder)
	: Component(_holder, "Component_FanRangeDetector", FanRangeDetector)
	, target_(nullptr)
{
}

void Component_FanRangeDetector::Initialize()
{
}

void Component_FanRangeDetector::Update()
{
}

void Component_FanRangeDetector::Release()
{
}

void Component_FanRangeDetector::Save(json& _saveObj)
{
}

void Component_FanRangeDetector::Load(json& _loadObj)
{
}

void Component_FanRangeDetector::DrawData()
{
}

bool Component_FanRangeDetector::Contains()
{
	//	�Ώۂ����݂��Ȃ��ꍇ�͏������s��Ȃ�
	if (target_ == nullptr)return false;

	//	��`�͈͔���
	
	// �ۗL�҂̈ʒu����Ώۂ̈ʒu�ւ̃x�N�g��
	XMVECTOR holderPositionToTargetPosition = 
		XMVectorSetY(XMLoadFloat3(&target_->GetPosition()),0) - XMVectorSetY(XMLoadFloat3(&holder_->GetPosition()), 0);

	// �ۗL�҂̈ʒu����Ώۂ̈ʒu�ւ̃x�N�g���̒���
	float length = XMVectorGetX(XMVector3Length(holderPositionToTargetPosition));

	
}
