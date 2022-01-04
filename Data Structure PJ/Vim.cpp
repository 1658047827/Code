#include "Vim.h"

Vim::Vim() :Mode(0), CommandCursorCol(8), QCommand(0) {
	screen = new Screen();
	screen->vim = this;
}

void Vim::Welcome() {
	printf("Simple Vim\n\n");
	printf("1 ����  :open filename  ���ļ�\n");
	printf("2 ����  :W filename     �����ļ�\n");
	printf("3 ����  :q              �˳�����\n");
	printf("4 ����  i               ��normal����insert\n");
	printf("5 ����  Esc             ��insert����normal\n");
	printf("6 ����  x               ɾ����ǰ��괦��һ���ַ�\n");
	printf("7 ����  u               ��������\n");
	printf("8 ����  r               ��������\n");
	printf("9 ����  /pattern        �ӵ�ǰ��꿪ʼ��������pattern\n");
	screen->FlushScreen();
	char t = _getch(); //ͣ��һ��
}

void Vim::GetInput() {
	char inputChar;
	inputChar = _getch();
	if (Mode == 0) { //normalģʽ
		if (inputChar == ':' || inputChar == '/') { //ת������ģʽ �� ����
			Mode = 2;
			Command.clear();
			Command.push_back(inputChar);
			CommandCursorCol = 8 + Command.size();
		}
		else {
			switch (inputChar) {
			case 'h': //��
				Text.MoveCursor(0);
				break;
			case 'j': //��
				Text.MoveCursor(3);
				break;
			case 'k': //��
				Text.MoveCursor(2);
				break;
			case 'l': //��
				Text.MoveCursor(1);
				break;
			case 'i': //ת�ɲ���ģʽ
				Mode = 1;
				break;
			case 'u': //����
				Text.urdo->Undo();
				break;
			case 'r': //����
				Text.urdo->Redo();
				break;
			case 'x': //ɾ��
				Text.Delete();
				break;
			}
		}
	}
	else if (Mode == 1) { //insertģʽ
		if (inputChar == 27) Mode = 0;
		else if (inputChar == -32) { //��ȡ�����
			inputChar = _getch();
			switch (inputChar) {
			case 72: //��
				Text.MoveCursor(2);
				break;
			case 80: //��
				Text.MoveCursor(3);
				break;
			case 75: //��
				Text.MoveCursor(0);
				break;
			case 77: //��
				Text.MoveCursor(1);
				break;
			}
		}
		else {
			switch (inputChar) {
			case 13: //�س�
				Text.Enter();
				break;
			case 8: //�˸�
				Text.Delete();
				break;
			case '\t': //�Ʊ��ת��Ϊ4���ո�
				for(int i=0;i<4;++i) Text.Insert(' ');
				break;
			default: //����
				Text.Insert(inputChar);
				break;
			}
		}
	}
	else { //commandģʽ
		if (Command.size() == 0) {
			Mode = 0;
			ProcessResult = "";
		}
		else {
			switch (inputChar) {
			case 13: //�س���������
				ProcessCommand();
				break;
			case 8: //���������˸�
				CommandDelete();
				break;
			case 27: //Esc�˳�commandģʽ
				Command.clear();
				ProcessResult.clear();
				Mode = 0;
			default: //�������ݲ���
				CommandInsert(inputChar);
				break;
			}
		}
	}
}

void Vim::Work() { //ѭ��ִ�к���
	Welcome();
	for (;;) {
		screen->cls();
		Text.Print();
		PrintCommand();
		screen->AdjustShowedCursor();
		screen->FlushScreen();
		GetInput();

		if (QCommand) break;
	}
}

void Vim::PrintCommand() {
	screen->StdGoToXY(0, 28);
	for (int i = 0; i < 31; ++i) putchar('-');
	printf("Command");
	for (int i = 0; i < 32; ++i) putchar('-');
	putchar('\n');

	//���<ģʽ>������ | ������
	putchar('<');
	if (Mode == 0 || Mode == 2) printf("Normal");
	else printf("Insert");
	putchar('>');
	if (Mode == 2) { //ֻ������ģʽ�²��������ʹ�����
		cout << Command << " | ";
		cout << ProcessResult;
	}
}

void Vim::CommandInsert(char ch) {
	if (ProcessResult.size() != 0) ProcessResult.clear(); //������һ������༭����Ҫ�Ѵ��������
	Command.push_back(ch);
	CommandCursorCol++;
}

void Vim::CommandDelete() {
	if (ProcessResult.size() != 0) ProcessResult.clear(); //������һ������༭����Ҫ�Ѵ��������
	if (Command.size()) Command.pop_back();
	CommandCursorCol--;
}

void Vim::ProcessCommand() {
	if (Command[0] == '/') {
		bool ret = Text.Search(Command.substr(1));

		if (ret) ProcessResult = "Found";
		else ProcessResult = "Not Found";
	}
	else if (Command.substr(0, 6) == ":open ") {
		if (Text.OpenFile(Command.substr(6)))
			ProcessResult = "Opened";
		else ProcessResult = "Open Failed";
	}
	else if (Command[1] == 'w' && Command[2] == ' ') {
		if (Text.SaveFile(Command.substr(3)))
			ProcessResult = "Saved";
		else ProcessResult = "Save Failed";
	}
	else if (Command.size() == 2 && Command[1] == 'q') {
		QCommand = 1;
		ProcessResult = "Quit";
	}
	else
		ProcessResult = "��Ч����";

	Command.clear();//�������
}



