#include "UserManager.h"

#include "../../Engine/Json/JsonReader.h"
#include <algorithm> // std::count

using std::count;
using std::sort;

UserManager& UserManager::GetInstance()
{
	static UserManager instance;
	return instance;
}

bool UserManager::RegisterUser(const string& _userName)
{
	// ユーザーを登録
	registeredUsers_[_userName] = new UserInfo(_userName);

	// ユーザーが登録されたかどうかを返す
	return registeredUsers_.find(_userName) != registeredUsers_.end();
}

bool UserManager::DeleteUser(const string& _userName)
{
	// ユーザーを削除
	registeredUsers_.erase(_userName);

	// ユーザーが削除されたかどうかを返す
	return registeredUsers_.find(_userName) == registeredUsers_.end();
}

bool UserManager::LoginUser(const string& _userName)
{
	// ユーザーが登録されているかどうかを確認
	if (registeredUsers_.find(_userName) != registeredUsers_.end())
	{
		// ログイン中のユーザーを設定
		loggedInUser_ = registeredUsers_[_userName];
		return true;
	}
	return false;
}

void UserManager::LogoutUser()
{
	// ログイン中のユーザーを解除
	loggedInUser_ = nullptr;
}

bool UserManager::SaveUser(const string& _filePath)
{
	// jsonオブジェクトを作成
	json saveData;

	// ユーザー情報をjsonに格納
	for (const auto& user : registeredUsers_)
		saveData.push_back({ {"userName", user.second->userName}, {"bestScore", user.second->bestScore}, {"playTotalTime", user.second->playTotalTime}, {"libraryStatus", user.second->libraryStatus} });

	// 保存
	return JsonReader::Save(_filePath, saveData);
}

bool UserManager::LoadUser(const string& _filePath)
{
	// jsonオブジェクトを作成
	json loadData;

	// ファイル読込
	if (!JsonReader::Load(_filePath, loadData)) return false;

	// ユーザー情報をクリア
	registeredUsers_.clear();

	// jsonの中身を取得
	for (auto& obj : loadData)
	{
		// ユーザーを登録
		registeredUsers_[obj["userName"]] = new UserInfo(obj["userName"]);
		registeredUsers_[obj["userName"]]->bestScore = obj["bestScore"];
		registeredUsers_[obj["userName"]]->playTotalTime = obj["playTotalTime"];
		std::copy(obj["libraryStatus"].begin(), obj["libraryStatus"].end(),registeredUsers_[obj["userName"]]->libraryStatus.begin());
	}
	return false;
}

void UserManager::UpdateLibraryStatus(const string& _userName, int _index)
{
	// 図鑑のステータスを更新
	registeredUsers_[_userName]->libraryStatus[_index] = true;
}

void UserManager::UpdatePlayTotalTime(const string& _userName, int _time)
{
	// プレイ時間を更新
	registeredUsers_[_userName]->playTotalTime = _time;
}

void UserManager::UpdateBestScore(const string& _userName, int _score)
{
	// ベストスコアを更新
	registeredUsers_[_userName]->bestScore = _score;
}

UserInfo* UserManager::GetUser(int _rank)
{
	// ユーザーが存在しない場合は nullptr を返す
	if (registeredUsers_.empty()) return nullptr;
	
	// _rank が無効な場合は nullptr を返す
	if (_rank <= 0 || _rank > static_cast<int>(registeredUsers_.size())) return nullptr;
	
	// ユーザーをスコア降順にソート
	vector<UserInfo*> sortedUsers;
	for (auto& user : registeredUsers_) sortedUsers.push_back(user.second);
	sort(sortedUsers.begin(), sortedUsers.end(), [](const UserInfo* a, const UserInfo* b) {return a->bestScore > b->bestScore;});

	// 引数 `_rank` が 1 の場合に先頭のユーザーを取得するよう調整
	int rank = _rank - 1;

	// ランキング順位のユーザーを取得
	return sortedUsers[rank];
}

int UserManager::GetLibraryCompletenessRate(const string& _userName)
{
	// 図鑑の完成率を取得
	int rate = static_cast<double>(
		std::count(registeredUsers_[_userName]->libraryStatus.begin(), registeredUsers_[_userName]->libraryStatus.end(), true))
		/ PlantCollection::GetPlants().size() * 100.0;
	return rate;
}

int UserManager::GetPlayTotalTime(const string& _userName)
{
	// プレイ時間を取得
	return registeredUsers_[_userName]->playTotalTime;
}

vector<bool> UserManager::GetPlantData(const string& _userName)
{
	// ユーザーの図鑑データを取得
	return registeredUsers_[_userName]->libraryStatus;

}

bool UserManager::isUserRegistered(const string& _userName)
{
	// ユーザーが登録されているかどうかを返す
	return registeredUsers_.find(_userName) != registeredUsers_.end();
}
