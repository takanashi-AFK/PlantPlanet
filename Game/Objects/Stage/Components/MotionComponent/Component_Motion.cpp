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

MotionData::MotionData(string _filePath, int _startFrame, int _endFrame, int _speed, bool _isLoop)
{
    // �t�@�C���p�X��ݒ�
    filePath = _filePath;

    // ���f���n���h����ݒ�
    modelHandle = Model::Load(_filePath);

    // �J�n�t���[����ݒ�
    startFrame = _startFrame;

    // �I���t���[����ݒ�
    endFrame = _endFrame;

    // �A�j���[�V�����X�s�[�h��ݒ�
    speed = _speed;

    // ���[�v���邩��ݒ�
    isLoop = _isLoop;
}
