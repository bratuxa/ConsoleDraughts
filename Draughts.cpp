
// Структура подключения файлов Draughts.cpp(Этот файл) -> Classes.h -> Functions.h -> Settings.h -> Все остальные подлючаймые файлы

#include "Classes.h"  // В этом файле ищите класс Draught отвечащий за шашки

using namespace std;
using namespace GameSettings; // Настройки величины поля

// Прототипы функций ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------->

const bool& showMultiAttackfMod();	 // Функция показывает в каком состоянии переменная multiAttackMod отвечающая за мультиатаку
void offMultiAttackfMod();			 // Функция выключает мультиатаку
void onMultiAttackfMod();			 // Функция включает мультиатаку

const bool& showEndOfMove();		 // Функция показывает в каком состоянии переменная endOfMove отвечающая за конец хода
void offEndOfMove();				 // Функция отключает конец хода
void onEndOfMove();					 // Функция включает конец хода

void createActiveZone(const Button ROUTE, const int X, const int Y, const int MULTIPLICATOR);	// Создает активную зону
void clearActiveZone();																			// Функция очистки активных зон

void createDraughts(const int Y, const ConsoleColor COLOR);										// Создает шашку и помещает в вектор
void newGame(const bool GAMEMOD, const ConsoleColor CHOICESIDE);								// Создание массива шашек и игроков
void endGame();																					// Удаление массива шашек и игроков

void multiAttackModCheck(Player* const movingPlayer);																																	// Проверяет будет ли работать мультиатака
void logicMoveAndAttack(const Button ROUTE, Player* const movingPlayer);																												// Отвечает за логику атаки или передвижения														
void logicActiveZone(const int X, const int Y, const int MULTIMOVE, const ConsoleColor againsThinkColor, bool& URBorderEnd, bool& ULBorderEnd, bool& DRBorderEnd, bool& DLBorderEnd);	// Функция отвечает за логику активных зон 
void logic(Player* const movingPlayer);																																					// Отвечает за логику всего игрового процесса

void drawEmptyString();																		 // Рисует пустую строку на поле
void drawHelp(const char* COLORWORD, const char* NORMAL, const ConsoleColor COLOR);			 // Функция создает шаблон для подсказки
void draw(Player* const movingPlayer, const bool isGameEnded);								 // Рисует игровое поле

void gameProcess(const bool GAMEMOD, const ConsoleColor CHOICESIDE);	// Функция упорядочивает другие для создания игрового процесса

void logicMenu();								// Функция отвечает за логику меню игры
void drawMenu(vector<SelectorMenu*>& LVLS);		// Функция рисует меню игры

// Контейннеры -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------->
namespace
{
	vector<Draught*> gameThinks;				// Вектор указателей на шашки
	vector<Draught*> rememberGameThinks;		// Вектор указателей на запомненые шашки

	Player* Players[2];							// Пара указателей на игроков

	vector<Coords*> activeZone;					// Вектор активных зон
}

// Реализация функций -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------->

// Функция показывает в каком состоянии переменная multiAttackMod отвечающая за мультиатаку
const bool& showMultiAttackfMod()
{
	static bool multiAttackMod = false;
	return multiAttackMod;
}

// Функция выключает мультиатаку
void offMultiAttackfMod()
{
	const_cast<bool&>(static_cast<const bool&>(showMultiAttackfMod())) = false;
}

// Функция включает мультиатаку
void onMultiAttackfMod()
{
	const_cast<bool&>(static_cast<const bool&>(showMultiAttackfMod())) = true;
}

// Функция показывает в каком состоянии переменная endOfMove отвечающая за конец хода
const bool& showEndOfMove()
{
	static bool endOfMove = false;
	return endOfMove;
}

// Функция отключает конец хода
void offEndOfMove()
{
	const_cast<bool&>(static_cast<const bool&>(showEndOfMove())) = false;
}

// Функция включает конец хода
void onEndOfMove()
{
	const_cast<bool&>(static_cast<const bool&>(showEndOfMove())) = true;
}

// Создает активную зону
void createActiveZone(const Button ROUTE, const int X, const int Y, const int MULTIPLICATOR)
{
	Coords* temp = new Coords;
	switch (ROUTE)
	{
	case UPLEFT:
		temp->x = X - MULTIPLICATOR;
		temp->y = Y - MULTIPLICATOR;
		break;
	case UPRIGHT:
		temp->x = X + MULTIPLICATOR;
		temp->y = Y - MULTIPLICATOR;
		break;
	case DOWNLEFT:
		temp->x = X - MULTIPLICATOR;
		temp->y = Y + MULTIPLICATOR;
		break;
	case DOWNRIGHT:
		temp->x = X + MULTIPLICATOR;
		temp->y = Y + MULTIPLICATOR;
		break;
	}
	activeZone.push_back(temp);
}

// Функция очистки активных зон
void clearActiveZone()
{
	for (int i = 0; i < MAXTHINGS; i++) gameThinks[i]->offActive();

	while (activeZone.size())
	{
		Coords* temp = activeZone.back();
		activeZone.pop_back();
		delete temp;
	}
}

// Создает шашку и помещает в вектор
void createDraughts(const int Y, const ConsoleColor COLOR)
{
	for (int x = 1; x <= MAXSIZE; x++)
	{
		if (Y % 2 == 0 && x % 2 != 0 || Y % 2 != 0 && x % 2 == 0)
		{
			Draught* temp = new Draught(static_cast<char>(x + 64), Y, COLOR);
			gameThinks.push_back(temp);
		}
	}
}

// Создание массива шашек и игроков
void newGame(const bool GAMEMOD, const ConsoleColor CHOICESIDE)
{
	for (int y = 1; y <= (MAXSIZE - 2) / 2; y++) createDraughts(y, BLACK);

	for (int y = MAXSIZE / 2 + 2; y <= MAXSIZE; y++) createDraughts(y, WHITE);

	if (GAMEMOD)
	{
		if (CHOICESIDE == WHITE)
		{
			Players[0] = new Player(static_cast<char>(MAXSIZE + 64), MAXSIZE, WHITE);
			Players[1] = new AIPlayer(static_cast<char>(1 + 64), 1, BLACK);
		}
		else
		{
			Players[0] = new AIPlayer(static_cast<char>(MAXSIZE + 64), MAXSIZE, WHITE);
			Players[1] = new Player(static_cast<char>(1 + 64), 1, BLACK);
		}
	}
	else
	{
		Players[0] = new Player(static_cast<char>(MAXSIZE + 64), MAXSIZE, WHITE);
		Players[1] = new Player(static_cast<char>(1 + 64), 1, BLACK);
	}
};

// Удаление массива шашек и игроков
void endGame()
{
	while (gameThinks.size())
	{
		delete gameThinks.back();
		gameThinks.pop_back();
	}

	delete Players[0];
	delete Players[1];
}

// Проверяет будет ли работать мультиатака
void multiAttackModCheck(Player* const movingPlayer)
{
	offMultiAttackfMod();

	// Проверяем есть ли враждебная шашка вокруг союзной шашки...
	for (int c = 0; c < MAXTHINGS && !showMultiAttackfMod(); c++)
	{
		if (movingPlayer->showCoords().x + 1 == gameThinks[c]->showCoords().x && movingPlayer->showCoords().y + 1 == gameThinks[c]->showCoords().y && movingPlayer->showColor() != gameThinks[c]->showColor() && gameThinks[c]->showCoords().x - 64 != MAXSIZE && gameThinks[c]->showCoords().x - 64 != 1 && gameThinks[c]->showCoords().y != MAXSIZE && gameThinks[c]->showCoords().y != 1)
		{
			for (int d = 0; d < MAXTHINGS; d++)
			{
				//...а так же проверяем есть ли за враждебной шашкой еще одна любая, если да то повторить атаку нельзя, иначе включаем эту возможность
				if (movingPlayer->showCoords().x + 2 == gameThinks[d]->showCoords().x && movingPlayer->showCoords().y + 2 == gameThinks[d]->showCoords().y)
				{
					offMultiAttackfMod();
					break;
				}
				else onMultiAttackfMod();
			}
		}
		else if (movingPlayer->showCoords().x - 1 == gameThinks[c]->showCoords().x && movingPlayer->showCoords().y + 1 == gameThinks[c]->showCoords().y && movingPlayer->showColor() != gameThinks[c]->showColor() && !showMultiAttackfMod() && gameThinks[c]->showCoords().x - 64 != MAXSIZE && gameThinks[c]->showCoords().x - 64 != 1 && gameThinks[c]->showCoords().y != MAXSIZE && gameThinks[c]->showCoords().y != 1)
		{
			for (int d = 0; d < MAXTHINGS; d++)
			{
				//...а так же проверяем есть ли за враждебной шашкой еще одна любая, если да то повторить атаку нельзя, иначе включаем эту возможность
				if (movingPlayer->showCoords().x - 2 == gameThinks[d]->showCoords().x && movingPlayer->showCoords().y + 2 == gameThinks[d]->showCoords().y)
				{
					offMultiAttackfMod();
					break;
				}
				else onMultiAttackfMod();
			}
		}
		else if (movingPlayer->showCoords().x + 1 == gameThinks[c]->showCoords().x && movingPlayer->showCoords().y - 1 == gameThinks[c]->showCoords().y && movingPlayer->showColor() != gameThinks[c]->showColor() && !showMultiAttackfMod() && gameThinks[c]->showCoords().x - 64 != MAXSIZE && gameThinks[c]->showCoords().x - 64 != 1 && gameThinks[c]->showCoords().y != MAXSIZE && gameThinks[c]->showCoords().y != 1)
		{
			for (int d = 0; d < MAXTHINGS; d++)
			{
				//...а так же проверяем есть ли за враждебной шашкой еще одна любая, если да то повторить атаку нельзя, иначе включаем эту возможность
				if (movingPlayer->showCoords().x + 2 == gameThinks[d]->showCoords().x && movingPlayer->showCoords().y - 2 == gameThinks[d]->showCoords().y)
				{
					offMultiAttackfMod();
					break;
				}
				else onMultiAttackfMod();
			}
		}
		else if (movingPlayer->showCoords().x - 1 == gameThinks[c]->showCoords().x && movingPlayer->showCoords().y - 1 == gameThinks[c]->showCoords().y && movingPlayer->showColor() != gameThinks[c]->showColor() && !showMultiAttackfMod() && gameThinks[c]->showCoords().x - 64 != MAXSIZE && gameThinks[c]->showCoords().x - 64 != 1 && gameThinks[c]->showCoords().y != MAXSIZE && gameThinks[c]->showCoords().y != 1)
		{
			for (int d = 0; d < MAXTHINGS; d++)
			{
				//...а так же проверяем есть ли за враждебной шашкой еще одна любая, если да то повторить атаку нельзя, иначе включаем эту возможность
				if (movingPlayer->showCoords().x - 2 == gameThinks[d]->showCoords().x && movingPlayer->showCoords().y - 2 == gameThinks[d]->showCoords().y)
				{
					offMultiAttackfMod();
					break;
				}
				else onMultiAttackfMod();
			}
		}
	}
}

// Отвечает за логику атаки или передвижения
void logicMoveAndAttack(const Button ROUTE, Player* const movingPlayer)
{
	for (int b = 1; b < rememberGameThinks.size(); b++)
	{
		if (rememberGameThinks[b]->showCoords().x == rememberGameThinks[0]->showCoords().x && rememberGameThinks[b]->showCoords().y == rememberGameThinks[0]->showCoords().y)
		{
			*rememberGameThinks[b] = Draught('@', 99, YELLOW); // Желтый объект - пустой объект имитирующий уничтоженную шашку
			multiAttackModCheck(movingPlayer);
			break;
		}
	}
	rememberGameThinks[0]->move(ROUTE);

	onEndOfMove();

	// Если запомниная шашка находится в самой верхней или нижней части поля то она становится дамкой
	if ((rememberGameThinks[0]->showCoords().y == 1 && rememberGameThinks[0]->showColor() == WHITE) || (rememberGameThinks[0]->showCoords().y == 8 && rememberGameThinks[0]->showColor() == BLACK) && rememberGameThinks[0]->showCoords().x >= 1 + 64 && rememberGameThinks[0]->showCoords().x <= MAXSIZE + 64 && typeid(*gameThinks[0]).name() != typeid(SuperDraught).name())
	{
		for (int i = 0; i < MAXTHINGS; i++)
		{
			if (gameThinks[i]->showCoords().x == rememberGameThinks[0]->showCoords().x && gameThinks[i]->showCoords().y == rememberGameThinks[0]->showCoords().y)
			{
				SuperDraught* temp = new SuperDraught(rememberGameThinks[0]->showCoords().x, rememberGameThinks[0]->showCoords().y, rememberGameThinks[0]->showColor(), MAXSIZE);
				delete gameThinks[i];
				rememberGameThinks[0] = gameThinks[i] = temp;
			}
		}
	}
}

// Функция отвечает за логику активных зон
void logicActiveZone(const int X, const int Y, const int MULTIMOVE, const ConsoleColor againsThinkColor, bool& URBorderEnd, bool& ULBorderEnd, bool& DRBorderEnd, bool& DLBorderEnd)
{

	// Проверяет находится ли шашка возле границы
	if (Y == 8 || X == 8 + 64) DRBorderEnd = false;
	if (Y == 1 || X == 1 + 64) ULBorderEnd = false;
	if (Y == 1 || X == 8 + 64) URBorderEnd = false;
	if (Y == 8 || X == 1 + 64) DLBorderEnd = false;

	for (int b = 0; b < MAXTHINGS; b++)
	{
		if (gameThinks[b]->showCoords().y == Y - MULTIMOVE && gameThinks[b]->showCoords().x == X + MULTIMOVE && URBorderEnd) // Если на пути шашки есть другая шашка...
		{
			if (gameThinks[b]->showColor() == againsThinkColor) // ...и она другого цвета 
			{
				for (int c = 0; c < MAXTHINGS; c++) // ...то ищем есть ли за ней на пути другая шашка любого цвета
				{
					if (gameThinks[c]->showCoords().y == Y - MULTIMOVE - 1 && gameThinks[c]->showCoords().x == X + MULTIMOVE + 1)
					{
						URBorderEnd = false;
						break;
					}
				}
				// Если за ней нет никакой шашки то делаем шашку активной, запоминаем её и делаем зону за ней активной
				if (URBorderEnd && gameThinks[b]->showCoords().y != MAXSIZE && gameThinks[b]->showCoords().x != MAXSIZE + 64 && gameThinks[b]->showCoords().y != 1 && gameThinks[b]->showCoords().x != 1 + 64)
				{
					gameThinks[b]->becomeActive();
					rememberGameThinks.push_back(gameThinks[b]);
					createActiveZone(UPRIGHT, X, Y, MULTIMOVE + 1);
				}
			}
			URBorderEnd = false;
		}

		if (gameThinks[b]->showCoords().y == Y - MULTIMOVE && gameThinks[b]->showCoords().x == X - MULTIMOVE && ULBorderEnd) // Если на пути шашки есть другая шашка...
		{
			if (gameThinks[b]->showColor() == againsThinkColor) //...и она другого цвета
			{
				for (int c = 0; c < MAXTHINGS; c++) // ...то ищем есть ли за ней на пути другая шашка любого цвета
				{
					if (gameThinks[c]->showCoords().y == Y - MULTIMOVE - 1 && gameThinks[c]->showCoords().x == X - MULTIMOVE - 1)
					{
						ULBorderEnd = false;
						break;
					}
				}
				// Если за ней нет никакой шашки то делаем шашку активной, запоминаем её и делаем зону за ней активной
				if (ULBorderEnd && gameThinks[b]->showCoords().y != MAXSIZE && gameThinks[b]->showCoords().x != MAXSIZE + 64 && gameThinks[b]->showCoords().y != 1 && gameThinks[b]->showCoords().x != 1 + 64)
				{
					gameThinks[b]->becomeActive();
					rememberGameThinks.push_back(gameThinks[b]);
					createActiveZone(UPLEFT, X, Y, MULTIMOVE + 1);
				}
			}
			ULBorderEnd = false;
		}

		if (gameThinks[b]->showCoords().y == Y + MULTIMOVE && gameThinks[b]->showCoords().x == X + MULTIMOVE && DRBorderEnd) // Если на пути шашки есть другая шашка...
		{
			if (gameThinks[b]->showColor() == againsThinkColor) //...и она другого цвета
			{
				for (int c = 0; c < MAXTHINGS; c++) // ...то ищем есть ли за ней на пути другая шашка любого цвета
				{
					if (gameThinks[c]->showCoords().y == Y + MULTIMOVE + 1 && gameThinks[c]->showCoords().x == X + MULTIMOVE + 1)
					{
						DRBorderEnd = false;
						break;
					}
				}
				// Если за ней нет никакой шашки то делаем шашку активной, запоминаем её и делаем зону за ней активной
				if (DRBorderEnd && gameThinks[b]->showCoords().y != MAXSIZE && gameThinks[b]->showCoords().x != MAXSIZE + 64 && gameThinks[b]->showCoords().y != 1 && gameThinks[b]->showCoords().x != 1 + 64)
				{
					gameThinks[b]->becomeActive();
					rememberGameThinks.push_back(gameThinks[b]);
					createActiveZone(DOWNRIGHT, X, Y, MULTIMOVE + 1);
				}
			}
			DRBorderEnd = false;
		}

		if (gameThinks[b]->showCoords().y == Y + MULTIMOVE && gameThinks[b]->showCoords().x == X - MULTIMOVE && DLBorderEnd) // Если на пути шашки есть другая шашка...
		{

			if (gameThinks[b]->showColor() == againsThinkColor) //...и она другого цвета
			{
				for (int c = 0; c < MAXTHINGS; c++) // ...то ищем есть ли за ней на пути другая шашка любого цвета
				{
					if (gameThinks[c]->showCoords().y == Y + MULTIMOVE + 1 && gameThinks[c]->showCoords().x == X - MULTIMOVE - 1)
					{
						DLBorderEnd = false;
						break;
					}
				}
				// Если за ней нет никакой шашки то делаем шашку активной, запоминаем её и делаем зону за ней активной
				if (DLBorderEnd && gameThinks[b]->showCoords().y != MAXSIZE && gameThinks[b]->showCoords().x != MAXSIZE + 64 && gameThinks[b]->showCoords().y != 1 && gameThinks[b]->showCoords().x != 1 + 64)
				{
					gameThinks[b]->becomeActive();
					rememberGameThinks.push_back(gameThinks[b]);
					createActiveZone(DOWNLEFT, X, Y, MULTIMOVE + 1);
				}
			}
			DLBorderEnd = false;
		}
	}
}

// Отвечает за логику всего игрового процесса
void logic(Player* const movingPlayer)
{
	// Если ходит человек то ...
	if (typeid(*movingPlayer).name() == typeid(Player).name())
	{
		// Если есть возможность множественной атаки то производим проверку активных зон другим способом
		if (showMultiAttackfMod())
		{
			while (rememberGameThinks.size() > 1)
				rememberGameThinks.pop_back();

			clearActiveZone();
			// Делаем шашку активной и запоминаем её
			rememberGameThinks[0]->becomeActive();

			/* Если активная зона достигла препятствия то **BorderEnd становится false,
				что не дает создавать новые активные зоны в конкретном направлении */
			bool URBorderEnd = true /*Сверху-Справа*/, ULBorderEnd = true /*Сверху-Слева*/, DRBorderEnd = true /*Снизу-Справа*/, DLBorderEnd = true /*Снизу-Слева*/;

			for (int y = rememberGameThinks[0]->showCoords().y, x = rememberGameThinks[0]->showCoords().x, multiMove = 1; multiMove <= rememberGameThinks[0]->showMaxMove(); multiMove++)
			{
				if (rememberGameThinks[0]->showColor() == WHITE) logicActiveZone(x, y, multiMove, BLACK, URBorderEnd, ULBorderEnd, DRBorderEnd, DLBorderEnd);
				else if (rememberGameThinks[0]->showColor() == BLACK) logicActiveZone(x, y, multiMove, WHITE, URBorderEnd, ULBorderEnd, DRBorderEnd, DLBorderEnd);
			}
		}

		Button button = NOTHINK;
		// Производим ввод на клавиатуре
		ImputButton(button);

		switch (button)
		{
		case UP:
			if (movingPlayer->showCoords().y != 1) movingPlayer->move(UP);
			break;
		case RIGHT:
			if (movingPlayer->showCoords().x - 64 != MAXSIZE) movingPlayer->move(RIGHT);
			break;
		case DOWN:
			if (movingPlayer->showCoords().y != MAXSIZE) movingPlayer->move(DOWN);
			break;
		case LEFT:
			if (movingPlayer->showCoords().x - 64 != 1) movingPlayer->move(LEFT);
			break;
		case ENTER:
			// Если активная зона была размеченна то клавиша отвечает за выбор позиции передвижения или атаки
			if (activeZone.size() && rememberGameThinks.size())
			{
				for (int a = 0; a < activeZone.size(); a++)
				{
					if (movingPlayer->showCoords().x == activeZone[a]->x && movingPlayer->showCoords().y == activeZone[a]->y)
					{
						while (rememberGameThinks[0]->showCoords().x < activeZone[a]->x && rememberGameThinks[0]->showCoords().y < activeZone[a]->y)
							logicMoveAndAttack(DOWNRIGHT, movingPlayer);

						while (rememberGameThinks[0]->showCoords().x > activeZone[a]->x&& rememberGameThinks[0]->showCoords().y < activeZone[a]->y)
							logicMoveAndAttack(DOWNLEFT, movingPlayer);

						while (rememberGameThinks[0]->showCoords().x < activeZone[a]->x && rememberGameThinks[0]->showCoords().y > activeZone[a]->y)
							logicMoveAndAttack(UPRIGHT, movingPlayer);

						while (rememberGameThinks[0]->showCoords().x > activeZone[a]->x&& rememberGameThinks[0]->showCoords().y > activeZone[a]->y)
							logicMoveAndAttack(UPLEFT, movingPlayer);

						clearActiveZone();

						while (rememberGameThinks.size() > 1)
							rememberGameThinks.pop_back();
					}
				}

			}
			else // Иначе она размечает досутуппную активную зону 
			{
				for (int a = 0; a < MAXTHINGS; a++)
				{
					if (movingPlayer->showCoords().x == gameThinks[a]->showCoords().x && movingPlayer->showCoords().y == gameThinks[a]->showCoords().y && movingPlayer->showColor() == gameThinks[a]->showColor())
					{
						while (rememberGameThinks.size())
							rememberGameThinks.pop_back();

						clearActiveZone();

						// Делаем шашку активной и запоминаем её
						gameThinks[a]->becomeActive();
						rememberGameThinks.push_back(gameThinks[a]);

						/* Если активная зона достигла препятствия то **BorderEnd становится false,
							что не дает создавать новые активные зоны в конкретном направлении */
						bool URBorderEnd = true /*Сверху-Справа*/, ULBorderEnd = true /*Сверху-Слева*/, DRBorderEnd = true /*Снизу-Справа*/, DLBorderEnd = true /*Снизу-Слева*/;

						// multiMove определяет сколько раз будет двигаться шашка
						for (int y = gameThinks[a]->showCoords().y, x = gameThinks[a]->showCoords().x, multiMove = 1; multiMove <= gameThinks[a]->showMaxMove(); multiMove++)
						{
							if (gameThinks[a]->showColor() == WHITE && typeid(*gameThinks[a]).name() == typeid(SuperDraught).name())
							{
								logicActiveZone(x, y, multiMove, BLACK, URBorderEnd, ULBorderEnd, DRBorderEnd, DLBorderEnd);

								if (DRBorderEnd) createActiveZone(DOWNRIGHT, x, y, multiMove);
								if (DLBorderEnd) createActiveZone(DOWNLEFT, x, y, multiMove);
								if (URBorderEnd) createActiveZone(UPRIGHT, x, y, multiMove);
								if (ULBorderEnd) createActiveZone(UPLEFT, x, y, multiMove);
							}

							else if (gameThinks[a]->showColor() == BLACK && typeid(*gameThinks[a]).name() == typeid(SuperDraught).name())
							{
								logicActiveZone(x, y, multiMove, WHITE, URBorderEnd, ULBorderEnd, DRBorderEnd, DLBorderEnd);

								if (URBorderEnd) createActiveZone(UPRIGHT, x, y, multiMove);
								if (ULBorderEnd) createActiveZone(UPLEFT, x, y, multiMove);
								if (DRBorderEnd) createActiveZone(DOWNRIGHT, x, y, multiMove);
								if (DLBorderEnd) createActiveZone(DOWNLEFT, x, y, multiMove);
							}

							else if (gameThinks[a]->showColor() == WHITE)
							{
								logicActiveZone(x, y, multiMove, BLACK, URBorderEnd, ULBorderEnd, DRBorderEnd, DLBorderEnd);

								if (URBorderEnd) createActiveZone(UPRIGHT, x, y, multiMove);
								if (ULBorderEnd) createActiveZone(UPLEFT, x, y, multiMove);
							}

							else if (gameThinks[a]->showColor() == BLACK)
							{
								logicActiveZone(x, y, multiMove, WHITE, URBorderEnd, ULBorderEnd, DRBorderEnd, DLBorderEnd);

								if (DRBorderEnd) createActiveZone(DOWNRIGHT, x, y, multiMove);
								if (DLBorderEnd) createActiveZone(DOWNLEFT, x, y, multiMove);
							}
						}
						break;
					}
				}
			}
			break;

		case ESCAPE:
			if (!showMultiAttackfMod())
			{
				clearActiveZone();
				while (rememberGameThinks.size()) rememberGameThinks.pop_back();
			}
			break;
		}
	}
	else // Иначе если ходит ИИ то ...
	{
		vector<Draught*> rememberMoveAI;	// Запоминает шашки которые могут передвигаться по полю
											// Вектор rememberGameThinks в этой части программы запоминает шашки которые могут атаковать
		Coords attackAIActiveZone;			// Координата на которою встает шашка после атаки 
		bool isCanAttack = false;			// Если есть шашка которая может атаковать то поиск прекращается

		for (int y = 1; y <= MAXSIZE && !isCanAttack; y++)
		{
			for (int x = 65; x <= MAXSIZE + 64 && !isCanAttack; x++)
			{
				for (int a = 0; a < MAXTHINGS && !isCanAttack; a++)
				{
					if (x == gameThinks[a]->showCoords().x && y == gameThinks[a]->showCoords().y && movingPlayer->showColor() == gameThinks[a]->showColor())
					{
						/* Если активная зона достигла препятствия то **BorderEnd становится false,
								что не дает создавать новые активные зоны в конкретном направлении */
						bool URBorderEnd = true /*Сверху-Справа*/, ULBorderEnd = true /*Сверху-Слева*/, DRBorderEnd = true /*Снизу-Справа*/, DLBorderEnd = true /*Снизу-Слева*/;

						clearActiveZone();

						// Создаем активные зоны
						// multiMove определяет сколько раз будет двигаться шашка
						for (int multiMove = 1; multiMove <= gameThinks[a]->showMaxMove(); multiMove++)
						{
							if (gameThinks[a]->showColor() == WHITE && typeid(*gameThinks[a]).name() == typeid(SuperDraught).name())
							{
								logicActiveZone(x, y, multiMove, BLACK, URBorderEnd, ULBorderEnd, DRBorderEnd, DLBorderEnd);

								if (DRBorderEnd) createActiveZone(DOWNRIGHT, x, y, multiMove);
								if (DLBorderEnd) createActiveZone(DOWNLEFT, x, y, multiMove);
								if (URBorderEnd) createActiveZone(UPRIGHT, x, y, multiMove);
								if (ULBorderEnd) createActiveZone(UPLEFT, x, y, multiMove);
							}

							else if (gameThinks[a]->showColor() == BLACK && typeid(*gameThinks[a]).name() == typeid(SuperDraught).name())
							{
								logicActiveZone(x, y, multiMove, WHITE, URBorderEnd, ULBorderEnd, DRBorderEnd, DLBorderEnd);

								if (URBorderEnd) createActiveZone(UPRIGHT, x, y, multiMove);
								if (ULBorderEnd) createActiveZone(UPLEFT, x, y, multiMove);
								if (DRBorderEnd) createActiveZone(DOWNRIGHT, x, y, multiMove);
								if (DLBorderEnd) createActiveZone(DOWNLEFT, x, y, multiMove);
							}

							else if (gameThinks[a]->showColor() == WHITE)
							{
								logicActiveZone(x, y, multiMove, BLACK, URBorderEnd, ULBorderEnd, DRBorderEnd, DLBorderEnd);

								if (URBorderEnd) createActiveZone(UPRIGHT, x, y, multiMove);
								if (ULBorderEnd) createActiveZone(UPLEFT, x, y, multiMove);
							}

							else if (gameThinks[a]->showColor() == BLACK)
							{
								logicActiveZone(x, y, multiMove, WHITE, URBorderEnd, ULBorderEnd, DRBorderEnd, DLBorderEnd);

								if (DRBorderEnd) createActiveZone(DOWNRIGHT, x, y, multiMove);
								if (DLBorderEnd) createActiveZone(DOWNLEFT, x, y, multiMove);
							}
						}

						while (rememberGameThinks.size())
							rememberGameThinks.pop_back();

						// В зависмости от доступных активных зон ИИ решает какие шашки будут атаковать или ходить
						// Сначала определяем может ли шашка gameThinks[a] атаковать и если может то запоминаем её и шашку которую она может атаковать а так же позицию на которую она встанет после атаки
						for (int b = 0; b < activeZone.size() && !isCanAttack; b++)
						{
							for (int multiMove = 0; multiMove < gameThinks[a]->showMaxMove(); multiMove++)
							{
								if (activeZone[b]->x + 2 + multiMove == gameThinks[a]->showCoords().x && activeZone[b]->y + 2 + multiMove == gameThinks[a]->showCoords().y)
								{
									for (int c = 0; c < MAXTHINGS; c++)
									{
										if (gameThinks[c]->showCoords().x == activeZone[b]->x + 1 && gameThinks[c]->showCoords().y == activeZone[b]->y + 1)
										{
											rememberGameThinks.push_back(gameThinks[a]);
											rememberGameThinks.push_back(gameThinks[c]);
											attackAIActiveZone.x = activeZone[b]->x;
											attackAIActiveZone.y = activeZone[b]->y;
											movingPlayer->move(*activeZone[b]);
											isCanAttack = true;
											break;
										}
									}
									break;
								}
								else if (activeZone[b]->x + 2 + multiMove == gameThinks[a]->showCoords().x && activeZone[b]->y - 2 - multiMove == gameThinks[a]->showCoords().y)
								{
									for (int c = 0; c < MAXTHINGS; c++)
									{
										if (gameThinks[c]->showCoords().x == activeZone[b]->x + 1 && gameThinks[c]->showCoords().y == activeZone[b]->y - 1)
										{
											rememberGameThinks.push_back(gameThinks[a]);
											rememberGameThinks.push_back(gameThinks[c]);
											attackAIActiveZone.x = activeZone[b]->x;
											attackAIActiveZone.y = activeZone[b]->y;
											movingPlayer->move(*activeZone[b]);
											isCanAttack = true;
											break;
										}
									}
									break;
								}
								else if (activeZone[b]->x - 2 - multiMove == gameThinks[a]->showCoords().x && activeZone[b]->y + 2 + multiMove == gameThinks[a]->showCoords().y)
								{
									for (int c = 0; c < MAXTHINGS; c++)
									{
										if (gameThinks[c]->showCoords().x == activeZone[b]->x - 1 && gameThinks[c]->showCoords().y == activeZone[b]->y + 1)
										{
											rememberGameThinks.push_back(gameThinks[a]);
											rememberGameThinks.push_back(gameThinks[c]);
											attackAIActiveZone.x = activeZone[b]->x;
											attackAIActiveZone.y = activeZone[b]->y;
											movingPlayer->move(*activeZone[b]);
											isCanAttack = true;
											break;
										}
									}
									break;
								}
								else if (activeZone[b]->x - 2 - multiMove == gameThinks[a]->showCoords().x && activeZone[b]->y - 2 - multiMove == gameThinks[a]->showCoords().y)
								{
									for (int c = 0; c < MAXTHINGS; c++)
									{
										if (gameThinks[c]->showCoords().x == activeZone[b]->x - 1 && gameThinks[c]->showCoords().y == activeZone[b]->y - 1)
										{
											rememberGameThinks.push_back(gameThinks[a]);
											rememberGameThinks.push_back(gameThinks[c]);
											attackAIActiveZone.x = activeZone[b]->x;
											attackAIActiveZone.y = activeZone[b]->y;
											movingPlayer->move(*activeZone[b]);
											isCanAttack = true;
											break;
										}
									}
									break;
								}
							}
						}

						//  Если она не может атаковать то определяем может ли она ходить и если может то запоминаем её
						if (!isCanAttack && activeZone.size())
						{
							rememberMoveAI.push_back(gameThinks[a]);
							break;
						}
					}
				}
			}
		}

		// Если есть шашки которые могут актаковать то производим атаку
		if (rememberGameThinks.size())
		{
			for (int a = 0; a < MAXTHINGS; a++)
			{
				if (rememberGameThinks[0] == gameThinks[a])
				{
					while (rememberGameThinks[0]->showCoords().x < attackAIActiveZone.x && rememberGameThinks[0]->showCoords().y < attackAIActiveZone.y)
						logicMoveAndAttack(DOWNRIGHT, movingPlayer);

					while (rememberGameThinks[0]->showCoords().x > attackAIActiveZone.x&& rememberGameThinks[0]->showCoords().y < attackAIActiveZone.y)
						logicMoveAndAttack(DOWNLEFT, movingPlayer);

					while (rememberGameThinks[0]->showCoords().x < attackAIActiveZone.x && rememberGameThinks[0]->showCoords().y > attackAIActiveZone.y)
						logicMoveAndAttack(UPRIGHT, movingPlayer);

					while (rememberGameThinks[0]->showCoords().x > attackAIActiveZone.x&& rememberGameThinks[0]->showCoords().y > attackAIActiveZone.y)
						logicMoveAndAttack(UPLEFT, movingPlayer);
				}
			}
		}
		// Иначе выбираем шашку из тех что могут ходить по полю и прозводим ход
		else if (rememberMoveAI.size())
		{
			srand(time(NULL));
			Draught* rememberDraugh = rememberMoveAI[rand() % rememberMoveAI.size()]; // Выбираем случаюную шашку из тех что могут ходить

			/* Если активная зона достигла препятствия то **BorderEnd становится false,
							что не дает создавать новые активные зоны в конкретном направлении */
			bool URBorderEnd = true /*Сверху-Справа*/, ULBorderEnd = true /*Сверху-Слева*/, DRBorderEnd = true /*Снизу-Справа*/, DLBorderEnd = true /*Снизу-Слева*/;

			if (rememberDraugh->showColor() == WHITE && typeid(*rememberDraugh).name() == typeid(SuperDraught).name())
			{
				logicActiveZone(rememberDraugh->showCoords().x, rememberDraugh->showCoords().y, 1, BLACK, URBorderEnd, ULBorderEnd, DRBorderEnd, DLBorderEnd);

				if (URBorderEnd) rememberDraugh->move(UPRIGHT);
				else if (ULBorderEnd) rememberDraugh->move(UPLEFT);
				else if (DRBorderEnd) rememberDraugh->move(DOWNRIGHT);
				else if (DLBorderEnd) rememberDraugh->move(DOWNLEFT);
			}

			else if (rememberDraugh->showColor() == BLACK && typeid(*rememberDraugh).name() == typeid(SuperDraught).name())
			{
				logicActiveZone(rememberDraugh->showCoords().x, rememberDraugh->showCoords().y, 1, WHITE, URBorderEnd, ULBorderEnd, DRBorderEnd, DLBorderEnd);

				if (URBorderEnd) rememberDraugh->move(UPRIGHT);
				else if (ULBorderEnd) rememberDraugh->move(UPLEFT);
				else if (DRBorderEnd) rememberDraugh->move(DOWNRIGHT);
				else if (DLBorderEnd)rememberDraugh->move(DOWNLEFT);
			}

			else if (rememberDraugh->showColor() == WHITE)
			{
				logicActiveZone(rememberDraugh->showCoords().x, rememberDraugh->showCoords().y, 1, BLACK, URBorderEnd, ULBorderEnd, DRBorderEnd, DLBorderEnd);

				if (URBorderEnd) rememberDraugh->move(UPRIGHT);
				else if (ULBorderEnd) rememberDraugh->move(UPLEFT);
			}

			else if (rememberDraugh->showColor() == BLACK)
			{
				logicActiveZone(rememberDraugh->showCoords().x, rememberDraugh->showCoords().y, 1, WHITE, URBorderEnd, ULBorderEnd, DRBorderEnd, DLBorderEnd);

				if (DRBorderEnd) rememberDraugh->move(DOWNRIGHT);
				else if (DLBorderEnd)rememberDraugh->move(DOWNLEFT);
			}

			// Если запомниная шашка находится в самой верхней или нижней части поля то она становится дамкой
			if ((rememberDraugh->showCoords().y == 1 && rememberDraugh->showColor() == WHITE) || (rememberDraugh->showCoords().y == 8 && rememberDraugh->showColor() == BLACK) && rememberDraugh->showCoords().x >= 1 + 64 && rememberDraugh->showCoords().x <= MAXSIZE + 64 && typeid(*rememberDraugh).name() != typeid(SuperDraught).name())
			{
				for (int i = 0; i < MAXTHINGS; i++)
				{
					if (gameThinks[i]->showCoords().x == rememberDraugh->showCoords().x && gameThinks[i]->showCoords().y == rememberDraugh->showCoords().y)
					{
						SuperDraught* temp = new SuperDraught(rememberDraugh->showCoords().x, rememberDraugh->showCoords().y, rememberDraugh->showColor(), MAXSIZE);
						delete gameThinks[i];
						gameThinks[i] = temp;
					}
				}
			}
		}

		onEndOfMove();
		clearActiveZone();
	}
}

// Рисует пустую строку на поле
void drawEmptyString()
{
	for (int x = 0; x < MAXSIZE + 2; x++)
	{
		if (x == MAXSIZE + 1) cout << "|---|";
		else cout << "|---";
	}

	cout << endl;
}

// Функция создает шаблон для подсказки
void drawHelp(const char* COLORWORD, const char* NORMAL, const ConsoleColor COLOR)
{
	SetColor(COLOR, BLACK);
	cout << COLORWORD;
	SetColor(WHITE, BLACK);
	cout << NORMAL;
}

// Рисует игровое поле
void draw(Player* const movingPlayer, const bool isGameEnded)
{
	system("cls"); // Очистим поле

	// Рисуем обозначение очердности хода или объявляем победителя если игра закончена
	drawEmptyString();

	if (isGameEnded)
		cout << "|              Победили:";

	else cout << "|          Сейчас ходят:";

	if (movingPlayer->showColor() == WHITE)
		cout << "БЕЛЫЕ           |";

	else drawHelp("ЧЕРНЫЕ          ", "|", RED);

	cout << endl;

	drawEmptyString();

	// Рисуем первую строку
	for (int x = 0; x < MAXSIZE + 2; x++)
	{
		if (x == 0)
			cout << "|   ";
		else if (x == MAXSIZE + 1)
			cout << "|   |";
		else cout << "| " << static_cast<char>(x + 64) << ' ';
	}

	cout << endl;

	drawEmptyString();

	// Рисуем игровое поле
	for (int y = 1; y < MAXSIZE + 1; y++)
	{
		for (int x = 0; x < MAXSIZE + 2; x++)
		{
			bool isBlanc = true;  // Пустой ли символ? 

			if (x == 0)
			{
				cout << "| " << y << ' ';
				isBlanc = false;
			}

			else if (x == MAXSIZE + 1 && isBlanc)
			{
				cout << "| " << y << " |";
				isBlanc = false;
			}

			else if (static_cast<int>(movingPlayer->showCoords().x - 64) == x && movingPlayer->showCoords().y == y && isBlanc)
			{
				movingPlayer->drawThis();
				isBlanc = false;
			}

			for (int i = 0; i < activeZone.size() && isBlanc; i++)
			{
				if (static_cast<int>(activeZone[i]->x - 64) == x && activeZone[i]->y == y)
				{
					cout << "| ";
					SetColor(WHITE, BLUE);
					cout << ' ';
					SetColor(WHITE, BLACK);
					cout << ' ';
					isBlanc = false;
					break;
				}
			}

			for (int i = 0; i < MAXTHINGS && isBlanc; i++)
			{
				if (static_cast<int>(gameThinks[i]->showCoords().x - 64) == x && gameThinks[i]->showCoords().y == y)
				{
					gameThinks[i]->drawThis();
					isBlanc = false;
					break;
				}
			}

			if (isBlanc)
			{
				cout << "|   ";
			}
		}

		cout << endl;

		drawEmptyString();
	}

	// Рисуем последнюю строку

	for (int x = 0; x < MAXSIZE + 2; x++)
	{
		if (x == 0)
			cout << "|   ";
		else if (x == MAXSIZE + 1)
			cout << "|   |";
		else cout << "| " << static_cast<char>(x + 64) << ' ';
	}

	cout << endl;

	drawEmptyString();

	// Рисуем поле подсказки управления

	cout << "|   ";

	drawHelp("WASD", "-ход", GREEN);
	drawHelp(" Enter", "-действие", GREEN);

	if (activeZone.size() && !showMultiAttackfMod)
		drawHelp(" Esc", "-отмена  |", GREEN);

	else if (!showMultiAttackfMod())
		drawHelp(" Esc", "-меню    |", GREEN);

	else cout << "              ";
	cout << endl;

	drawEmptyString();
}

// Функция упорядочивает другие для создания игрового процесса
void gameProcess(const bool GAMEMOD, const ConsoleColor CHOICESIDE = WHITE)
{
	newGame(GAMEMOD, CHOICESIDE);
	ConsoleColor whoseMove = WHITE;
	int counterOfBlack = 0, counterOfWhite = 0;

	do
	{
		draw(Players[!whoseMove], false);
		logic(Players[!whoseMove]);

		if (showEndOfMove() && !showMultiAttackfMod() && whoseMove == BLACK) whoseMove = WHITE;
		else if (showEndOfMove() && !showMultiAttackfMod() && whoseMove == WHITE) whoseMove = BLACK;
		offEndOfMove();

		counterOfBlack = counterOfWhite = 0;
		for (int i = 0; i < gameThinks.size(); i++)
		{
			if (gameThinks[i]->showColor() == WHITE) counterOfWhite++;
			else if (gameThinks[i]->showColor() == BLACK) counterOfBlack++;
		}

	} while (counterOfBlack && counterOfWhite);

	draw(Players[static_cast<bool>(whoseMove)], true);
	endGame();
	_getch();
}

// Функция отвечает за логику меню игру
void logicMenu()
{
	static statusMenu nowStatus = MAINMENU;		// Текущие статус меню
	static vector <SelectorMenu*> lvlsOfMenu;	// Пункты меню

	switch (nowStatus)
	{
		SelectorMenu* temp;				// Веременная переменная
		bool gameMod;					// Сохраняет результат режима игры 
		ConsoleColor choiceSide;		// Сохраняет результат выбранного цвета шашек

	case MAINMENU:	// Создаем пункты главного меню
		if (!lvlsOfMenu.size())
		{
			temp = new SelectorMenu("Начать игру", NEWGAME);
			lvlsOfMenu.push_back(temp);
			lvlsOfMenu.back()->becomeActive();

			temp = new SelectorMenu("Выйти", EXIT);
			lvlsOfMenu.push_back(temp);
		}
		break;

	case NEWGAME:	// Создаем пункты начала новой игры
		if (!lvlsOfMenu.size())
		{
			temp = new SelectorMenu("< Против ИИ >", AIMOD);
			lvlsOfMenu.push_back(temp);
			lvlsOfMenu.back()->becomeActive();

			temp = new SelectorMenu("< Белые >", FIRSTWHITE);
			lvlsOfMenu.push_back(temp);

			temp = new SelectorMenu("Играть!", PLAY);
			lvlsOfMenu.push_back(temp);

			temp = new SelectorMenu("Назад", MAINMENU);
			lvlsOfMenu.push_back(temp);
		}
		break;

	case PLAY:	// Начинаем игру
		nowStatus = MAINMENU;

		choiceSide = lvlsOfMenu[1]->showStatus() == FIRSTWHITE ? WHITE : BLACK;
		gameMod = lvlsOfMenu[0]->showStatus() == AIMOD;

		while (lvlsOfMenu.size())
		{
			delete lvlsOfMenu.back();
			lvlsOfMenu.pop_back();
		}

		gameProcess(gameMod, choiceSide);
		return;

	case EXIT:	// Выходим из игры
		exit(0);
	}

	// Рисуем меню
	drawMenu(lvlsOfMenu);

	Button button = NOTHINK;
	// Производим ввод на клавиатуре
	ImputButton(button);

	switch (button)
	{
	case UP:
		for (int i = 0; i < lvlsOfMenu.size(); i++)
		{
			if (lvlsOfMenu[i]->showActive() && i != 0)
			{
				lvlsOfMenu[i]->offActive();
				lvlsOfMenu[i - 1]->becomeActive();
				break;
			}
		}
		break;

	case DOWN:
		for (int i = 0; i < lvlsOfMenu.size(); i++)
		{
			if (lvlsOfMenu[i]->showActive() && i != lvlsOfMenu.size() - 1)
			{
				lvlsOfMenu[i]->offActive();
				lvlsOfMenu[i + 1]->becomeActive();
				break;
			}
		}
		break;

	case RIGHT:
	case LEFT:
		if (nowStatus == NEWGAME)	// Если текущий статус "Новая игра" то есть возможность поменять пункты в текущем меню
		{
			for (int i = 0; i < lvlsOfMenu.size(); i++)
			{
				if (lvlsOfMenu[i]->showActive())
				{
					if (lvlsOfMenu[i]->showStatus() == AIMOD)
					{
						delete lvlsOfMenu[i];
						delete lvlsOfMenu[i + 1];
						lvlsOfMenu[i] = new SelectorMenu("< Поочердно >", TWOPLAYERMOD);
						lvlsOfMenu[i + 1] = new SelectorMenu("Белые", FIRSTWHITE);
						lvlsOfMenu[i]->becomeActive();
					}
					else if (lvlsOfMenu[i]->showStatus() == TWOPLAYERMOD)
					{
						delete lvlsOfMenu[i];
						delete lvlsOfMenu[i + 1];
						lvlsOfMenu[i + 1] = new SelectorMenu("< Белые >", FIRSTWHITE);
						lvlsOfMenu[i] = new SelectorMenu("< Против ИИ >", AIMOD);
						lvlsOfMenu[i]->becomeActive();
					}
					else if (lvlsOfMenu[i]->showStatus() == FIRSTWHITE && lvlsOfMenu[i - 1]->showStatus() == AIMOD)
					{
						delete lvlsOfMenu[i];
						lvlsOfMenu[i] = new SelectorMenu("< Черные >", FIRSTBLACK);
						lvlsOfMenu[i]->becomeActive();
					}
					else if (lvlsOfMenu[i]->showStatus() == FIRSTBLACK && lvlsOfMenu[i - 1]->showStatus() == AIMOD)
					{
						delete lvlsOfMenu[i];
						lvlsOfMenu[i] = new SelectorMenu("< Белые >", FIRSTWHITE);
						lvlsOfMenu[i]->becomeActive();
					}
					break;
				}
			}
		}
		break;

	case ENTER:
		for (int i = 0; i < lvlsOfMenu.size(); i++) 
		{
			if (lvlsOfMenu[i]->showActive())
			{
				// Меняем статус на новый если пункт не из "Начать игру" или PLAY
				switch (lvlsOfMenu[i]->showStatus())
				{
				case MAINMENU:
				case NEWGAME:
				case PLAY:
				case EXIT:
					nowStatus = lvlsOfMenu[i]->showStatus();
				}

				// Если в пунктах меню пункты из "Начать игру" то пункты не удаляются
				if (lvlsOfMenu[i]->showStatus() != PLAY && lvlsOfMenu[i]->showStatus() != AIMOD && lvlsOfMenu[i]->showStatus() != TWOPLAYERMOD && lvlsOfMenu[i]->showStatus() != FIRSTWHITE && lvlsOfMenu[i]->showStatus() != FIRSTBLACK)
				{
					while (lvlsOfMenu.size())
					{
						delete lvlsOfMenu.back();
						lvlsOfMenu.pop_back();
					}
				}
			}
		}
		break;
	}
}

// Функция рисует меню игры
void drawMenu(vector<SelectorMenu*>& LVLS)
{
	system("cls"); // Очистим поле

	// Рисуем название игры
	drawEmptyString();

	for (int x = 1; x < (MAXSIZE) * 3 / 2; x++)
	{
		if (x == 1) cout << "|";
		else cout << " ";
	}

	cout << "И Г Р А: Ш А Ш К И";

	for (int x = 1; x < (MAXSIZE) * 3 / 2 + 1; x++)
	{
		if (x == MAXSIZE * 3 / 2) cout << "|";
		else cout << " ";
	}

	cout << endl;
	drawEmptyString();

	// Рисуем меню
	for (int y = 1; y < MAXSIZE + 2; y++)
	{
		for (int x = 1; x < MAXSIZE * 5 + 2; x++)
		{
			if (x == 1 || x == MAXSIZE * 5 + 1) cout << "|";
			else cout << " ";
		}

		cout << endl;

		if (LVLS.size() > y - 1)
		{
			for (int x = 1; x < MAXSIZE * 5 + 3 - LVLS[y - 1]->sizeOfSelectorName(); x++)
			{
				if (x == 1 || x == MAXSIZE * 5 + 2 - LVLS[y - 1]->sizeOfSelectorName()) cout << "|";
				else if (x == MAXSIZE * 5 / 2 + 1 - LVLS[y - 1]->sizeOfSelectorName() / 2) LVLS[y - 1]->drawThis();
				else cout << " ";
			}
		}

		else
		{
			for (int x = 1; x < MAXSIZE * 5 + 2; x++)
			{
				if (x == 1 || x == MAXSIZE * 5 + 1) cout << "|";
				else cout << " ";
			}
		}

		cout << endl;
	}

	for (int x = 1; x < MAXSIZE * 5 + 2; x++)
	{
		if (x == 1 || x == MAXSIZE * 5 + 1) cout << "|";
		else cout << " ";
	}

	cout << endl;

	drawEmptyString();

	// Рисуем поле подсказки управления

	cout << "|   ";

	drawHelp("WASD", "-ход", GREEN);
	drawHelp(" Enter", "-действие", GREEN);
	drawHelp(" Esc", "-отмена  |", GREEN);

	cout << endl;

	drawEmptyString();

}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	while (true) logicMenu();
}


