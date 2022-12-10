#pragma once
#include<iostream>
#include"User.h"

class Message final {
public:
	Message() = default;
	Message(std::wstring const& sender, std::wstring const& receiver, std::wstring const& content, time_t&& time);
	Message(std::wstring const& sender, std::wstring const& content, time_t&& time);
	~Message() = default;

	auto getSender()   ->std::wstring&;
	auto getReceiver() ->std::wstring&;
	auto getContent()  ->std::wstring&;
	auto getTime()     ->time_t&;

	auto setSender(std::wstring const&)   ->void;
	auto setReceiver(std::wstring const&) ->void;
	auto setContent(std::wstring const&)  ->void;
	auto setTime(time_t const&)           ->void;


private:
	std::wstring        _sender;
	std::wstring        _receiver;
	std::wstring        _content;
	time_t              _time;

};

