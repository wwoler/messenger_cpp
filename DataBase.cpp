#include "DataBase.h"
#include <iostream>
#include <chrono>
#include<stdio.h>

#include <filesystem>

auto DataBase::login(/*User const& userID*/std::string const& userID)             -> bool
{
	_fout.reset(nullptr);
	_fin.reset(nullptr);

	_fout = std::make_unique<std::ofstream>(userID.c_str(), std::ios_base::app);
	_fin  = std::make_unique<std::ifstream>(userID.c_str(), std::ios_base::app);
	std::string username, pass;
	std::getline(*_fin, username, '\n');
	std::getline(*_fin, pass, '\n');


	std::string un, ps;
	std::cin >> un >> ps;

	if (!username.compare(un) && !pass.compare(ps))
		return 1;
	else return 0;

	//sendMessage(userID);

	//getMessages(userID);

	return _fin->is_open();
}


auto DataBase::signUp(/*User const& userID*/std::string const& userID)            -> bool
{
	_fout.reset(nullptr);
	_fin.reset(nullptr);

	if (isExisting(userID))
		return false;

	createDir(userID);


	*_fout << userID << '\n';
	*_fout << "111222\n";



	auto rez = _fout->is_open();

	_fout->close();


	return rez;
}


auto DataBase::isExisting(std::string const& userID)       -> bool
{
	std::fstream temp(userID.c_str());

	bool rez = temp.is_open();

	temp.close();

	return rez;
}



auto DataBase::deleteUser()          -> void
{
	remove("ww.txt");
}


auto DataBase::sendMessage(/*Message const& message,*/ std::string const& userID) -> void
{

	


	/*std::string to;
	std::cin >> to;

	_fout = std::make_unique<std::ofstream>(_path + "/" + )
	_fout->seekp(std::ios_base::end);*/


}


auto DataBase::getMessages(std::string const& userID)                         ->  void
{
	_fout->seekp(std::ios_base::end);
	_fin->seekg(std::ios_base::beg);

	std::string str;

	std::getline(*_fin, str);
	for (int i = 0; i < str.size(); ++i)
	{
		std::cout << str[i] << '\n';
	}

}


auto DataBase::createDir(std::string const& dirName) -> void
{
	std::filesystem::create_directories(_path + dirName);
	std::filesystem::create_directories(_path + dirName + "/Messages_List");
	_fout = std::make_unique<std::ofstream>(_path + dirName + "/" + dirName + ".txt", std::ios_base::app);
}