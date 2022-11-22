#pragma once
#include<iostream>

class User {
public:
	User() = delete;
	User(const User & other);
	User(const std::string & login, const std::string & pass, const std::string & nic);
	~User() = default;

	auto operator=(const User & right)		-> User &;
	auto getLogin() const					-> std::string;
	auto getPass() const					-> std::string;
	auto getNic() const						-> std::string;

	friend auto operator<<(std::ostream & output, const User & user) -> std::ostream &;
private:
	std::string _login;
	std::string _pass;
	std::string _nic;
};