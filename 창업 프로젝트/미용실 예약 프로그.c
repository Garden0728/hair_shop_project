#define _CRT_SECURE_NO_WARNINGS
#define MAX 200 //ȸ�� �ִ� �ο� ��
#include<stdio.h>
#include<Windows.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>
#include"resource.h"
#define MAX_X 174
#define MAX_Y 40
INPUT_RECORD rec;
DWORD dwNOER;
HANDLE CIN = 0;
int member_count = 0;
int designer_count = 0;
int design_count = 0;
int take_menu_count = 0;
typedef struct { //�����̳ʿ� ���� �޴� �����ֱ� ����
	int index;
	char name[20];
	int price;
	char account[240];
}d_menu;
d_menu D_MENU[20];

// �ܼ� ������ â�� �ڵ鰪    
static HWND hWnd;
// ���α׷��� �ν��Ͻ� �ڵ鰪
static HINSTANCE hInst;

typedef struct { // ȸ�� ����ü
	char name[20];
	char phone[20];
	char gender[5];
	int brith;
	char pw[15];
}member;
member all[MAX];
typedef struct {
	char name[20];
	char phone[20];
	char gender[15];
	int brith;
	char n_name[15];
	char introduce[50];
}DESIGNER;
DESIGNER d_all[3];
typedef struct {
	char designer_name[20];
	char sort[15];
	char name[20];
	int price;
	char account[220];
}style;
style STYLE[100];
void long_box_UI(int x, int y) {
	goto_xy(x, y);
	printf("��");
	for (int i = 0; i < 30; i++)
	{
		printf("��");
	}
	printf("��");

	for (int i = 0; i < 1; i++) {
		y += 1;
		goto_xy(x, y);
		printf("��                                                            ��");
	}
	goto_xy(x, y + 1);
	printf("��");
	for (int i = 0; i < 30; i++)
	{
		printf("��");
	}
	printf("��");
}
int getMaskedInput() { //�����̳� ��й�ȣ ���� �Է� �ҋ� *�� ��� ���ִ� �Լ�
	int birth = 0;
	int ch;
	int i = 0;

	while (i < 8) {
		ch = _getch();
		if (ch == 13) { // ���͸� ������ ��
			break; // �Է� ����
		}
		if (ch >= '0' && ch <= '9') {
			putchar('*');
			birth = birth * 10 + (ch - '0');
			i++;
		}
		else if (ch == 8 && i > 0) { // �齺���̽��� ������ ��
			putchar('\b'); // �齺���̽� ���� ���
			putchar(' ');  // ���� ���ڷ� ���
			putchar('\b'); // �齺���̽� ���� ���
			birth = birth / 10; // ���� ������ �ڸ� ����
			i--;
		}
	}

	return birth;
}
void clearCurrentChar(int x, int y) {
	goto_xy(x, y);
	printf(" "); // Print a space to clear the character
	goto_xy(x, y); // Move the cursor back to the original position
}

void handleNewline(int* x, int* y) {
	(*y)++;
	goto_xy(*x, *y); // Move the cursor to the new position
}

void handleBackspace(char* str, int* len, int* x, int* y) {
	if (*len > 0) {
		(*len)--;
		clearCurrentChar((*x) + (*len) % MAX_X, *y); //�̺κ�
		str[(*x - 124) + (*len) + (MAX_X - 124) * (*y - 36)] = '\0'; // Remove the deleted character from the string
		goto_xy(*x + (*len) % MAX_X, *y); // Move the cursor back to the original position
	}
	else {
		if (*y > 36) { // Check if y is greater than 0 (not on the first line)
			(*y)--;
			clearCurrentChar(MAX_X - 1, *y); // Clear the last character on the current line
			goto_xy(*x, *y);
			*len = (MAX_X - 124) - 1; // Set len to the last index of the previous line
		}
		else {
			return;
		}
	}
}

void file_read() { // ���� �о ����ü�� �����ϴ� �Լ�
	char c;
	FILE* member = fopen("member.txt", "r");
	if (member == NULL) {
		return 0;
	}
	else {
		while (c = fgetc(member) != EOF) {

			fseek(member, -1, SEEK_CUR);
			if (feof(member) != 0) {
				break;
			}
			fscanf(member, "%s %s %s %d %s\n", all[member_count].name, all[member_count].phone, all[member_count].gender, &all[member_count].brith, all[member_count].pw);
			member_count++;
		}
	}
}
void file_append() { //���� �̾�� �Լ�
	FILE* member;
	member = fopen("member.txt", "a");
	fprintf(member, "%s %s %s %d %s\n", all[member_count].name, all[member_count].phone, all[member_count].gender, all[member_count].brith, all[member_count].pw);
	fclose(member);
}
void file_write() { // �����̳� ���� ����
	FILE* member;
	member = fopen("member.txt", "w");
	for (int i = 0; i < member_count; i++) {
		fprintf(member, "%s %s %s %d %s\n", all[i].name, all[i].phone, all[i].gender, all[i].brith, all[i].pw);

	}
	fclose(member);
}
void d_file_read() { // �����̳� ���� �б�
	char c;
	FILE* designer = fopen("designer.txt", "r");
	if (designer == NULL) {
		return 0;
	}
	else {
		while (c = fgetc(designer) != EOF) {

			fseek(designer, -1, SEEK_CUR);
			if (feof(designer) != 0) {
				break;
			}
			fscanf(designer,"%s %s %s %d %s %[^\n]\n", d_all[designer_count].name, d_all[designer_count].phone, d_all[designer_count].gender, &d_all[designer_count].brith, d_all[designer_count].n_name, d_all[designer_count].introduce);
			designer_count++;
		}
	}
}
void d_file_write() { // �����̳� ���� ����
	FILE* designer = fopen("designer.txt", "w");
	for (int i = 0; i < designer_count; i++) {
		fprintf(designer, "%s %s %s %d %s %s\n", d_all[i].name, d_all[i].phone, d_all[i].gender, d_all[i].brith, d_all[i].n_name, d_all[i].introduce);
	}
	fclose(designer);
}
void design_file_read() { //������ ���� �б�
	char c;
	FILE* design = fopen("design.txt", "r");
	if (design == NULL) {
		return 0;
	}
	else {
		while (c = fgetc(design) != EOF) {

			fseek(design, -1, SEEK_CUR);
			if (feof(design) != 0) {
				break;
			}
			fscanf(design,"%s %s %s %d/%[^\n]\n", STYLE[design_count].designer_name, STYLE[design_count].sort, STYLE[design_count].name, &STYLE[design_count].price, &STYLE[design_count].account);
			design_count++;
		}
	}
	fclose(design);
}
void design_file_append() {
	FILE* design;
	design = fopen("design.txt", "a");
	fprintf(design, "%s %s %s %d/%s\n", STYLE[design_count].designer_name, STYLE[design_count].sort, STYLE[design_count].name, STYLE[design_count].price, STYLE[design_count].account);
	fclose(design);
}
void HideCursor() { //���콺 Ŀ�� ����� �Լ�
	CONSOLE_CURSOR_INFO cursor_info = { 1, FALSE };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
void EnableConsoleCursor() { //���콺 Ŀ�� ���̰� �ϴ� �Լ�
	CONSOLE_CURSOR_INFO cursor_info = { 1, TRUE };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
void design_take_menu(int index, char *str) {
	take_menu_count = 0;
	for (int i = 0; i < 20; i++) {
		D_MENU[i].index = 0;
		strcpy(D_MENU[i].name, " ");
		D_MENU[i].price = 0;
		strcpy(D_MENU[i].account," ");
	}
	for (int i = 0; i < design_count; i++) {
		if (strcmp(d_all[index].name, STYLE[i].designer_name) == 0 && strcmp(str, STYLE[i].sort) == 0) {
			D_MENU[take_menu_count].index = i;
			D_MENU[take_menu_count].price = STYLE[i].price;
			strcpy(D_MENU[take_menu_count].name, STYLE[i].name);
			strcpy(D_MENU[take_menu_count].account, STYLE[i].account);
			take_menu_count++;
		}
	}
}
void textcolor(int colorNum) { // �۾� �÷� �ٲ��ִ� �Լ�
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}
int goto_xy(int x, int y)  // ��ǥ��
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos); // x�� y�� �Է�
}
void click(int* xx, int* yy) { // ���콺 Ŭ�� �̺�Ʈ 
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
enum ColorType {  // �۾� �÷� 
	BLACK,  	//0
	darkBLUE,	//1
	DarkGreen,	//2
	darkSkyBlue,    //3
	DarkRed,  	//4
	DarkPurple,	//5
	DarkYellow,	//6
	GRAY,		//7
	DarkGray,	//8
	BLUE,		//9
	GREEN,		//10
	SkyBlue,	//11
	RED,		//12
	PURPLE,		//13
	YELLOW,		//14
	WHITE		//15
} COLOR;
void clearconsole() { //�ܼ�â Ŭ����
	COORD Coor = { 0, 0 };
	DWORD dw;
	FillConsoleOutputCharacter(GetStdHandle(STD_OUTPUT_HANDLE), ' ', 100 * 100, Coor, &dw); // �ܼ�â ȭ���� �����.
}
void Mouse() { //���콺 Ȱ��ȭ �ڵ�
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT);

	DWORD CIN;
	DWORD mode;
	CIN = GetStdHandle(STD_INPUT_HANDLE); //���콺 ��Ȱ��ȭ
	GetConsoleMode(CIN, &mode);
	SetConsoleMode(CIN, mode | ENABLE_MOUSE_INPUT);
	//���콺 �Է¸��� �ٲ�
}
void ExClick() { // Ŭ�� ��ǥ �� Ȯ�� UI
	int xx = 0, yy = 0, lr = 0;
	while (1) {
		click(&xx, &yy);
		printf("%3d %3d\n", xx, yy);
	}
}
void add_box_UI(int x, int y, int color, int xx, int yy, char* str) {
	textcolor(color);
	goto_xy(x, y);
	printf("��");
	for (int i = 0; i < 26; i++)
	{
		printf("��");
	}
	printf("��");

	for (int i = 0; i < 8; i++) {
		y += 1;
		goto_xy(x, y);
		printf("��                                                    ��");
	}
	goto_xy(x, y + 1);
	printf("��");
	for (int i = 0; i < 26; i++)
	{
		printf("��");
	}
	printf("��");
	textcolor(4);
	goto_xy(xx, yy);
	printf("%s", str);
}
void basic_UI_DELETE(int x, int y) {
	while(1) {
		goto_xy(x, y);
		printf("                                                                                                            ");
		y++;
		if (y == 50) {
			break;
		}
	}
}
void basic_UI(int x, int y) { // �⺻ UI
	textcolor(DarkYellow);
	goto_xy(x, y);
	printf("��");
	for (int i = 0; i < 35; i++)
	{
		printf("��");
	}
	printf("��");
	for (int i = 0; i < 45; i++) {
		y += 1;
		if (i == 3 || i == 40) {
			goto_xy(x, y);
			printf("��");
			for (int i = 0; i < 35; i++) {
				printf("��");
			}
			printf("��");
		}
		else {
			goto_xy(x, y);
			printf("��                                                                      ��");
		}
	}
	goto_xy(x, y + 1);
	printf("��");
	for (int i = 0; i < 35; i++)
	{
		printf("��");
	}
	printf("��");
}
void m_basic_UI() { // �α��� �� ȸ�� �⺻ UI
	int x = 30;
	int y = 3;
	textcolor(DarkYellow);
	goto_xy(x, y);
	printf("��");
	for (int i = 0; i < 35; i++)
	{
		printf("��");
	}
	printf("��");
	for (int i = 0; i < 45; i++) {
		y += 1;
		if (i == 3 || i == 7 || i == 40) {
			goto_xy(x, y);
			printf("��");
			for (int i = 0; i < 35; i++) {
				printf("��");
			}
			printf("��");
		}
		else {
			goto_xy(x, y);
			printf("��                                                                      ��");
		}
	}
	goto_xy(x, y + 1);
	printf("��");
	for (int i = 0; i < 35; i++)
	{
		printf("��");
	}
	printf("��");
}
void small2_box(int x, int y, int color, int xx, int yy, char* str, int color2) {
	textcolor(color);
	goto_xy(x, y);
	printf("��");
	for (int i = 0; i < 8; i++)
	{
		printf("��");
	}
	printf("��");

	for (int i = 0; i < 1; i++) {
		y += 1;
		goto_xy(x, y);
		printf("��                ��");
	}
	goto_xy(x, y + 1);
	printf("��");
	for (int i = 0; i < 8; i++)
	{
		printf("��");
	}
	printf("��");
	textcolor(color2);
	goto_xy(xx, yy);
	printf("%s", str);
}
void small_box(int x, int y, int color, int xx, int yy, char* str, int color2) { // üũ �ڽ� UI
	textcolor(color);
	goto_xy(x, y);
	printf("��");
	for (int i = 0; i < 6; i++)
	{
		printf("��");
	}
	printf("��");

	for (int i = 0; i < 1; i++) {
		y += 1;
		goto_xy(x, y);
		printf("��            ��");
	}
	goto_xy(x, y + 1);
	printf("��");
	for (int i = 0; i < 6; i++)
	{
		printf("��");
	}
	printf("��");
	textcolor(color2);
	goto_xy(xx, yy);
	printf("%s", str);

}
void big_box(int x, int y, int color, int xx, int yy, char* str) { // üũ�ڽ� ū�� UI
	textcolor(color);
	goto_xy(x, y);
	printf("��");
	for (int i = 0; i < 9; i++)
	{
		printf("��");
	}
	printf("��");

	for (int i = 0; i < 3; i++) {
		y += 1;
		goto_xy(x, y);
		printf("��                  ��");
	}
	goto_xy(x, y + 1);
	printf("��");
	for (int i = 0; i < 9; i++)
	{
		printf("��");
	}
	printf("��");
	textcolor(6);
	goto_xy(xx, yy);
	printf("%s", str);
}

void box_clear() { //�⺻ UI Ŭ���� ���ִ� �Լ�
	int x = 62;
	int y = 5;
	goto_xy(60, 5);
	printf("                   ");
	x = 32;
	y = 8;
	for (int i = 0; i < 35; i++) {
		goto_xy(x, y);
		printf("                                                            ");
		y += 1;
	}
	x = 32;
	y = 45;
	for (int i = 0; i < 4; i++) {
		goto_xy(x, y);
		printf("                                                                     ");
		y += 1;
	}

}
void Render(int x, int y, int num)
{
	const int pictureWidth = 130;
	const int pictureHeight = 150;
	// DC�� �ڵ鰪�� ���� ������ �����Ѵ�.(hDC : ����ȭ��DC, hMemDC : �̹����� ���� DC)
	// Device Context�� �׷��ȿ� �ʿ��� ��� �ɼ��� �־�� ����ü��� �� �� �ִ�.
	// �׸��׸��� �׸��� �׸� ȭ���̶� ����ȴ�.
	HDC hDC, hMemDC;
	// �ĸ�����̴�. static ������ ���������μ�, �޸��� Data������ ����Ǵ� �Լ��� ������ ������� �ʰ� �޸𸮿� �����ִ� �����̴�.
	static HDC hBackDC;
	// �̹��� ��Ʈ���� �ڵ鰪�� ������ �����̴�.
	HBITMAP hBitmap = NULL, hOldBitmap, hBackBitmap;
	// �ڵ鰪���κ��� ���� ���� ��Ʈ�� ����ü.
	BITMAP Bitmap;
	// ���� �������� Rect��(ũ��)�� ���´�. Rect�� ������, ������ �Ʒ��� ���� ������ ���簢���� ��Ÿ���� ����ü�̴�.
	RECT WindowRect;
	GetWindowRect(hWnd, &WindowRect);

	// ���� �������� DC �ڵ鰪�� ���´�. GetWindowDC(hWnd)�� �����ϴ�.
	hDC = GetDC(hWnd);
	// hDC�� ȣȯ�Ǵ� DC�� �޸𸮿� ����� �ڵ鰪�� ��ȯ�Ѵ�.
	hBackDC = CreateCompatibleDC(hDC);
	hMemDC = CreateCompatibleDC(hDC);

	// ��Ʈ�� �޸𸮸� �Ҵ��ϰ� �ڵ��� ��ȯ�Ѵ�.
	hBackBitmap = CreateCompatibleBitmap(hDC, WindowRect.right, WindowRect.bottom);
	// �׸� ��ȭ���� �غ��Ѵ�.
	hOldBitmap = (HBITMAP)SelectObject(hBackDC, hBackBitmap);
	// ��Ʈ���� �ε��Ͽ� �ڵ��� ��ȯ�Ѵ�. resource.h�� ������ define�Ǿ��ִ� �ҷ��� ���ҽ��� �ε��Ѵ�.
	if (num == 0) {
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	}
	else if (num == 1) {
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP2));
	}
	else if (num == 2) {
		hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP3));
	}
	// ũ�⸦ �ޱ����� ��Ʈ�� ����ü�� ��Ʈ�� �ڵ�κ��� �����Ѵ�.
	GetObject(hBitmap, sizeof(BITMAP), &Bitmap);

	HBITMAP hResizedBitmap = (HBITMAP)CopyImage(hBitmap, IMAGE_BITMAP, pictureWidth, pictureHeight, LR_COPYDELETEORG);
	GetObject(hResizedBitmap, sizeof(BITMAP), &Bitmap);
	// �о�� ��Ʈ�� ���ҽ��� �޸� DC�� �����Ѵ�.
	SelectObject(hMemDC, hResizedBitmap);

	// hMemDC�� �̹����� hBackDC�� ���ϴ� ��ġ�� ��Ӻ����Ų��.(����Ϸ��� �̹����� �ĸ���ۿ� �����Ų��.)
	BitBlt(hBackDC, 0, 0, Bitmap.bmWidth, Bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);
	// hBackDC(�ĸ� ����)�� �ϼ��� �׸��� ȭ������ ��Ӻ����Ų��.
	BitBlt(hDC, x, y, x + Bitmap.bmWidth, y + Bitmap.bmHeight, hBackDC, 0, 0, SRCCOPY);

	// �޸𸮿� ������Ʈ�� �����Ѵ�.
	DeleteObject(SelectObject(hBackDC, hBackBitmap));
	DeleteObject(hResizedBitmap);
	DeleteDC(hBackDC);
	DeleteDC(hMemDC);

	ReleaseDC(hWnd, hDC);
}
int buid(int num) {
	// �׽�Ʈ������ �Է��� ���� ����
	char buf[100] = { 0, };
	int i = 0;
	// �ܼ� ������ â�� �ڵ鰪�� ���´�.
	hWnd = GetConsoleWindow();
	// ���α׷��� �ν��Ͻ� �ڵ鰪�� ���´�.
	hInst = GetModuleHandle(NULL);
	// ������ ����.
	bool isFinished = true;
	while (1) {
		// �׸��� �׸���.
		Render(10, 10, num);

		// �׸� �׸��� �۾��� ó������ �Ϸ�Ǿ��ٰ� �����ϰ� isFinished�� true�� �ʱ�ȭ�Ѵ�.

		if (isFinished) {
			break; // �ݺ����� �����Ѵ�.
		}
	}
}
int add_design(int index,char* string) {
	basic_UI(108,3);
	int xx, yy = 0;
	char sort[15] = " ";
	char name[20] = " ";
	int price = 0;
	char str[240] = " ";// ���ڿ��� ���� �� 100����
	int len = 0;
	int x = 124;
	int y = 36; 
	char ch = ' ';
	small_box(158, 46, 6, 164, 47, "���", 6);
	textcolor(6);
	goto_xy(177, 4);
	printf("[X]");
	goto_xy(140, 5);
	textcolor(6);
	printf("������ �߰�");
	goto_xy(122, 13);
	printf("����      :");
	strcpy(sort, string);
	goto_xy(143, 13);
	EnableConsoleCursor();
	printf("%s", sort);
	HideCursor();
	goto_xy(122, 19);
	printf("�̸�      :");
	goto_xy(122, 25);
	printf("����      :");
	goto_xy(122, 31);
	printf("���� (�Է� �� ENTER) ");
	add_box_UI(122,33,6,143,37,"���ִ� 100����");
	textcolor(6);
	//ExClick();
	while (1) {
		xx = 0, yy = 0;
		click(&xx, &yy);
		if (xx > 174 && xx < 182) {
			if (yy > 2 && yy < 6) {
				textcolor(4);
				goto_xy(177, 4);
				printf("[X]");
				Sleep(500);
				basic_UI_DELETE(108, 3);
				return;
			}
		}
		if (xx > 132 && xx < 171) {			
			if (yy > 17 && yy < 21) {
				strcpy(name, "");
				goto_xy(140, 19);
				printf("                                   ");
				goto_xy(143, 19);
				EnableConsoleCursor();
				gets(name);
				HideCursor();
				if (strcmp(name, "") == 0) {
					goto_xy(143, 19);
					printf("������ �ùٸ��� �ʽ��ϴ�.");
				}
			}
			else if (yy > 23 && yy < 27) {
				price = 0;
				char input[30]= "";
				strcpy(input,"");
				goto_xy(140, 25);
				printf("                                   ");
				goto_xy(143, 25);
				EnableConsoleCursor();
				fgets(input, sizeof(input), stdin);
				HideCursor();

				if (input[0] == '\n') {
					price = 0;
				}
				else {
					price = atoi(input);
				}

				if (price == 0) {
					goto_xy(143, 25);
					printf("������ �ùٸ��� �ʽ��ϴ�.");
				}
			}
		
		}
		if (xx > 122 && xx < 175) {
			if (yy > 32 && yy < 43) {
				strcpy(str," ");
				add_box_UI(122, 33, 6, 143, 37, " ");// ������� ���� ���°� �ؾ���
				len = 0;
				x = 124;
				y = 36;
				EnableConsoleCursor();
				goto_xy(x, y);
				textcolor(6);
				while (1) {
					 ch = ' ';
					 ch = _getch();
					if (ch == '\r') { // Enter key
						ch = ' ';
						break;
					}
					else if (ch == '\b') { // Backspace key
						if (str[(x - 124) + (len)+(MAX_X - 124) * (y - 36) - 1] & 0x80) {
							if (str[(x - 124) + (len)+(MAX_X - 124) * (y - 36) - 2] & 0x80){
								handleBackspace(str, &len, &x, &y);
								handleBackspace(str, &len, &x, &y);
							}
							else {
								handleBackspace(str, &len, &x, &y);
							}
						}
						else {
							handleBackspace(str, &len, &x, &y);
						}
					}
					else if (len < sizeof(str) - 2) {
						if (len >= MAX_X - 124 && (len % (MAX_X - 124)) == 0) { // Check if the line length is multiple of MAX_X
							if (y >= MAX_Y - 1) {
								continue;
							}
							else {

								handleNewline(&x, &y);
								len = 0;
								str[(x - 124) + (len)+((MAX_X - 124) * (y - 36))] = ch;
								len++;
								str[(x - 124) + (len)+((MAX_X - 124) * (y - 36))] = '\0';
								printf("%c", ch);
							}
						}
						else {
							str[(x - 124) + (len)+((MAX_X - 124) * (y - 36))] = ch;
							len++;
							str[(x - 124) + (len)+((MAX_X - 124) * (y - 36))] = '\0';
							printf("%c", ch);
						}
					}
				}
				HideCursor();
			}
		}
		if (xx > 158 && xx < 174) {
			if (yy > 45 && yy < 49) {
				if (strcmp(name, " ") != 0 && strcmp(sort, " ") != 0 && price != 0) {
					small_box(158, 46, 10, 164, 47, "���", 6);
					Sleep(500);
					small_box(158, 46, 6, 164, 47, "���", 6);
					strcpy(STYLE[design_count].designer_name, d_all[index].name);
					strcpy(STYLE[design_count].name, name);
					strcpy(STYLE[design_count].sort,sort);
					STYLE[design_count].price = price;
					strcpy(STYLE[design_count].account,str);
					design_file_append();
					design_count++;
					clearconsole();
					Sleep(1500);
					return 1;
				}
			}
		}
	}
}
int style_management(int index) {
	char str[15] = "��";
	int page_count = 1;
	int choice = 1;
	int x = 35, y = 12;
	box_clear();
	int i = 0;
	int count = 6;
	int len = 0;
	while (1) {
		x = 35, y = 12;
		basic_UI(30, 3);
		int xx, yy = 0;
		int check = 0;
		goto_xy(61, 5);
		textcolor(6);
		printf("������ ����");
		small_box(35, 8, 6, 41, 9, "ĿƮ", 6);
		small_box(51, 8, 6, 58, 9, "��", 6);
		small_box(67, 8, 6, 73, 9, "�÷�", 6);
		small_box(83, 8, 6, 89, 9, "��Ÿ", 6);
		goto_xy(61, 39);
		printf("��");
		goto_xy(66, 39);
		printf("%d", page_count);
		goto_xy(71, 39);
		printf("��");
		small_box(38, 46, 6, 44, 47, "����", 6);
		small_box(80, 46, 6, 86, 47, "�߰�", 6);
		//ExClick();
		if (choice == 1) {
			strcpy(str,"ĿƮ");
			small_box(35, 8, 10, 41, 9, "ĿƮ", 6);
		}
		else if (choice == 2) {
			strcpy(str, "��");
			small_box(51, 8, 10, 58, 9, "��", 6);
		}
		else if (choice == 3) {
			strcpy(str, "�÷�");
			small_box(67, 8,10, 73, 9, "�÷�", 6);
		}
		else if (choice == 4) {
			strcpy(str, "��Ÿ");
			small_box(83, 8, 10, 89, 9, "��Ÿ", 6);
		}
		design_take_menu(index,str);
		for (i; i < count*page_count; i++) {
			x = 35;
			if (strcmp(D_MENU[i].name, " ") == 0) {
				break;
			}
			else {
				long_box_UI(x, y);
				x = 39;
				goto_xy(x, y+1);
				printf("%s", D_MENU[i].name);
				goto_xy(x+15, y+1);
				printf("%d", D_MENU[i].price);
				goto_xy(x + 30, y+1);
				if (strcmp(D_MENU[i].account," ")==0) {
					goto_xy(x + 39, y + 1);
					printf("X");
				}
				else {
					len = strlen(D_MENU[i].account);
					for (int k = 0; k < len; k++) {
						if (k > 19) {
							if (D_MENU[i].account[k] & 0x80) {
								printf("%c", D_MENU[i].account[k]);
							}
							printf("...");
							break;
						}
						else {
							printf("%c", D_MENU[i].account[k]);
						}
					}
				}
				y += 4;
			}
		}
		if (i == (count * page_count) - 1) {
			i++;
		}

		while (1) {
			xx = 0, yy = 0;
			click(&xx, &yy);
			if (xx > 38 && xx < 53) {
				if (yy > 45 && yy < 49) {
					small_box(38, 46, 10, 44, 47, "����", 6);
					Sleep(500);
					return;
				}
			}
			if (xx > 59 && xx < 65 && yy > 37 && yy < 41) {
				if (page_count != 1) {
					textcolor(10);
					goto_xy(61, 39);
					printf("��");
					Sleep(500);
					i = (page_count - 2) * count;
					page_count--;
					break;
				}
			}
			if (xx > 67 && xx < 74 && yy > 37 && yy < 41) {
				if (strcmp(D_MENU[i].name, " ") != 0) {
					textcolor(10);
					goto_xy(71, 39);
					printf("��");
					Sleep(500);
					page_count++;
					break;
				}
			}
			if (xx > 80 && xx < 95) {
				if (yy > 45 && yy < 49) {
					small_box(80, 46, 10, 86, 47, "�߰�", 6);
					Sleep(500);
					small_box(80, 46, 6, 86, 47, "�߰�", 6);
					check = add_design(index,str);
					if (check == 1) {
						i = 0;
						page_count = 1;
						break;
					}
				}
			}
			if (yy > 6 && yy < 11) {
				if (xx > 34 && xx < 50) {
					if (choice != 1) {
						choice = 1;
						i = 0;
						page_count = 1;
						break;
					}
				}
				else if (xx > 50 && xx < 67) {
					if (choice != 2) {
						choice = 2;
						i = 0;
						page_count = 1;
						break;
					}
				}
				else if (xx > 67 && xx < 82) {
					if (choice != 3) {
						choice = 3;
						i = 0;
						page_count = 1;
						break;
					}
				}
				else if (xx > 83 && xx < 98) {
					if (choice != 4) {
						choice = 4;
						i = 0;
						page_count = 1;
						break;
					}
				}
			}

		}
	}
}
void copy() {
	char sourcePath[100];  // �Է� ���� ��θ� ������ ����
	char destinationPath[] = "C:\\Users\\chlwj\\source\\repos\\�׸�\\�׸�\\bitmap2.bmp";
	goto_xy(5, 40);
	printf("���� ��θ� �Է��ϼ���: ");
	fgets(sourcePath, sizeof(sourcePath), stdin);
	sourcePath[strcspn(sourcePath, "\n")] = '\0';  // ���� ���� ����

	// ���� ���� ����
	FILE* sourceFile = fopen(sourcePath, "rb");
	FILE* destinationFile = fopen(destinationPath, "wb");

	if (sourceFile == NULL) {
		printf("�Է��� ��ο� ������ ã�� �� �����ϴ�.\n");
		return 1;
	}

	if (destinationFile == NULL) {
		printf("������ ��ο� ������ ������ �� �����ϴ�.\n");
		fclose(sourceFile);
		return 1;
	}

	int bufferSize = 1024;  // ������ �� ����� ���� ũ��
	char* buffer = (char*)malloc(bufferSize);

	if (buffer == NULL) {
		printf("�޸� �Ҵ翡 �����߽��ϴ�.\n");
		fclose(sourceFile);
		fclose(destinationFile);
		return 1;
	}

	size_t bytesRead;
	while ((bytesRead = fread(buffer, 1, bufferSize, sourceFile)) > 0) {
		fwrite(buffer, 1, bytesRead, destinationFile);
	}

	printf("������ ���������� ����Ǿ����ϴ�.\n");

	fclose(sourceFile);
	fclose(destinationFile);
	free(buffer);

}
int isValidPhone_or_pw_Number(char* str, int check) { // ��ȭ��ȣ ��й�ȣ ��ȿ���� üũ���ִ� �Լ�
	if (check == 1) {
		int length = strlen(str);

		// ��ȭ��ȣ�� 11�ڸ����� ��
		if (length != 11)
			return 0;

		// ��ȭ��ȣ�� "010xxxxxxxx" �����̾�� ��
		if (str[0] != '0' || str[1] != '1' || str[2] != '0')
			return 0;

		// ������ �ڸ��� ���ڿ��� ��
		for (int i = 3; i < length; i++) {
			if (str[i] < '0' || str[i] > '9')
				return 0;
		}

		// ��� ������ �����ϴ� ��� ��ȿ�� ��ȭ��ȣ
		return 1;
	}
	else if(check == 2) {
		int length = strlen(str);
		if (length != 4) {
			return 0;
		}
		else {
			for (int i = 0; i < length; i++) {
				if (str[i] < '0' || str[i] > '9') {
					return 0;
				}
				else {
					return 1;
				}
			}
		}
		
	}
}
int isValidDate(int date) { //������� ��ȿ���� üũ���ִ� �Լ�
	int year = date / 10000;
	int month = (date % 10000) / 100;
	int day = date % 100;

	// ��������� 8�ڸ����� ��
	if (date < 10000000 || date > 99999999)
		return 0;

	// ����, ��, ���� ������ Ȯ��
	if (year < 1900 || month < 1 || month > 12 || day < 1 || day > 31)
		return 0;

	// ��� ������ �����ϴ� ��� ��ȿ�� �������
	return 1;
}
int designer_choice(int index) {
	d_file_read();
}
int designer_initial_screen(int index) { //�����̳� �ʱ� ȭ��
	int xx, yy, lr = 0;
	int choice = 0;
	while (1) {
		box_clear();
		m_basic_UI();
		xx = 0, yy = 0;
		goto_xy(63, 5);
		textcolor(6);
		printf("%s��",d_all[index].name);
		big_box(57, 14, 6, 64, 16, "���� ����");
		big_box(57, 22, 6, 63, 24, "������ ����");
		big_box(57, 30, 6, 63, 32, "������ ����");
		big_box(57, 38, 6, 64, 40, "���� ����");
		small_box(38, 46, 6, 42, 47, "�α׾ƿ�", 6);
		small_box(80, 46, 6, 86, 47, "����", 6);
		//ExClick();
		while (1) {
			xx = 0, yy = 0;
			click(&xx, &yy);
			if (xx > 38 && xx < 53) {
				if (yy > 45 && yy < 49) {
					small_box(38, 46, 10, 42, 47, "�α׾ƿ�", 6);
					Sleep(500);
					return;
				}
			}
			if (xx > 57 && xx < 78) {
				if (yy > 21 && yy < 27) {
					big_box(57, 22, 10, 63, 24, "������ ����");
					big_box(57, 14, 6, 64, 16, "���� ����");
					big_box(57, 30, 6, 63, 32, "������ ����");
					big_box(57, 38, 6, 64, 40, "���� ����");
					choice = 2;
					xx, yy = 0;
				}
			}
			if (xx > 80 && xx < 95) {
				if (yy > 45 && yy < 49) {
					if (choice != 0) {
						small_box(80, 46, 10, 86, 47, "����", 6);
						Sleep(500);
						if (choice == 2) {
							style_management(index);
							break;
						}
					}
				}
			}
			xx = 0, yy = 0;
		}
		xx = 0, yy = 0;
	}
}
int designer_login() { //�����̳� �α��� 
	d_file_read();
	box_clear();
	goto_xy(60, 5);
	textcolor(6);
	printf("�����̳� �α���");
	small_box(38, 46, 6, 44, 47, "����", 6);
	small_box(80, 46, 6, 85, 47, "�α���", 6);
	char phone[20] = " ";
	int brith = 0;
	int xx = 0, yy = 0, lr = 0;
	int check = 0;
	goto_xy(53, 19);
	printf("��ȭ��ȣ (11�ڸ�) ex) 010xxxxxxxx ");
	goto_xy(53, 21);
	printf(":");
	goto_xy(53, 28);
	printf("������� (8�ڸ�) ex) xxxxxxxx");
	goto_xy(53, 30);
	printf(":");
	while (1) {
		xx = 0, yy = 0;
		click(&xx, &yy);
		if (xx > 38 && xx < 53) {
			if (yy > 45 && yy < 49) {
				small_box(38, 46, 10, 44, 47, "����", 6);
				Sleep(500);
				return;
			}
		}
		if (xx > 53 && xx < 85) {
			if (yy > 19 && yy < 23) {
				textcolor(6);
				goto_xy(55, 21);
				printf("                                 ");
				goto_xy(55, 21);
				EnableConsoleCursor();
				gets(phone);
				HideCursor();
				check = isValidPhone_or_pw_Number(phone, 1);
				if (check == 0) {
					strcpy(phone, " ");
					goto_xy(55, 21);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}

			}
		}
		if (xx > 53 && xx < 85) {
			if (yy > 28 && yy < 32) {
				textcolor(6);
				goto_xy(55, 30);
				printf("                                 ");
				goto_xy(55, 30);
				EnableConsoleCursor();
				brith = getMaskedInput();
				HideCursor();
				check = isValidDate(brith);
				if (check == 0) {
					brith = 0;
					goto_xy(55, 30);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}
			}
		}
		if (xx > 80 && xx < 95) {
			if (yy > 45 && yy < 49) {
				if (brith != 0 && strcmp(phone, " ") != 0) {
					small_box(80, 46, 10, 85, 47, "�α���", 6);
					check = -1;
					for (int i = 0; i < designer_count; i++) {
						if (strcmp(d_all[i].phone, phone) == 0 && d_all[i].brith == brith) {
							check = i;
							break;
						}
					}
					Sleep(500);
					if (check >= 0) {
						designer_initial_screen(check);
						return 1;
					}
					else {
						small_box(80, 46, 6, 85, 47, "�α���", 6);
						goto_xy(48, 40);
						textcolor(4);
						printf("��ȭ��ȣ Ȥ�� ��й�ȣ�� �ٽ� �Է� ���ּ���.");
					}
				}

			}

		}
		xx = 0, yy = 0;
	}
}
void modifying_membership(int index) { //ȸ������ ����
	int n_len = 0;
	int pw_check = 1;
	int ph_check = 1;
	int xx, yy, lr = 0;
	char name[20] = " ";
	char phone[15] = " ";
	char pw[15] = " ";
	basic_UI(30,3);
	goto_xy(62, 5);
	textcolor(6);
	printf("ȸ������");
	small_box(38, 46, 6, 44, 47, "����", 6);
	small_box(80, 46, 6, 86, 47, "����", 6);
	goto_xy(51, 11);
	textcolor(4);
	printf("�̸�, ��ȭ��ȣ, ��й�ȣ�� ���� ����");
	textcolor(6);
	goto_xy(51, 17);
	printf("�̸� :");
	goto_xy(68, 17);
	printf("%s", all[index].name);
	goto_xy(51, 22);
	printf("��ȭ��ȣ :");
	goto_xy(68, 22);
	printf("%s", all[index].phone);
	goto_xy(51, 27);
	printf("���� :");  
	goto_xy(68, 27);
	if (strcmp(all[index].gender, "��") == 0) {\
		printf("����");
	}
	else {
		printf("����");
	}
	goto_xy(51, 32);
	printf("�������(8�ڸ�) :");
	goto_xy(68, 32);
	printf("%d", all[index].brith);
	goto_xy(51, 37);
	printf("��й�ȣ(4�ڸ�) :");
	goto_xy(68, 37);
	printf("%s", all[index].pw);
	strcpy(name, all[index].name);
	strcpy(pw,all[index].pw);
	strcpy(phone,all[index].phone);
	while (1) {
		xx = 0, yy = 0;
		click(&xx, &yy);
		if (xx > 38 && xx < 53) {
			if (yy > 45 && yy < 49) {
				small_box(38, 46, 10, 44, 47, "����", 6);
				Sleep(500);
				return;
			}
		}
		if (xx > 56 && xx < 87) {
			if (yy > 15 && yy < 19) {
				textcolor(6);
				goto_xy(58, 17);
				printf("                                    ");
				goto_xy(68, 17);
				EnableConsoleCursor();
				scanf("%s", name);
				HideCursor();
			}
		}
		if (xx > 60 && xx < 87) {
			if (yy > 20 && yy < 24) {
				textcolor(6);
				goto_xy(65, 22);
				printf("                                 ");
				goto_xy(68, 22);
				EnableConsoleCursor();
				scanf("%s", phone);
				HideCursor();
				ph_check = isValidPhone_or_pw_Number(phone, 1);
				if (ph_check == 0) {
					strcpy(phone, all[index].phone);
					goto_xy(68, 22);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}

			}
		}
		if (xx > 67 && xx < 87) {
			if (yy > 35 && yy < 39) {
				textcolor(6);
				goto_xy(68, 37);
				printf("                                 ");
				goto_xy(68, 37);
				EnableConsoleCursor();
				scanf("%s", pw);
				HideCursor();
				pw_check = isValidPhone_or_pw_Number(pw, 2);
				if (pw_check == 0) {
					strcpy(pw, all[index].pw);
					goto_xy(68, 37);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}
			}
		}
		if (xx > 80 && xx < 95) {
			if (yy > 45 && yy < 49) {
				if (pw_check != 0 && ph_check != 0) {
					small_box(80, 46, 10, 86, 47, "����", 6);
					strcpy(all[index].name, name);
					strcpy(all[index].pw, pw);
					strcpy(all[index].phone, phone);
					file_write();
					Sleep(500);
					return;
				}
			}
		}
		xx = 0, yy = 0;
	}
	//ExClick();
}
int member_initial_screen(int index) { //�α��� ������ ȸ�� �ʱ�ȭ��
	int xx, yy, lr = 0;
	int choice = 0;
	while (1) {
		box_clear();
		m_basic_UI();
		xx = 0, yy = 0;
		goto_xy(63, 5);
		textcolor(6);
		printf("%s��", all[index].name);
		big_box(57, 18, 6, 66, 20, "����");
		big_box(57, 25, 6, 61, 27, "���� ���� ��ȸ");
		big_box(57, 32, 6, 63, 34, "���� ����");
		small_box(38, 46, 6, 42, 47, "�α׾ƿ�", 6);
		small_box(80, 46, 6, 86, 47, "����", 6);
		//ExClick();
		while (1) {
			xx = 0, yy = 0;
			click(&xx, &yy);
			if (xx > 38 && xx < 53) {
				if (yy > 45 && yy < 49) {
					small_box(38, 46, 10, 42, 47, "�α׾ƿ�", 6);
					Sleep(500);
					return;
				}
			}
			if (xx > 57 && xx < 78) {
				if (yy > 31 && yy < 37) {
					big_box(57, 18, 6, 66, 20, "����", 6);
					big_box(57, 25, 6, 61, 27, "���� ���� ��ȸ", 6);
					big_box(57, 32, 10, 63, 34, "���� ����", 6);
					choice = 3;
					xx, yy = 0;
				}
			}
			if (xx > 57 && xx < 78) {
				if (yy > 17 && yy < 22) {
					big_box(57, 18, 10, 66, 20, "����", 6);
					big_box(57, 25, 6, 61, 27, "���� ���� ��ȸ", 6);
					big_box(57, 32, 6, 63, 34, "���� ����", 6);
					choice = 1;
					xx, yy = 0;
				}
			}
			if (xx > 80 && xx < 95) {
				if (yy > 45 && yy < 49) {
					if (choice != 0) {
						small_box(80, 46, 10, 86, 47, "����", 6);
						Sleep(500);
						if (choice == 3) {
							modifying_membership(index);
							break;
						}
						else if (choice == 1) {
							designer_choice(index);
							break;
						}
					}
				}
			}
			xx = 0, yy = 0;
		}
		xx = 0, yy = 0;
	}

}
int member_login() { //ȸ�� �α��� �ϴ� �κ�
	box_clear();
	goto_xy(62, 5);
	textcolor(6);
	printf("ȸ�� �α���");
	small_box(38, 46, 6,44,47,"����",6);
	small_box(80, 46, 6,85,47,"�α���",6);
	char phone[20] = " ";
	char pw[15] = " ";
	int xx = 0, yy = 0, lr = 0;
	int check = 0;
	goto_xy(53, 19);
	printf("��ȭ��ȣ(11�ڸ� ex) 010xxxxxxxx ");
	goto_xy(53, 21);
	printf(":");
	goto_xy(53, 28);
	printf("��й�ȣ(���� 4�ڸ�) ex) 0000");
	goto_xy(53, 30);
	printf(":");
	//ExClick();
	while (1) {
		xx = 0, yy = 0;
		click(&xx, &yy);
		if (xx > 38 && xx < 53) {
			if (yy > 45 && yy < 49) {
				small_box(38, 46, 10,44,47,"����",6);
				Sleep(500);
				return;
			}
		}
		if (xx > 53 && xx < 85) {
			if (yy > 19 && yy < 23) {
				textcolor(6);
				goto_xy(55, 21);
				printf("                                 ");
				goto_xy(55, 21);
				EnableConsoleCursor();
				gets(phone);
				HideCursor();
				check = isValidPhone_or_pw_Number(phone, 1);
				if (check == 0) {
					strcpy(phone, " ");
					goto_xy(55, 21);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}

			}
		}
		if (xx > 53 && xx < 85) {
			if (yy > 28 && yy < 32) {
				textcolor(6);
				goto_xy(55, 30);
				printf("                                 ");
				goto_xy(55, 30);
				EnableConsoleCursor();
				char ch;
				int i = 0;
				while (1) {
					ch = _getch();

					// Check if Enter key is pressed (ASCII value 13)
					if (ch == 13) {
						pw[i] = '\0'; // Null-terminate the password string
						break;
					}

					// Check if the input is a printable character (ASCII range 32 to 126)
					if (ch == 8 && i > 0) { // Backspace (ASCII value 8)
						i--;
						printf("\b \b"); // Move the cursor back, print space to erase the last character, move the cursor back again
					}
					else if (ch >= 32 && ch <= 126 && i < 4) { // Printable characters (ASCII range 32 to 126)
						printf("*");
						pw[i] = ch; // Store the character in the password array
						i++;
					}
				}
				HideCursor();
				check = isValidPhone_or_pw_Number(pw, 2);
				if (check == 0) {
					strcpy(pw, " ");
					goto_xy(55, 30);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}
			}
		}
		if (xx > 80 && xx < 95) {
			if (yy > 45 && yy < 49) {
				if (strcmp(pw, " ") != 0 && strcmp(phone, " ") != 0) {
					small_box(80, 46, 10,85,47,"�α���",6);
					check = -1;
					for (int i = 0; i < member_count; i++) {
						if (strcmp(all[i].phone, phone) == 0 && strcmp(all[i].pw, pw) == 0) {
							check = i;
							break;
						}
					}
					Sleep(500);
					if (check >= 0) {
						member_initial_screen(check);
						return 1;
					}
					else {
						small_box(80, 46, 6,85,47,"�α���",6);
						goto_xy(48, 40);
						textcolor(4);
						printf("��ȭ��ȣ Ȥ�� ��й�ȣ�� �ٽ� �Է� ���ּ���.");
					}
				}

			}
		}
		xx = 0, yy = 0;
	}

}
void login_menu_choice() { // �α��� ���� �ϴ� �κ� (ȸ�� ������ �����̳�)
	int xx = 0, yy = 0, lr = 0;
	int choice = 0;
	int check = 0;
	while (1) {
		xx = 0, yy = 0;
		box_clear();
		goto_xy(64, 5);
		textcolor(6);
		printf("�α���");
		big_box(57, 15, 6,66,17,"ȸ��",6);
		big_box(57, 23, 6,64,25,"�����̳�",6);
		big_box(57, 31, 6,65,33,"������",6);
		small_box(38, 46, 6,44,47,"����",6);
		small_box(80, 46, 6,86,47,"����",6);
		//ExClick();
		while (1) {
			check = 0;
			xx = 0, yy = 0;
			click(&xx, &yy);
			if (xx > 38 && xx < 53) {
				if (yy > 45 && yy < 49) {
					small_box(38, 46, 10,44,47,"����",6);
					Sleep(500);
					return;
				}
			}
			if (xx > 57 && xx < 78) {
				if (yy > 14 && yy < 19) {
					big_box(57, 15, 10,66,17,"ȸ��", 6);
					big_box(57, 23, 6,64,25,"�����̳�",6);
					big_box(57, 31, 6,65,33,"������",6);
					choice = 1;
				}
				else if (yy > 22 && yy < 28) {
					big_box(57, 15, 6, 66, 17, "ȸ��", 6);
					big_box(57, 23, 10, 64, 25, "�����̳�", 6);
					big_box(57, 31, 6, 65, 33, "������", 6);
					choice = 2;
				}
			}
			if (xx > 80 && xx < 95) {
				if (yy > 45 && yy < 49) {
					if (choice != 0) {
						small_box(80, 46, 10,86,47,"����", 6);
						Sleep(500);
						if (choice == 1) {
							check = member_login();
							choice = 0;
							break;
						}
						else if (choice == 2) {
							check = designer_login();
							choice = 0;
							break;
						}
					}
				}
			}
			xx = 0, yy = 0;
		}
		if (check == 1) {
			break;
		}
		xx = 0, yy = 0;
	}

} // ��й�ȣ ã�� �Լ�
void pw_find() { //��й�ȣ ã�� 
	box_clear();
	int xx, yy, lr = 0;
	char name[20] = " ";
	char phone[15] = " ";
	int brith = 0;
	int check = 0;
	basic_UI(30,3);
	goto_xy(61, 5);
	printf("��й�ȣ ã��");
	small_box(38, 46, 6,44,47,"����",6);
	small_box(80, 46, 6,86,47,"ã��",6);
	goto_xy(53, 17);
	printf("�̸� ex) ȫ�浿");
	goto_xy(53, 19);
	printf(":");
	goto_xy(53, 26);
	printf("��ȭ��ȣ 11�ڸ� ex) 010xxxxxxxx ");
	goto_xy(53, 28);
	printf(":");
	goto_xy(53, 34);
	printf("�������(8�ڸ�) ex) 20030728 ");
	goto_xy(53, 36);
	printf(":");
	//ExClick();
	while (1) {
		xx = 0, yy = 0;
		click(&xx, &yy);
		if (xx > 38 && xx < 53) {
			if (yy > 45 && yy < 49) {
				small_box(38, 46, 10,44,47,"����",6);
				Sleep(500);
				return;
			}
		}

		if (xx > 53 && xx < 87) {
			if (yy > 18 && yy < 21) {
				strcpy(name, " ");
				textcolor(6);
				goto_xy(58, 19);
				printf("                                 ");
				goto_xy(58, 19);
				EnableConsoleCursor();
				gets(name);
				HideCursor();
			}
		}
		if (xx > 53 && xx < 87) {
			if (yy > 27 && yy < 30) {
				textcolor(6);
				goto_xy(58, 28);
				printf("                                 ");
				goto_xy(58, 28);
				EnableConsoleCursor();
				scanf("%s", phone);
				HideCursor();
				check = isValidPhone_or_pw_Number(phone, 1);
				if (check == 0) {
					strcpy(phone, " ");
					goto_xy(58, 28);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}

			}
		}
		if (xx > 53 && xx < 87) {
			if (yy > 34 && yy < 38) {
				textcolor(6);
				goto_xy(58, 36);
				printf("                                 ");
				goto_xy(58, 36);
				EnableConsoleCursor();
				scanf("%d", &brith);
				HideCursor();
				check = isValidDate(brith);
				if (check == 0) {
					brith = 0;
					goto_xy(58, 36);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}
			}
		}
		if (xx > 80 && xx < 95) {
			if (yy > 45 && yy < 49) {
				if (strcmp(name, " ") != 0 &&  strcmp(phone, " ") != 0 && brith != 0) {
					check = -1;
					for (int i = 0; i < member_count; i++) {
						if (strcmp(all[i].name,name) == 0 && strcmp(all[i].phone, phone) == 0 && all[i].brith == brith) {
							check = i;
						}
					}
					if (check >= 0) {
						small_box(80, 46, 10,86,47,"ã��",6);
						Sleep(500);
						box_clear();
						goto_xy(61, 5);
						printf("��й�ȣ ã��");
						small_box(38, 46, 6,44,47,"����",6);
						small_box(80, 46, 6,85,47,"�α���",6);
						goto_xy(54, 24);
						printf("%s���� ��й�ȣ�� %s�Դϴ�.", all[check].name,all[check].pw);
						xx = 0;
						yy = 0;
						click(&xx, &yy);
						while (1) {
							if (xx > 38 && xx < 53) {
								if (yy > 45 && yy < 49) {
									small_box(38, 46, 10,44,47,"����",6);
									Sleep(500);
									break;
								}
							}
							else if (xx > 80 && xx < 95) {
								if (yy > 45 && yy < 49) {
									small_box(80, 46, 10,85,47,"�α���",6);
									Sleep(500);
									member_login();
									break;
								}
							}
						}
					}
					else {
						textcolor(4);
						goto_xy(49, 40);
						printf("��ġ�ϴ� ȸ�������� ã�� �� �����ϴ�");
						continue;
					}
					break;
				}
			}
		}
		xx = 0, yy = 0;

	}
}
void membership() { // ȸ������ �Լ� 
	HideCursor();
	int xx, yy, lr = 0;
	char name[20] = " ";
	char phone[20] = " ";
	char gender[5] = " ";
	int brith = 0;
	char pw[15] = " ";
	int check = 0;
	goto_xy(62, 5);
	textcolor(6);
	printf("ȸ������");
	small_box(50, 10, 6,56,11,"����",9);
	small_box(67, 10, 6,73,11,"����",12);
	small_box(38, 46, 6,44,47,"����",6);
	small_box(80, 46, 6,86,47,"����",6);
	goto_xy(50, 18);
	printf("�̸� ex) ȫ�浿");
	goto_xy(50, 20);
	printf(":");
	goto_xy(50, 24);
	printf("��ȭ��ȣ 11�ڸ� ex) 010xxxxxxxx ");
	goto_xy(50, 26);
	printf(":");
	goto_xy(50, 30);
	printf("�������(8�ڸ�) ex) 20030728 ");
	goto_xy(50, 32);
	printf(":");
	goto_xy(50, 36);
	printf("��й�ȣ(���� 4�ڸ�) ex) 0000");
	goto_xy(50, 38);
	printf(":");
	//Mouse();
	//ExClick();
	while (1) {
		xx = 0, yy = 0;
		click(&xx, &yy);
		if (xx > 38 && xx < 53) {
			if (yy > 45 && yy < 49) {
				small_box(38, 46, 10,44,47,"����",6);
				Sleep(500);
				return;
			}
		}
		if (xx > 50 && xx < 64) {
			if (yy > 9 && yy < 13) {
				strcpy(gender, "��");
				small_box(67, 10, 6,73,11,"����",12);
				small_box(50, 10, 10,56,11,"����",9);
			}
		}
		if (xx > 67 && xx < 82) {
			if (yy > 9 && yy < 13) {
				strcpy(gender, "��");
				small_box(50, 10,6,56,11,"����",9);
				small_box(67, 10, 10,73,11,"����",12);
			}
		}
	    if (xx > 38 && xx < 80) {
			if (yy > 18 && yy < 22) {
				strcpy(name, " ");
				textcolor(6);
				goto_xy(58, 20);
				printf("                                 ");
				goto_xy(58, 20);
				EnableConsoleCursor();
				gets(name);
				HideCursor();
			}
		}
		if (xx > 50 && xx < 82) {
			if (yy > 25 && yy < 28) {
				textcolor(6);
				goto_xy(58, 26);
				printf("                                 ");
				goto_xy(58, 26);
				EnableConsoleCursor();
				scanf("%s", phone);
				HideCursor();
				check = isValidPhone_or_pw_Number(phone,1);
				if (check == 0) {
					strcpy(phone, " ");
					goto_xy(58, 26);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}

			}
		}
		if (xx > 52 && xx < 78) {
			if (yy > 29 && yy < 34) {
				textcolor(6);
				goto_xy(58, 32);
				printf("                                 ");
				goto_xy(58, 32);
				EnableConsoleCursor();
				scanf("%d", &brith);
				HideCursor();
				check = isValidDate(brith);
				if (check == 0) {
					brith = 0;
					goto_xy(58, 32);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}
			}
		}
		if (xx > 52 && xx < 81) {
			if (yy > 36 && yy < 40) {
				textcolor(6);
				goto_xy(58, 38);
				printf("                                 ");
				goto_xy(58, 38);
				EnableConsoleCursor();
				scanf("%s", pw);
				HideCursor();
				check = isValidPhone_or_pw_Number(pw, 2);
				if (check == 0) {
					strcpy(pw, " ");
					goto_xy(58, 38);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}
			}
		}
		if (xx > 80 && xx < 95) {
			if (yy > 45 && yy < 49) {
				if(strcmp(name ," ") !=0 && strcmp(gender," ")!=0 && strcmp(pw," ")!=0 && strcmp(phone," ")!=0 && brith != 0) {
					check = 0;
					for (int i = 0; i < member_count; i++) {
						if (strcmp(all[i].phone, phone )==0) {
							check = 1;
						}
					}
					if (check == 1) {
						textcolor(4);
						goto_xy(56, 41);
						printf("�̹� ���Ե� ȸ�� �Դϴ�.");
						continue;
					}
					else {
						small_box(80, 46, 10,86,47,"����",6);
						Sleep(500);
						box_clear();
						goto_xy(62, 5);
						printf("ȸ������");
						small_box(38, 46, 6,44,47,"����",6);
						small_box(80, 46, 6,85,47,"�α���",6);
						goto_xy(54, 24);
						printf("ȸ�������� �Ϸ�Ǿ����ϴ�.");
						strcpy(all[member_count].name, name);
						strcpy(all[member_count].phone, phone);
						strcpy(all[member_count].pw, pw);
						strcpy(all[member_count].gender, gender);
						all[member_count].brith = brith;
						file_append();
						member_count += 1;
						while (1) {
							xx = 0;
							yy = 0;
							click(&xx, &yy);
							if (xx > 38 && xx < 53) {
								if (yy > 45 && yy < 49) {
									small_box(38, 46, 10,44,47,"����",6);
									Sleep(500);
									break;
								}
							}
							else if (xx > 80 && xx < 95) {
								if (yy > 45 && yy < 49) {
									small_box(80, 46, 10,85,47,"�α���",6);
									Sleep(500);
									member_login();
									break;
								}
							}
						}
					}
					xx = 0;
					yy = 0;
					break;
				}
			}
		}		
	}
}
void initial_screen() { // �ʱ�ȭ��
	box_clear(30,3);
	HideCursor();
	int xx = 0, yy = 0, lr = 0;
	basic_UI(30,3);
	goto_xy(62, 5);
	printf("���� ��");
	big_box(57,20, 6, 66, 22,"����");
	big_box(57, 27, 6, 61, 29,"���� ���� ��ȸ");
	small_box(36, 46, 6, 41, 47, "�α���",6);
	small_box(59, 46, 6, 63, 47,"ȸ������",6);
	small_box(82, 46, 6, 87, 47,"PW ã��",6);
	Mouse();
	//ExClick();
	while (1) {
		xx = 0, yy = 0, lr = 0;
		click(&xx, &yy);
		if (xx > 59 && xx < 74) {
			if (yy > 46 && yy < 49) {
				small_box(59, 46, 10,63,47,"ȸ������",6);
				Sleep(500);
				box_clear();
				membership();
				break;
			}
		}
		if (xx > 36 && xx < 50) {
			if (yy > 46 && yy < 49) {
				small_box(36, 46, 10,41,47,"�α���",6);
				Sleep(500);
				login_menu_choice();
				break;
			}
		}
		if (xx > 82 && xx < 97) {
			if (yy > 46 && yy < 49) {
				small_box(82, 46, 10,87,47,"PW ã��",6);
				Sleep(500);
				pw_find();
				break;
			}
		}
		
	}
	xx = 0, yy = 0;
	return initial_screen();
}
int main(void) { //�����Լ�
	file_read();
	design_file_read();

	initial_screen();
}
