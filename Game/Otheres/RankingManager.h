#pragma once

#include <vector>
#include <string>

using namespace std;

class RankingManager
{
private:
    std::vector<int> scores_;  // �X�R�A�݂̂�ۑ�

public:
    /// <summary> �V���O���g���C���X�^���X���擾 </summary>
    static RankingManager& GetInstance();
    
    /// <summary> �Ǎ��iCSV�`���j </summary>
    bool Load(string _filePath);

    /// <summary> �ۑ��iCSV�`���j </summary>
    bool Save(string _filePath);

    /// <summary> �X�R�A��ǉ� </summary>
    void AddScore(int _score) { scores_.push_back(_score); }

    /// <summary> �X�R�A���\�[�g </summary>
    void SortScores();

/*
getter :*/
    /// <summary> �w�肵�������N�̃X�R�A���擾 </summary>
    int GetScore(int _rank) const;

    /// <summary> �X�R�A�̐����擾 </summary>
    vector<int> GetAllScores() const { return scores_; }

private:
    // �R���X�g���N�^�̓v���C�x�[�g
    RankingManager() {}

    // �R�s�[�R���X�g���N�^�E������Z�q���폜
    RankingManager(const RankingManager&) = delete;
    RankingManager& operator=(const RankingManager&) = delete;

};