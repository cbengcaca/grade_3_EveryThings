#include "head.h"
//小明种苹果
void program1() {
	int treeNum = 0;
	int opNum = 0;
	cin >> treeNum >>opNum;
	long** matrix = new long* [treeNum];
	for (int i = 0; i < treeNum; i++)
	{
		matrix[i] = new long[opNum + 1];
	}

	int totalSum = 0;
	int cutNum = 0;
	int maxCutNum = -1;
	int maxIndex = -1;
	int P = 0;
	for (int i = 0; i < treeNum; i++)
	{
		cin >> matrix[i][0];
		int totalNum = matrix[i][0];
		int thisCutNum = 0;
		for (int j = 0; j < opNum; j++)
		{
			cin >> matrix[i][j + 1];
			totalNum += matrix[i][j + 1];
			thisCutNum -= matrix[i][j + 1];
		}
		totalSum += totalNum;
		if (thisCutNum > maxCutNum) {
			maxCutNum = thisCutNum;
			maxIndex = i;
		}
		cutNum = thisCutNum;
	}
	cout << totalSum << " " << maxIndex+1 << " " << maxCutNum;
	delete []matrix;
}

//小明种苹果续
void program2() {
	int treeNum = 0;
	cin >> treeNum;
	
	//最终留下的果数
	int leftTotalNum = 0;
	//落果数
	int dropNum = 0;
	//3树落果组数
	int groupNum = 0;
	//根据最大果树数简历的标记落果情况的数组
	int dropArray[1000] = {0};

	//接收输入
	for (int i = 0; i < treeNum; i++)
	{
		dropArray[i] = 0;
		//本行参数数
		int num = 0;
		cin >> num;

		//参数1：苹果起始个数
		int thisBegin = 0;
		cin >> thisBegin;

		//标记苹果先前的剩余量
		int previous = thisBegin;
		for (int j = 1; j < num; j++)
		{
			//新的输入量
			int newNum = 0;
			cin >> newNum;

			//大于零表示计数
			if (newNum>0) {
				//小于先前果数表示落果
				if (newNum < previous) {
					//防止树重复落果被重复计算
					dropArray[i] = 1;
					previous = newNum;
				}
			}
			//小于零表示梳果
			else {
				previous += newNum;
			}
		}
		//一行输入处理完毕将剩余果量归并
		leftTotalNum += previous;
	}

	//利用标记连续落果棵数计算组数
	int flag = 0;
	//果树指针
	int cursor = 0;
	//treeNum+2表示果树是环状，有边界条件
	for (int i = 0; i < treeNum+2; i++)
	{
		if (dropArray[cursor] == 1) {
			flag++;
			//在此处进行落果情况计算
			if (i<treeNum) {
				dropNum++;
			}
		}
		//当未有落果，表示需重新计算连续落果
		else {
			flag = 0;
		}
		//3棵树落果，符合条件
		if (flag >=3) {
			groupNum++;
		}
		cursor++;
		//边界条件的处理，需要指针从尾部连接至首部
		cursor %= treeNum;
	}
	cout << leftTotalNum << " " << dropNum << " " << groupNum;

}

void program3() {

}

void program4() {

}

void program5() {

}

int main() {

	program2();
	return 0;
}