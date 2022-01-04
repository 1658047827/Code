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

	list<string> Content; //�ı�����
	list<string>::iterator point; //���������Ӧ���ַ����ĵ�����
	list<string>::iterator top; //��ǰҳ��Ķ���
	int ColP; //������ڵ��У�ʵ���Ϲ�����������һ�е����
	int RowP; //����ڵ�ǰҳ����ʾ����

	UndoRedo* urdo; //��������ģ��
public:
	Text();
	void Enter(); //�س�
	void Insert(char ch); //����
	char Delete(); //ɾ��
	void MoveCursor(int d); //ֻ���Ĺ������꣬���ʵ���ϻ�δ�ƶ�

	bool OpenFile(string FileNameAndExt); //��ȡ�ļ�
	bool SaveFile(string FileNameAndExt); //д���ļ�

	bool Search(string pattern); //�ӵ�ǰ��꿪ʼ����Ѱ��

	void Print(); //��ӡҳ�浽Ĭ�ϻ�����
};

