#pragma once
#include "Vim.h"

class Screen {
private:
	friend class Vim;

	HANDLE hOutBuff;	//显示缓冲区 句柄
	Vim* vim;			//指向vim对象的指针
public:
	Screen();
	void FlushScreen();
	void StdGoToXY(int x, int y);
	void AdjustShowedCursor();
	bool cls(); //清空默认缓冲区
};
