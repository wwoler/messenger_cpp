cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)

project(chat LANGUAGES CXX)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(external/json)

add_executable(rez 
	main.cpp
	User.h 
	User.cpp
	Message.h 
	Message.cpp
	ConsoleManager.h 
	ConsoleManager.cpp
	DataBase.h 
	DataBase.cpp
	Chat.h
	Chat.cpp
	Singleton.h
)


target_link_libraries(rez nlohmann_json)




