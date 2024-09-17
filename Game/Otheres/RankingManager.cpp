#include "RankingManager.h"
#include "../../../Engine/ResourceManager/CsvReader.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
namespace fs = std::filesystem;

RankingManager& RankingManager::GetInstance()
{
    static RankingManager instance;
    return instance;
}

bool RankingManager::Load(string _filePath)
{
    // csv���[�_�[���쐬
    CsvReader csvReader;

    // �t�@�C���ǂݍ���
    if (!csvReader.Load(_filePath)) return false;

    // �X�R�A�z����N���A���čč\�z
    scores_.clear();
    for (int y = 0; y < csvReader.GetHeight(); y++)
    {
        // csv��1��ڂɃ��[�U�[���A2��ڂɃX�R�A������Ɖ���
        string userName = csvReader.GetString(1, y);
        int score = csvReader.GetValue(2, y);
        scores_.push_back(make_pair(userName, score));
    }

    // �X�R�A�����݂��邩�ǂ�����Ԃ�
    return !scores_.empty();
}

bool RankingManager::Save(string _filePath)
{
    // �t�@�C�����J��
    ofstream ofs(_filePath);
    if (!ofs.is_open()) return false;

    // �X�R�A���\�[�g
    SortScores();

    // �X�R�A����������
    for (const auto& entry : scores_)
    {
        ofs << entry.first << "," << entry.second << endl; // ���[�U�[��,�X�R�A��ۑ�
    }

    // �t�@�C�������
    ofs.close();
    return true;
}

void RankingManager::AddScore(const std::string& _userName, int _score)
{
	// �X�R�A��ǉ�
	scores_.push_back(make_pair(_userName, _score));
}

void RankingManager::SortScores()
{
    // �X�R�A���~���Ƀ\�[�g
    sort(scores_.begin(), scores_.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
        });
}

int RankingManager::GetScore(int _rank) const
{
    // �����N���͈͊O�̏ꍇ��0��Ԃ�
    if (_rank < 0 || _rank >= scores_.size()) return 0;

    // �����N�ɑΉ�����X�R�A��Ԃ�
    return scores_[_rank].second;
}

string RankingManager::GetUser(int _rank) const
{
    // �����N���͈͊O�̏ꍇ�͋󕶎���Ԃ�
    if (_rank < 0 || _rank >= scores_.size()) return "";

    // �����N�ɑΉ����郆�[�U�[����Ԃ�
    return scores_[_rank].first;
}
