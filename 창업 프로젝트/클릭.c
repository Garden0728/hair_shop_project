#pragma once
#include<Windows.h>
#include<stdio.h>
INPUT_RECORD rec;
DWORD dwNOER;
HANDLE CIN = 0;
void goto_xy(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos); // x�� y�� �Է�
}
void click(int* xx, int* yy) {
	while (1)
	{
		ReadConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &rec, 1, &dwNOER); // �ܼ�â �Է��� �޾Ƶ���.
		if (rec.EventType == MOUSE_EVENT) {// ���콺 �̺�Ʈ�� ���
			if (rec.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) { // ���� ��ư�� Ŭ���Ǿ��� ���
				int mouse_x = rec.Event.MouseEvent.dwMousePosition.X; // X�� �޾ƿ�
				int mouse_y = rec.Event.MouseEvent.dwMousePosition.Y; // Y�� �޾ƿ�
				
				*xx = mouse_x; //x���� �ѱ�
				*yy = mouse_y; //y���� �ѱ�
				break;
			}
		}		
	}
}
void clearconsole() { // system("cls")�� �۵� �ϴµ�? (�˾ƺ� ����)
	COORD Coor = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 100 * 100, Coor, &dw); // �ܼ�â ȭ���� �����.
}
void Mouse() {
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

	DWORD CIN;
	DWORD mode;
	CIN = GetStdHandle(STD_INPUT_HANDLE); //���콺 ��Ȱ��ȭ
	GetConsoleMode(CIN, &mode);
	SetConsoleMode(CIN, mode | ENABLE_MOUSE_INPUT);
	//���콺 �Է¸��� �ٲ�
}
void ExClick() {
	int xx, yy, lr;
	while (1) {
		click(&xx, &yy);
		printf("%3d %3d\n", xx, yy);
	}
}
int main(void) {
	Mouse();
	ExClick();
}