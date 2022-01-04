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
	int Mode;				//��ǰ��ģʽ:0-normal 1-insert 2-command

	Text Text;				//�ı�����

	string Command;			//�����ַ���
	string ProcessResult;	//�������
	int CommandCursorCol;	//����ģʽ�¹�����λ��
	int QCommand;			//����Ƿ���Ҫִ���˳�����

	Screen* screen;			//��Ļ��������ʾ��
	friend class Screen;
public:
	Vim();
	void Welcome();
	void GetInput();
	void Work(); //ѭ��ִ�е�����
	
	void PrintCommand();
	void CommandInsert(char ch);
	void CommandDelete();
	void ProcessCommand();
};


