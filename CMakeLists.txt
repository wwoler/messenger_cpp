cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)

project(chat LANGUAGES CXX)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(external/json)

add_executable(executable
	src/main.cpp
	src/User.h 
	src/User.cpp
	src/Message.h 
	src/Message.cpp
	src/ConsoleManager.h 
	src/ConsoleManager.cpp
	src/DataBase.h 
	src/DataBase.cpp
	src/Chat.h
	src/Chat.cpp
	src/Singleton.h
)


target_link_libraries(executable nlohmann_json)




