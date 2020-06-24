#pragma once

#include "Settings.h"  // Настройки игры

// В этом файле находятся разные функции которые не связанны с шашками напрямую

// Цвета
enum ConsoleColor
{
	BLACK = 0,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	LIGHTGRAY,
	DARKGRAY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTCYAN,
	LIGHTRED,
	LIGHTMAGENTA,
	YELLOW,
	WHITE
};

// Рисующая функция
void SetColor(ConsoleColor text, ConsoleColor background);

// Кнопки выбора
enum Button
{
	NOTHINK = 0,
	UP, RIGHT, DOWN, LEFT, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT, ENTER, ESCAPE
};

// Функция ввода
void ImputButton(Button& button);
