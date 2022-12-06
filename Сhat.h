#pragma once
#include <Windows.h>
#include "DataBase.h"
#include "Singleton.h"
#include "User.h"
#include <string>
#include <iostream>
#include <regex>
#include <iomanip>



class Chat : public Singleton<Chat>
{
private:
	DataBase*             _DB;
	std::unique_ptr<User> _currentUser;
	void (Chat::*         _state)();
	bool                  _bUserStatus;
	bool                  _bChatStatus;


	auto chatMenu() const                                          ->void;
	auto userMenu() const                                          ->void;
	auto chatBox(std::wstring const&, int const& count) const      ->void;

	auto login()                                                   ->void;
	auto signUp()                                                  ->void;

	auto chatLoop()                                                ->void;
	auto userLoop()                                                ->void;
	
	auto action_for_chat()                                         ->void;
	auto action_for_user()                                         ->void;

	auto getMessages()                                             ->void;
	auto sendMessage()                                             ->void;

	auto clear_chat()                                              ->void;
	auto get_info()                                                ->void;
	
	auto set_user_data(std::wstring& data, std::wregex const& reg) ->bool;
	auto set_current_user(std::unique_ptr<User> user)              ->void;

	auto logout()                                                  ->void;
	auto exit()                                                    ->void;
	auto time_to_string(time_t& time)                              ->std::wstring;
protected:

	Chat() :
		_DB(nullptr), _currentUser(), _state(nullptr),
		_bChatStatus(false), _bUserStatus(false)
	{}

public:
	auto set_data_base(DataBase* db)                               ->void
	{
		_DB = db;
	}

	auto run()    -> void;

};