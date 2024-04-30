#pragma once

// �C���N���[�h
#include "Engine/GameObject/GameObject.h"
#include "Component.h"
#include <vector>

// using�錾
using std::vector;

// �X�e�[�W�ɓo�ꂷ��I�u�W�F�N�g�̃N���X
class StageObject:public GameObject
{
private:
    vector<Component*> myComponents_;
    string modelFilePath_;

public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    StageObject(string _name,string _modelFilePath,GameObject* _parent);

    /// <summary>
    /// �g�p����R���|�[�l���g��ǉ�
    /// </summary>
    /// <param name="_comp">�g�p����R���|�[�l���g�̃A�h���X</param>
    /// <returns>�������s����</returns>
    bool AddComponent(Component* _comp);

    /// <summary>
    /// �ǉ����ꂽ�R���|�[�l���g���폜
    /// </summary>
    /// <param name="_comp">�폜����R���|�[�l���g�̃A�h���X</param>
    /// <returns></returns>
    bool DeleteComponent(Component* _comp);

    /// <summary>
    /// ������
    /// </summary>
    void Initialize();

    /// <summary>
    /// �X�V
    /// </summary>
    void Update();

    /// <summary>
    /// �`��
    /// </summary>
    void Draw() override;

    /// <summary>
    /// �J��
    /// </summary>
    void Release() override;
};


StageObject* CreateStageObject(string _name, string _modelFilePath, GameObject* _parent);