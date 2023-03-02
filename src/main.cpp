#include "DataBase.h"
#include "Chat.h"
#include <exception>


auto init_and_start()  -> void
{

#if defined(_WIN32) || defined(_WIN64)												 /*запрет на изменение размера окна cmd*/
	_setmode(_fileno(stdout), _O_U16TEXT);                        /*установка в UTF-16 формат ввода/вывода*/
	_setmode(_fileno(stdin), _O_U16TEXT);
	SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong
	(GetConsoleWindow(), GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
#endif 

	
	auto& data_base = DataBase::get_instance();
	auto& chat = Chat::get_instance();
	chat.set_data_base(&data_base);
	try
	{
		cls
		chat.run();
	}

	catch (std::exception const& ex)
	{
		cls
		std::wcerr << L"Error: " << ex.what() << std::endl;
		return;
	}
	catch (...)
	{
		cls
		std::wcerr << L"Error: Unknown error" << std::endl;
		return;
	}
}

auto main() -> int
{
	init_and_start();
	return EXIT_SUCCESS;
}