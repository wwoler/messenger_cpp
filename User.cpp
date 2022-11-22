#pragma once
#include"User.h"

User::User(const std::string & login, const std::string & pass, const std::string & nic):_login(login), _pass(pass), _nic(nic) {}

User::User(const User & other){
	_login = other._login;
	_pass = other._pass;
	_nic = other._nic;
}

auto User::getLogin() const	-> std::string {
	return _login;
}

auto User::getPass() const	-> std::string {
	return _pass;
}

auto User::getNic() const	-> std::string {
	return _nic;
}

auto operator<<(std::ostream & output, const User & user) -> std::ostream & {
	output << "Login: " << user._login << " Password: " << user._pass << " Nicname: " << user._nic;
	return output;
}

auto User::operator=(const User & right) -> User & {
	_login = right._login;
	_pass = right._pass;
	_nic = right._nic;
}