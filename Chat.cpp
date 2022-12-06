#include "Сhat.h"
#include <time.h>

extern HANDLE h;

auto Chat::set_user_data(std::wstring& data, std::wregex const& reg)  -> bool
{
	bool rez;
	std::getline(std::wcin, data);
	while (!std::regex_match(data, reg))
	{
		SetConsoleTextAttribute(h, 4);
		std::wcout << L"Incorrect input, try again(quit to quit)\n> ";
		SetConsoleTextAttribute(h, 11);
		std::getline(std::wcin, data);
	}

	(data.compare(L"quit")) ? rez = 1 : rez = 0;
	return rez;
}


auto Chat::set_current_user(std::unique_ptr<User> user)            -> void
{
	_currentUser.reset(user.get());
	user.release();
}


auto Chat::time_to_string(time_t& time)                            -> std::wstring 
{
	std::time_t t = time;
	std::tm time_point;
	localtime_s(&time_point, &t);

	std::wstring year = std::to_wstring(time_point.tm_year + 1900);
	std::wstring month = std::to_wstring(time_point.tm_mon + 1);


	std::wstring day;
	if (time_point.tm_mday < 10)
	{
		day.append(L"0");
	}
	day.append(std::to_wstring(time_point.tm_mday));

	std::wstring hour;
	if (time_point.tm_hour < 10)
	{
		hour.append(L"0");
	}
	hour.append(std::to_wstring(time_point.tm_hour));

	std::wstring min;
	if (time_point.tm_min < 10)
	{
		min.append(L"0");
	}
	min.append(std::to_wstring(time_point.tm_min));

	return { day + L"-" + month + L"-" + year + L" " + hour + L":" + min };
}

auto Chat::login()  -> void
{
	auto max_ = std::numeric_limits<std::streamsize>::max;
	std::wcin.ignore(max_(), '\n');

	std::unique_ptr<User> user = std::make_unique<User>();
	std::wcout << L"Enter login\n> ";
	std::getline(std::wcin, user->getLogin());
	std::wcout << L"Enter password\n> ";
	std::getline(std::wcin, user->getPass());
	system("cls");
	if (_DB->login(*user))
	{
		SetConsoleTextAttribute(h, 10);
		std::wcout << L"Login success\n\n";
		set_current_user(std::move(user));
		extern HANDLE h;
		SetConsoleTextAttribute(h, 0x9);
		userLoop();
		return;
	}
	SetConsoleTextAttribute(h, 4);
	std::wcout << L"Incorrect login or password\n\n";

} 

auto Chat::signUp() -> void
{
	auto max_ = std::numeric_limits<std::streamsize>::max;
	std::wcin.ignore(max_(), '\n');

	std::wregex regular(L"([\\w]{4,15})");
	std::wstring login, password, username;
	
	std::wcout << L"Enter login | min;max length[4;15] | allowed characters[a-z A-Z 0-9 _] | (quit to quit):\n> ";
	if (!set_user_data(login, regular))
	{
		system("cls");
		SetConsoleTextAttribute(h, 4);
		std::wcout << L"Registration has been denied\n\n";
		return;
	}
	std::wcout << L"Enter password | min;max length[4;15] | allowed characters[a-z A-Z 0-9 _] | (quit to quit):\n> ";
	if (!set_user_data(password, regular))
	{
		system("cls");
		SetConsoleTextAttribute(h, 4);
		std::wcout << L"Registration has been denied\n\n";
		return;
	}

	std::wcout << L"Enter username | min;max length[4;15] | allowed characters[a-z A-Z 0-9 _] | (quit to quit):\n> ";
	if (!set_user_data(username, regular))
	{
		system("cls");
		SetConsoleTextAttribute(h, 4);
		std::wcout << L"Registration has been denied\n\n";
		return;
	}

	system("cls");
	std::unique_ptr<User> user = std::make_unique<User>(login, password, username);
	if (!_DB->signUp(*user))
	{
		SetConsoleTextAttribute(h, 4);
		std::wcout << L"Account with this name/login has already been registered\n\n";
		return;
	}

	SetConsoleTextAttribute(h, 10);
	std::wcout << L"Registration was successful\n\n";
}

auto Chat::getMessages() -> void
{
	auto max_ = std::numeric_limits<std::streamsize>::max;
	std::wcin.ignore(max_(), '\n');

	std::wstring chat_with;
	std::wcout << L"Choose user/chat to get messages:\n> ";
	std::getline(std::wcin, chat_with);
	User temp(chat_with);
	if (!_DB->isExisting(temp) && chat_with.compare(L"common_chat"))
	{
		system("cls");
		SetConsoleTextAttribute(h, 4);
		std::wcout << L"User/chat isn't existing\n\n";
		return;
	}
	system("cls");
	

	auto messages = _DB->getMessages(chat_with, _currentUser->getUsername());

	if (messages.empty())
	{
		SetConsoleTextAttribute(h, 4);
		std::wcout << L"You don't have any messages in this chat\n\n";
		return;
	}

	COORD c;
	c.X = 1;
	c.Y = 3;

	SetConsoleTextAttribute(h, 9);
	chatBox(chat_with, messages.size() * 3);

	SetConsoleTextAttribute(h, 15);
	for (int i = 0; i < messages.size(); ++i)
	{
		SetConsoleCursorPosition(h, c);
		std::wcout << messages[i].getSender()
			<< ' ' << time_to_string(messages[i].getTime());
		c.Y += 1;
		SetConsoleCursorPosition(h, c);
		std::wcout << messages[i].getContent();
		c.Y += 2;
	}
	c.Y += 2;
	c.X = 0;
	SetConsoleCursorPosition(h, c);

}

auto Chat::sendMessage() -> void
{
	auto max_ = std::numeric_limits<std::streamsize>::max;
	std::wcin.ignore(max_(), '\n');

	std::wstring receiver, content;
	std::wcout << L"Choose user/chat to send message:\n> ";
	std::getline(std::wcin, receiver);
	User temp(receiver);
	if (!_DB->isExisting(temp) && receiver.compare(L"common_chat"))
	{
		system("cls");
		SetConsoleTextAttribute(h, 4);
		std::wcout << L"User/chat isn't existing\n\n";
		return;
	}

	system("cls");
	std::wcout << L"Write message to " << receiver << L"{max symbols 129}\n> ";
	std::getline(std::wcin, content);
	if (content.size() > 129)
	{
		content.erase(content.begin() + 129, content.end());
	}
	std::unique_ptr<Message> message = std::make_unique<Message>
		(_currentUser->getUsername(), receiver, content, time(0));

	_DB->sendMessage(*message);

	system("cls");
	SetConsoleTextAttribute(h, 10);
	std::wcout << "Message sent\n";
	std::wcout << std::endl;
}


auto Chat::logout()                                                -> void
{
	_bUserStatus = false;
}

auto Chat::exit()                                                  -> void
{
	_bChatStatus = false;
}

auto Chat::clear_chat()                                            ->void
{
	auto max_ = std::numeric_limits<std::streamsize>::max;
	std::wcin.ignore(max_(), '\n');
	std::wstring username;
	std::wcout << "Enter username to clear chat with him:\n> ";
	std::getline(std::wcin, username);
	User temp(username);
	if (!_DB->isExisting(temp))
	{
		system("cls");
		SetConsoleTextAttribute(h, 4);
		std::wcout << L"User/chat isn't existing\n\n";
		return;
	}

	std::wcout << "Are you sure? Y/N\n> ";
	bool done = false;
	wchar_t ch; std::wcin >> ch;
	while (!done)
	{
		switch (ch)
		{
		case'y':
		case'Y':
			_DB->clearChat(_currentUser->getUsername(), username);
			done = true;
			break;
		case'n':
		case'N':
			done = true;
			break;
;		default:
	        std::wcout << "Are you sure? Y/N\n> ";
			break;
		}
	}
	std::wcout << std::endl;
}

auto Chat::get_info()                                              -> void
{
	SetConsoleTextAttribute(h, 5);

	std::wcout << L'┌' << std::setfill(L'─') << std::setw(27) << L'┐' << std::endl;

	std::wcout << L'│' << std::setfill(L' ') << std::right << "Username: " << _currentUser->getUsername() 
		<< std::setw(27 - _currentUser->getUsername().size() - 10) << L'│' << std::endl;
	std::wcout << L'│' << std::setfill(L' ') << std::right << std::setw(27) << L'│' << std::endl;

	std::wcout << L'│' << std::setfill(L' ') << std::right << "Login: " << _currentUser->getLogin()
		<< std::setw(27 - _currentUser->getLogin().size() - 7) << L'│' << std::endl;
	std::wcout << L'│' << std::setfill(L' ') << std::right << std::setw(27) << L'│' << std::endl;

	std::wcout << L'│' << std::setfill(L' ') << std::right << "Password: "<< _currentUser->getPass()
		<< std::setw(27 - _currentUser->getPass().size() - 10) << L'│' << std::endl;


	std::wcout << L'└' << std::setfill(L'─') << std::setw(27) << L'┘' << std::endl;
	std::wcout << std::endl;
}

auto Chat::chatBox(std::wstring const& username, int const& count) const             -> void
{

	std::wcout << L'┌' << std::setfill(L'─') << std::setw(130) << L'┐' << std::endl;
	std::wcout << std::left;
	std::wcout << L'│' << std::setfill(L' ') << std::setw(129) << username << L'│' << std::endl; 
	std::wcout << std::right;
	std::wcout << L'├' << std::setfill(L'─') << std::setw(130) << L'┤' << std::endl;
	std::wcout << std::left;

	for (int i = 0u; i < count; ++i)
		std::wcout << L'│' << std::setfill(L' ') << std::right << std::setw(130) << L'│' << std::endl;

	std::wcout << L'└' << std::setfill(L'─') << std::setw(130) << L'┘' << std::endl;
}



auto Chat::chatMenu() const   -> void
{
	SetConsoleTextAttribute(h, 11);
	std::wstring menu_words[] =
	{
		{L"Login"},
		{L"SignUp"},
		{L"Exit"}
	};

	std::wcout << L'┌' << std::setfill(L'─') << std::setw(27) << L'┐' << std::endl;
	for (int i = 0, j = 0; i < 5; ++i)
	{
		if (i % 2 == 0)
		{
			std::wcout << L'│' << std::setfill(L' ') << j + 1 << "." << menu_words[j]
				<< std::right << std::setw(27 - menu_words[j++].size() - 2) << L'│' << std::endl;
			continue;
		}
		std::wcout << L'│' << std::setfill(L' ') << std::right << std::setw(27) << L'│' << std::endl;
	}

	std::wcout << L'└' << std::setfill(L'─') << std::setw(27) << L'┘' << std::endl;
	std::wcout << L"> ";

	
}

auto Chat::userMenu() const   -> void
{
	SetConsoleTextAttribute(h, 11);
	std::wstring menu_words[] =
	{
		{L"Send message"},
		{L"Get messages"},
		{L"Get info about account"},
		{L"Clear chat"},
		{L"Logout"}
	};

	std::wcout << L'┌' << std::setfill(L'─') << std::setw(27) << L'┐' << std::endl;
	for (int i = 0, j = 0; i < 9; ++i)
	{
		if (i % 2 == 0)
		{
			std::wcout << L'│' << std::setfill(L' ') << j + 1 << "." << menu_words[j]
				<< std::right << std::setw(27 - menu_words[j++].size() - 2) << L'│' << std::endl;
			continue;
		}
		std::wcout << L'│' << std::setfill(L' ') << std::right << std::setw(27) << L'│' << std::endl;
	}

	std::wcout << L'└' << std::setfill(L'─') << std::setw(27) << L'┘' << std::endl;
	std::wcout << L"> ";

}

auto Chat::action_for_chat()                                      -> void
{
	void (Chat::*fact[])() = {
		&Chat::login, &Chat::signUp,
		&Chat::exit
	};
	int act; 
	while (!(std::wcin >> act) || act < 1 || act > 3)
	{
		SetConsoleTextAttribute(h, 4);
		std::wcin.clear();
		std::wcout << L"Bad input, try again\n> ";
		auto max_ = std::numeric_limits<std::streamsize>::max;
		std::wcin.ignore(max_(), '\n');
		SetConsoleTextAttribute(h, 11);
	}

	_state = fact[act - 1];
}


auto Chat::action_for_user()                                -> void
{
	void (Chat::*fact[])() = {
		&Chat::sendMessage, &Chat::getMessages,
		&Chat::get_info, &Chat::clear_chat,
		&Chat::logout, 
	};
	int act;
	while (!(std::wcin >> act) || act < 1 || act > 5)
	{
		SetConsoleTextAttribute(h, 4);
		std::wcin.clear();
		std::wcout << L"Bad input, try again\n> ";
		auto max_ = std::numeric_limits<std::streamsize>::max;
		std::wcin.ignore(max_(), '\n');
		SetConsoleTextAttribute(h, 11);
	}

	_state = fact[act - 1];
}


auto Chat::userLoop()  -> void
{
	_bUserStatus = true;
	while (_bUserStatus)
	{
		userMenu();
		action_for_user();
		system("cls");
		(this->*_state)();
	}
}

auto Chat::chatLoop() -> void
{
	_bChatStatus = true;
	while (_bChatStatus)
	{
		chatMenu();
		action_for_chat();
		system("cls");
		(this->*_state)();
	}

}


auto Chat::run()    -> void
{
	chatLoop();
}

