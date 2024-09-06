#include "Component_Motion.h"

Component_Motion::Component_Motion(string _name, StageObject* _holder, ComponentType _type, Component* _parent)
 :Component(_holder, _name, _type, _parent)
{
}

void Component_Motion::Initialize()
{
}

void Component_Motion::Update()
{


}

void Component_Motion::Release()
{
}

void Component_Motion::DrawData()
{
}

MotionData::MotionData(string _filePath, int startFrame, int endFrame, int speed)
{
    // �t�@�C���p�X��ݒ�
    filePath = _filePath;

    // ���f���n���h����ݒ�
    modelHandle = Model::Load(filePath);

    // �J�n�t���[����ݒ�
    startFrame = startFrame;

    // �I���t���[����ݒ�
    endFrame = endFrame;

    // �A�j���[�V�����X�s�[�h��ݒ�
    speed = speed;
}
