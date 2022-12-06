#pragma once
#include <filesystem>
#include "Singleton.h"
#include "User.h"
#include "Message.h"
#include <fstream>
#include <string>
#include <memory>
#include <iostream>
#include <chrono>
#include <nlohmann/json.hpp>

class DataBase : public Singleton<DataBase>
{
private:
	friend class Chat;

	std::fstream                    _finout;
	std::wstring                    _path;
	auto to_json(User& user)        ->void;
	auto to_json(Message& mess)     ->void;
	auto from_json(User& user)      ->void;
	auto from_json(Message& mess)   ->void;

	auto login(User& userID)                                                 ->bool;

	auto signUp(User& userID)                                                ->bool;

	auto isExisting(User& userID)                                            ->bool;

	auto clearChat(std::wstring const& user1, std::wstring const& user2)     ->void;

	auto getMessages(std::wstring const& from, std::wstring const& to)       ->std::vector<Message>;

	auto sendMessage(Message& message)                                       ->void;

protected:
	DataBase(std::wstring const&& path = L"./base/");

public:

	~DataBase() = default;
};