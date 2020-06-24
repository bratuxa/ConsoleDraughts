#pragma once

#include "Functions.h" // Здесь находятся функции цвета и ввода
using namespace std;

// В этом файле находятся классы

/* ---------------------------------------> Иеархия классов <-----------------------------------------------------

						| virtual class DrawedObject |    | class ActiveObject |
							|					|			   |			|
						| class Player |		 --------------			| class SelectorMenu |
							|							|
						| class AIPlayer |		| class Draught |
														|
												| class SuperDraught |		
*/

// Координаты x и y
struct Coords
{
	char x;
	int y;
};

// Класс Активный объект - общий класс для объектов которые могут быть активны
class ActiveObject
{
public:
	ActiveObject(const bool ISACTIVE = false) :isActive(ISACTIVE) {}

	// Включает активность объекта
	void becomeActive() { isActive = true; }
	// Выключает активность объекта
	void offActive() { isActive = false; }
	// Показывает активность объекта
	const bool& showActive() const { return isActive; }
private:
	bool isActive;	// Активен ли объект?
};

// Абстрактный(А) класс Нарисованный объект - общий класс всех объектов на поле
class DrawedObject
{
protected:
	// Текущая позиция
	Coords ofThis;
	// Цвет
	ConsoleColor color;

public:

	// Констуктор нарисованного объекта - X координата, Y координата, цвет объекта
	DrawedObject(const char X, const int Y, const ConsoleColor COLOR) : color(COLOR)
	{
		ofThis.x = X;
		ofThis.y = Y;
	}
	// Констуктор копирования нарисованного объекта
	const DrawedObject& operator =(const DrawedObject& COPY)
	{
		color = COPY.color;
		ofThis.x = COPY.showCoords().x;
		ofThis.y = COPY.showCoords().y;
		return *this;
	}

	// Метод отвечающий за движение 
	virtual	void move(const Button BUTTON = NOTHINK) = 0;
	// Метод рисующий объект на поле		
	virtual void drawThis() const = 0;

	// Возвращение координат
	const Coords& showCoords() const { return ofThis; }
	// Возвращение цвета 
	const ConsoleColor& showColor() const { return color; }
};

// Класс Шашки -> (А)Класс Нарисованый объект(DrawedObject)
//             -> Класс Активный объект(ActiveObject)
class Draught : public DrawedObject, public ActiveObject
{
protected:
	// Максимальная длина передвижения
	const int MAXMOVE;

public:
	// Констуктор шашки - X координата, Y координата, цвет шашки
	Draught(const char X, const int Y, const ConsoleColor COLOR, const int MAXMOVE = 1) : DrawedObject(X, Y, COLOR), MAXMOVE(MAXMOVE), ActiveObject() {}

	// Констуктор копирования шашки
	const Draught& operator =(const Draught& COPY)
	{
		if (showActive()) becomeActive();
		else offActive();
		color = COPY.color;
		ofThis.x = COPY.showCoords().x;
		ofThis.y = COPY.showCoords().y;
		return *this;
	}

	// Метод отвечающий за движение шашки
	void move(const Button BUTTON = NOTHINK)
	{
		switch (BUTTON)
		{
		case UPLEFT:
			ofThis.y--;
			ofThis.x--;
			break;
		case UPRIGHT:
			ofThis.y--;
			ofThis.x++;
			break;
		case DOWNLEFT:
			ofThis.y++;
			ofThis.x--;
			break;
		case DOWNRIGHT:
			ofThis.y++;
			ofThis.x++;
			break;
		}
	}
	// Метод рисующий шашку на поле		
	void drawThis() const
	{
		if (color == WHITE)
		{
			if (showActive())
			{
				cout << "| ";
				SetColor(WHITE, BLUE);
				cout << 'X';
				SetColor(WHITE, BLACK);
				cout << ' ';
			}
			else
			{
				cout << "| O ";
			}
		}
		else if (color == BLACK)
		{
			if (showActive())
			{
				cout << "| ";
				SetColor(RED, BLUE);
				cout << 'X';
				SetColor(WHITE, BLACK);
				cout << ' ';
			}
			else
			{
				cout << "| ";
				SetColor(RED, BLACK);
				cout << 'O';
				SetColor(WHITE, BLACK);
				cout << ' ';
			}
		}
	}
	const int showMaxMove() const { return MAXMOVE; }
};

// Класс Дамки -> Класс Шашки(Draught) -> (А)Класс Нарисованый объект(DrawedObject)
class SuperDraught : public Draught
{
public:
	// Констуктор дамки - X координата, Y координата, цвет дамки, максимальный диапозон движения
	SuperDraught(const char X, const int Y, const ConsoleColor COLOR, const int MAXMOVE) : Draught(X, Y, COLOR, MAXMOVE) {}

	// Метод рисующий дамку на поле		
	void drawThis() const
	{
		if (color == WHITE)
		{
			if (showActive())
			{
				cout << "| ";
				SetColor(WHITE, BLUE);
				cout << '$';
				SetColor(WHITE, BLACK);
				cout << ' ';
			}
			else
			{
				cout << "| 0 ";
			}
		}
		else if (color == BLACK)
		{
			if (showActive())
			{
				cout << "| ";
				SetColor(RED, BLUE);
				cout << '$';
				SetColor(WHITE, BLACK);
				cout << ' ';
			}
			else
			{
				cout << "| ";
				SetColor(RED, BLACK);
				cout << '0';
				SetColor(WHITE, BLACK);
				cout << ' ';
			}
		}
	}
};

// Класс Игрок -> (А)Класс Нарисованый объект(DrawedObject)
class Player : public DrawedObject
{
public:
	// Констуктор игрока - X координата, Y координата, цвет команды игрока
	Player(const char X, const int Y, const ConsoleColor COLOR) : DrawedObject(X, Y, COLOR) {}

	// Метод отвечающий за движение игрока
	void move(const Button BUTTON = NOTHINK)
	{
		switch (BUTTON)
		{
		case UP:
			ofThis.y--;
			break;
		case RIGHT:
			ofThis.x++;
			break;
		case DOWN:
			ofThis.y++;
			break;
		case LEFT:
			ofThis.x--;
			break;
		}
	}

	void move(const Coords COORDS)
	{
		ofThis.x = COORDS.x;
		ofThis.y = COORDS.y;
	}

	// Метод рисующий игрока на поле	
	void drawThis() const
	{
		cout << "| ";
		SetColor(WHITE, WHITE);
		cout << ' ';
		SetColor(WHITE, BLACK);
		cout << ' ';
	}
};

// Класс Игрок ИИ -> Класс Игрок(Player) -> (А)Класс Нарисованый объект(DrawedObject)
class AIPlayer : public Player
{
public:
	AIPlayer(const char X, const int Y, const ConsoleColor COLOR) : Player(X, Y, COLOR) {}
};

// Перечисление статусов меню
enum statusMenu
{
	MAINMENU, NEWGAME, LOADGAME, SAVEGAME, EXIT, AIMOD, TWOPLAYERMOD, FIRSTWHITE, FIRSTBLACK, PLAY
};

// Класс пункт в меню -> Класс Активный объект(ActiveObject)
class SelectorMenu : public ActiveObject
{
public:
	// Конструктор пункта меню - название пукта, статус пункта
	SelectorMenu(const string& NAMEOFSELECTOR, const statusMenu STATUS) : nameOfSelector(NAMEOFSELECTOR), status(STATUS) {}
	// Вывод названия меню
	void drawThis() const
	{
		if (showActive())
		{
			SetColor(WHITE, BLUE);
			cout << nameOfSelector;
			SetColor(WHITE, BLACK);
		}
		else cout << nameOfSelector;
	}
	// Длинна названия меню
	const int sizeOfSelectorName() const { return nameOfSelector.size(); }
	// Вывод статуса меню
	const statusMenu& showStatus() const { return status; }

private:
	string nameOfSelector;  // Название меню
	statusMenu status;		// Статус текущего пункта меню
};