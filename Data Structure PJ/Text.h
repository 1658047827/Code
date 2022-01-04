#pragma once
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include "UndoRedo.h"

using std::list;
using std::string;
using std::cout;
using std::ios;
using std::ifstream;
using std::ofstream;

class Text {
private:
	friend class Vim;
	friend class UndoRedo;
	friend class Screen;

	list<string> Content; //文本内容
	list<string>::iterator point; //光标行所对应的字符串的迭代器
	list<string>::iterator top; //当前页面的顶部
	int ColP; //光标所在的列，实际上光标的竖线在这一列的左边
	int RowP; //光标在当前页面显示的行

	UndoRedo* urdo; //撤消重做模块
public:
	Text();
	void Enter(); //回车
	void Insert(char ch); //插入
	char Delete(); //删除
	void MoveCursor(int d); //只更改光标的坐标，光标实际上还未移动

	bool OpenFile(string FileNameAndExt); //读取文件
	bool SaveFile(string FileNameAndExt); //写入文件

	bool Search(string pattern); //从当前光标开始往后寻找

	void Print(); //打印页面到默认缓冲区
};

