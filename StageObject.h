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
    vector<Component*> myComponents_;   // ���g���ۗL����R���|�[�l���g�Q
    string modelFilePath_;              // ���f���̃t�@�C���p�X
    int modelHandle_;                   // ���f���ԍ�
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    StageObject(string _name,string _modelFilePath,GameObject* _parent);

    /// <summary>
    /// �g�p����R���|�[�l���g��ǉ�
    /// </summary>
    /// <param name="_comp">�g�p����R���|�[�l���g�̃A�h���X</param>
    /// <returns>�ǉ��ł������ǂ���</returns>
    bool AddComponent(Component* _comp);

    /// <summary>
    /// �ǉ����ꂽ�R���|�[�l���g���폜
    /// </summary>
    /// <param name="_comp">�폜����R���|�[�l���g�̃A�h���X</param>
    /// <returns>�����ł������ǂ���</returns>
    bool DeleteComponent(Component* _comp);

    /// <summary>
    /// ������
    /// </summary>
    void Initialize() override;

    /// <summary>
    /// �X�V
    /// </summary>
    void Update() override;

    /// <summary>
    /// �`��
    /// </summary>
    void Draw() override;

    /// <summary>
    /// �J��
    /// </summary>
    void Release() override;

    /// <summary>
    /// �ۑ�
    /// </summary>
    /// <param name="_saveObj">�ۑ����json�I�u�W�F�N�g</param>
    void Save(json& _saveObj);

    /// <summary>
    /// �Ǎ�
    /// </summary>
    /// <param name="_loadObj">�ǂݍ���json�I�u�W�F�N�g</param>
    void Load(json& _loadObj);
};

/// <summary>
/// �X�e�[�W�I�u�W�F�N�g�𐶐�����֐�
/// </summary>
/// <param name="_name"> �I�u�W�F�N�g��</param>
/// <param name="_modelFilePath"> ���f���̃t�@�C���p�X</param>
/// <param name="_parent"> �e�I�u�W�F�N�g</param>
/// <returns>�I�u�W�F�N�g�̃A�h���X</returns>
StageObject* CreateStageObject(string _name, string _modelFilePath, GameObject* _parent);