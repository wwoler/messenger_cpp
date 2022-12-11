#include "DataBase.h"
#include "Сhat.h"
#include <io.h>
#include <fcntl.h>
#include <exception>

HANDLE h;

auto init_and_start()  -> void
{
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	HWND consoleWindow = GetConsoleWindow();
	MoveWindow(GetConsoleWindow(), 50, 50, 1100, 700, true);
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong
	(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX); //запрет на изменение размера окна cmd
	_setmode(_fileno(stdout), _O_U16TEXT);                       // установка в UTF-16 формат ввода/вывода
	_setmode(_fileno(stdin), _O_U16TEXT);
	auto& data_base = DataBase::get_instance();
	auto& chat = Chat::get_instance();
	chat.set_data_base(&data_base);

	try
	{
		chat.run();
	}
	catch (std::exception const& ex)
	{
		system("cls");
		std::wcerr << L"Error: " << ex.what() << std::endl;
		return;
	}
	catch (...)
	{
		system("cls");
		std::wcerr << L"Error: Unknown error" << std::endl;
		return;
	}
}

auto main() -> int
{
	init_and_start();
	system("pause");
	return EXIT_SUCCESS;
}