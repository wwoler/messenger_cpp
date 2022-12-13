#include "DataBase.h"

using json = nlohmann::json;

DataBase::DataBase(std::wstring const&& path) :
	_path(path)
{
	std::filesystem::create_directories(_path);
	std::filesystem::create_directories(_path + L"/Messages_List");

	std::ofstream test(_path + L"users.json", std::ios::app);
	test.close();

	_messageStream.open(_path + L"./Messages_List/common_chat.json", std::ios::in | std::ios::out | std::ios::app);
	_userStream.open(_path + L"users.json", std::ios::in | std::ios::out | std::ios::ate);

	_messageStream.close();
}

auto DataBase::adjustment()  ->void
{
	auto pos = _userStream.tellp();
	_userStream.seekp(pos);           //טח פאיכא
	int count = 0;
	char ch = _userStream.get();
	while (ch != '\n')
	{
		count++;
		ch = _userStream.get();
	}

	_userStream.seekg(pos);          // ג פאיכ
	for (int i = 0; i < count; ++i)
	{
		_userStream.put(' ');
	}
}

auto DataBase::to_json(User& user)  ->void
{
	json js;
	js["Login"]    = user.getLogin();
	js["Password"] = user.getPass();
	js["Username"] = user.getUsername();
	_userStream << js;
}


auto DataBase::to_json(Message& mess)  ->void
{
	json js;
	js["Content"]  = mess.getContent();
	js["Sender"]   = mess.getSender();
	js["Time"]     = mess.getTime();
	_messageStream << js;
}

auto DataBase::from_json(User& user)  ->void
{
	json js;
	_userStream >> js;
	
	user.setLogin(js["Login"]);
	user.setPass(js["Password"]);
	user.setUsername(js["Username"]);
}

auto DataBase::from_json(Message& mess)  ->void
{
	json js;
	_messageStream >> js;

	mess.setSender(js["Sender"]);
	mess.setContent(js["Content"]);
	mess.setTime(js["Time"]);
}

auto DataBase::login(User& userID)  ->bool
{
	std::wstring buff = userID.getPass();

	if (!isExisting(userID) || userID.getPass().compare(buff))
	{
		return false;
	}
	return true;
}


auto DataBase::signUp(User& userID)  ->bool
{
	if (isExisting(userID))
	{
		return false;
	}

	_userStream.seekp(0u, std::ios::end);

	/*219 - לאךסטלאכüםûי נאחלונ הכÿ מבתוךעא ג פאיכו*/ 
	_userStream << std::setw(219) << '\n';

	_userStream.seekp(-220, std::ios::cur);

	to_json(userID);

	return true;
}


auto DataBase::isExisting(User& userID)  ->bool
{
	_userStream.seekg(0u, std::ios::beg);
	_userStream.seekp(0u, std::ios::beg);
	
	_userStream.get();

	User temp(userID);

	User buff;
	while (_userStream.good())
	{
		_userStream.unget();

		_currentUserPos = _userStream.tellp();

		from_json(buff);


		if (!buff.getUsername().compare(temp.getUsername()) || !buff.getLogin().compare(temp.getLogin()))
		{

			userID.setUsername(buff.getUsername());
			userID.setPass(buff.getPass());
			return true;
		}

		while (_userStream.get() != '\n');
		_userStream.get();
	}

	_userStream.clear();

	return false;
}

auto DataBase::clearChat(std::wstring const& user1, std::wstring const& user2)  ->void
{
	std::wstring dialog;
	user1 > user2 ?
		dialog.assign(user1 + user2) :
		dialog.assign(user2 + user1);

	_messageStream.open(_path + L"Messages_List/" + dialog + L".json", std::ios::out | std::ios::trunc);
	_messageStream.close();
}


auto DataBase::changePassword(User& newData)  ->void
{
	_userStream.seekp(_currentUserPos);

	to_json(newData);

	adjustment();
}

auto DataBase::changeLogin(User& newData)  ->void
{
	_userStream.seekp(_currentUserPos);

	to_json(newData);

	adjustment();
}

auto DataBase::sendMessage(Message& message)  ->void
{
	if (!message.getReceiver().compare(L"common_chat"))
	{
		_messageStream.open(_path + L"Messages_List/common_chat.json", std::ios::app | std::ios::out);
	}

	else
	{
		std::wstring dialog;
		message.getReceiver() > message.getSender() ?
			dialog.assign(message.getReceiver() + message.getSender()) :
			dialog.assign(message.getSender() + message.getReceiver());

		_messageStream.open(_path + L"Messages_List/" + dialog + L".json", std::ios::app | std::ios::out);
	}

	to_json(message);

	_messageStream.close();
}


auto DataBase::getMessages(std::wstring const& from, std::wstring const& to)  ->std::vector<Message>
{

	if (!from.compare(L"common_chat"))
	{
		_messageStream.open(_path + L"Messages_List/common_chat.json", std::ios::app | std::ios::in);
	}
	else
	{
		std::wstring dialog;
		to > from ?
			dialog.assign(to + from) :
			dialog.assign(from + to);

		_messageStream.open(_path + L"Messages_List/" + dialog + L".json", std::ios::app | std::ios::in);
	}

	_messageStream.get();

	std::vector<Message> messages;
	messages.reserve(20);
	Message buff;

	while (_messageStream.good())
	{
		_messageStream.unget();

		from_json(buff);
		messages.push_back(buff);

		_messageStream.get();
	}

	_messageStream.close();
	return messages;

}
