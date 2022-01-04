#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <conio.h>
#include <Windows.h>
#include "Text.h"
#include "Screen.h"

using std::cin;
using std::cout;
using std::vector;
using std::string;
using std::list;

class Vim {
private:
	int Mode;				//当前的模式:0-normal 1-insert 2-command

	Text Text;				//文本对象

	string Command;			//命令字符串
	string ProcessResult;	//命令处理结果
	int CommandCursorCol;	//命令模式下光标的列位置
	int QCommand;			//标记是否需要执行退出命令

	Screen* screen;			//屏幕缓冲区显示类
	friend class Screen;
public:
	Vim();
	void Welcome();
	void GetInput();
	void Work(); //循环执行的主体
	
	void PrintCommand();
	void CommandInsert(char ch);
	void CommandDelete();
	void ProcessCommand();
};


