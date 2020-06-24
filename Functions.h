#pragma once

#include "Settings.h"  // ��������� ����

// � ���� ����� ��������� ������ ������� ������� �� �������� � ������� ��������

// �����
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

// �������� �������
void SetColor(ConsoleColor text, ConsoleColor background);

// ������ ������
enum Button
{
	NOTHINK = 0,
	UP, RIGHT, DOWN, LEFT, UPLEFT, UPRIGHT, DOWNLEFT, DOWNRIGHT, ENTER, ESCAPE
};

// ������� �����
void ImputButton(Button& button);
