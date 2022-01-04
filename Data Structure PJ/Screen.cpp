#include "Screen.h"

Screen::Screen() {
	//��ʼ�����ڻ�����
	hOutBuff = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	//�����µĻ�����Ϊ���ʾ������
	SetConsoleActiveScreenBuffer(hOutBuff);

	COORD size = { 70, 30 };  //����һ������ṹ�� {����}
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), size);    // �������û�������С
	SetConsoleScreenBufferSize(hOutBuff, size);
	SMALL_RECT rc = { 0, 0, 70 - 1, 30 - 1 }; // ���ô���λ�úʹ�С
	SetConsoleWindowInfo(hOutBuff, 1, &rc);
}
void Screen::FlushScreen() { //ˢ����Ļ
	//ָ��Ŀ�껺������ָ�룬�û��������մӿ���̨��Ļ��������ȡ�����ݡ� 
	//��ָ�뱻��Ϊ��dwBufferSize����ָ����CHAR_INFO�ṹ�Ķ�ά�����ԭ�㡣
	CHAR_INFO chiBuffer[30 * 70]; //[30][70]

	SMALL_RECT srctReadRect; //����Դ�������ľ���
	srctReadRect.Top = 0;    //���ϣ� row 0, col 0 
	srctReadRect.Left = 0;
	srctReadRect.Bottom = 29; //���£� row 29, col 69 
	srctReadRect.Right = 69;

	COORD coordBufSize;
	COORD coordBufCoord;
	//�м仺�����Ĵ�С�� 30 rows x 70 columns
	coordBufSize.Y = 30;
	coordBufSize.X = 70;

	//�м仺���������Ͻ� row 0, col 0
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;

	ReadConsoleOutput(
		GetStdHandle(STD_OUTPUT_HANDLE),	//��Ĭ�ϻ�������ȡ
		chiBuffer,							//д�뵽�м仺����
		coordBufSize,						//��������С
		coordBufCoord,						//���������Ͻ�λ��
		&srctReadRect						//����������
	);

	SMALL_RECT srctWriteRect;  //����д�뻺�����ľ���
	srctWriteRect.Top = 0;     //���Ͻ� row 0, col 0 
	srctWriteRect.Left = 0;
	srctWriteRect.Bottom = 29; //���½� row 29, col 69 
	srctWriteRect.Right = 69;

	WriteConsoleOutput(
		hOutBuff,		  //��Ҫд����Ļ��ʾ������
		chiBuffer,        //���м仺������ȡ
		coordBufSize,     //�������Ĵ�С
		coordBufCoord,    //�����������Ͻ�λ��
		&srctWriteRect	  //Ŀ�ĵػ������ľ���
	);
}

void Screen::StdGoToXY(int x, int y) { //Ĭ�ϻ������Ĺ���Ƶ�(x,y)λ��
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Screen::AdjustShowedCursor() {
	COORD coord = { 0,0 };
	if (vim->Mode == 1 || vim->Mode == 0) { //����ģʽ�ͳ���ģʽ
		coord.X = vim->Text.ColP;
		coord.Y = vim->Text.RowP;
	}
	else if (vim->Mode == 2) { //����ģʽ
		coord.X = vim->CommandCursorCol;
		coord.Y = 29;
	}
	SetConsoleCursorPosition(hOutBuff, coord);
}

bool Screen::cls() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return false;
	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	if (!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten))
		return false;

	StdGoToXY(0, 0);
	return true;
}
