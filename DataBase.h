#pragma once
#include <fstream>
#include <string>
#include <memory>


class Message;
class User;

class Erase
{
public:
	Erase() = default;
	Erase(Erase const&) = delete;
	Erase& operator=(Erase const&) = delete;
	Erase(Erase const&&) = delete;
	Erase& operator=(Erase const&&) = delete;

	virtual ~Erase() = default;
};


template <class T>
class Singleton : private Erase
{
public:
	static T& get_instance()
	{
		static T instance;
		return instance;
	}

protected:
	Singleton() = default;
};



class DataBase : public Singleton<DataBase>
{
private:
	std::unique_ptr<std::fstream> _finout;

public:
	DataBase() :
		_finout(nullptr) {};

	DataBase(std::unique_ptr<std::fstream>&& src) :
		_finout(std::move(src)) {}


	auto login(User const& userID)                                      -> bool;

	auto signUp(User const& UserID)                                     -> bool;

	auto isExisting(std::string const& userID)                          -> bool;

	auto deleteUser(std::string const& userID)                          -> void;

	auto clearChar(std::string const& userID)                           -> void;

	auto getMessages(std::string const& userID)                         -> Message const* const;

	auto sendMessage(Message const& message, std::string const& userID) -> void;


	~DataBase() = default;




};