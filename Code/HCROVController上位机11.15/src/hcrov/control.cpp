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
		if (_kbhit()) {//����а������£���_kbhit()����������
			//cout << "Key press!!!!!!!" << endl;
			ch = _getch();//ʹ��_getch()������ȡ���µļ�ֵ
			//cout << (char)ch << endl;
			/*printf("%c", ch);*/
			if (ch == 27) { break; }//������ESCʱѭ����ESC���ļ�ֵʱ27.
		}
	}
	system("pause");
}

void Control::close()
{
	quit();
}