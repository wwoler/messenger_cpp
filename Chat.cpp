#include "Сhat.h"
#include <time.h>

extern HANDLE h;

auto Chat::flush_input_buffer()  ->void
{
	auto max_ = std::numeric_limits<std::streamsize>::max;
	std::wcin.ignore(max_(), '\n');
}

auto Chat::set_user_data(std::wstring& data, std::wregex const& reg)  ->bool
{
	std::getline(std::wcin, data);
	while (!std::regex_match(data, reg))
	{
		SetConsoleTextAttribute(h, 4u);
		std::wcout << L"Incorrect input, try again(quit to quit)\n> ";
		SetConsoleTextAttribute(h, 11u);
		std::getline(std::wcin, data);
	}

	if(!(data.compare(L"quit")) || !data.compare(L"common_chat"))
		return false;



	return true;
}


auto Chat::set_current_user(std::unique_ptr<User> user)  ->void
{
	_currentUser.reset(user.get());
	user.release();
}


auto Chat::time_to_string(time_t& time)  ->std::wstring 
{
	std::tm time_point;
	localtime_s(&time_point, &time);

	std::wstring year = std::to_wstring(time_point.tm_year + 1900u);
	std::wstring month = std::to_wstring(time_point.tm_mon + 1u);


	std::wstring day;
	if (time_point.tm_mday < 10u)
	{
		day.append(L"0");
	}
	day.append(std::to_wstring(time_point.tm_mday));

	std::wstring hour;
	if (time_point.tm_hour < 10u)
	{
		hour.append(L"0");
	}
	hour.append(std::to_wstring(time_point.tm_hour));

	std::wstring min;
	if (time_point.tm_min < 10u)
	{
		min.append(L"0");
	}
	min.append(std::to_wstring(time_point.tm_min));

	return { day + L"-" + month + L"-" + year + L" " + hour + L":" + min };
}

auto Chat::login()  ->void
{
	flush_input_buffer();

	std::unique_ptr<User> user = std::make_unique<User>();

	std::wcout << L"Enter login\n> ";
	std::getline(std::wcin, user->getLogin());

	std::wcout << L"Enter password\n> ";
	std::getline(std::wcin, user->getPass());


	system("cls");
	if (_DB->login(*user))
	{
		SetConsoleTextAttribute(h, 10u);
		std::wcout << L"Login success\n\n";
		set_current_user(std::move(user));
		SetConsoleTextAttribute(h, 9u);
		userLoop();
		return;
	}
	SetConsoleTextAttribute(h, 4u);
	std::wcout << L"Incorrect login or password\n\n";

} 

auto Chat::signUp()  ->void
{
	flush_input_buffer();

	std::wregex regular(L"([A-Za-z0-9_]{4,15})");
	std::wstring login, password, username; 
	
	std::wcout << L"Enter login | min;max length[4;15] | allowed characters[a-z A-Z 0-9 _] | (quit to quit):\n> ";
	if (!set_user_data(login, regular))
	{
		system("cls");
		SetConsoleTextAttribute(h, 4u);
		std::wcout << L"Registration has been denied\n\n";
		return;
	}
	std::wcout << L"Enter password | min;max length[4;15] | allowed characters[a-z A-Z 0-9 _] | (quit to quit):\n> ";
	if (!set_user_data(password, regular))
	{
		system("cls");
		SetConsoleTextAttribute(h, 4u);
		std::wcout << L"Registration has been denied\n\n";
		return;
	}

	std::wcout << L"Enter username | min;max length[4;15] | allowed characters[a-z A-Z 0-9 _] | (quit to quit):\n> ";
	if (!set_user_data(username, regular))
	{
		system("cls");
		SetConsoleTextAttribute(h, 4u);
		std::wcout << L"Registration has been denied\n\n";
		return;
	}



	system("cls");
	std::unique_ptr<User> user = std::make_unique<User>(login, password, username);
	if (!_DB->signUp(*user))
	{
		SetConsoleTextAttribute(h, 4u);
		std::wcout << L"Account with this name/login has already been registered\n\n";
		return;
	}

	SetConsoleTextAttribute(h, 10u);
	std::wcout << L"Registration was successful\n\n";
}

auto Chat::getMessages()  ->void
{
	flush_input_buffer();

	std::wstring chat_with;
	std::wcout << L"Enter username/chatname to get messages:\n> ";
	std::getline(std::wcin, chat_with);
	User temp(chat_with);
	if (!_DB->isExisting(temp) && chat_with.compare(L"common_chat"))
	{
		system("cls");
		SetConsoleTextAttribute(h, 4u);
		std::wcout << L"User/chat isn't existing\n\n";
		return;
	}
	system("cls");
	
	auto messages = _DB->getMessages(chat_with, _currentUser->getUsername());

	if (messages.empty())
	{
		SetConsoleTextAttribute(h, 4u);
		std::wcout << L"You don't have any messages in this chat\n\n";
		return;
	}

	/*Для разметки консоли*/
	COORD c;
	c.X = 1;
	c.Y = 3;

	SetConsoleTextAttribute(h, 9u);
	chatBox(chat_with, messages.size() * 3u);

	SetConsoleTextAttribute(h, 15u);
	for (int i = 0; i < messages.size(); ++i)
	{
		SetConsoleCursorPosition(h, c);
		std::wcout << messages[i].getSender() 
			<< L' ' << time_to_string(messages[i].getTime());
		c.Y += 1;
		SetConsoleCursorPosition(h, c);
		std::wcout << messages[i].getContent();
		c.Y += 2;
	}
	c.Y += 2;
	c.X = 0;
	SetConsoleCursorPosition(h, c);

}

auto Chat::sendMessage()  ->void
{
	flush_input_buffer();

	std::wstring receiver, content;
	std::wcout << L"Enter username/chatname to send message:\n> ";
	std::getline(std::wcin, receiver);
	User temp(receiver);
	if (!_DB->isExisting(temp) && receiver.compare(L"common_chat"))
	{
		system("cls");
		SetConsoleTextAttribute(h, 4u);
		std::wcout << L"User/chat isn't existing\n\n";
		return;
	}

	system("cls");
	std::wcout << L"Write message to " << receiver << L"{max symbols: 129}\n> ";
	std::getline(std::wcin, content);
	if (content.size() > 129)
	{
		content.erase(content.begin() + 129u, content.end());
	}
	std::unique_ptr<Message> message = std::make_unique<Message>
		(_currentUser->getUsername(), receiver, content, time(0));

	_DB->sendMessage(*message);

	system("cls");
	SetConsoleTextAttribute(h, 10u);
	std::wcout << "Message sent\n";
	std::wcout << std::endl;
}


auto Chat::logout()  ->void
{
	_bUserStatus = false;
	_currentUser.reset(nullptr);
}

auto Chat::exit()  ->void
{
	throw CloseChat{};
}


auto Chat::clear_chat()  ->void
{
	flush_input_buffer();

	std::wstring username;
	std::wcout << "Enter username to clear chat with him:\n> ";
	std::getline(std::wcin, username);
	if (!username.compare(L"common_chat"))
	{
		system("cls");
		SetConsoleTextAttribute(h, 4u);
		std::wcout << "The shared chat cannot be cleared\n\n";
		return;
	}

	User temp(username);

	if (!_DB->isExisting(temp))
	{
		system("cls");
		SetConsoleTextAttribute(h, 4u);
		std::wcout << L"User/chat isn't existing\n\n";
		return;
	}

	bool done = false;
	wchar_t ch;
	while (!done)
	{
		std::wcout << "Are you sure? Y/N\n> ";
		std::wcin >> ch;
		switch (ch)
		{
		case'y':
		case'Y':
			_DB->clearChat(_currentUser->getUsername(), username);
			done = true;
			break;
		case'n':
		case'N':
			std::wcout << std::endl;
			return;
			break;
		default:
			flush_input_buffer();
			break;
		}
	}
	system("cls");
	SetConsoleTextAttribute(h, 10u);
	std::wcout << "Chat has been cleared" << std::endl;
	std::wcout << std::endl;
}


auto Chat::changePassword()  ->void
{
	flush_input_buffer();

	std::wregex regular(L"([A-Za-z0-9_]{4,15})");
	std::wstring newPassword;

	std::wcout << L"Enter new password | min;max length[4;15] | allowed characters[a-z A-Z 0-9 _] | (quit to quit):\n> ";
	if (!set_user_data(newPassword, regular))
	{
		system("cls");
		SetConsoleTextAttribute(h, 4u);
		std::wcout << L"Changing login has been denied\n\n";
		return;
	}

	_DB->changePassword(*_currentUser);


	system("cls");
	SetConsoleTextAttribute(h, 10u);
	std::wcout << "Password has been changed" << std::endl;
	std::wcout << std::endl;

}

auto Chat::changeLogin()  ->void
{
	flush_input_buffer();

	std::wregex regular(L"([A-Za-z0-9_]{4,15})");
	std::wstring newLogin;

	std::wcout << L"Enter new login | min;max length[4;15] | allowed characters[a-z A-Z 0-9 _] | (quit to quit):\n> ";
	if (!set_user_data(newLogin, regular))
	{
		system("cls");
		SetConsoleTextAttribute(h, 4u);
		std::wcout << L"Changing password has been denied\n\n";

		return;
	}


	auto buffCurrentPos = _DB->_currentUserPos;
	User tempUser(newLogin);

	if (_DB->isExisting(tempUser))
	{
		system("cls");
		SetConsoleTextAttribute(h, 4u);
		std::wcout << "Account with this login has already exited\n\n";
		return;
	}

	_currentUser->setLogin(newLogin);

	_DB->_currentUserPos = buffCurrentPos;
	_DB->changeLogin(*_currentUser);


	system("cls");
	SetConsoleTextAttribute(h, 10u);
	std::wcout << "Login has been changed" << std::endl;
	std::wcout << std::endl;
}


auto Chat::get_info()  ->void
{
	SetConsoleTextAttribute(h, 5u);

	std::wcout << L'┌' << std::setfill(L'─') << std::setw(27u) << L'┐' << std::endl;

	std::wcout << L'│' << std::setfill(L' ') << std::right << "Username: " << _currentUser->getUsername() 
		<< std::setw(27u - _currentUser->getUsername().size() - 10u) << L'│' << std::endl;
	std::wcout << L'│' << std::setfill(L' ') << std::right << std::setw(27u) << L'│' << std::endl;

	std::wcout << L'│' << std::setfill(L' ') << std::right << "Login: " << _currentUser->getLogin()
		<< std::setw(27u - _currentUser->getLogin().size() - 7u) << L'│' << std::endl;
	std::wcout << L'│' << std::setfill(L' ') << std::right << std::setw(27u) << L'│' << std::endl;

	std::wcout << L'│' << std::setfill(L' ') << std::right << "Password: "<< _currentUser->getPass()
		<< std::setw(27u - _currentUser->getPass().size() - 10u) << L'│' << std::endl;


	std::wcout << L'└' << std::setfill(L'─') << std::setw(27u) << L'┘' << std::endl;
	std::wcout << std::endl;
}

auto Chat::chatBox(std::wstring const& username, int const& count) const  ->void
{

	std::wcout << L'┌' << std::setfill(L'─') << std::setw(130u) << L'┐' << std::endl;
	std::wcout << std::left;
	std::wcout << L'│' << std::setfill(L' ') << std::setw(129u) << username << L'│' << std::endl; 
	std::wcout << std::right;
	std::wcout << L'├' << std::setfill(L'─') << std::setw(130u) << L'┤' << std::endl;
	std::wcout << std::left;

	for (size_t i = 0u; i < count; ++i)
		std::wcout << L'│' << std::setfill(L' ') << std::right << std::setw(130u) << L'│' << std::endl;

	std::wcout << L'└' << std::setfill(L'─') << std::setw(130u) << L'┘' << std::endl;
}



auto Chat::chatMenu() const  ->void
{
	SetConsoleTextAttribute(h, 11u);
	std::wstring menu_words[] =
	{
		{L"Login"},
		{L"SignUp"},
		{L"Exit"}
	};

	std::wcout << L'┌' << std::setfill(L'─') << std::setw(27u) << L'┐' << std::endl;
	for (size_t i = 0u, j = 0; i < 5u; ++i)
	{
		if (i % 2 == 0u)
		{
			std::wcout << L'│' << std::setfill(L' ') << j + 1 << "." << menu_words[j]
				<< std::right << std::setw(27u - menu_words[j++].size() - 2u) << L'│' << std::endl;
			continue;
		}
		std::wcout << L'│' << std::setfill(L' ') << std::right << std::setw(27u) << L'│' << std::endl;
	}

	std::wcout << L'└' << std::setfill(L'─') << std::setw(27u) << L'┘' << std::endl;
	std::wcout << L"> ";

	
}

auto Chat::userMenu() const  ->void
{
	SetConsoleTextAttribute(h, 11u);
	std::wstring menu_words[] =
	{
		{L"Send message"},
		{L"Get messages"},
		{L"Get info about account"},
		{L"Clear chat"},
		{L"Change password"},
		{L"Change login"},
		{L"Logout"}
	};

	std::wcout << L'┌' << std::setfill(L'─') << std::setw(27u) << L'┐' << std::endl;
	for (size_t i = 0u, j = 0; i < 13u; ++i)
	{
		if (i % 2 == 0u)
		{
			std::wcout << L'│' << std::setfill(L' ') << j + 1u << "." << menu_words[j]
				<< std::right << std::setw(27u - menu_words[j++].size() - 2u) << L'│' << std::endl;
			continue;
		}
		std::wcout << L'│' << std::setfill(L' ') << std::right << std::setw(27u) << L'│' << std::endl;
	}

	std::wcout << L'└' << std::setfill(L'─') << std::setw(27u) << L'┘' << std::endl;
	std::wcout << L"> ";

}

auto Chat::action_for_chat()  ->void
{
	void (Chat::*fact[])() = {
		&Chat::login, &Chat::signUp,
		&Chat::exit
	};
	int act; 
	while (!(std::wcin >> act) || act < 1 || act > 3)
	{
		SetConsoleTextAttribute(h, 4u);
		std::wcin.clear();
		std::wcout << L"Bad input, try again\n> ";
		flush_input_buffer();
		SetConsoleTextAttribute(h, 11u);
	}

	_state = fact[act - 1];
}


auto Chat::action_for_user()  ->void
{
	void (Chat:: * fact[])() = {
		&Chat::sendMessage,     &Chat::getMessages,
		&Chat::get_info,        &Chat::clear_chat,
		&Chat::changePassword, &Chat::changeLogin,
	    &Chat::logout
	};
	int act;
	while (!(std::wcin >> act) || act < 1 || act > 7)
	{
		SetConsoleTextAttribute(h, 4u);
		std::wcin.clear();
		std::wcout << L"Bad input, try again\n> ";
		flush_input_buffer();
		SetConsoleTextAttribute(h, 11u);
	}

	_state = fact[act - 1];
}


auto Chat::userLoop()  ->void
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

auto Chat::chatLoop()  ->void
{
	try
	{
		while (true)
		{
			chatMenu();
			action_for_chat();
			system("cls");
			(this->*_state)();
		}
	}
	catch (CloseChat const& done)
	{
		system("cls");
		std::wcerr << "Chat closing.....\n\n";
	}

}


auto Chat::run()  ->void
{
	chatLoop();
}

