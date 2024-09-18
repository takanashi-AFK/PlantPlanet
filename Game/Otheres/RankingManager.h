#pragma once

#include <vector>
#include <string>
#include <utility>

using namespace std;

class RankingManager
{
private:
    // ���[�U�[���ƃX�R�A�̃y�A��ۑ�����
    std::vector<std::pair<std::string, int>> scores_;

public:
    /// <summary> �V���O���g���C���X�^���X���擾 </summary>
    static RankingManager& GetInstance();

    /// <summary> �Ǎ��iCSV�`���j </summary>
    bool Load(string _filePath);

    /// <summary> �ۑ��iCSV�`���j </summary>
    bool Save(string _filePath);

    /// <summary> �X�R�A��ǉ� </summary>
    void AddScore(const std::string& _userName, int _score);

    /// <summary> �X�R�A���\�[�g </summary>
    void SortScores();

/*
getter :*/
    /// <summary> �w�肵�������N�̃X�R�A���擾 </summary>
    int GetScore(int _rank) const;

    /// <summary> �w�肵�������N�̃��[�U�[�����擾 </summary>
    std::string GetUser(int _rank) const;

    /// <summary> �S�ẴX�R�A�ƃ��[�U�[�����擾 </summary>
    std::vector<std::pair<std::string, int>> GetAllScores() const { return scores_; }

private:
    // �R���X�g���N�^�̓v���C�x�[�g
    RankingManager() {}

    // �R�s�[�R���X�g���N�^�E������Z�q���폜
    RankingManager(const RankingManager&) = delete;
    RankingManager& operator=(const RankingManager&) = delete;
};
