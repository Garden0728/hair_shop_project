#define _CRT_SECURE_NO_WARNINGS
#define MAX 200 //ȸ�� �ִ� �ο� ��
#include<stdio.h>
#include<Windows.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>
INPUT_RECORD rec;
DWORD dwNOER;
HANDLE CIN = 0;
int member_count = 0;
typedef struct { // ȸ�� ����ü
	char name[20];
	char phone[20];
	char gender[5];
	int brith;
	char pw[15];
}member;
member all[MAX];
void HideCursor() { //���콺 Ŀ�� ����� �Լ�
	CONSOLE_CURSOR_INFO cursor_info = { 1, FALSE };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
void EnableConsoleCursor() { //���콺 Ŀ�� ���̰� �ϴ� �Լ�
	CONSOLE_CURSOR_INFO cursor_info = { 1, TRUE };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
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
			fscanf(member,"%s %s %s %d %s\n", all[member_count].name, all[member_count].phone, all[member_count].gender,&all[member_count].brith,all[member_count].pw);
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
void textcolor(int colorNum) { // �۾� �÷� �ٲ��ִ� �Լ�
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}
void goto_xy(int x, int y)  // ��ǥ��
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
void clearconsole() { //�ܼ�â Ŭ����
	printf("Asd");
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
	int xx, yy, lr;
	while (1) {
		click(&xx, &yy);
		printf("%3d %3d\n", xx, yy);
	}
}
void basic_UI() { // �⺻ UI
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
void small_box(int x,int y,int color) { // üũ �ڽ� UI
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

} 
void big_box(int x, int y,int color) { // üũ�ڽ� ū�� UI
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
}
void box_clear() { //�⺻ UI Ŭ���� ���ִ� �Լ�
	int x = 62;
	int y = 5;
	goto_xy(62, 5);
	printf("                 ");
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
int  member_login() { //ȸ�� �α��� �ϴ� �κ�
	box_clear();
	small_box(38, 46, 6);
	small_box(80, 46, 6);
	char phone[20] = " ";
	char pw[15] = " ";
	int xx, yy, lr = 0;
	int check = 0;
	goto_xy(62, 5);
	textcolor(6);
	printf("ȸ�� �α���");
	goto_xy(44, 47);
	printf("����");
	goto_xy(85, 47);
	printf("�α���");
	goto_xy(53, 19);
	printf("��ȭ��ȣ 11�ڸ� ex) 010xxxxxxxx ");
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
				small_box(38, 46, 10);
				textcolor(6);
				goto_xy(44, 47);
				printf("����");
				Sleep(500);
				break;
			}
		}
		if (xx > 53 && xx < 85) {
			if (yy > 19 && yy < 23) {
				textcolor(6);
				goto_xy(55, 21);
				printf("                                 ");
				goto_xy(55, 21);
				EnableConsoleCursor();
				scanf("%s", phone);
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
					small_box(80, 46, 10);
					textcolor(6);
					goto_xy(85, 47);
					printf("�α���");
					check = -1;
					for (int i = 0; i < member_count; i++) {
						if (strcmp(all[i].phone, phone) == 0 && strcmp(all[i].pw, pw) == 0) {
							check = i;
							break;
						}
					}
					Sleep(500);
					if (check >= 0) {
						return 1;
					}
					else {
						small_box(80, 46, 6);
						textcolor(6);
						goto_xy(85, 47);
						printf("�α���");
						goto_xy(49, 40);
						textcolor(4);
						printf("��ȭ��ȣ Ȥ�� ��й�ȣ�� ã�� �� �����ϴ�.");
					}
				}

			}
		}
	}

}
void login_menu_choice() { // �α��� ���� �ϴ� �κ� (ȸ�� ������ �����̳�)
	int xx, yy, lr = 0;
	int choice = 0;
	int check = 0;
	//ExClick();
	while (1) {
		box_clear();
		big_box(57, 15, 6);
		big_box(57, 23, 6);
		big_box(57, 31, 6);
		small_box(38, 46, 6);
		small_box(80, 46, 6);
		goto_xy(64, 5);
		textcolor(6);
		printf("�α���");
		goto_xy(66, 17);
		printf("ȸ��");
		goto_xy(64, 25);
		printf("�����̳�");
		goto_xy(65, 33);
		printf("������");
		goto_xy(44, 47);
		printf("����");
		goto_xy(86, 47);
		printf("����");
		while (1) {
			xx = 0, yy = 0;
			click(&xx, &yy);
			if (xx > 38 && xx < 53) {
				if (yy > 45 && yy < 49) {
					small_box(38, 46, 10);
					textcolor(6);
					goto_xy(44, 47);
					printf("����");
					check = 1;
					Sleep(400);
					break;
				}
			}
			if (xx > 57 && xx < 78) {
				if (yy > 14 && yy < 19) {
					big_box(57, 15, 10);
					goto_xy(66, 17);
					textcolor(6);
					printf("ȸ��");
					big_box(57, 23, 6);
					goto_xy(64, 25);
					printf("�����̳�");
					big_box(57, 31, 6);
					goto_xy(65, 33);
					printf("������");
					choice = 1;
				}
			}
			if (xx > 80 && xx < 95) {
				if (yy > 45 && yy < 49) {
					if (choice != 0) {
						small_box(80, 46, 10);
						textcolor(6);
						goto_xy(86, 47);
						printf("����");
						Sleep(500);
						choice = 0;
						check = member_login();
						break;
					}
				}
			}
		}
		if (check == 1) {
			break;
		}
		check = 0;
	}

}
void pw_find() {
	box_clear();
	int xx, yy, lr = 0;
	char name[20] = " ";
	char phone[15] = " ";
	int brith = 0;
	int check = 0;
	basic_UI();
	goto_xy(61, 5);
	textcolor(6);
	printf("��й�ȣ ã��");
	small_box(38, 46, 6);
	small_box(80, 46, 6);
	goto_xy(44, 47);
	printf("����");
	goto_xy(86, 47);
	printf("����");
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
	ExClick();
	while (1) {
		xx = 0, yy = 0;
		click(&xx, &yy);
		if (xx > 38 && xx < 53) {
			if (yy > 45 && yy < 49) {
				small_box(38, 46, 10);
				textcolor(6);
				goto_xy(44, 47);
				printf("����");
				Sleep(500);
				return;
			}
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
			check = isValidPhone_or_pw_Number(phone, 1);
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
	small_box(50, 10, 6);
	small_box(67, 10, 6);
	small_box(38, 46, 6);
	small_box(80, 46, 6);
	goto_xy(56, 11);
	textcolor(BLUE);
	printf("����");
	goto_xy(73,11);
	textcolor(RED);
	printf("����");
	goto_xy(44, 47);
	textcolor(6);
	printf("����");
	goto_xy(86, 47);
	textcolor(6);
	printf("����");
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
				small_box(38, 46, 10);
				textcolor(6);
				goto_xy(44, 47);
				printf("����");
				Sleep(500);
				return;
			}
		}
		if (xx > 50 && xx < 64) {
			if (yy > 9 && yy < 13) {
				strcpy(gender, "��");
				small_box(67, 10, 6);
				goto_xy(73, 11);
				textcolor(RED);
				printf("����");
				small_box(50, 10, 10);
				goto_xy(56, 11);
				textcolor(BLUE);
				printf("����");
			}
		}
		if (xx > 67 && xx < 82) {
			if (yy > 9 && yy < 13) {
				strcpy(gender, "��");
				small_box(50, 10,6);
				goto_xy(56, 11);
				textcolor(BLUE);
				printf("����");
				small_box(67, 10, 10);
				goto_xy(73, 11);
				textcolor(RED);
				printf("����");
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
					small_box(80, 46, 10);
					textcolor(6);
					goto_xy(86, 47);
					printf("����");
					Sleep(500);
					box_clear();
					goto_xy(62, 5);
					textcolor(6);
					printf("ȸ������");
					small_box(38, 46, 6);
					small_box(80, 46, 6);
					goto_xy(44, 47);
					textcolor(6);
					printf("����");
					goto_xy(85, 47);
					textcolor(6);
					printf("�α���");
					if (check == 1) {
						goto_xy(52, 24);
						printf("�̹� ���Ե� ��ȭ��ȣ �Դϴ�.");
					}
					else {
						goto_xy(54, 24);
						printf("ȸ�������� �Ϸ�Ǿ����ϴ�.");
						strcpy(all[member_count].name, name);
						strcpy(all[member_count].phone, phone);
						strcpy(all[member_count].pw, pw);
						strcpy(all[member_count].gender, gender);
						all[member_count].brith = brith;
						file_append();
					}
					xx = 0;
					yy = 0;
					click(&xx, &yy);
					while (1) {
						if (xx > 38 && xx < 53) {
							if (yy > 45 && yy < 49) {
								small_box(38, 46, 10);
								textcolor(6);
								goto_xy(44, 47);
								printf("����");
								Sleep(500);
								break;
							}
						}
						else if (xx > 80 && xx < 95) {
							if (yy > 45 && yy < 49) {
								small_box(80, 46, 10);
								textcolor(6);
								goto_xy(85, 47);
								printf("�α���");
								Sleep(500);
								member_login();
								break;
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
	box_clear();
	HideCursor();
	int xx, yy, lr = 0;
	basic_UI();
	big_box(57,20, 6);
	big_box(57, 27, 6);
	small_box(36, 46, 6);
	small_box(59, 46, 6);
	small_box(82, 46, 6);
	goto_xy(62, 5);
	printf("���� ��");
	goto_xy(66, 22);
	printf("����");
	goto_xy(61, 29);
	printf("���� ���� ��ȸ");
	goto_xy(41, 47);
	printf("�α���");
	goto_xy(63, 47);
	printf("ȸ������");
	goto_xy(87, 47);
	printf("PW ã��");
	Mouse();
	//ExClick();
	while (1) {
		click(&xx, &yy);
		if (xx > 59 && xx < 74) {
			if (yy > 46 && yy < 49) {
				small_box(59, 46, 10);
				goto_xy(63, 47);
				textcolor(6);
				printf("ȸ������");
				Sleep(500);
				box_clear();
				membership();
				break;
			}
		}
		if (xx > 36 && xx < 50) {
			if (yy > 46 && yy < 49) {
				small_box(36, 46, 10);
				goto_xy(41, 47);
				textcolor(6);
				printf("�α���");
				Sleep(500);
				login_menu_choice();
				break;
			}
		}
		if (xx > 82 && xx < 97) {
			if (yy > 46 && yy < 49) {
				small_box(82, 46, 10);
				goto_xy(87, 47);
				textcolor(6);
				printf("PW ã��");
				Sleep(500);
				pw_find();

			}
		}
		
	}
	initial_screen();
}
int main(void) { //�����Լ�
	file_read();
	initial_screen();
}
