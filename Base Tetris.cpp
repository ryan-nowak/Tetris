

#include <iostream>
#include<conio.h>
#include <windows.h>
#include <time.h>
using namespace std;

struct Point {
	int X;
	int Y;
};

struct Shape {
	Point Cells[4];
	int Length;
	int Color;
};

void SetColor(int ForgC) {

	WORD wColor;

	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;

	//We use csbi for the wAttributes word.

	if (GetConsoleScreenBufferInfo(hStdOut, &csbi))

	{

		//Mask out all but the background attribute, and add in the forgournd color

		wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);

		SetConsoleTextAttribute(hStdOut, wColor);

	}

	return;
}

void hidecursor() {

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO info;

	info.dwSize = 100;

	info.bVisible = FALSE;

	SetConsoleCursorInfo(consoleHandle, &info);

}

void gotoxy(short x, short y) {

	COORD coord = { x, y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

}

void PrintGrid() {
	int length = 34;
	int height = 29;
	
	for (int i = 0; i < length; i++) {
		gotoxy(i, 0);
		cout << char(219) << char(219);
	}

	for (int i = 0; i < height; i++) {
		gotoxy(0, i);
		cout << char(219) << char(219);
	}

	for (int i = 0; i < height + 1; i++) {
		gotoxy(34, i);
		cout << char(219) << char(219);
	}

	for (int i = 0; i < length; i++) {
		gotoxy(i, 29);
		cout << char(219) << char(219);
	}

}

void PrintShape(Point Shape[], int col, int L) {
	SetColor(col);
	for (int i = 0; i < L; i++)
	{
		gotoxy(Shape[i].X, Shape[i].Y);
		cout << (char)219;
		cout << (char)219;
	}
}

void ClearShape(Point Shape[], int L) {
	SetColor(0); // changes the color to black
	for (int i = 0; i < L; i++)
	{
		gotoxy(Shape[i].X, Shape[i].Y);
		cout << (char)219;
		cout << (char)219;
	}
}

int MoveDown(Point Shape[], int L, Point occupiedPoints[], int occIndex) {
	for (int i = 0; i < 4; i++)
	{
		if (Shape[i].Y > 27)
			return 1; // hit
	}

	for (int i = 0; i < occIndex; i++) {
		for (int j = 0; j < 4; j++) {
			if (Shape[j].X == occupiedPoints[i].X && Shape[j].Y + 1 == occupiedPoints[i].Y) {
				return 1;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		Shape[i].Y++;
	}
	return 0;
}

void MoveRight(Point Shape[], int L, Point occupiedPoints[], int occIndex) {
	for (int i = 0; i < 4; i++) {
		if (Shape[i].X == 32) {
			return;
		}
	}

	for (int i = 0; i < occIndex; i++) {
		for (int j = 0; j < 4; j++) {
			if (Shape[j].X + 2 == occupiedPoints[i].X && Shape[j].Y + 1 == occupiedPoints[i].Y) {
				return;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		Shape[i].X += 2;
	}
	return;
}

void MoveLeft(Point Shape[], int L, Point occupiedPoints[], int occIndex) {
	for (int i = 0; i < 4; i++) {
		if (Shape[i].X == 2) {
			return;
		}
	}

	for (int i = 0; i < occIndex; i++) {
		for (int j = 0; j < 4; j++) {
			if (Shape[j].X - 2 == occupiedPoints[i].X && Shape[j].Y + 1 == occupiedPoints[i].Y) {
				return;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		Shape[i].X -= 2;
	}

	return;
}

void RotateClock90(Point Shape[], int L, int pivotCell, Point occupiedPoints[], int occIndex) {
	int shiftX = Shape[pivotCell].X;
	int shiftY = Shape[pivotCell].Y;

	for (int i = 0; i < L; i++) {
		if (Shape[i].X == 2 || Shape[i].X == 32) {
			return;
		}
	}

	for (int i = 0; i < L; i++) {
		if (Shape[i].Y < 1 || Shape[i].X > 27) {
			return;
		}
	}

	for (int i = 0; i < occIndex; i++) {
		for (int j = 0; j < 4; j++) {
			if (Shape[j].X + 2 == occupiedPoints[i].X && Shape[j].Y + 1 == occupiedPoints[i].Y || Shape[j].X - 2 == occupiedPoints[i].X && Shape[j].Y + 1 == occupiedPoints[i].Y) {
				return;
			}
		}
	}

	for (int i = 0; i < L; i++)
	{
		Shape[i].X -= shiftX;
		Shape[i].Y -= shiftY;
	}

	for (int i = 0; i < L; i++)
	{
		int temp = Shape[i].X;
		Shape[i].X = -Shape[i].Y * 2;
		Shape[i].Y = temp / 2;
	}

	for (int i = 0; i < L; i++)
	{
		Shape[i].X += shiftX;
		Shape[i].Y += shiftY;
	}

	return;
}

void RotateCounterClock90(Point Shape[], int L, int pivotCell, Point occupiedPoints[], int occIndex) {
	int shiftX = Shape[pivotCell].X;
	int shiftY = Shape[pivotCell].Y;

	for (int i = 0; i < L; i++) {
		if (Shape[i].X == 2 || Shape[i].X == 32) {
			return;
		}
	}

	for (int i = 0; i < L; i++) {
		if (Shape[i].Y < 1 || Shape[i].X > 27) {
			return;
		}
	}

	for (int i = 0; i < occIndex; i++) {
		for (int j = 0; j < 4; j++) {
			if (Shape[j].X + 2 == occupiedPoints[i].X && Shape[j].Y + 1 == occupiedPoints[i].Y || Shape[j].X - 2 == occupiedPoints[i].X && Shape[j].Y + 1 == occupiedPoints[i].Y) {
				return;
			}
		}
	}

	for (int i = 0; i < L; i++)
	{
		Shape[i].X -= shiftX;
		Shape[i].Y -= shiftY;
	}

	for (int i = 0; i < L; i++)
	{
		int temp = Shape[i].X;
		Shape[i].X = Shape[i].Y * 2;
		Shape[i].Y = -temp / 2;
	}

	for (int i = 0; i < L; i++)
	{
		Shape[i].X += shiftX;
		Shape[i].Y += shiftY;
	}

	return;
}

Shape newShape(int xoffset, int len, int col, int type) {
	Shape shape;

	switch (type) {
	case 0:			//t shape
		shape.Cells[0].X = 0 + xoffset;
		shape.Cells[0].Y = 1;

		shape.Cells[1].X = 2 + xoffset;
		shape.Cells[1].Y = 1;

		shape.Cells[2].X = 4 + xoffset;
		shape.Cells[2].Y = 1;

		shape.Cells[3].X = 2 + xoffset;
		shape.Cells[3].Y = 2;
		break;
	case 1:			//l shape
		shape.Cells[0].X = 0 + xoffset;
		shape.Cells[0].Y = 1;

		shape.Cells[1].X = 0 + xoffset;
		shape.Cells[1].Y = 2;

		shape.Cells[2].X = 0 + xoffset;
		shape.Cells[2].Y = 3;

		shape.Cells[3].X = 2 + xoffset;
		shape.Cells[3].Y = 3;
		break;
	case 2:			//o shape
		shape.Cells[0].X = 0 + xoffset;
		shape.Cells[0].Y = 1;

		shape.Cells[1].X = 2 + xoffset;
		shape.Cells[1].Y = 1;

		shape.Cells[2].X = 0 + xoffset;
		shape.Cells[2].Y = 2;

		shape.Cells[3].X = 2 + xoffset;
		shape.Cells[3].Y = 2;
		break;
	case 3:			//i shape
		shape.Cells[0].X = 0 + xoffset;
		shape.Cells[0].Y = 1;

		shape.Cells[1].X = 2 + xoffset;
		shape.Cells[1].Y = 1;

		shape.Cells[2].X = 4 + xoffset;
		shape.Cells[2].Y = 1;

		shape.Cells[3].X = 6 + xoffset;
		shape.Cells[3].Y = 1;
		break;
	case 4:			//j shape
		shape.Cells[0].X = 0 + xoffset;
		shape.Cells[0].Y = 1;

		shape.Cells[1].X = 2 + xoffset;
		shape.Cells[1].Y = 1;

		shape.Cells[2].X = 4 + xoffset;
		shape.Cells[2].Y = 1;

		shape.Cells[3].X = 4 + xoffset;
		shape.Cells[3].Y = 2;
		break;
	case 5:			//z shape
		shape.Cells[0].X = 0 + xoffset;
		shape.Cells[0].Y = 1;

		shape.Cells[1].X = 2 + xoffset;
		shape.Cells[1].Y = 1;

		shape.Cells[2].X = 2 + xoffset;
		shape.Cells[2].Y = 2;

		shape.Cells[3].X = 4 + xoffset;
		shape.Cells[3].Y = 2;
		break;
	case 6:			//s shape
		shape.Cells[0].X = 2 + xoffset;
		shape.Cells[0].Y = 1;

		shape.Cells[1].X = 4 + xoffset;
		shape.Cells[1].Y = 1;

		shape.Cells[2].X = 0 + xoffset;
		shape.Cells[2].Y = 2;

		shape.Cells[3].X = 2 + xoffset;
		shape.Cells[3].Y = 2;
	default:
		break;
	}


	shape.Length = len;
	shape.Color = col;

	return shape;
}

Shape* newShapeDyn(int xoffset, int len, int col, int type) {
	Shape* s = (Shape*)malloc(sizeof(Shape));

	(*s) = newShape(xoffset, len, col, type);

	return s;
}

int main(void) {
	srand(time(NULL));
	hidecursor();
	PrintGrid();

	bool gameOver = false;

	Shape* allShapes[100];
	int index = -1;
	int hit = 1;

	Point occupiedPoints[1000];
	int occIndex = 0;

	Shape* shape;

	char key;

	while (!gameOver) {
		while (1) {

			if (hit) // if the previous shape hit the ground a new one is created dynamically
			{
				index++;
				int xOffset = 2 * (rand() % 14) + 2; // random even position for x
				allShapes[index] = newShapeDyn(xOffset, 4, 1 + (rand() % 14), 3);//(rand() % 7)); // a new Shape is created
				hit = 0;
			}
			shape = allShapes[index];

			ClearShape(shape->Cells, shape->Length);
			if (_kbhit()) { //is true when a key was pressed
				key = _getch(); //capture the key code and insert into c

				switch (key)
				{
				case 'd':
					MoveRight(shape->Cells, shape->Length, occupiedPoints, occIndex);
					break;

				case 'a':
					MoveLeft(shape->Cells, shape->Length, occupiedPoints, occIndex);
					break;

				case 'e':
					RotateClock90(shape->Cells, shape->Length, 1, occupiedPoints, occIndex);
					break;

				case 'q':
					RotateCounterClock90(shape->Cells, shape->Length, 1, occupiedPoints, occIndex);
					break;

				case 's':
					MoveDown(shape->Cells, shape->Length, occupiedPoints, occIndex);
					break;
				default:
					break;
				}
				PrintShape(shape->Cells, shape->Color, shape->Length);
				continue;

			}

			hit = MoveDown(shape->Cells, shape->Length, occupiedPoints, occIndex);
			PrintShape(shape->Cells, shape->Color, shape->Length);
			Sleep(200);

			if (hit) {
				for (int i = 0; i < 4; i++) {
					occupiedPoints[occIndex] = shape->Cells[i];
					occIndex++;
				}
			}
		}
	}

	return 0;

}