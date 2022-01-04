#include "Vim.h"

Vim::Vim() :Mode(0), CommandCursorCol(8), QCommand(0) {
	screen = new Screen();
	screen->vim = this;
}

void Vim::Welcome() {
	printf("Simple Vim\n\n");
	printf("1 输入  :open filename  打开文件\n");
	printf("2 输入  :W filename     保存文件\n");
	printf("3 输入  :q              退出程序\n");
	printf("4 按下  i               从normal进入insert\n");
	printf("5 按下  Esc             从insert进入normal\n");
	printf("6 按下  x               删除当前光标处的一个字符\n");
	printf("7 按下  u               撤销操作\n");
	printf("8 按下  r               重做操作\n");
	printf("9 输入  /pattern        从当前光标开始往后搜索pattern\n");
	screen->FlushScreen();
	char t = _getch(); //停顿一下
}

void Vim::GetInput() {
	char inputChar;
	inputChar = _getch();
	if (Mode == 0) { //normal模式
		if (inputChar == ':' || inputChar == '/') { //转成命令模式 或 搜索
			Mode = 2;
			Command.clear();
			Command.push_back(inputChar);
			CommandCursorCol = 8 + Command.size();
		}
		else {
			switch (inputChar) {
			case 'h': //左
				Text.MoveCursor(0);
				break;
			case 'j': //下
				Text.MoveCursor(3);
				break;
			case 'k': //上
				Text.MoveCursor(2);
				break;
			case 'l': //右
				Text.MoveCursor(1);
				break;
			case 'i': //转成插入模式
				Mode = 1;
				break;
			case 'u': //撤销
				Text.urdo->Undo();
				break;
			case 'r': //重做
				Text.urdo->Redo();
				break;
			case 'x': //删除
				Text.Delete();
				break;
			}
		}
	}
	else if (Mode == 1) { //insert模式
		if (inputChar == 27) Mode = 0;
		else if (inputChar == -32) { //读取方向键
			inputChar = _getch();
			switch (inputChar) {
			case 72: //上
				Text.MoveCursor(2);
				break;
			case 80: //下
				Text.MoveCursor(3);
				break;
			case 75: //左
				Text.MoveCursor(0);
				break;
			case 77: //右
				Text.MoveCursor(1);
				break;
			}
		}
		else {
			switch (inputChar) {
			case 13: //回车
				Text.Enter();
				break;
			case 8: //退格
				Text.Delete();
				break;
			case '\t': //制表符转化为4个空格
				for(int i=0;i<4;++i) Text.Insert(' ');
				break;
			default: //插入
				Text.Insert(inputChar);
				break;
			}
		}
	}
	else { //command模式
		if (Command.size() == 0) {
			Mode = 0;
			ProcessResult = "";
		}
		else {
			switch (inputChar) {
			case 13: //回车处理命令
				ProcessCommand();
				break;
			case 8: //命令内容退格
				CommandDelete();
				break;
			case 27: //Esc退出command模式
				Command.clear();
				ProcessResult.clear();
				Mode = 0;
			default: //命令内容插入
				CommandInsert(inputChar);
				break;
			}
		}
	}
}

void Vim::Work() { //循环执行函数
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

	//输出<模式>：命令 | 处理结果
	putchar('<');
	if (Mode == 0 || Mode == 2) printf("Normal");
	else printf("Insert");
	putchar('>');
	if (Mode == 2) { //只在命令模式下才输出命令和处理结果
		cout << Command << " | ";
		cout << ProcessResult;
	}
}

void Vim::CommandInsert(char ch) {
	if (ProcessResult.size() != 0) ProcessResult.clear(); //进行下一次命令编辑，需要把处理结果清空
	Command.push_back(ch);
	CommandCursorCol++;
}

void Vim::CommandDelete() {
	if (ProcessResult.size() != 0) ProcessResult.clear(); //进行下一次命令编辑，需要把处理结果清空
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
		ProcessResult = "无效命令";

	Command.clear();//清空命令
}



