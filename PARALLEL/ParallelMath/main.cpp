#include <math.h>
#include <iostream>
using namespace std;

float serial(int n) {
	return n * n;
}

float parallel(int n, int p) {
	return (float)((n * n) / p) + (float)(log(p) / log(2));
}

int main(int argc,char argv[]) {
	int n = 0;
	int p = 0;
	cout << "请输入初始程序规模n与并行程序数：" << endl;
	do {
		cout << "请按 整数 的格式输入(np不为0)：" << endl;
		cin >> n;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		cin >> p;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
		else {
			break;
		}
	} while (n < 1 || p < 1);

	float acc = 0;
	
	int select = 0;
	do {
		cout << "请按照 整数 的格式输入(0增加输入规模，1增加并行数量)：" << endl;
		cin >> select;
		cout << endl;
		if (cin.good() != 1) {
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			continue;
		}
	} while (select != 0 && select != 1);

	float timeS = 0;
	float timeP = 0;
	cout << endl << "行号\t规模\t并行数\t串行\t\t并行\t\t串行/并行\n" ;
	if (select == 0) {
		for (int i = 0; i < 101; i++) {
			timeS = serial(n);
			timeP = parallel(n,p);
			
			cout << i+1  << "\t";
			cout << n << "\t" << p << "\t";
			cout << timeS << "\t\t";
			cout << timeP << "\t\t";
			cout << timeP / timeS << "\t" << endl;
			n++;
		}
	}
	else {
		for (int i = 0; i < 101; i++) {
			timeS = serial(n);
			timeP = parallel(n, p);
			
			cout << i+1 << "\t";
			cout << n << "\t" << p << "\t";
			cout << timeS << "\t\t";
			cout << timeP << "\t\t";
			cout << timeP / timeS << "\t" << endl;
			p+=100;
		}
	}
	
	


}