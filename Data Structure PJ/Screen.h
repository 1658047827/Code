#pragma once
#include "Vim.h"

class Screen {
private:
	friend class Vim;

	HANDLE hOutBuff;	//��ʾ������ ���
	Vim* vim;			//ָ��vim�����ָ��
public:
	Screen();
	void FlushScreen();
	void StdGoToXY(int x, int y);
	void AdjustShowedCursor();
	bool cls(); //���Ĭ�ϻ�����
};
