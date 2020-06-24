#include "Functions.h"

// Рисующая функция
void SetColor(ConsoleColor text, ConsoleColor background)
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

// Функция ввода
void ImputButton(Button& button)
{
	switch (_getch())
	{
	case 'w':
	case 72:
		button = UP;
		break;
	case 'a':
	case 75:
		button = LEFT;
		break;
	case 's':
	case 80:
		button = DOWN;
		break;
	case 'd':
	case 77:
		button = RIGHT;
		break;
	case static_cast<char>(13) :
		button = ENTER;
		break;
	case static_cast<char>(27) :
		button = ESCAPE;
		break;
	}
};


