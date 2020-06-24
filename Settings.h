#pragma once

// Все файлы которые нужны для работы программы подключаются тут
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <list>

// Настройки игры
namespace GameSettings
{
	// Величина поля и кол-во шашек
	const int MAXSIZE = 8,
		MAXTHINGS = 24;
}