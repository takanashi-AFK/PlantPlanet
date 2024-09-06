#pragma once
#include "../Component.h"
#include "../../Components/BehaviorComponents/Component_PlayerBehavior.h"
#include <map>

struct MotionData {
    string filePath;    // �t�@�C���p�X
    int modelHandle;    // ���f���n���h��
    int startFrame;        // �J�n�t���[��
    int endFrame;        // �I���t���[��
    int speed;            // �A�j���[�V�����X�s�[�h

    /// <summary> �R���X�g���N�^ </summary>
    MotionData(string _filePath, int startFrame, int endFrame, int speed);

    /// <summary> �f�t�H���g�R���X�g���N�^ </summary>
    MotionData() :filePath(""), modelHandle(-1), startFrame(0), endFrame(0), speed(0) {}
};

class Component_Motion : public Component
{
protected:
    // ���f���̃n���h�����X�g
    // FIX: PlayerState ���Q�Ƃ���͈̂ˑ��֌W����������׏C�����K�v
	std::map<PlayerState, MotionData>modelHandleList_;

public:
    /// <summary> �R���X�g���N�^ </summary>
	Component_Motion(string _name, StageObject* _holder,ComponentType _type, Component* _parent);
	
    /// <summary> ������ </summary>
    void Initialize() override;
	
    /// <summary> �X�V </summary>
    void Update() override;

    /// <summary> ��� </summary>
	void Release() override;

    /// <summary> ImGui�p�l���\�� </summary>
	void DrawData() override;
};

