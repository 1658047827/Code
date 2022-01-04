#include "UndoRedo.h"

UndoRedo::UndoRedo() :IsAddNode(true),Text(nullptr) {
	URlist.emplace_back(-1, ' ', 0, 0);
	cur = URlist.begin();
}

void UndoRedo::AddUndo(int type, char ch) {
	auto temp = cur;
	if (++temp != URlist.end())  //��Ҫ�Ѻ���ļ�¼ȫ��ɾ��
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
	if (cur == URlist.begin()) return false; //�޷��ٳ�����
	else {
		int line = cur->Line;
		Text->ColP = cur->Col; //�Ȱѹ���ƹ���
		int record = -1;
		for (Text->point = Text->Content.begin(); Text->point != Text->Content.end() && line > 0; Text->point++, --line)
			if (Text->top == Text->point) record = line;
		if (record == -1 || record > 27) { //������Ҫ��ת���ĵط����ڵ�ǰҳ��
			Text->RowP = 0;
			Text->top = Text->point;
		}
		else Text->RowP = record;

		IsAddNode = false;
		switch (cur->opeType) { //-1~������ 0~���� 1~ɾ�� 2~�س�
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

		//ֱ�Ӹ���λ����Ϣ
		int ln = 0;
		auto temp = Text->Content.begin();
		while (temp != Text->point) {
			ln++;
			temp++;
		}
		cur->Line = ln;
		cur->Col = Text->ColP;

		int type = cur->opeType;
		cur--; //������ǰ��
		if (type == cur->opeType && cur->ch != ' ') Undo(); //�������ͬ�Ĳ������ͼ�������
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
		Text->ColP = cur->Col; //�Ȱѹ���ƹ���
		int record = -1;
		for (Text->point = Text->Content.begin(); Text->point != Text->Content.end() && line > 0; Text->point++, --line)
			if (Text->top == Text->point) record = line;
		if (record == -1 || record > 27) { //������Ҫ��ת���ĵط����ڵ�ǰҳ��
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

		//ֱ�Ӹ���λ����Ϣ
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