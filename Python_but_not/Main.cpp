#include "Hed.h"	
#include "windows.h"
#include <conio.h>
#include <vector>
#include <random>

void SetWindow(int Width, int Height)
{
	_COORD coord;
	coord.X = Width;
	coord.Y = Height;
	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = Height - 1;
	Rect.Right = Width - 1;
	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(Handle, coord);
	SetConsoleWindowInfo(Handle, TRUE, &Rect);
}

void main() 
{
	while (true)
	{
		srand(time(0));

		bool win = false;
		bool out = false;
		bool eated = false;

		char space = '.';
		char player = 'P';
		char tailCh = 't';
		char food = '@';

		int ScreenWidth = 120;
		int ScreenHeight = 29;
		int WidthStart = 52;
		int WidthEnd = 68;
		int Height = 10;
		int PlayerPos = 0;
		int PlayerPosX = 0;
		int PlayerPosY = 0;
		int tail = 0;
		int foodPos = 0;

		SetWindow(ScreenWidth, ScreenHeight+1);

		double consoleRatio = (double)11 / 24;

		char* screen = new char[ScreenHeight * ScreenWidth + 1];
		screen[ScreenWidth * ScreenHeight] = '\0';

		int* nums = new int[(WidthEnd - WidthStart) * Height + 1];

		vector <int> tailNums;
		vector <int> freeSpace;

		for (int i = 0; i < ScreenWidth; i++)
		{
			for (int j = 0; j < ScreenHeight; j++)
			{
				if (i >= WidthStart and i < WidthEnd and j < Height)
				{
					screen[i + j * ScreenWidth] = space;
					nums[PlayerPosX] = i + j * ScreenWidth;
					PlayerPosX++;
				}
				else
				{
					screen[i + j * ScreenWidth] = ' ';
				}
			}
		}

		double foodFactor = 0.1;
		double speed = 8 + foodFactor * (tail / 2);  // сколько движений за 1ну секунду
		//double speed = 41;

		int button = 0;
		int buttonBefore = 0;
		int tailCount = 0;

		int tick = 0; //с последнего движения кадров

		PlayerPosX = 0;
		PlayerPos = nums[PlayerPosY + PlayerPosX * 10];

		for (int i = 0; i < ((WidthEnd - WidthStart) * Height); i++)
		{
			if (nums[i] != PlayerPos)
			{
				bool isTail = false;
				for (int q = 0; q < tailNums.size(); q++)
				{
					if (nums[i] == tailNums[q])
					{
						isTail = true;
						break;
					}
				}
				if (!isTail)
				{
					freeSpace.push_back(nums[i]);
				}
			}
		}

		foodPos = freeSpace[rand() % freeSpace.size()];
		screen[foodPos] = food;

		while (true)
		{
			if (tail == (WidthEnd - WidthStart) * Height - 2)
			{
				win = true;
				break;
			}

			printf(screen);

			if (foodPos == PlayerPos)
			{
				freeSpace.clear();

				for (int i = 0; i < ((WidthEnd - WidthStart) * Height); i++)
				{
					bool notTail = true;
					if (nums[i] != PlayerPos)
					{
						for (int q = 0; q < tail; q++)
						{
							if (nums[i] == tailNums[q])
							{
								notTail = false;
							}
						}
					}
					if (notTail)
					{
						freeSpace.push_back(nums[i]);
					}
				}

				tail++;
				tailNums.push_back(PlayerPos);

				foodPos = freeSpace[rand() % freeSpace.size()];
				screen[foodPos] = food;
			}

			//if (tick >= 60 / speed) //при 60
			if (tick >= 120 / speed) //при 120
			{
				buttonBefore = button;

				if (!eated)
				{
					for (int i = 0; i < tail; i++)
					{
						screen[tailNums[i]] = space;
					}

					if (tail > 0)
					{
						if (tailCount < tail)
						{
							for (int i = 0; i < tail - 1; i++)
							{
								tailNums[i] = tailNums[i + 1];
							}
							tailNums[tailCount] = PlayerPos;
							tailCount++;
						}
						else
						{
							for (int i = 0; i < tail - 1; i++)
							{
								tailNums[i] = tailNums[i + 1];
							}
							tailCount--;
							tailNums[tailCount] = PlayerPos;
						}
					}
				}

				if (button == 119) //w
				{
					speed = 8 + foodFactor * (tail / 2);
					speed /= 1.1;
					for (int i = 1; i < tail - 1; i++)
					{
						if (nums[(PlayerPosY - 1) + PlayerPosX * 10] == tailNums[i])
						{
							out = true;
							break;
						}
					}
					if (PlayerPosY - 1 == -1 or out)
					{
						break;
					}
					PlayerPosY--;
				}
				if (button == 115) //s
				{
					speed = 8 + foodFactor * (tail / 2);
					speed /= 1.1;
					for (int i = 1; i < tail - 1; i++)
					{
						if (nums[(PlayerPosY + 1) + PlayerPosX * 10] == tailNums[i])
						{
							out = true;
							break;
						}
					}
					if (PlayerPosY + 1 == Height or out)
					{
						break;
					}
					PlayerPosY++;
				}
				if (button == 97) //a
				{
					speed = 8 + foodFactor * (tail / 2);
					for (int i = 1; i < tail - 1; i++)
					{
						if (nums[PlayerPosY + (PlayerPosX - 1) * 10] == tailNums[i])
						{
							out = true;
							break;
						}
					}
					if (PlayerPosX - 1 == -1 or out)
					{
						break;
					}
					PlayerPosX--;
				}
				if (button == 100) //d
				{
					speed = 8 + foodFactor * (tail / 2);
					for (int i = 1; i < tail - 1; i++)
					{
						if (nums[PlayerPosY + (PlayerPosX + 1) * 10] == tailNums[i])
						{
							out = true;
							break;
						}
					}
					if (PlayerPosX + 1 == (WidthEnd - WidthStart) or out)
					{
						break;
					}
					PlayerPosX++;
				}


				screen[PlayerPos] = space;
				PlayerPos = nums[PlayerPosY + PlayerPosX * 10];
				screen[PlayerPos] = player;

				for (int i = 0; i < tail; i++)
				{
					screen[tailNums[i]] = tailCh;
				}

				tick = 0;
			}
			else
			{
				tick++;
			}

			if (_kbhit())
			{
				button = _getch();

				if (button != buttonBefore)
				{
					tick = 121;
				}
				

				if (button == 119 or button == 115 or button == 97 or button == 100)
				{
					if (button == 119 and buttonBefore == 115)
					{
						button = buttonBefore;
					}
					if (button == 115 and buttonBefore == 119)
					{
						button = buttonBefore;
					}
					if (button == 97 and buttonBefore == 100)
					{
						button = buttonBefore;
					}
					if (button == 100 and buttonBefore == 97)
					{
						button = buttonBefore;
					}
				}
				else
				{
					button = buttonBefore;
				}
			}

			Sleep(8); //120
			//Sleep(16); //60 кадров в сек
			//Sleep(34); //30 кадров в сек
			screen[PlayerPos] = player;
			screen[foodPos] = food;
		}

		if (win)
		{
			system("CLS");
			cout << "$$__$$___$$$$___$$__$$\n";
			Sleep(25);
			cout << "_$$$$___$$__$$__$$__$$\n";
			Sleep(25);
			cout << "__$$____$$__$$__$$__$$\n";
			Sleep(25);
			cout << "__$$____$$__$$__$$__$$\n";
			Sleep(25);
			cout << "__$$_____$$$$____$$$$\n";
			Sleep(25);
			cout << "\n";
			Sleep(25);
			cout << "$$___$$_$$$$$$__$$__$$\n";
			Sleep(25);
			cout << "$$___$$___$$____$$$_$$\n";
			Sleep(25);
			cout << "$$_$_$$___$$____$$_$$$\n";
			Sleep(25);
			cout << "$$$$$$$___$$____$$__$$\n";
			Sleep(25);
			cout << "_$$_$$__$$$$$$__$$__$$\n";
		}
		else
		{
			system("CLS");
			cout << "$$$$____$$$$___$$___$$_$$$$$\n";
			Sleep(25);
			cout << "$$______$$__$$__$$$_$$$_$$\n";
			Sleep(25);
			cout << "$$_$$$__$$$$$$__$$_$_$$_$$$$\n";
			Sleep(25);
			cout << "$$__$$__$$__$$__$$___$$_$$\n";
			Sleep(25);
			cout << "_$$$$___$$__$$__$$___$$_$$$$$\n\n";
			Sleep(25);
			cout << "_$$$$___$$__$$__$$$$$___$$$$$\n";
			Sleep(25);
			cout << "$$__$$__$$__$$__$$______$$__$$\n";
			Sleep(25);
			cout << "$$__$$__$$__$$__$$$$____$$$$$\n";
			Sleep(25);
			cout << "$$__$$___$$$$___$$______$$__$$\n";
			Sleep(25);
			cout << "_$$$$_____$$____$$$$$___$$__$$\n";
		}
		system("pause");
	}
}