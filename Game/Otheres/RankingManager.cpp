#include "RankingManager.h"

#include "../../../Engine/ResourceManager/CsvReader.h"

#include <filesystem>
#include <fstream>
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
    for (int y = 0; y < csvReader.GetHeight(); y++)scores_.push_back(csvReader.GetValue(0, y));

    // �X�R�A�����݂��邩�ǂ�����Ԃ�
    return scores_.empty() == false;
}

bool RankingManager::Save(string _filePath)
{
    // �t�@�C�����J��
    ofstream ofs(_filePath);
    if(!ofs.is_open())return false;

    // �X�R�A���\�[�g
    SortScores();

    // �X�R�A����������
    for(int score : scores_)ofs << score << endl;

    // �t�@�C�������
    ofs.close();
    return true;
}

void RankingManager::SortScores()
{
    // �X�R�A���~���Ƀ\�[�g
	sort(scores_.begin(), scores_.end(), greater<int>());
}

int RankingManager::GetScore(int _rank) const
{
    // �����N���͈͊O�̏ꍇ��0��Ԃ�
    if(_rank < 0 || _rank >= scores_.size())return 0;
	
    // �����N�ɑΉ�����X�R�A��Ԃ�
    return scores_[_rank];
}