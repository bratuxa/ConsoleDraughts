#pragma once

#include "Functions.h" // ����� ��������� ������� ����� � �����
using namespace std;

// � ���� ����� ��������� ������

/* ---------------------------------------> ������� ������� <-----------------------------------------------------

						| virtual class DrawedObject |    | class ActiveObject |
							|					|			   |			|
						| class Player |		 --------------			| class SelectorMenu |
							|							|
						| class AIPlayer |		| class Draught |
														|
												| class SuperDraught |		
*/

// ���������� x � y
struct Coords
{
	char x;
	int y;
};

// ����� �������� ������ - ����� ����� ��� �������� ������� ����� ���� �������
class ActiveObject
{
public:
	ActiveObject(const bool ISACTIVE = false) :isActive(ISACTIVE) {}

	// �������� ���������� �������
	void becomeActive() { isActive = true; }
	// ��������� ���������� �������
	void offActive() { isActive = false; }
	// ���������� ���������� �������
	const bool& showActive() const { return isActive; }
private:
	bool isActive;	// ������� �� ������?
};

// �����������(�) ����� ������������ ������ - ����� ����� ���� �������� �� ����
class DrawedObject
{
protected:
	// ������� �������
	Coords ofThis;
	// ����
	ConsoleColor color;

public:

	// ���������� ������������� ������� - X ����������, Y ����������, ���� �������
	DrawedObject(const char X, const int Y, const ConsoleColor COLOR) : color(COLOR)
	{
		ofThis.x = X;
		ofThis.y = Y;
	}
	// ���������� ����������� ������������� �������
	const DrawedObject& operator =(const DrawedObject& COPY)
	{
		color = COPY.color;
		ofThis.x = COPY.showCoords().x;
		ofThis.y = COPY.showCoords().y;
		return *this;
	}

	// ����� ���������� �� �������� 
	virtual	void move(const Button BUTTON = NOTHINK) = 0;
	// ����� �������� ������ �� ����		
	virtual void drawThis() const = 0;

	// ����������� ���������
	const Coords& showCoords() const { return ofThis; }
	// ����������� ����� 
	const ConsoleColor& showColor() const { return color; }
};

// ����� ����� -> (�)����� ����������� ������(DrawedObject)
//             -> ����� �������� ������(ActiveObject)
class Draught : public DrawedObject, public ActiveObject
{
protected:
	// ������������ ����� ������������
	const int MAXMOVE;

public:
	// ���������� ����� - X ����������, Y ����������, ���� �����
	Draught(const char X, const int Y, const ConsoleColor COLOR, const int MAXMOVE = 1) : DrawedObject(X, Y, COLOR), MAXMOVE(MAXMOVE), ActiveObject() {}

	// ���������� ����������� �����
	const Draught& operator =(const Draught& COPY)
	{
		if (showActive()) becomeActive();
		else offActive();
		color = COPY.color;
		ofThis.x = COPY.showCoords().x;
		ofThis.y = COPY.showCoords().y;
		return *this;
	}

	// ����� ���������� �� �������� �����
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
	// ����� �������� ����� �� ����		
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

// ����� ����� -> ����� �����(Draught) -> (�)����� ����������� ������(DrawedObject)
class SuperDraught : public Draught
{
public:
	// ���������� ����� - X ����������, Y ����������, ���� �����, ������������ �������� ��������
	SuperDraught(const char X, const int Y, const ConsoleColor COLOR, const int MAXMOVE) : Draught(X, Y, COLOR, MAXMOVE) {}

	// ����� �������� ����� �� ����		
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

// ����� ����� -> (�)����� ����������� ������(DrawedObject)
class Player : public DrawedObject
{
public:
	// ���������� ������ - X ����������, Y ����������, ���� ������� ������
	Player(const char X, const int Y, const ConsoleColor COLOR) : DrawedObject(X, Y, COLOR) {}

	// ����� ���������� �� �������� ������
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

	// ����� �������� ������ �� ����	
	void drawThis() const
	{
		cout << "| ";
		SetColor(WHITE, WHITE);
		cout << ' ';
		SetColor(WHITE, BLACK);
		cout << ' ';
	}
};

// ����� ����� �� -> ����� �����(Player) -> (�)����� ����������� ������(DrawedObject)
class AIPlayer : public Player
{
public:
	AIPlayer(const char X, const int Y, const ConsoleColor COLOR) : Player(X, Y, COLOR) {}
};

// ������������ �������� ����
enum statusMenu
{
	MAINMENU, NEWGAME, LOADGAME, SAVEGAME, EXIT, AIMOD, TWOPLAYERMOD, FIRSTWHITE, FIRSTBLACK, PLAY
};

// ����� ����� � ���� -> ����� �������� ������(ActiveObject)
class SelectorMenu : public ActiveObject
{
public:
	// ����������� ������ ���� - �������� �����, ������ ������
	SelectorMenu(const string& NAMEOFSELECTOR, const statusMenu STATUS) : nameOfSelector(NAMEOFSELECTOR), status(STATUS) {}
	// ����� �������� ����
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
	// ������ �������� ����
	const int sizeOfSelectorName() const { return nameOfSelector.size(); }
	// ����� ������� ����
	const statusMenu& showStatus() const { return status; }

private:
	string nameOfSelector;  // �������� ����
	statusMenu status;		// ������ �������� ������ ����
};