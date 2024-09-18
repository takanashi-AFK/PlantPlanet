#include "RankingManager.h"
#include "../../Engine/Json/JsonReader.h"
using namespace std;

RankingManager& RankingManager::GetInstance()
{
    static RankingManager instance;
    return instance;
}

bool RankingManager::Load(string _filePath)
{
    // json�I�u�W�F�N�g���쐬
    json loadData;

    // �t�@�C���Ǎ�
    if (!JsonReader::Load(_filePath, loadData)) return false;

    // �X�R�A�z����N���A���čč\�z
    scores_.clear();

    // json�̒��g���擾
    for (auto& obj : loadData) {
        // json�̒��g��userName��score�������Ă��邩�m�F
		if(obj.contains("userName") && obj.contains("score"))
		scores_.push_back(make_pair(obj["userName"],obj["score"]));
	}

    // �X�R�A���\�[�g
    SortScores();

    // �X�R�A�����݂��邩�ǂ�����Ԃ�
    return !scores_.empty();
}

bool RankingManager::Save(string _filePath)
{
    // json�I�u�W�F�N�g���쐬
    json saveData;

    // �X�R�A���\�[�g
    SortScores();

    // �X�R�A��json�Ɋi�[
    for (const auto& entry : scores_)
    {
		saveData.push_back({ {"userName", entry.first}, {"score", entry.second} });
	}

    // �t�@�C�������
    return (JsonReader::Save(_filePath, saveData));
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
