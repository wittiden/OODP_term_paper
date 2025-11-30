//РАЗРАБОТКА АВТОМАТИЗИРОВАННОЙ СИСТЕМЫ ОЦЕНКИ НЕДВИЖИМОСТИ
//taskkill /F /IM term_paper.exe

#include <iostream>
#include <windows.h>

#include "main_menu.h"
#include "cursor_visibility.h"

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	hideCursor();

	main_term_paper_menu();

	return 0;
}