#include "DataBase.h"
#include <io.h>
#include <fcntl.h>

using json = nlohmann::json;

DataBase::DataBase(std::wstring const&& path) :
	_finout(), _path(path)
{
	std::filesystem::create_directories(_path);
	std::filesystem::create_directories(_path + L"/Messages_List");
	_finout.open(_path + L"./Messages_List/common_chat.json", std::ios::app);
	_finout.close();

	_finout.open(_path + L"users.json", std::ios::app);
	_finout.close();
}

auto DataBase::to_json(User& user)                      -> void
{
	json js;
	js["Login"]    = user.getLogin();
	js["Password"] = user.getPass();
	js["Username"] = user.getUsername();
	_finout << js;
}

auto DataBase::to_json(Message& mess)                   -> void
{
	json js;
	js["Content"]  = mess.getContent();
	js["Sender"]   = mess.getSender();
	js["Time"]     = mess.getTime();
	_finout << js;
}

auto DataBase::from_json(User& user)                    -> void
{
	json js;
	_finout >> js;
	
	user.setLogin(js["Login"]);
	user.setPass(js["Password"]);
	user.setUsername(js["Username"]);
}

auto DataBase::from_json(Message& mess)                 -> void
{
	json js;
	_finout >> js;

	mess.setSender(js["Sender"]);
	mess.setContent(js["Content"]);
	mess.setTime(js["Time"]);
}

auto DataBase::login(User& userID)                      -> bool
{
	std::wstring buff = userID.getPass();

	if (!isExisting(userID) || userID.getPass().compare(buff))
	{
		_finout.close();
		return false;
	}
	_finout.close();
	return true;
}


auto DataBase::signUp(User& userID)                    -> bool
{
	if (isExisting(userID))
	{
		return false;
	}

	_finout.clear();
	_finout.setf(std::ios::out | std::ios::app);

	to_json(userID);

	_finout.close();

	return true;
}


auto DataBase::isExisting(User& userID)               -> bool
{
	_finout.open(_path + L"users.json", std::ios::in | std::ios::app);
	
	_finout.get();

	User buff;

	while (_finout.good())
	{
		_finout.unget();

		from_json(buff);
		if (!buff.getUsername().compare(userID.getUsername()) || !buff.getLogin().compare(userID.getLogin()))
		{
			userID.setUsername(buff.getUsername());
			userID.setPass(buff.getPass());
			return true;
		}
		_finout.get();
	}
	return false;
}


auto DataBase::clearChat(std::wstring const& user1, std::wstring const& user2)                               ->void
{
	std::wstring dialog;
	user1 > user2 ?
		dialog.assign(user1 + user2) :
		dialog.assign(user2 + user1);
	_finout.close();
	_finout.open(_path + L"Messages_List/" + dialog + L".json", std::ios::out | std::ios::trunc );
}

 


auto DataBase::sendMessage(Message& message) -> void
{
	_finout.close();

	if (!message.getReceiver().compare(L"common_chat"))
	{
		_finout.open(_path + L"Messages_List/common_chat.json", std::ios::app | std::ios::out);
	}

	else
	{
		std::wstring dialog;
		message.getReceiver() > message.getSender() ?
			dialog.assign(message.getReceiver() + message.getSender()) :
			dialog.assign(message.getSender() + message.getReceiver());

		_finout.open(_path + L"Messages_List/" + dialog + L".json", std::ios::app);
	}

	to_json(message);

	_finout.close();
}


auto DataBase::getMessages(std::wstring const& from, std::wstring const& to)   ->  std::vector<Message>
{
	_finout.close();

	if (!from.compare(L"common_chat"))
	{
		_finout.open(_path + L"Messages_List/common_chat.json", std::ios::app | std::ios::in);
	}
	else
	{
		_finout.close();
		std::wstring dialog;
		to > from ?
			dialog.assign(to + from) :
			dialog.assign(from + to);

		_finout.open(_path + L"Messages_List/" + dialog + L".json", std::ios::app | std::ios::in);
	}

	_finout.get();

	std::vector<Message> messages;
	messages.reserve(25);
	Message buff;

	while (_finout.good())
	{
		_finout.unget();

		from_json(buff);
		messages.push_back(buff);

		_finout.get();
	}
	_finout.close();
	return messages;

}
