#define _CRT_SECURE_NO_WARNINGS
#define MAX 200 //ȸ�� �ִ� �ο� ��
//�߰� �Ұ� �� �����̳ʴ� ������ �޴��� 50�� ���� �ɾ�Ѱ�
#include<stdio.h>
#include<Windows.h>
#include<conio.h>
#include<stdlib.h>
#include <time.h>
#include<string.h>
#include <stdbool.h>
#include"resource.h"
#include <wchar.h>
#define MAX_X 174
#define MAX_2_X 135
#define MAX_Y 40
INPUT_RECORD rec;
DWORD dwNOER;
HANDLE CIN = 0;
int member_count = 0;
int designer_count = 0;
int design_count = 0;
int take_menu_count = 0;
int style_i = 0;
int previous_choice = -1;
int date_check = 0;
int date_index[37];
typedef struct { //�����̳ʿ� ���� �޴� �����ֱ� ����
	int index;
	char name[20];
	int price;
	char account[240];
}d_menu;
d_menu D_MENU[50];

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
void handleNewline_2(int* print_x, int* print_y) {
	(*print_y)++;
	goto_xy(*print_x, *print_y); // Move the cursor to the new position
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
void clearInputBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}
void file_read() { // ���� �о ����ü�� �����ϴ� �Լ�
	member_count = 0;
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
	designer_count = 0;
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
	design_count = 0;
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
void design_file_write() {
	FILE* design = fopen("design.txt", "w");
	for (int i = 0; i < design_count; i++) {
		fprintf(design, "%s %s %s %d/%s\n", STYLE[i].designer_name, STYLE[i].sort, STYLE[i].name, STYLE[i].price, STYLE[i].account);
	}
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
	for (int i = 0; i < 50; i++) {
		D_MENU[i].index = -1;
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
int textcolor(int colorNum) { // �۾� �÷� �ٲ��ִ� �Լ�
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
void design_see_UI(int x, int y, int color, int i) {
	if (D_MENU[i].index == -1) {
		return;
	}
	textcolor(color);
	goto_xy(x, y);
	printf("��");
	for (int i = 0; i < 22; i++)
	{
		printf("��");
	}
	printf("��");

	for (int i = 0; i < 6; i++) {
		y += 1;
		goto_xy(x, y);
		printf("��                                            ��");
	}
	goto_xy(x, y + 1);
	printf("��");
	for (int i = 0; i < 22; i++)
	{
		printf("��");
	}
	printf("��");
}
void basic_UI_DELETE(int x, int y) {
	while(1) {
		goto_xy(x, y);
		printf("                                                                                                                                                                        ");
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
	int x = 60;
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
void long_box_UI(int x, int y, int i, int color) {
	int len = 0;
	if (D_MENU[i].index == -1) {
		return;
	}
	textcolor(color);
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
	textcolor(6);
	x += 9;
	goto_xy(x, y);
	printf("%s", D_MENU[i].name);
	goto_xy(x + 15, y);
	printf("%d", D_MENU[i].price);
	goto_xy(x + 30, y);
	if (strcmp(D_MENU[i].account, " ") == 0 || strcmp(D_MENU[i].account, "") == 0) {
		goto_xy(x + 39, y);
		printf("X");
	}
	else {
		len = strlen(D_MENU[i].account);
		for (int k = 0; k < len; k++) {
			if (D_MENU[i].account[k] == ' ') {
				printf("...");
				break;
			}
			else {
				printf("%c", D_MENU[i].account[k]);
			}
		}
	}
}
void delete_modify_finish(int x,int y, char* str) {
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
		goto_xy(x, y);
		printf("��                                                                      ��");
	}
	goto_xy(x, y + 1);
	printf("��");
	for (int i = 0; i < 35; i++)
	{
		printf("��");
	}
	printf("��");
	y = 14;
	x = x + 5;
	goto_xy(x, y);
	printf("��");
	for (int i = 0; i < 30; i++)
	{
		printf("��");
	}
	printf("��");
	for (int i = 0; i < 15; i++) {
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
	y = 22;
	goto_xy(x + 20, y);
	textcolor(4);
	printf("%s", str);
	Sleep(2000);
	basic_UI_DELETE(30, 3);
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
void time_box(int x, int y, int color, int xx, int yy,int hour, int min, int color2) {
	textcolor(color);
	goto_xy(x, y);
	printf("��");
	for (int i = 0; i < 4; i++)
	{
		printf("��");
	}
	printf("��");

	for (int i = 0; i < 1; i++) {
		y += 1;
		goto_xy(x, y);
		printf("��        ��");
	}
	goto_xy(x, y + 1);
	printf("��");
	for (int i = 0; i < 4; i++)
	{
		printf("��");
	}
	printf("��");
	textcolor(color2);
	goto_xy(xx, yy);
	printf("%2d:%02d",hour, min);
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
const char* style_management_print(int num, int page_count, int choice, int count, int index ,char*str,int design_choice) {
	int x = 60, y = 3;
	int style_i_copy = style_i;
	if (num == 1) {
		x = 30;
		style_i = (page_count * count) - count;
	}
	basic_UI(x, y);
	goto_xy(x+31, y+2);
	textcolor(6);
	printf("������ ����");
	small_box(x+5, 8, 6, x+11, 9, "ĿƮ", 6);
	small_box(x+21, 8, 6, x+28, 9, "��", 6);
	small_box(x+37, 8, 6, x+43, 9, "�÷�", 6);
	small_box(x+53, 8, 6, x+59, 9, "��Ÿ", 6);
	goto_xy(x+31, 39);
	printf("��");
	goto_xy(x+36, 39);
	printf("%d", page_count);
	goto_xy(x+41, 39);
	printf("��");
	small_box(x+8, 46, 6, x+14, 47, "����", 6);
	small_box(x+50, 46, 6, x+56, 47, "�߰�", 6);
	if (choice == 1) {
		strcpy(str, "ĿƮ");
		small_box(x+5, 8, 10, x+11, 9, "ĿƮ", 6);
	}
	else if (choice == 2) {
		strcpy(str, "��");
		small_box(x+21, 8, 10, x+28, 9, "��", 6);
	}
	else if (choice == 3) {
		strcpy(str, "�÷�");
		small_box(x+37, 8, 10, x+43, 9, "�÷�", 6);
	}
	else if (choice == 4) {
		strcpy(str, "��Ÿ");
		small_box(x+53, 8, 10, x+59, 9, "��Ÿ", 6);
	}
	design_take_menu(index, str);
	x = x+5;
	y = 12;
	for (style_i; style_i < count * page_count; style_i++) {
		goto_xy(x, y);
		if (strcmp(D_MENU[style_i].name, " ") == 0) {
			break;
		}
		else {
			long_box_UI(x, y, style_i, 6);
			y += 4;
		}
	}
	if (design_choice != 0) {
		if (design_choice == 1) {
			long_box_UI(x, 12, (page_count * count) - 6, 10);
		}
		else if (design_choice == 2) {
			long_box_UI(x, 16, (page_count * count) - 5, 10);
		}
		else if (design_choice == 3) {
			long_box_UI(x, 20, (page_count * count) - 4, 10);
		}
		else if (design_choice == 4) {
			long_box_UI(x, 24, (page_count * count) - 3, 10);
		}
		else if (design_choice == 5) {
			long_box_UI(x, 28, (page_count * count) - 2, 10);
		}
		else if (design_choice == 6) {
			long_box_UI(x, 32, (page_count * count) - 1, 10);
		}
	}
	if (num == 1) {
		style_i = style_i_copy;
	}
	return str;
}
void box_clear() { //�⺻ UI Ŭ���� ���ִ� �Լ�
	int x = 92;
	int y = 5;
	goto_xy(90, 5);
	printf("                   ");
	x = 62;
	y = 8;
	for (int i = 0; i < 35; i++) {
		goto_xy(x, y);
		printf("                                                                     ");
		y += 1;
	}
	x = 62;
	y = 45;
	for (int i = 0; i < 4; i++) {
		goto_xy(x, y);
		printf("                                                                     ");
		y += 1;
	}

}
void Render(int x, int y, int num, int check)
{
	int pictureWidth = 130;
	int pictureHeight = 150;
	if (check == 1) {
		pictureWidth = 300;
		pictureHeight = 400;
	}
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
	BitBlt(hDC, x, y, Bitmap.bmWidth, Bitmap.bmHeight, hBackDC, 0, 0, SRCCOPY);

	// �޸𸮿� ������Ʈ�� �����Ѵ�.
	DeleteObject(SelectObject(hBackDC, hBackBitmap));
	DeleteObject(hResizedBitmap);
	DeleteDC(hBackDC);
	DeleteDC(hMemDC);

	ReleaseDC(hWnd, hDC);
}
int buid(int num, int x, int y, int check) {
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
		Render(x, y, num,check);

		// �׸� �׸��� �۾��� ó������ �Ϸ�Ǿ��ٰ� �����ϰ� isFinished�� true�� �ʱ�ȭ�Ѵ�.

		if (isFinished) {
			break; // �ݺ����� �����Ѵ�.
		}
	}
}
int delete_modify(int index) {
	basic_UI(108, 3);
	int xx, yy = 0;
	char name[20] = " ";
	int price = 0;
	char str[240] = " ";// ���ڿ��� ���� �� 100����
	int len = 0;
	int x = 124;
	int y = 36;
	char ch = ' ';
	goto_xy(139, 5);
	printf("���� �� ����");
	goto_xy(124, 9);
	textcolor(4);
	printf("���� �� ���� ���� ����(����, �̸� ���� X)");
	small_box(116, 46, 6, 122, 47, "����", 6);
	small_box(158, 46, 6, 164, 47, "����", 6);
	goto_xy(177, 4);
	printf("[X]");
	goto_xy(122, 13);
	printf("����      :");
	goto_xy(143, 13);
	printf("%s", STYLE[index].sort);
	goto_xy(122, 19);
	printf("�̸�      :");
	goto_xy(143, 19);
	printf("%s", STYLE[index].name);
	goto_xy(122, 25);
	printf("����      :");
	goto_xy(143, 25);
	printf("%d", STYLE[index].price);
	goto_xy(122, 31);
	printf("���� (�Է� �� ENTER) ");
	add_box_UI(122, 33, 6, 143, 37, "");
	textcolor(6);
	strcpy(str, STYLE[index].account);
	price = STYLE[index].price;
	x = 124;
	y = 36;
	goto_xy(x, y);
	len = strlen(STYLE[index].account);
	int len_2 = 0;
	for (int i = 0; i < len; i++) {
		if (len < sizeof(str) - 2) {
			if (len_2 >= MAX_X - 124 && (len_2 % (MAX_X - 124)) == 0) {
				if (y >= MAX_Y - 1) {
					break;
				}
				else {
					handleNewline(&x, &y);
					len_2 = 0;
					len_2++;
					printf("%c", STYLE[index].account[i]);
				}
			}
			else {
				len_2++;
				printf("%c", STYLE[index].account[i]);
			}
		}
	}
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
				basic_UI_DELETE(30, 3);
				return 1;
			}
		}
		if (xx > 132 && xx < 171) {
			if (yy > 23 && yy < 27) {
				price = 0;
				char input[30] = "";
				strcpy(input, "");
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
				strcpy(str, " ");
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
							if (str[(x - 124) + (len)+(MAX_X - 124) * (y - 36) - 2] & 0x80) {
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
		if (yy > 45 && yy < 49) {
			if (xx > 158 && xx < 174) {
				if (strcmp(str, STYLE[index].account) != 0 || price != STYLE[index].price) {
					if (price != 0) {
						strcpy(STYLE[index].account, str);
						STYLE[index].price = price;
						small_box(158, 46, 10, 164, 47, "����", 6);
						Sleep(500);
						delete_modify_finish(108,3,"�ؼ����� �Ϸ�Ǿ����ϴ�.��");
						design_file_write();
						return 1;
					}	
				}
			}
			else if (xx > 116 && xx < 132) {
				small_box(116, 46, 10, 122, 47, "����", 6);
				Sleep(500);
				for (int i = index; i < design_count; i++) {
					strcpy(STYLE[i].designer_name, STYLE[i + 1].designer_name);
					strcpy(STYLE[i].sort, STYLE[i + 1].sort);
					strcpy(STYLE[i].name, STYLE[i + 1].name);
					STYLE[i].price = STYLE[i + 1].price;
					strcpy(STYLE[i].account, STYLE[i + 1].account);
				}
				design_count--;
				delete_modify_finish(108, 3, "�ػ����� �Ϸ�Ǿ����ϴ�.��");
				design_file_write();
				clearconsole();
				Sleep(1000);
				return 2;
			}
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
				basic_UI_DELETE(30, 3);
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
					Sleep(1000);
					return 1;
				}
			}
		}
	}
}
int style_management(int index) {
	char str[15] = "ĿƮ";
	int page_count = 1;
	int choice = 1;
	int x = 95, y = 12;
	int xx, yy = 0;
	int check = 0;
	box_clear();
	style_i = 0;
	int count = 6;
	int len = 0;
	while (1) {
		x = 65, y = 12;
		xx = 0, yy = 0;
		check = 0;
		const char* string = style_management_print(2,page_count,choice,count,index ,str,0);
		strcpy(str, string);
		if (style_i == (count * page_count) - 1) {
			style_i++;
		}
		//ExClick();
		while (1) {
			xx = 0, yy = 0;
			click(&xx, &yy);
			if (xx > 68 && xx < 83) {
				if (yy > 45 && yy < 49) {
					small_box(68, 46, 10, 74, 47, "����", 6);
					Sleep(500);
					return;
				}
			}
			if (xx > 64 && xx < 129) {
				if (yy > 11 && yy < 15) {
					if (D_MENU[(page_count * count)-6].index != -1) {
						long_box_UI(65, 12, (page_count * count) - 6,10);
						long_box_UI(65, 16, (page_count * count) - 5, 6);
						long_box_UI(65, 20, (page_count * count) - 4, 6);
						long_box_UI(65, 24, (page_count * count) - 3, 6);
						long_box_UI(65, 28, (page_count * count) - 2, 6);
						long_box_UI(65, 32, (page_count * count) - 1, 6);
						Sleep(200);
						basic_UI_DELETE(30, 3);
						style_management_print(1, page_count, choice, count, index, str,1);
						check = delete_modify(D_MENU[(page_count * count) - 6].index);
					}
				}
				if (yy > 15 && yy < 19) {
					if (D_MENU[(page_count * count) - 5].index != -1) {
						long_box_UI(65, 12, (page_count * count) - 6, 6);
						long_box_UI(65, 16, (page_count * count) - 5, 10);
						long_box_UI(65, 20, (page_count * count) - 4, 6);
						long_box_UI(65, 24, (page_count * count) - 3, 6);
						long_box_UI(65, 28, (page_count * count) - 2, 6);
						long_box_UI(65, 32, (page_count * count) - 1, 6);
						Sleep(200);
						basic_UI_DELETE(30, 3);
						style_management_print(1, page_count, choice, count, index, str,2);
						check = delete_modify(D_MENU[(page_count * count) - 5].index);
					}
				}
				if (yy > 19 && yy < 23) {
					if (D_MENU[(page_count * count) - 4].index != -1) {
						long_box_UI(65, 12, (page_count * count) - 6, 6);
						long_box_UI(65, 16, (page_count * count) - 5, 6);
						long_box_UI(65, 20, (page_count * count) - 4, 10);
						long_box_UI(65, 24, (page_count * count) - 3, 6);
						long_box_UI(65, 28, (page_count * count) - 2, 6);
						long_box_UI(65, 32, (page_count * count) - 1, 6);
						Sleep(200);
						basic_UI_DELETE(30, 3);
						style_management_print(1, page_count, choice, count, index, str, 3);
						check = delete_modify(D_MENU[(page_count * count) - 4].index);
					}
				}
				if (yy > 23 && yy < 27) {
					if (D_MENU[(page_count * count) - 3].index != -1) {
						long_box_UI(65, 12, (page_count * count) - 6, 6);
						long_box_UI(65, 16, (page_count * count) - 5, 6);
						long_box_UI(65, 20, (page_count * count) - 4, 6);
						long_box_UI(65, 24, (page_count * count) - 3, 10);
						long_box_UI(65, 28, (page_count * count) - 2, 6);
						long_box_UI(65, 32, (page_count * count) - 1, 6);
						Sleep(200);
						basic_UI_DELETE(30, 3);
						style_management_print(1, page_count, choice, count, index, str, 4);
						check = delete_modify(D_MENU[(page_count * count) - 3].index);
					}
				}
				if (yy > 27 && yy < 31) {
					if (D_MENU[(page_count * count) - 2].index != -1) {
						long_box_UI(65, 12, (page_count * count) - 6, 6);
						long_box_UI(65, 16, (page_count * count) - 5, 6);
						long_box_UI(65, 20, (page_count * count) - 4, 6);
						long_box_UI(65, 24, (page_count * count) - 3, 6);
						long_box_UI(65, 28, (page_count * count) - 2, 10);
						long_box_UI(65, 32, (page_count * count) - 1, 6);
						Sleep(200);
						basic_UI_DELETE(30, 3);
						style_management_print(1, page_count, choice, count, index, str,5);
						check = delete_modify(D_MENU[(page_count * count) - 2].index);
					}
				}
				if (yy > 31 && yy < 35) {
					if (D_MENU[(page_count * count) - 1].index != -1) {
						long_box_UI(65, 12, (page_count * count) - 6, 6);
						long_box_UI(65, 16, (page_count * count) - 5, 6);
						long_box_UI(65, 20, (page_count * count) - 4, 6);
						long_box_UI(65, 24, (page_count * count) - 3, 6);
						long_box_UI(65, 28, (page_count * count) - 2, 6);
						long_box_UI(65, 32, (page_count * count) - 1, 10);
						Sleep(200);
						basic_UI_DELETE(30, 3);
						style_management_print(1, page_count, choice, count, index, str,6);
						check = delete_modify(D_MENU[(page_count* count) - 1].index);
					}
				}
				if (check == 1) {
					style_i = (page_count * count) - count;
					break;
				}
				else if (check == 2) {
					page_count = 1;
					style_i = 0;
					break;
				}
			}
			if (xx > 89 && xx < 95 && yy > 37 && yy < 41) {
				if (page_count != 1) {
					textcolor(10);
					goto_xy(91, 39);
					printf("��");
					Sleep(500);
					style_i = (page_count - 2) * count;
					page_count--;
					break;
				}
			}
			if (xx > 97 && xx < 104 && yy > 37 && yy < 41) {
				if (strcmp(D_MENU[style_i].name, " ") != 0) {
					textcolor(10);
					goto_xy(101, 39);
					printf("��");
					Sleep(500);
					page_count++;
					break;
				}
			 }
		    if (xx > 110 && xx < 125) {
				if (yy > 45 && yy < 49) {
					small_box(110, 46, 10, 116, 47, "�߰�", 6);
					Sleep(500);
					small_box(110, 46, 6, 116, 47, "�߰�", 6);
					basic_UI_DELETE(30,3);
					style_management_print(1, page_count, choice, count, index, str,0);
					check = add_design(index,str);
					if (check == 1) {
						style_i = 0;
						page_count = 1;
					}
					else {
						style_i = (page_count * count) - count;
					}
					break;
				}
			}
		    if (yy > 6 && yy < 11) {
				if (xx > 64 && xx < 80) {
					if (choice != 1) {
						choice = 1;
						style_i = 0;
						page_count = 1;
						break;
					}
				}
				if (xx > 80 && xx < 97) {
					if (choice != 2) {
						choice = 2;
						style_i = 0;
						page_count = 1;
						break;
					}
				}
				else if (xx > 97 && xx < 112) {
					if (choice != 3) {
						choice = 3;
						style_i = 0;
						page_count = 1;
						break;
					}
				}
				else if (xx > 113 && xx < 128) {
					if (choice != 4) {
						choice = 4;
						style_i = 0;
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
void designer_seeUI(int x, int y, int color) {
	textcolor(color);
	goto_xy(x, y);
	printf("��");
	for (int i = 0; i < 33; i++)
	{
		printf("��");
	}
	printf("��");

	for (int i = 0; i < 9; i++) {
		y += 1;
		goto_xy(x, y);
		printf("��                                                                  ��");
	}
	goto_xy(x, y + 1);
	printf("��");
	for (int i = 0; i < 33; i++)
	{
		printf("��");
	}
	printf("��");
}
void designer_print(int choice) {
	int x = 62, y = 8;
	int px = 900, py = 170;
	int nx = 70, ny = 10;
	int len;
	int color = 6;
	for (int i = 0; i < 3; i++) {
		designer_seeUI(x, y, 6);
		if (i < designer_count) {
			if (choice == i) {
				designer_seeUI(x, y, 10);
				textcolor(6);
			}
			goto_xy(nx, ny);
			printf("%s �����̳�", d_all[i].n_name);
			len = strlen(d_all[i].introduce);
			goto_xy(nx + 10, ny + 3);
			textcolor(7);
			for (int k = 0; k < len; k++) {
				printf("%c", d_all[i].introduce[k]);
				if (k == 20) {
					goto_xy(nx + 10, ny + 4);
				}
			}
			textcolor(6);
			goto_xy(nx, ny + 7);
			printf("��");
			buid(i, px, py,0);
			y += 12;
			ny += 12;
			py += 215;
		}
		else {
			goto_xy(x + 31, y + 5);
			textcolor(4);
			printf("�������");
			y += 12;
		}
	}
	previous_choice = choice;
}
int designer_choice(int index) {
	d_file_read();
	int xx, yy, lr = 0;
	int choice = -1;
	while (1) {
		box_clear();
		xx, yy, lr = 0;
		choice = -1;
		previous_choice = -1;
		basic_UI(60, 3);
		goto_xy(92, 5);
		printf("�����̳�");
		small_box(68, 46, 6, 74, 47, "����", 6);
		designer_print(choice);
		//ExClick();
		while (1) {
			xx = 0, yy = 0;
			click(&xx, &yy);
			if (xx > 68 && xx < 83) {
				if (yy > 45 && yy < 49) {
					small_box(68, 46, 10, 74, 47, "����", 6);
					Sleep(500);
					return;
				}
			}
			if (xx > 62 && xx < 131) {
				if (yy > 7 && yy < 19) {
					choice = 0;
				}
				else if (yy > 19 && yy < 31) {
					choice = 1;
				}
				else if (yy > 31 && yy < 43) {
					choice = 2;
				}
			}
			if (choice != previous_choice) {
				if (choice < designer_count) {
					designer_print(choice);
					Sleep(700);
					date_choice(index,choice);
					break;
				}
			}
		}
	}
}
int day_of_week(int year, int month) //�� �ϼ��� ���ϴ� �Լ�(�ش� �� 1���� ������������ �˱�����)

{

	int temp = 0; //�ӽ÷� ��꿡 ����� ����

	int i; //for ������ ����� ����



	for (i = 1; i < year; i++) { //�⵵�� �ϼ�

		if ((i % 4 == 0) && (i % 100 != 0) || (i % 400 == 0)) {

			temp += 366;

		}
		else {

			temp += 365;

		}

	}



	for (i = 1; i < month; i++) { //�� �� �ϼ�

		if (i == 2) { // 2���ϰ�� ���� �˻�

			if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))

				temp += 29;

			else

				temp += 28;

		}

		switch (i) {

		case 1:

		case 3:

		case 5:

		case 7:

		case 8:

		case 10:

		case 12:

			temp += 31; //�Ѵ��� 31���� ���

			break;

		case 4:

		case 6:

		case 9:

		case 11:

			temp += 30; //�Ѵ��� 30���� ���

			break;

		}

	}



	temp = temp + 1; //���������� �ϼ��� ���� �� �� ���� ���Ѵ�



	return temp % 7; //1=��,2=ȭ...6=��,0=��

}
void print_calendar(int sd, int year, int month, int x, int y,int d_day) {
	date_check = 0;
	int i, j;
	int temp;
	goto_xy(x, y);
	for (int i = 0; i < 17; i++) {
		goto_xy(x, y+i);
		printf("                                            ");
	}
	goto_xy(x, y);

	switch (month) {
	case 1: case 3: case 5: case 7: case 8: case 10: case 12:
		temp = 31;
		break;
	case 4: case 6: case 9: case 11:
		temp = 30;
		break;
	case 2:
		if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))
			temp = 29;
		else
			temp = 28;
	}
	for (i = 0; i < sd; i++) {
		printf("%c      ", "       \n"[i == sd]);
		date_index[i] = 0;
	}
	j = sd;
	date_check = sd;
	int check = 0;

	for (i = 1; i <= temp; i++) {
		if (j == 6) {
			textcolor(9);
			if (i < d_day) {
				check = 1;
				textcolor(8);
			}
			else if (i == d_day) {
				textcolor(10);
			}
			printf("%2d", i);
			textcolor(15);
			y += 3;
			goto_xy(x, y);
			j = -1;
		}
		else {
			textcolor(15);
			if (j == 0) {
				textcolor(12);
			}
			if (i < d_day) {
				check = 1;
				textcolor(8);
			}
			else if (i == d_day) {
				textcolor(10);
			}
			printf("%2d     ", i);
		}
		j++;
		if (check == 1) {
			date_index[date_check] = 0;
			check = 0;
		}
		else {
			date_index[date_check] = i;
		}
		date_check++;
	}
}
int xx_yy_date_find(int xx, int yy) {
	int index = -1;
	int date_x = 101;
	int date_y = 19;
	if ((xx - date_x) % 7 == 0 && (yy - date_y) % 3 == 0) {
		index = (((yy - date_y) / 3) * 7) + ((xx - date_x) / 7);
		if (index >= date_check || date_index[index] == 0) {
			return 0;
		}
		else {
			return date_index[index];
		}
		
	}
	else {
		return 0;
	}
}
int date_choice(int index,int choice) {
	time_t seconds = time(NULL);
	struct tm* now = localtime(&seconds);
	int xx, yy, lr = 0;
	basic_UI_DELETE(60, 3);
	textcolor(6);
	big_designer_print();
	buid(choice, 395, 180, 1);
	while (1) {
		textcolor(6);
		goto_xy(44, 7);
		printf("��--");
		textcolor(15);
		goto_xy(55, 34);
		printf("�����̳� : %s �����̳�", d_all[choice].n_name);
		goto_xy(55, 36);
		printf("��    ¥ :                 ");
		textcolor(6);
		date_and_time_choice_UI(95, 7);
		goto_xy(118, 8);
		textcolor(6);
		printf("��¥ ����");
		int year = 1900 + now->tm_year;
		int mon = now->tm_mon + 1;
		int day = day_of_week(year, mon);
		int x = 100;
		int y = 12;
		int d_day = now->tm_mday;
		int choice_day = 0;
		int check = 0;
		textcolor(15);
		goto_xy(x + 14, y);
		printf("��");
		goto_xy(x + 17, y);
		printf("%d�� %02d��", year, mon);
		goto_xy(x + 29, y);
		printf("��");
		goto_xy(x, y + 4);
		printf("��     ��     ȭ     ��     ��     ��     ��");
		goto_xy(132, 36);
		textcolor(8);
		printf("��");
		textcolor(15);
		goto_xy(135, 36);
		printf("���úҰ�");
		while (1) {
			goto_xy(x + 17, y);
			printf("%d�� %02d��", year, mon);
			print_calendar(day, year, mon, x, y + 7, d_day);
			//ExClick();
			while (1) {
				xx = 0, yy = 0;
				click(&xx, &yy);
				if (yy > 10 && yy < 13) {
					if (xx > 127 && xx < 132) {
						goto_xy(x + 29, y);
						textcolor(10);
						printf("��");
						mon += 1;
						if (mon > 12) {
							year += 1;
							mon = 1;
						}
						day = day_of_week(year, mon);
						Sleep(500);
						goto_xy(x + 29, y);
						textcolor(15);
						d_day = 0;
						printf("��");
						break;
					}
					else if (xx > 112 && xx < 117) {
						if (year == 1900 + now->tm_year && mon == now->tm_mon + 1) {
							continue;
						}
						textcolor(10);
						goto_xy(x + 14, y);
						printf("��");
						mon -= 1;
						if (mon < 1) {
							year -= 1;
							mon = 12;
						}
						if (year == 1900 + now->tm_year && mon == now->tm_mon + 1) {
							d_day = now->tm_mday;
						}
						day = day_of_week(year, mon);
						Sleep(500);
						textcolor(15);
						goto_xy(x + 14, y);
						printf("��");
						break;
					}
				}
				if (yy > 5 && yy < 8) {
					if (xx > 42 && xx < 48) {
						textcolor(10);
						goto_xy(44, 7);
						printf("��--");
						Sleep(500);
						clearconsole();
						return;
					}
				}
				if (xx > 100 && xx < 145) {
					if (yy > 18 && yy < 35) {
						choice_day = xx_yy_date_find(xx, yy);
						if (choice_day != 0) {
							goto_xy(xx - 1, yy);
							textcolor(10);
							printf("%2d", choice_day);
							textcolor(15);
							time_choice(index, choice, year, mon, choice_day);
							check = 1;
							break;
						}
					}
				}
			}
			if (check == 1) {
				check = 0;
				break;
			}
		}
	}
}
int time_choice(int index, int choice,int year, int mon, int choice_day) {
	int hour = 10; 
	int min = 0;
	int x = 98;
	int y = 16;
	int xx = 0, yy = 0, lr = 0;
	int check = 0;
	goto_xy(66, 36);
	printf("%d.%02d.%02d", year, mon, choice_day);
	Sleep(700);
	while (1) {
		hour = 10;
		min = 0;
		x = 98;
		y = 16;
		textcolor(6);
		goto_xy(44, 7);
		printf("��--");
		textcolor(15);
		goto_xy(55, 38);
		printf("��    �� :             ");
		textcolor(6);
		date_and_time_choice_UI(95, 7);
		goto_xy(118, 8);
		printf("�ð� ����");
		textcolor(15);
		goto_xy(121, 12);
		printf("����");
		goto_xy(121, 22);
		printf("����");
		goto_xy(132, 38);
		textcolor(8);
		printf("��");
		textcolor(15);
		goto_xy(135, 38);
		printf("���úҰ�");
		goto_xy(132, 39);
		textcolor(15);
		printf("��");
		goto_xy(135, 39);
		printf("���ð���");
		textcolor(15);
		//���� ������ ��¥ �� �ݺ��� �ȿ��ִ� �ð��̶� ���� ����ؼ� �Լ��� ������ �� �������̳ʿ� �� ��¥�� �� �ð� ������ �ִ��� Ȯ�� ���ִ� �Լ� �������� // ���� ���� �� ��� ���� �ð��� ���� ���ϰ� ���ƾ���
		for (int i = 1; i <= 16; i++) {
			time_box(x, y, 15, x + 3, y + 1, hour, min, 15);
			x += 12;
			min += 30;
			if (min == 60) {
				hour += 1;
				min = 0;
				if (hour > 12) {
					hour = 1;
				}
			}
			if (i % 4 == 0) {
				if (i == 4) {
					y += 10;
				}
				else {
					y += 4;
				}
				x = 98;
			}
		}
		hour = 0;
		min = 0;
		int e_hour = 0;
		while (1) {
			xx = 0, yy = 0;
			click(&xx, &yy);
			if (yy > 5 && yy < 8) {
				if (xx > 42 && xx < 48) {
					textcolor(10);
					goto_xy(44, 7);
					printf("��--");
					Sleep(500);
					goto_xy(55, 38);
					printf("              ");
					return;
				}
			}
			if (xx > 97 && xx < 146) {
				if (yy > 15 && yy < 19) {
					yy = 16;
					e_hour = 10;
				}
				if (yy > 25 && yy < 29) {
					yy = 26;
					e_hour = 12;
				}
				if (yy > 29 && yy < 33) {
					yy = 30;
					e_hour = 2;
				}
				if (yy > 33 && yy < 37) {
					yy = 34;
					e_hour = 4;
				}
				if (e_hour != 0) {
					if (xx > 98 && xx < 109) {
						xx = 98;
						hour = e_hour;
						min = min;
					}
					else if (xx > 110 && xx < 121) {
						xx = 110;
						hour = e_hour;
						min += 30;
					}
					else if (xx > 122 && xx < 133) {
						xx = 122;
						min = 0;
						hour = e_hour + 1;
						if (hour > 12) {
							hour = 1;
						}

					}
					else if (xx > 134 && xx < 146) {
						xx = 134;
						hour = e_hour + 1;
						if (hour > 12) {
							hour = 1;
						}
						min += 30;
					}
					else {
						e_hour = 0;
						continue;
					}
					break;
				}
			}
		}
		if (e_hour != 0 && xx != 0 && yy != 0) {
			time_box(xx, yy, 10, xx + 3, yy + 1, hour, min, 15);
			check = member_design_choice(index, choice, year, mon, choice_day, hour, min);
			if (check == 1) {
				return;
			}
		}
	}
}
const char* m_design_print(int index, int design_column, int page_count, int count, char *str,int design_choice) {
	int x = 98;
	int y = 10;
	design_column_UI(x, y, 6, x + 4, y + 1, "ĿƮ", 6);
	design_column_UI(x + 12, y, 6, x + 17, y + 1, "��", 6);
	design_column_UI(x + 24, y, 6, x + 28, y + 1, "�÷�", 6);
	design_column_UI(x + 36, y, 6, x + 40, y + 1, "��Ÿ", 6);

	if (design_column == 1) {
		strcpy(str, "ĿƮ");
		design_column_UI(x, y, 10, x + 4, y + 1, "ĿƮ", 6);
	}
	else if (design_column == 2) {
		strcpy(str, "��");
		design_column_UI(x + 12, y, 10, x + 17, y + 1, "��", 6);
	}
	else if (design_column == 3) {
		strcpy(str, "�÷�");
		design_column_UI(x + 24, y, 10, x + 28, y + 1, "�÷�", 6);
	}
	else if (design_column == 4) {
		strcpy(str, "��Ÿ");
		design_column_UI(x + 36, y, 10, x + 40, y + 1, "��Ÿ", 6);
	}
	design_take_menu(index, str);
	y = 13;
	for (int i = 0; i < 25; i++) {
		goto_xy(x, y + i);
		printf("                                                 ");
	}
	for (style_i; style_i < count * page_count; style_i++) {
		if (strcmp(D_MENU[style_i].name, " ") == 0) {
			break;
		}
		else {
			design_see_UI(x, y, 6, style_i);
			goto_xy(x+2, y+1);
			textcolor(7);
			printf("%s", D_MENU[style_i].name);
			int print_x = x + 7;
			int print_y = y + 3;
			int len = 0;
			len = strlen(D_MENU[style_i].account);
			int len_2 = 0;
			textcolor(8);
			goto_xy(print_x, print_y);
			if (strcmp(D_MENU[style_i].account, " ") != 0) {
				for (int i = 0; i < len; i++) {
					if (len < sizeof(D_MENU[style_i].account) - 1) {
						if (len_2 >= MAX_2_X - 103 && (len_2 % (MAX_2_X - 103)) == 0) {
							if (print_y >= y + 6) {
								break;
							}
							handleNewline_2(&print_x, &print_y);
							len_2 = 0;
							len_2++;
							printf("%c", D_MENU[style_i].account[i]);
						}
						else {
							len_2++;
							printf("%c", D_MENU[style_i].account[i]);
						}
					}
				}
			}
			else {
				goto_xy(x + 20, y + 3);
				printf("�������");
			}
			goto_xy(x + 38,y + 6);
			textcolor(7);
			printf("%d��", D_MENU[style_i].price);
			y += 8;
		}
	}
	textcolor(6);
	goto_xy(117, 38);
	printf("��");
	goto_xy(122, 38);
	printf("%d", page_count);
	goto_xy(126, 38);
	printf("��");
	return str;
}
int member_design_choice(int index, int choice, int year, int mon, int choice_day, int hour, int min) {
	design_file_read();
	goto_xy(66, 38);
	char str[15] = "ĿƮ";
	int page_count = 1;
	int design_column = 1;
	style_i = 0;
	int count = 3;
	int x = 98;
	int y = 10;
	int xx, yy = 0;
	if (hour < 10) {
		printf("%02d:%02d", hour + 12, min);
	}
	else {
		printf("%02d:%02d", hour, min);
	}
	Sleep(700);
	goto_xy(55, 40);
	printf("�� �� �� :             ");
	textcolor(6);
	date_and_time_choice_UI(95, 7);
	goto_xy(117, 8);
	printf("������ ����");
	strcpy(str, "ĿƮ");
	while (1) {
		const char* string = m_design_print(choice, design_column, page_count, count, str, 0);
		//ExClick();
		strcpy(str, string);
		if (style_i == (count * page_count) - 1) {
			style_i++;
		}
		while (1) {
			xx = 0, yy = 0;
			click(&xx, &yy);
			if (yy > 5 && yy < 8) {
				if (xx > 42 && xx < 48) {
					textcolor(10);
					goto_xy(44, 7);
					printf("��--");
					Sleep(500);
					goto_xy(55, 40);
					printf("              ");
					return;
				}
			}
			if (xx > 98 && xx < 145) {
				if (yy > 12 && yy < 21) {

				}
			}
			if (yy > 9 && yy < 13) {
				if (xx > 97 && xx < 109) {
					if (design_column != 1) {
						design_column = 1;
						style_i = 0;
						page_count = 1;
						break;
					}
				}
				if (xx > 109 && xx < 121) {
					if (design_column != 2) {
						design_column = 2;
						style_i = 0;
						page_count = 1;
						break;
					}
				}
				if (xx > 122 && xx < 133) {
					if (design_column != 3) {
						design_column = 3;
						style_i = 0;
						page_count = 1;
						break;
					}
				}
				if (xx > 135 && xx < 145) {
					if (design_column != 4) {
						design_column = 4;
						style_i = 0;
						page_count = 1;
						break;
					}
				}
			}
			if (yy > 36 && yy < 39) {
				if (xx > 116 && xx < 120) {
					if (page_count != 1) {
						textcolor(10);
						goto_xy(117, 38);
						printf("��");
						Sleep(500);
						style_i = (page_count - 2) * count;
						page_count--;
						break;
					}
				}
				if (xx > 125 && xx < 129) {
					if (strcmp(D_MENU[style_i].name, " ") != 0) {
						textcolor(10);
						goto_xy(126, 38);
						printf("��");
						Sleep(500);
						page_count++;
						break;
					}
				}
			}

		}
	}
}
int design_column_UI(int x, int y, int color, int xx, int yy, char* str, int color2) {
	textcolor(color);
	goto_xy(x, y);
	printf("��");
	for (int i = 0; i < 4; i++)
	{
		printf("��");
	}
	printf("��");

	for (int i = 0; i < 1; i++) {
		y += 1;
		goto_xy(x, y);
		printf("��        ��");
	}
	goto_xy(x, y + 1);
	printf("��");
	for (int i = 0; i < 4; i++)
	{
		printf("��");
	}
	printf("��");
	textcolor(color2);
	goto_xy(xx, yy);
	printf("%s", str);
}
int big_designer_print(){
	textcolor(DarkYellow);
	int x = 40, y = 6;
	goto_xy(x, y);
	printf("��");
	for (int i = 0; i < 54; i++)
	{
		printf("��");
	}
	printf("��");
	for (int i = 0; i < 35; i++) {
		y += 1;
		goto_xy(x, y);
		printf("��                                                                                                            ��");
	}
	goto_xy(x, y + 1);
	printf("��");
	for (int i = 0; i < 54; i++)
	{
		printf("��");
	}
	printf("��");
}
int date_and_time_choice_UI(int x,int y) {
	goto_xy(x, y);
	printf("��");
	for (int i = 0; i < 25; i++)
	{
		printf("��");
	}
	printf("��");
	for (int i = 0; i < 33; i++) {
		y += 1;
		if (i == 1) {
			goto_xy(x, y);
			printf("��");
			for (int i = 0; i < 25; i++) {
				printf("��");
			}
			printf("��");
		}
		else {
			goto_xy(x, y);
			printf("��                                                  ��");
		}
	}
	goto_xy(x, y + 1);
	printf("��");
	for (int i = 0; i < 25; i++)
	{
		printf("��");
	}
	printf("��");
}
int designer_initial_screen(int index) { //�����̳� �ʱ� ȭ��
	int xx, yy, lr = 0;
	int choice = 0;
	design_file_read();
	while (1) {
		box_clear();
		m_basic_UI();
		xx = 0, yy = 0;
		goto_xy(93, 5);
		textcolor(6);
		printf("%s��",d_all[index].name);
		big_box(87, 14, 6, 94, 16, "���� ����");
		big_box(87, 22, 6, 93, 24, "������ ����");
		big_box(87, 30, 6, 93, 32, "������ ����");
		big_box(87, 38, 6, 94, 40, "���� ����");
		small_box(68, 46, 6, 72, 47, "�α׾ƿ�", 6);
		small_box(110, 46, 6, 116, 47, "����", 6);
		//ExClick();
		while (1) {
			xx = 0, yy = 0;
			click(&xx, &yy);
			if (xx > 68 && xx < 83) {
				if (yy > 45 && yy < 49) {
					small_box(68, 46, 10, 72, 47, "�α׾ƿ�", 6);
					Sleep(500);
					return;
				}
			}
			if (xx > 87 && xx < 108) {
				if (yy > 21 && yy < 27) {
					big_box(87, 22, 10, 93, 24, "������ ����");
					big_box(87, 14, 6, 94, 16, "���� ����");
					big_box(87, 30, 6, 93, 32, "������ ����");
					big_box(87, 38, 6, 94, 40, "���� ����");
					choice = 2;
					xx, yy = 0;
				}
			}
			if (xx > 110 && xx < 125) {
				if (yy > 45 && yy < 49) {
					if (choice != 0) {
						small_box(110, 46, 10, 116, 47, "����", 6);
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
	goto_xy(90, 5);
	textcolor(6);
	printf("�����̳� �α���");
	small_box(68, 46, 6, 74, 47, "����", 6);
	small_box(110, 46, 6, 115, 47, "�α���", 6);
	char phone[20] = " ";
	int brith = 0;
	int xx = 0, yy = 0, lr = 0;
	int check = 0;
	goto_xy(83, 19);
	printf("��ȭ��ȣ (11�ڸ�) ex) 010xxxxxxxx ");
	goto_xy(83, 21);
	printf(":");
	goto_xy(83, 28);
	printf("������� (8�ڸ�) ex) xxxxxxxx");
	goto_xy(83, 30);
	printf(":");
	while (1) {
		xx = 0, yy = 0;
		click(&xx, &yy);
		if (xx > 68 && xx < 83) {
			if (yy > 45 && yy < 49) {
				small_box(68, 46, 10, 74, 47, "����", 6);
				Sleep(500);
				return;
			}
		}
		if (xx > 83 && xx < 115) {
			if (yy > 19 && yy < 23) {
				textcolor(6);
				goto_xy(85, 21);
				printf("                                 ");
				goto_xy(85, 21);
				EnableConsoleCursor();
				fflush(stdin);
				gets(phone);
				fflush(stdin);
				HideCursor();
				check = isValidPhone_or_pw_Number(phone, 1);
				if (check == 0) {
					strcpy(phone, " ");
					goto_xy(85, 21);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}

			}
		}
		if (xx > 83 && xx < 115) {
			if (yy > 28 && yy < 32) {
				textcolor(6);
				goto_xy(85, 30);
				printf("                                 ");
				goto_xy(85, 30);
				EnableConsoleCursor();
				brith = getMaskedInput();
				HideCursor();
				check = isValidDate(brith);
				if (check == 0) {
					brith = 0;
					goto_xy(85, 30);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}
			}
		}
		if (xx > 110 && xx < 125) {
			if (yy > 45 && yy < 49) {
				if (brith != 0 && strcmp(phone, " ") != 0) {
					small_box(110, 46, 10, 115, 47, "�α���", 6);
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
						small_box(110, 46, 6, 115, 47, "�α���", 6);
						goto_xy(78, 40);
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
	basic_UI(60,3);
	goto_xy(92, 5);
	textcolor(6);
	printf("ȸ������");
	small_box(68, 46, 6, 74, 47, "����", 6);
	small_box(110, 46, 6, 116, 47, "����", 6);
	goto_xy(81, 11);
	textcolor(4);
	printf("�̸�, ��ȭ��ȣ, ��й�ȣ�� ���� ����");
	textcolor(6);
	goto_xy(81, 17);
	printf("��          ��  :");
	goto_xy(98, 17);
	printf("%s", all[index].name);
	goto_xy(81, 22);
	printf("��  ȭ  ��  ȣ  :");
	goto_xy(98, 22);
	printf("%s", all[index].phone);
	goto_xy(81, 27);
	printf("��          ��  :");  
	goto_xy(98, 27);
	if (strcmp(all[index].gender, "��") == 0) {\
		printf("����");
	}
	else {
		printf("����");
	}
	goto_xy(81, 32);
	printf("�������(8�ڸ�) :");
	goto_xy(98, 32);
	printf("%d", all[index].brith);
	goto_xy(81, 37);
	printf("��й�ȣ(4�ڸ�) :");
	goto_xy(98, 37);
	printf("%s", all[index].pw);
	strcpy(name, all[index].name);
	strcpy(pw,all[index].pw);
	strcpy(phone,all[index].phone);
	while (1) {
		xx = 0, yy = 0;
		click(&xx, &yy);
		if (xx > 68 && xx < 83) {
			if (yy > 45 && yy < 49) {
				small_box(68, 46, 10, 74, 47, "����", 6);
				Sleep(500);
				return;
			}
		}
		if (xx > 97 && xx < 117) {
			if (yy > 20 && yy < 24) {
				textcolor(6);
				goto_xy(98, 22);
				printf("                              ");
				goto_xy(98, 22);
				EnableConsoleCursor();
				scanf("%s", phone);
				HideCursor();
				ph_check = isValidPhone_or_pw_Number(phone, 1);
				if (ph_check == 0) {
					strcpy(phone, all[index].phone);
					goto_xy(98, 22);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}

			}
			else if (yy > 35 && yy < 39) {
				textcolor(6);
				goto_xy(98, 37);
				printf("                                 ");
				goto_xy(98, 37);
				EnableConsoleCursor();
				scanf("%s", pw);
				HideCursor();
				pw_check = isValidPhone_or_pw_Number(pw, 2);
				if (pw_check == 0) {
					strcpy(pw, all[index].pw);
					goto_xy(98, 37);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}
			}
			else if (yy > 15 && yy < 19) {
				textcolor(6);
				goto_xy(98, 17);
				printf("                                ");
				goto_xy(98, 17);
				EnableConsoleCursor();
				scanf("%s", name);
				HideCursor();
			}
		}
		if (xx > 110 && xx < 125) {
			if (yy > 45 && yy < 49) {
				if (pw_check != 0 && ph_check != 0) {
					small_box(110, 46, 10, 116, 47, "����", 6);
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
		goto_xy(93, 5);
		textcolor(6);
		printf("%s��", all[index].name);
		big_box(87, 18, 6, 96, 20, "����");
		big_box(87, 25, 6, 91, 27, "���� ���� ��ȸ");
		big_box(87, 32, 6, 93, 34, "���� ����");
		small_box(68, 46, 6, 72, 47, "�α׾ƿ�", 6);
		small_box(110, 46, 6, 116, 47, "����", 6);
		//ExClick();
		while (1) {
			xx = 0, yy = 0;
			click(&xx, &yy);
			if (xx > 68 && xx < 83) {
				if (yy > 45 && yy < 49) {
					small_box(68, 46, 10, 72, 47, "�α׾ƿ�", 6);
					Sleep(500);
					return;
				}
			}
			if (xx > 87 && xx < 108) {
				if (yy > 31 && yy < 37) {
					big_box(87, 18, 6, 96, 20, "����", 6);
					big_box(87, 25, 6, 91, 27, "���� ���� ��ȸ", 6);
					big_box(87, 32, 10, 93, 34, "���� ����", 6);
					choice = 3;
					xx, yy = 0;
				}
			    else if (yy > 17 && yy < 22) {
					big_box(87, 18, 10, 96, 20, "����", 6);
					big_box(87, 25, 6, 91, 27, "���� ���� ��ȸ", 6);
					big_box(87, 32, 6, 93, 34, "���� ����", 6);
					choice = 1;
					xx, yy = 0;
				}
			}
			if (xx > 110 && xx < 125) {
				if (yy > 45 && yy < 49) {
					if (choice != 0) {
						small_box(110, 46, 10, 116, 47, "����", 6);
						Sleep(500);
						if (choice == 3) {
							modifying_membership(index);
							choice = 0;
							break;
						}
						else if (choice == 1) {
							designer_choice(index);
							choice = 0;
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
	file_read();
	box_clear();
	goto_xy(92, 5);
	textcolor(6);
	printf("ȸ�� �α���");
	small_box(68, 46, 6,74,47,"����",6);
	small_box(110, 46, 6,115,47,"�α���",6);
	char phone[20] = " ";
	char pw[15] = " ";
	int xx = 0, yy = 0, lr = 0;
	int check = 0;
	goto_xy(83, 19);
	printf("��ȭ��ȣ(11�ڸ� ex) 010xxxxxxxx ");
	goto_xy(83, 21);
	printf(":");
	goto_xy(83, 28);
	printf("��й�ȣ(���� 4�ڸ�) ex) 0000");
	goto_xy(83, 30);
	printf(":");
	//ExClick();
	while (1) {
		xx = 0, yy = 0;
		click(&xx, &yy);
		if (xx > 68 && xx < 83) {
			if (yy > 45 && yy < 49) {
				small_box(68, 46, 10,74,47,"����",6);
				Sleep(500);
				return;
			}
		}
		if (xx > 83 && xx < 115) {
			if (yy > 19 && yy < 23) {
				textcolor(6);
				goto_xy(85, 21);
				printf("                                 ");
				goto_xy(85, 21);
				EnableConsoleCursor();
				fflush(stdin);
				scanf("%s",phone);
				fflush(stdin);
				HideCursor();
				check = isValidPhone_or_pw_Number(phone, 1);
				if (check == 0) {
					strcpy(phone, " ");
					goto_xy(85, 21);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}

			}
		}
		if (xx > 83 && xx < 115) {
			if (yy > 28 && yy < 32) {
				textcolor(6);
				goto_xy(85, 30);
				printf("                                 ");
				goto_xy(85, 30);
				EnableConsoleCursor();
				char ch;
				int i = 0;
				while (1) {
					ch = _getch();
					fflush(stdin);
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
					goto_xy(85, 30);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}
			}
		}
		if (xx > 110 && xx < 125) {
			if (yy > 45 && yy < 49) {
				if (strcmp(pw, " ") != 0 && strcmp(phone, " ") != 0) {
					small_box(110, 46, 10,115,47,"�α���",6);
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
						small_box(110, 46, 6,115,47,"�α���",6);
						goto_xy(78, 40);
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
		goto_xy(94, 5);
		textcolor(6);
		printf("�α���");
		big_box(87, 15, 6,96,17,"ȸ��",6);
		big_box(87, 23, 6,94,25,"�����̳�",6);
		big_box(87, 31, 6,95,33,"������",6);
		small_box(68, 46, 6,74,47,"����",6);
		small_box(110, 46, 6,116,47,"����",6);
		//ExClick();
		while (1) {
			check = 0;
			xx = 0, yy = 0;
			click(&xx, &yy);
			if (xx > 68 && xx < 83) {
				if (yy > 45 && yy < 49) {
					small_box(68, 46, 10,74,47,"����",6);
					Sleep(500);
					return;
				}
			}
			if (xx > 87 && xx < 108) {
				if (yy > 14 && yy < 19) {
					big_box(87, 15, 10,96,17,"ȸ��", 6);
					big_box(87, 23, 6,94,25,"�����̳�",6);
					big_box(87, 31, 6,95,33,"������",6);
					choice = 1;
				}
				else if (yy > 22 && yy < 28) {
					big_box(87, 15, 6, 96, 17, "ȸ��", 6);
					big_box(87, 23, 10, 94, 25, "�����̳�", 6);
					big_box(87, 31, 6, 95, 33, "������", 6);
					choice = 2;
				}
			}
			if (xx > 110 && xx < 125) {
				if (yy > 45 && yy < 49) {
					if (choice != 0) {
						small_box(110, 46, 10,116,47,"����", 6);
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
void pw_find() { //��й�ȣ ã�� ;
	file_read();
	box_clear();
	int xx, yy, lr = 0;
	char name[20] = " ";
	char phone[15] = " ";
	int brith = 0;
	int check = 0;
	basic_UI(60,3);
	goto_xy(91, 5);
	printf("��й�ȣ ã��");
	small_box(68, 46, 6,74,47,"����",6);
	small_box(110, 46, 6,116,47,"ã��",6);
	goto_xy(83, 17);
	printf("�̸� ex) ȫ�浿");
	goto_xy(83, 19);
	printf(":");
	goto_xy(83, 26);
	printf("��ȭ��ȣ 11�ڸ� ex) 010xxxxxxxx ");
	goto_xy(83, 28);
	printf(":");
	goto_xy(83, 34);
	printf("�������(8�ڸ�) ex) 20030728 ");
	goto_xy(83, 36);
	printf(":");
	//ExClick();
	while (1) {
		xx = 0, yy = 0;
		click(&xx, &yy);
		if (xx > 68 && xx < 83) {
			if (yy > 45 && yy < 49) {
				small_box(68, 46, 10,74,47,"����",6);
				Sleep(500);
				return;
			}
		}

		if (xx > 83 && xx < 117) {
			if (yy > 18 && yy < 21) {
				strcpy(name, " ");
				textcolor(6);
				goto_xy(88, 19);
				printf("                                 ");
				goto_xy(88, 19);
				EnableConsoleCursor();
				gets(name);
				HideCursor();
			}
		}
		if (xx > 83 && xx < 117) {
			if (yy > 27 && yy < 30) {
				textcolor(6);
				goto_xy(88, 28);
				printf("                                 ");
				goto_xy(88, 28);
				EnableConsoleCursor();
				scanf("%s", phone);
				HideCursor();
				check = isValidPhone_or_pw_Number(phone, 1);
				if (check == 0) {
					strcpy(phone, " ");
					goto_xy(88, 28);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}

			}
		}
		if (xx > 83 && xx < 117) {
			if (yy > 34 && yy < 38) {
				textcolor(6);
				goto_xy(88, 36);
				printf("                                 ");
				goto_xy(88, 36);
				EnableConsoleCursor();
				scanf("%d", &brith);
				HideCursor();
				check = isValidDate(brith);
				if (check == 0) {
					brith = 0;
					goto_xy(88, 36);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}
			}
		}
		if (xx > 110 && xx < 125) {
			if (yy > 45 && yy < 49) {
				if (strcmp(name, " ") != 0 &&  strcmp(phone, " ") != 0 && brith != 0) {
					check = -1;
					for (int i = 0; i < member_count; i++) {
						if (strcmp(all[i].name,name) == 0 && strcmp(all[i].phone, phone) == 0 && all[i].brith == brith) {
							check = i;
						}
					}
					if (check >= 0) {
						small_box(110, 46, 10,116,47,"ã��",6);
						Sleep(500);
						box_clear();
						goto_xy(91, 5);
						printf("��й�ȣ ã��");
						small_box(68, 46, 6,74,47,"����",6);
						small_box(110, 46, 6,115,47,"�α���",6);
						goto_xy(84, 24);
						printf("%s���� ��й�ȣ�� %s�Դϴ�.", all[check].name,all[check].pw);
						xx = 0;
						yy = 0;
						click(&xx, &yy);
						while (1) {
							if (xx > 68 && xx < 83) {
								if (yy > 45 && yy < 49) {
									small_box(68, 46, 10,74,47,"����",6);
									Sleep(500);
									break;
								}
							}
							else if (xx > 110 && xx < 125) {
								if (yy > 45 && yy < 49) {
									small_box(110, 46, 10,115,47,"�α���",6);
									Sleep(500);
									member_login();
									break;
								}
							}
						}
					}
					else {
						textcolor(4);
						goto_xy(79, 40);
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
	goto_xy(92, 5);
	textcolor(6);
	printf("ȸ������");
	small_box(80, 10, 6,86,11,"����",9);
	small_box(97, 10, 6,103,11,"����",12);
	small_box(68, 46, 6,74,47,"����",6);
	small_box(110, 46, 6,116,47,"����",6);
	goto_xy(80, 18);
	printf("�̸� ex) ȫ�浿");
	goto_xy(80, 20);
	printf(":");
	goto_xy(80, 24);
	printf("��ȭ��ȣ 11�ڸ� ex) 010xxxxxxxx ");
	goto_xy(80, 26);
	printf(":");
	goto_xy(80, 30);
	printf("�������(8�ڸ�) ex) 20030728 ");
	goto_xy(80, 32);
	printf(":");
	goto_xy(80, 36);
	printf("��й�ȣ(���� 4�ڸ�) ex) 0000");
	goto_xy(80, 38);
	printf(":");
	//Mouse();
	//ExClick();
	while (1) {
		xx = 0, yy = 0;
		click(&xx, &yy);
		if (xx > 68 && xx < 88) {
			if (yy > 45 && yy < 49) {
				small_box(68, 46, 10,74,47,"����",6);
				Sleep(500);
				return;
			}
		}
		if (xx > 80 && xx < 94) {
			if (yy > 9 && yy < 13) {
				strcpy(gender, "��");
				small_box(97, 10, 6,103,11,"����",12);
				small_box(80, 10, 10,86,11,"����",9);
			}
		}
		if (xx > 97 && xx < 112) {
			if (yy > 9 && yy < 13) {
				strcpy(gender, "��");
				small_box(80, 10,6,86,11,"����",9);
				small_box(97, 10, 10,103,11,"����",12);
			}
		}
	    if (xx > 68 && xx < 110) {
			if (yy > 18 && yy < 22) {
				strcpy(name, " ");
				textcolor(6);
				goto_xy(88, 20);
				printf("                                 ");
				goto_xy(88, 20);
				EnableConsoleCursor();
				gets(name);
				HideCursor();
			}
		}
		if (xx > 80 && xx < 112) {
			if (yy > 25 && yy < 28) {
				textcolor(6);
				goto_xy(88, 26);
				printf("                                 ");
				goto_xy(88, 26);
				EnableConsoleCursor();
				scanf("%s", phone);
				HideCursor();
				check = isValidPhone_or_pw_Number(phone,1);
				if (check == 0) {
					strcpy(phone, " ");
					goto_xy(88, 26);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}

			}
		}
		if (xx > 82 && xx < 108) {
			if (yy > 29 && yy < 34) {
				textcolor(6);
				goto_xy(88, 32);
				printf("                                 ");
				goto_xy(88, 32);
				EnableConsoleCursor();
				scanf("%d", &brith);
				HideCursor();
				check = isValidDate(brith);
				if (check == 0) {
					brith = 0;
					goto_xy(88, 32);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}
			}
		}
		if (xx > 82 && xx < 111) {
			if (yy > 36 && yy < 40) {
				textcolor(6);
				goto_xy(88, 38);
				printf("                                 ");
				goto_xy(88, 38);
				EnableConsoleCursor();
				scanf("%s", pw);
				HideCursor();
				check = isValidPhone_or_pw_Number(pw, 2);
				if (check == 0) {
					strcpy(pw, " ");
					goto_xy(88, 38);
					printf("������ �ùٸ��� �ʽ��ϴ�..");
				}
			}
		}
		if (xx > 110 && xx < 125) {
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
						goto_xy(86, 41);
						printf("�̹� ���Ե� ȸ�� �Դϴ�.");
						continue;
					}
					else {
						small_box(110, 46, 10,116,47,"����",6);
						Sleep(500);
						box_clear();
						goto_xy(92, 5);
						printf("ȸ������");
						small_box(68, 46, 6,74,47,"����",6);
						small_box(110, 46, 6,115,47,"�α���",6);
						goto_xy(84, 24);
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
							if (xx > 68 && xx < 83) {
								if (yy > 45 && yy < 49) {
									small_box(68, 46, 10,74,47,"����",6);
									Sleep(500);
									break;
								}
							}
							else if (xx > 110 && xx < 125) {
								if (yy > 45 && yy < 49) {
									small_box(110, 46, 10,115,47,"�α���",6);
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
	box_clear(60,3);
	HideCursor();
	int xx = 0, yy = 0, lr = 0;
	basic_UI(60,3);
	goto_xy(92, 5);
	printf("���� ��");
	big_box(87,20, 6, 96, 22,"����");
	big_box(87, 27, 6, 91, 29,"���� ���� ��ȸ");
	small_box(66, 46, 6, 71, 47, "�α���",6);
	small_box(89, 46, 6, 93, 47,"ȸ������",6);
	small_box(112, 46, 6, 117, 47,"PW ã��",6);
	Mouse();
	//ExClick();
	while (1) {
		xx = 0, yy = 0, lr = 0;
		click(&xx, &yy);
		if (xx > 89 && xx < 104) {
			if (yy > 46 && yy < 49) {
				small_box(89, 46, 10,93,47,"ȸ������",6);
				Sleep(500);
				box_clear();
				membership();
				break;
			}
		}
		if (xx > 66 && xx < 80) {
			if (yy > 46 && yy < 49) {
				small_box(66, 46, 10,71,47,"�α���",6);
				Sleep(500);
				login_menu_choice();
				break;
			}
		}
		if (xx > 112 && xx < 127) {
			if (yy > 46 && yy < 49) {
				small_box(112, 46, 10,117,47,"PW ã��",6);
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
	initial_screen();
}
