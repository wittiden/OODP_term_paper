//РАЗРАБОТКА АВТОМАТИЗИРОВАННОЙ СИСТЕМЫ ОЦЕНКИ НЕДВИЖИМОСТИ
//taskkill /F /IM term_paper.exe

#include <iostream>
#include <windows.h>
#include "main_menu.h"
#include "cursor_visibility.h"
#include "threads_options.h"

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	HANDLE consoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
	std::thread timer(inactivity_timer, 10);
	timer.detach();

	hideCursor();

	main_term_paper_menu();

	return 0;
} 