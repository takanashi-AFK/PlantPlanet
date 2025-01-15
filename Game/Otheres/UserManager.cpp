#include "UserManager.h"

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

void UserManager::UpdateLibraryStatus(const string& _userName, int _index)
{
	// 図鑑のステータスを更新
	registeredUsers_[_userName]->libraryStatus[_index] = true;
}

void UserManager::UpdatePlayTotalTime(const string& _userName, int _time)
{
	// プレイ時間を更新
	registeredUsers_[_userName]->playTotalTime += _time;
}

void UserManager::UpdateBestScore(const string& _userName, int _score)
{
	// ベストスコアを更新
	registeredUsers_[_userName]->bestScore = _score;
}

bool UserManager::isUserRegistered(const string& _userName)
{
	// ユーザーが登録されているかどうかを返す
	return registeredUsers_.find(_userName) != registeredUsers_.end();
}
