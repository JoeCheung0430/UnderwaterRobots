#include "control.h"
#include <conio.h>
#include <iostream>

using namespace std;

Control::Control()
{

}

Control::~Control()
{
	quit();
	exit();
}

void Control::run()
{
	int ch;
	while (1) {
		//cout << "Keyboard init success!" << endl;
		if (_kbhit()) {//如果有按键按下，则_kbhit()函数返回真
			//cout << "Key press!!!!!!!" << endl;
			ch = _getch();//使用_getch()函数获取按下的键值
			//cout << (char)ch << endl;
			/*printf("%c", ch);*/
			if (ch == 27) { break; }//当按下ESC时循环，ESC键的键值时27.
		}
	}
	system("pause");
}

void Control::close()
{
	quit();
}