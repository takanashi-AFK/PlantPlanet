#include "UserManager.h"

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

bool UserManager::isUserRegistered(const string& _userName)
{
	// ���[�U�[���o�^����Ă��邩�ǂ�����Ԃ�
	return registeredUsers_.find(_userName) != registeredUsers_.end();
}
