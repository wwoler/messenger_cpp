#pragma once
#include <iostream>

class User final {
public:
	User() = default;
	User(const std::wstring& login, const std::wstring& pass);
	User(std::wstring const& login,std::wstring const& pass,
		std::wstring const& username)
		:_login(login), _pass(pass), _username(username)
	{}
	User(std::wstring const& username):
		_username(username)
	{}
	~User() = default;

	auto getLogin()				                   ->std::wstring&;
	auto getPass()       			               ->std::wstring&;
	auto getUsername() 			                   ->std::wstring&;

	auto setLogin(std::wstring const&)             ->void;
	auto setPass(std::wstring const&)              ->void;
	auto setUsername(std::wstring const&)          ->void;

private:
	std::wstring _login;
	std::wstring _pass;
	std::wstring _username;
};