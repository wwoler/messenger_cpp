#pragma once
#include <fstream>
#include <string>
#include <memory>
#include "Singleton.h"

class Message;
class User;




class DataBase final : public Singleton<DataBase>
{
private:
	std::unique_ptr<std::ofstream> _fout;
	std::unique_ptr<std::ifstream> _fin;
	std::string                    _path;

	auto createDir(std::string const& dirName)                           -> void;

public:
	DataBase(std::string const&& path = "./Users/") :
		_fin(nullptr), _fout(nullptr), _path(path) {};

	DataBase(std::unique_ptr<std::ofstream>&& srcof,
		std::unique_ptr<std::ifstream>&& srcif,
		std::string const&& path = "./Users/") :
		_fout(std::move(srcof)), _fin(std::move(srcif)), _path(path) {}


	auto login(/*User const& userID*/std::string const& userID)         -> bool;

	auto signUp(/*User const& userID*/std::string const& userID)        -> bool;

	auto isExisting(std::string const& userID)                          -> bool;

	auto deleteUser()                                                   -> void;

	auto clearChat(std::string const& userID)                           -> void;

	auto getMessages(std::string const& userID)                         -> void;//std::unique_ptr<Message>;

	auto sendMessage(/*Message const& message,*/ std::string const& userID) -> void;


	~DataBase() = default;




};