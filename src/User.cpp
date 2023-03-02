#include"User.h"

User::User(std::wstring const& login, std::wstring const& pass) :
	_login(login), _pass(pass) {}

auto User::getLogin()	                                    -> std::wstring&
{
	return _login;
}

auto User::getPass() 	                                    ->std::wstring&
{
	return _pass;
}

auto User::getUsername()	                                ->std::wstring&
{
	return _username;
}

auto User::setLogin(std::wstring const& login)              ->void
{
	_login.assign(login);
}
auto User::setPass(std::wstring const& pass)                ->void
{
	_pass.assign(pass);
}
auto User::setUsername(std::wstring const& username)        ->void
{
	_username.assign(username);
}