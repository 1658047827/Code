#pragma once
#include "text.h"
using std::list;

struct Ope {
	int opeType;	//记录上一次操作的类型 -1~无意义 0~插入 1~删除 2~回车
	char ch;		//记录操作的内容
	int Line;		//记录操作的行位置（从文本的开头开始算起），操作位置
	int Col;		//记录操作的列位置
	Ope() :opeType(-1), ch(' '), Line(0), Col(0) {}
	Ope(int _type, char _ch, int _line, int _col) :opeType(_type), ch(_ch), Line(_line), Col(_col) {}
};

class UndoRedo {
private:
	friend class Text;
	list<Ope> URlist;			//存储撤销重做信息的双向链表
	list<Ope>::iterator cur;	//如果要撤销，使用当前指向的这个节点的信息
	Text* Text;					//指向文本对象的指针
public:
	UndoRedo();
	bool Undo();
	bool Redo();
	void AddUndo(int type, char ch);
	bool IsAddNode;
};