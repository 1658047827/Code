#include "Screen.h"

Screen::Screen() {
	//初始化窗口缓冲区
	hOutBuff = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL
	);
	//设置新的缓冲区为活动显示缓冲区
	SetConsoleActiveScreenBuffer(hOutBuff);

	COORD size = { 70, 30 };  //定义一个坐标结构体 {宽，高}
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), size);    // 重新设置缓冲区大小
	SetConsoleScreenBufferSize(hOutBuff, size);
	SMALL_RECT rc = { 0, 0, 70 - 1, 30 - 1 }; // 重置窗口位置和大小
	SetConsoleWindowInfo(hOutBuff, 1, &rc);
}
void Screen::FlushScreen() { //刷新屏幕
	//指向目标缓冲区的指针，该缓冲区接收从控制台屏幕缓冲区读取的数据。 
	//此指针被视为由dwBufferSize参数指定的CHAR_INFO结构的二维数组的原点。
	CHAR_INFO chiBuffer[30 * 70]; //[30][70]

	SMALL_RECT srctReadRect; //设置源缓冲区的矩形
	srctReadRect.Top = 0;    //左上： row 0, col 0 
	srctReadRect.Left = 0;
	srctReadRect.Bottom = 29; //右下： row 29, col 69 
	srctReadRect.Right = 69;

	COORD coordBufSize;
	COORD coordBufCoord;
	//中间缓冲区的大小是 30 rows x 70 columns
	coordBufSize.Y = 30;
	coordBufSize.X = 70;

	//中间缓冲区的左上角 row 0, col 0
	coordBufCoord.X = 0;
	coordBufCoord.Y = 0;

	ReadConsoleOutput(
		GetStdHandle(STD_OUTPUT_HANDLE),	//从默认缓冲区读取
		chiBuffer,							//写入到中间缓冲区
		coordBufSize,						//缓冲区大小
		coordBufCoord,						//缓冲区左上角位置
		&srctReadRect						//缓冲区矩形
	);

	SMALL_RECT srctWriteRect;  //设置写入缓冲区的矩形
	srctWriteRect.Top = 0;     //左上角 row 0, col 0 
	srctWriteRect.Left = 0;
	srctWriteRect.Bottom = 29; //右下角 row 29, col 69 
	srctWriteRect.Right = 69;

	WriteConsoleOutput(
		hOutBuff,		  //将要写入屏幕显示缓冲区
		chiBuffer,        //从中间缓冲区读取
		coordBufSize,     //缓冲区的大小
		coordBufCoord,    //缓冲区的左上角位置
		&srctWriteRect	  //目的地缓冲区的矩形
	);
}

void Screen::StdGoToXY(int x, int y) { //默认缓冲区的光标移到(x,y)位置
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Screen::AdjustShowedCursor() {
	COORD coord = { 0,0 };
	if (vim->Mode == 1 || vim->Mode == 0) { //插入模式和常规模式
		coord.X = vim->Text.ColP;
		coord.Y = vim->Text.RowP;
	}
	else if (vim->Mode == 2) { //命令模式
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
