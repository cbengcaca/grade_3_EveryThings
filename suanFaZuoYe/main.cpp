#include <iostream>
#include <sstream>
#include <vector>
#define MAXNUM 10
#define MAXWEIGHT 15
using namespace std;

vector<string> *namesRecord = new vector<string>();
vector<int> *weightsRecord = new vector<int>();
vector<int> *valuesRecord = new vector<int>();
int existingNum = 0;
int totalUnion = 0;

bool isNum(string str)  {
	stringstream sin(str);
	double d;
	char c;
	if(!(sin >> d))
	{
		return false;
	}
	if
        (sin >> c)
	{
		return false;
	}
	return true;
}

void getInThings(string names[], int weights[], int values[]){
	for (int i=0 ; i<existingNum; i++){
		cout << "Input object" << i+1 << " name please: " ;
		cin  >> names[i];
		while(true){
			string weight;
			cout << "Input object" << i+1 << " weight please: ";
			cin >> weight;
			if (isNum(weight) ){
				weights[i] = stoi(weight,0,10);
				break;
			}
			else{
				cout << "Input weight is not a number input again" << endl;
				continue;
			}

		}
		while(true){
			string value;
			cout << "Input object" << i+1 << " value please: ";
			cin >> value;
			if (isNum(value) ){
				values[i] = stoi(value,0,10);
				break;
			}
			else{
				cout << "Input value is not a number input again" << endl;
				continue;
			}
		}
	}
}

void volient(string names[], int values[], int weights[], int level/*继续向下层搜索的层数*/, string nameHasBeenSelect /*已经选择的项目名*/,int valueHasBeenSelect /*已经选择的总价值*/, int weightHasBeenSelect/*已经选择的总重量*/
             ,int selectFirstFlag/*当前层的物品在数组中起始位置*/){
    string newNameHasBeenSelect = "";
    newNameHasBeenSelect += nameHasBeenSelect + " " + names[selectFirstFlag];
    int newValueHasBeenSelect = 0;
    newValueHasBeenSelect += valueHasBeenSelect + values[selectFirstFlag];
    int newWeightHasBeenSelect = 0;
    newWeightHasBeenSelect += weightHasBeenSelect + weights[selectFirstFlag];

    if (level == 0){ /*不向下层搜索*/
        totalUnion ++;
        namesRecord->push_back(newNameHasBeenSelect);
        valuesRecord->push_back(newValueHasBeenSelect);
        weightsRecord->push_back(newWeightHasBeenSelect);

    }else{ /*向下层进行搜索*/
        for(int i=selectFirstFlag+1; i< existingNum -(level-1); i++){
            volient(names, values, weights, level-1, newNameHasBeenSelect, newValueHasBeenSelect, newWeightHasBeenSelect, i);
        }
    }
}

int main() {
    while (true){
        string existingNumStr;
        cout << "Input the num of objects" <<"(<" << MAXNUM << "): ";
        cin >> existingNumStr;
        if (isNum(existingNumStr)){
            existingNum = stoi(existingNumStr,0,10);
            break;
        }
		else{
			cout << "Input num is not a number input again" << endl;
			continue;
        }
	}
	string names[existingNum];
    int weights[existingNum];
    int values[existingNum];
    getInThings(names, weights, values);

    string name = "";
    int value = 0;
    int weight = 0;

	for (int i=0; i<existingNum; i++){
        for (int j=0; j<existingNum - i; j++){
            volient(names, values, weights, i,name, value, weight, j);
        }
	}

	int wantWeight;
	cout << "Input the weight you want: " ;
	cin >> wantWeight;

	int maxValue;
	int maxIndex;
	for (int i=0; i< totalUnion; i++){
        if (weightsRecord->at(i) == wantWeight){
            if (valuesRecord->at(i) > maxValue){
                maxValue = valuesRecord->at(i);
                maxIndex = i;
            }
        }
	}
	cout << "MAX UNION is " << namesRecord->at(maxIndex) << endl;
	cout << "WEIGHT is " << wantWeight << endl;
	cout << "MAX VALUE is " << maxValue << endl;
	return 0;
}
