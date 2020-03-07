#include "pch.h"
#include <conio.h>
#include <stdio.h>
#include "time.h"
#include <Windows.h>
#define W 50
#define H 25
#define FOOD   1234
#define WALL_H 4321
#define WALL_V 4322
#define ROW(x)   ((x)/W)
#define COL(x)   ((x)%W)
#define MOVE_UP   (-W)
#define MOVE_DOWN (W)
#define MOVE_LEFT  (-1)
#define MOVE_RIGHT (1)
#define MOVE_SPPED    5 // per second
#define KEY_SPEED   200 // per second
int map[W * H];
HANDLE hOut;
void display_init()
{
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut, &cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hOut, &cci);
}

void display_update(int p)
{
	int end = p >= 0 ? p + 1 : W * H;
	for (int i = p >= 0 ? p : 0; i < end; i++) {
		COORD pos = { COL(i) * 2, ROW(i) };
		SetConsoleCursorPosition(hOut, pos);
		printf(
			map[i] == 0 ? "  " :
			map[i] == WALL_V ? "||" :
			map[i] == WALL_H ? "==" :
			map[i] == FOOD ? "()" :
			map[i] == MOVE_UP * 2 ? "/\\" :
			map[i] == MOVE_DOWN * 2 ? "\\/" :
			map[i] == MOVE_LEFT * 2 ? "《" :
			map[i] == MOVE_RIGHT * 2 ? "》" :
			"[]");
	}
}

void new_food()
{
	int p;
	while (map[p = rand() % (W * H)]);
	map[p] = FOOD;
	display_update(p);
}

int main()
{
	int move = MOVE_LEFT;
	int head = (H / 2) * W + W / 2;
	int tail = head;
	for (int i = 0; i < W * H; i++) {
		map[i] = ROW(i) == 0 || ROW(i) == (H - 1) ? WALL_H :
			COL(i) == 0 || COL(i) == (W - 1) ? WALL_V : 0;
	}
	map[head] = move; head += move;
	map[head] = move * 2;
	display_init();
	srand((unsigned int)time(NULL));
	new_food();
	display_update(-1);
	while (1) {
		for (int i = 0; i < KEY_SPEED / MOVE_SPPED; i++, Sleep(1000 / KEY_SPEED)) {
			if (_kbhit()) {
				int ch = _getch();
				if (ch == 'w' && move != MOVE_DOWN)  move = MOVE_UP;
				else if (ch == 's' && move != MOVE_UP)    move = MOVE_DOWN;
				else if (ch == 'a' && move != MOVE_RIGHT) move = MOVE_LEFT;
				else if (ch == 'd' && move != MOVE_LEFT)  move = MOVE_RIGHT;
				else if (ch == 'q')  return 0;
				else continue;
				break;
			}
		}
		map[head] = move;
		display_update(head);
		head += move;
		if (map[head] == FOOD) new_food();
		else if (map[head]) break;
		else {
			int t = map[tail];
			map[tail] = 0;
			display_update(tail);
			tail += t;
		}
		map[head] = move * 2;
		display_update(head);
	}
}