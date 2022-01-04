#include "UndoRedo.h"

UndoRedo::UndoRedo() :IsAddNode(true),Text(nullptr) {
	URlist.emplace_back(-1, ' ', 0, 0);
	cur = URlist.begin();
}

void UndoRedo::AddUndo(int type, char ch) {
	auto temp = cur;
	if (++temp != URlist.end())  //需要把后面的记录全部删除
		URlist.erase(temp, URlist.end());
	
	auto tmp = Text->Content.begin();
	int ln = 0;
	while (tmp != Text->point) {
		ln++;
		tmp++;
	}
	URlist.emplace_back(type, ch, ln, Text->ColP);
	cur++;
}

bool UndoRedo::Undo() {
	if (cur == URlist.begin()) return false; //无法再撤销了
	else {
		int line = cur->Line;
		Text->ColP = cur->Col; //先把光标移过来
		int record = -1;
		for (Text->point = Text->Content.begin(); Text->point != Text->Content.end() && line > 0; Text->point++, --line)
			if (Text->top == Text->point) record = line;
		if (record == -1 || record > 27) { //如果光标要跳转到的地方不在当前页面
			Text->RowP = 0;
			Text->top = Text->point;
		}
		else Text->RowP = record;

		IsAddNode = false;
		switch (cur->opeType) { //-1~无意义 0~插入 1~删除 2~回车
		case 0:
			Text->Delete();
			break;
		case 1:
			if (cur->ch == '\n') Text->Enter();
			else Text->Insert(cur->ch);
			break;
		case 2:
			Text->Delete();
			break;
		}
		IsAddNode = true;

		//直接覆盖位置信息
		int ln = 0;
		auto temp = Text->Content.begin();
		while (temp != Text->point) {
			ln++;
			temp++;
		}
		cur->Line = ln;
		cur->Col = Text->ColP;

		int type = cur->opeType;
		cur--; //迭代器前移
		if (type == cur->opeType && cur->ch != ' ') Undo(); //如果是相同的操作，就继续撤销
		return true;
	}
}

bool UndoRedo::Redo() {
	if ((++cur) == URlist.end()) {
		cur--;
		return false;
	}
	else {
		int line = cur->Line;
		Text->ColP = cur->Col; //先把光标移过来
		int record = -1;
		for (Text->point = Text->Content.begin(); Text->point != Text->Content.end() && line > 0; Text->point++, --line)
			if (Text->top == Text->point) record = line;
		if (record == -1 || record > 27) { //如果光标要跳转到的地方不在当前页面
			Text->RowP = 0;
			Text->top = Text->point;
		}
		else Text->RowP = record;

		IsAddNode = false;
		switch (cur->opeType) {
		case -1:break;
		case 0:
			Text->Insert(cur->ch);
			break;
		case 1:
			Text->Delete();
			break;
		case 2:
			Text->Enter();
			break;
		}
		IsAddNode = true;

		//直接覆盖位置信息
		int ln = 0;
		auto temp = Text->Content.begin();
		while (temp != Text->point) {
			ln++;
			temp++;
		}
		cur->Line = ln;
		cur->Col = Text->ColP;

		return true;
	}
}