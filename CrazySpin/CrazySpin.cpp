// CrazySpin.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>

enum Direction
{
	NONE,
	LEFT,
	RIGHT
} direction;

uint32_t speed = 1;
int moved = 0;
int screenWidth;

DWORD WINAPI move_func(LPVOID lpParameter)
{
	while (direction)
	{
		if (direction == LEFT)
		{
			mouse_event(MOUSEEVENTF_MOVE, -speed, 0, 0, 0);
			moved -= speed;
		}
		else if (direction == RIGHT)
		{
			mouse_event(MOUSEEVENTF_MOVE, +speed, 0, 0, 0);
			moved += speed;
		}

		printf("Went forward %d pixels\n", moved);
		Sleep(5);
	}

	printf("Going back %d pixels\n", -moved);

	//fix this, virtual screen resolution does not seem to work
	if (abs(moved) >= screenWidth)
	{
		moved %= screenWidth;
	}

	printf("Actual parameter: %d", -moved);
	mouse_event(MOUSEEVENTF_MOVE, -moved, 0, 0, 0);
	moved = 0;
	return 0;
}

int main()
{
	screenWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);

	//FreeConsole();
	while (true)
	{
		bool longSleep = false;
		if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		{
			if (GetAsyncKeyState(VK_UP) & 0x8000)
			{
				if (speed < UINT_MAX)
					speed++;
				Sleep(30);
				longSleep = false;
			}

			if (GetAsyncKeyState(VK_DOWN) & 0x8000)
			{
				if (speed > 0)
					speed--;
				Sleep(30);
				longSleep = false;
			}

			if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			{
				if (direction == RIGHT)
				{
					direction = NONE;
				}
				else if (direction == NONE)
				{
					direction = LEFT;
					CreateThread(NULL, 0, move_func, NULL, 0, 0);
				}

				longSleep = true;
			}

			if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			{
				if (direction == LEFT)
				{
					direction = NONE;
				}
				else if (direction == NONE)
				{
					direction = RIGHT;
					CreateThread(NULL, 0, move_func, NULL, 0, 0);
				}

				longSleep = true;
			}

			if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			{
				direction = NONE;
				exit(0);
			}
		}

		if (longSleep)
		{
			Sleep(150);
		}
	}

    return 0;
}

