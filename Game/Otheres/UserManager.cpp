#include "UserManager.h"

#include "../../Engine/Json/JsonReader.h"
#include <algorithm> // std::count

using std::count;

UserManager& UserManager::GetInstance()
{
	static UserManager instance;
	return instance;
}

bool UserManager::RegisterUser(const string& _userName)
{
	// ���[�U�[��o�^
	registeredUsers_[_userName] = new UserInfo(_userName);

	// ���[�U�[���o�^���ꂽ���ǂ�����Ԃ�
	return registeredUsers_.find(_userName) != registeredUsers_.end();
}

bool UserManager::DeleteUser(const string& _userName)
{
	// ���[�U�[���폜
	registeredUsers_.erase(_userName);

	// ���[�U�[���폜���ꂽ���ǂ�����Ԃ�
	return registeredUsers_.find(_userName) == registeredUsers_.end();
}

bool UserManager::LoginUser(const string& _userName)
{
	// ���[�U�[���o�^����Ă��邩�ǂ������m�F
	if (registeredUsers_.find(_userName) != registeredUsers_.end())
	{
		// ���O�C�����̃��[�U�[��ݒ�
		loggedInUser_ = registeredUsers_[_userName];
		return true;
	}
	return false;
}

void UserManager::LogoutUser()
{
	// ���O�C�����̃��[�U�[������
	loggedInUser_ = nullptr;
}

bool UserManager::SaveUser(const string& _filePath)
{
	// json�I�u�W�F�N�g���쐬
	json saveData;

	// ���[�U�[����json�Ɋi�[
	for (const auto& user : registeredUsers_)
		saveData.push_back({ {"userName", user.second->userName}, {"bestScore", user.second->bestScore}, {"playTotalTime", user.second->playTotalTime}, {"libraryStatus", user.second->libraryStatus} });

	// �ۑ�
	return JsonReader::Save(_filePath, saveData);
}

bool UserManager::LoadUser(const string& _filePath)
{
	// json�I�u�W�F�N�g���쐬
	json loadData;

	// �t�@�C���Ǎ�
	if (!JsonReader::Load(_filePath, loadData)) return false;

	// ���[�U�[�����N���A
	registeredUsers_.clear();

	// json�̒��g���擾
	for (auto& obj : loadData)
	{
		// ���[�U�[��o�^
		registeredUsers_[obj["userName"]] = new UserInfo(obj["userName"]);
		registeredUsers_[obj["userName"]]->bestScore = obj["bestScore"];
		registeredUsers_[obj["userName"]]->playTotalTime = obj["playTotalTime"];
		std::copy(obj["libraryStatus"].begin(), obj["libraryStatus"].end(),registeredUsers_[obj["userName"]]->libraryStatus.begin());
	}
	return false;
}

void UserManager::UpdateLibraryStatus(const string& _userName, int _index)
{
	// �}�ӂ̃X�e�[�^�X���X�V
	registeredUsers_[_userName]->libraryStatus[_index] = true;
}

void UserManager::UpdatePlayTotalTime(const string& _userName, int _time)
{
	// �v���C���Ԃ��X�V
	registeredUsers_[_userName]->playTotalTime += _time;
}

void UserManager::UpdateBestScore(const string& _userName, int _score)
{
	// �x�X�g�X�R�A���X�V
	registeredUsers_[_userName]->bestScore = _score;
}

int UserManager::GetLibraryCompletenessRate(const string& _userName)
{
	// �}�ӂ̊��������擾
	int rate = static_cast<double>(
		std::count(registeredUsers_[_userName]->libraryStatus.begin(), registeredUsers_[_userName]->libraryStatus.end(), true))
		/ PlantCollection::GetPlants().size() * 100.0;
	return rate;
}

int UserManager::GetPlayTotalTime(const string& _userName)
{
	// �v���C���Ԃ��擾
	return registeredUsers_[_userName]->playTotalTime;
}

bool UserManager::isUserRegistered(const string& _userName)
{
	// ���[�U�[���o�^����Ă��邩�ǂ�����Ԃ�
	return registeredUsers_.find(_userName) != registeredUsers_.end();
}
