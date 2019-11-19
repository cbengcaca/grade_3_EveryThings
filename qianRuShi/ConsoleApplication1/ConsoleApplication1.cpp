// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h> 
using namespace std;
int main()
{
	char* temp[20];
	for (int i = 0; i < 20; i++)
	{
		temp[i] = 0;
	}
	FILE* fpCom;
	if ((fpCom = fopen("com4", "r+")) == NULL) {
		cout << "can t open";
	}
	fprintf(fpCom,"AT\r\n");
	fread(temp,1,20,fpCom);
	
	
	for (int i = 0; i < 20; i++)
	{
		cout << temp+i;
	}
}


