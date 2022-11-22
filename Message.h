#pragma once
#include<iostream>
#include"User.h"

class Message{
public:
	Message() = delete;
	Message(const User & whom, const std::string & toWhom, const std::string message);
	
	~Message() = default;

private:
	std::shared_ptr<User> _fromWhom;
	std::string _toWhom;
	std::string _message;
};

