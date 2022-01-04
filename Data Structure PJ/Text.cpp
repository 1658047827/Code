#include "Text.h"

Text::Text() :ColP(0), RowP(0), Content(1, ""){
	point = Content.begin();
	top = Content.begin();
	urdo = new UndoRedo();
	urdo->Text = this;
}

void Text::Enter() {
	int CurLineLen = point->size();
	string temp = point->substr(ColP);
	point->erase(point->begin() + ColP, point->end());
	point = Content.insert(++point, temp);

	if (RowP >= 27) top++;
	else RowP++;
	ColP = 0;
	//创建新的撤销节点
	if (urdo->IsAddNode) urdo->AddUndo(2, '\n');
}

void Text::Insert(char ch) { //不考虑回车的插入
	point->insert(ColP, 1, ch);
	ColP++;
	//创建新的撤销节点
	if (urdo->IsAddNode) urdo->AddUndo(0, ch);
}

char Text::Delete() {
	char del;
	if (point == Content.begin() && ColP == 0) return '\0';
	else if (point != Content.begin() && ColP == 0) { //如果在某一行的行首
		del = '\n';

		if (RowP == 0) top--;
		else if (RowP > 0) RowP--;
		auto temp = point;
		point--;

		ColP = point->size();
		(*point) += (*temp);
		Content.erase(temp);
	}
	else { //在某一行的中间
		del = (*point)[ColP - 1];

		point->erase(point->begin() + (ColP - 1));
		ColP--;
	}
	//创建新的撤销节点
	if (urdo->IsAddNode) urdo->AddUndo(1, del);
	return del;
}

void Text::Print() {
	//默认缓冲区光标事先复位到{0,0}
	int LineCount = 0;
	for (list<string>::iterator it = top; it != Content.end() && LineCount < 28; ++it, ++LineCount)
		cout << (*it) << '\n';
}

void Text::MoveCursor(int d) {
	//移动方向 0 left, 1 right, 2 up, 3 down
	switch (d) {
	case 0:
		if (ColP > 0) ColP--;
		break;
	case 1:
		if (ColP < point->size()) ColP++;
		break;
	case 2:
		if (point != Content.begin()) {
			if (RowP == 0) top--;
			else if (RowP > 0) RowP--;
			point--;
			if (point->size() >= ColP); //如果移过去的那一行长度够长，ColP不用改变
			else ColP = point->size();
		}
		break;
	case 3:
		if (++point != Content.end()) { //注意此处是++point
			if (RowP == 27) top++;
			else if (RowP < 27) RowP++;
			if (point->size() >= ColP);
			else ColP = point->size();
		}
		else point--;
		break;
	}
}

bool Text::OpenFile(string FileNameAndExt) {
	ifstream infile;
	string temp;
	infile.open(FileNameAndExt, ios::in); //不会自动创建
	if (!infile.good()) return false;

	Content.clear(); //清空原本的文本

	while (!infile.eof()) {
		getline(infile, temp);
		Content.push_back(temp);
	}
	infile.close(); //关闭文件读取

	//由于清空后，原本的迭代器会失效，所以需要重新赋值
	point = Content.begin();
	top = Content.begin();
	ColP = RowP = 0;

	urdo->URlist.clear();
	urdo->URlist.emplace_back(-1, ' ', 0, 0);
	urdo->cur = urdo->URlist.begin();

	return true;
}

bool Text::SaveFile(string FileNameAndExt) {
	ofstream outfile;
	outfile.open(FileNameAndExt); //不存在会自动自动创建，默认覆盖
	if (outfile.fail()) return false;
	for (list<string>::iterator it = Content.begin(); it != Content.end(); ++it) {
		outfile << (*it) << '\n';
	}
	outfile.close(); //关闭文件写入

	return true;
}

bool Text::Search(string pattern) { //从当前光标开始往后寻找
	int retColPo = 0;
	list<string>::iterator retLine, it = point;
	if ((retColPo = it->find(pattern, ColP)) != string::npos) {
		ColP = retColPo;
		top = it;
		RowP = 0;
		return true;
	}
	++it;
	for (; it != Content.end(); ++it)
		if ((retColPo = it->find(pattern)) != string::npos) {
			ColP = retColPo;
			top = it;
			RowP = 0;
			point = it;
			return true;
		}
	return false;
}
