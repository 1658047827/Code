#pragma once
#include "text.h"
using std::list;

struct Ope {
	int opeType;	//��¼��һ�β��������� -1~������ 0~���� 1~ɾ�� 2~�س�
	char ch;		//��¼����������
	int Line;		//��¼��������λ�ã����ı��Ŀ�ͷ��ʼ���𣩣�����λ��
	int Col;		//��¼��������λ��
	Ope() :opeType(-1), ch(' '), Line(0), Col(0) {}
	Ope(int _type, char _ch, int _line, int _col) :opeType(_type), ch(_ch), Line(_line), Col(_col) {}
};

class UndoRedo {
private:
	friend class Text;
	list<Ope> URlist;			//�洢����������Ϣ��˫������
	list<Ope>::iterator cur;	//���Ҫ������ʹ�õ�ǰָ�������ڵ����Ϣ
	Text* Text;					//ָ���ı������ָ��
public:
	UndoRedo();
	bool Undo();
	bool Redo();
	void AddUndo(int type, char ch);
	bool IsAddNode;
};