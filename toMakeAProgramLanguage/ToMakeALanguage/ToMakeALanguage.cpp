// 将从文件读入3个参数x,y,z
// 依次代表：
// 状态为x时，进行识别关键字操作
// 状态为y时，进行识别字符串操作
// 状态为z时，进行识别数字操作
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <regex>
#include <map>
#include <string>
using namespace std;

int runningFlag = -1;//正在读取标识
int keyWordReadFlag = -1;//关键字识别标识
int stringReadFlag = -1;//字符串识别标识
int numsReadFlag = -1;//数字识别标识
int typeOfString = -1;//字符串类型码
int typeOfNums = -1;//数字类型码

vector<int> vectorBase;//标识本状态的开始搜索地址
vector<int> vectorNext;//本状态读取某字符跳转的下一个状态
vector<string> vectorReadChar;//跳转状态前读取的字符
vector<int> vectorCheck;//保留跳转状态来自于哪一个当前状态
vector<int> vectorStatusType;//状态是否跳转

vector<string> wordComplete;//已经识别好的单词
vector<int> wordType;//已经读好的单词的类别

string analysis = "";//分析完成等待装载的字符串
string reading = ""; //正在判别区当中的字符串
int statusNow = 0;//当前状态机状态

map<string, int> WORDTYPEREAD; //预定的字符类别及其编号

string wholeCodes;

vector<string> split(string str, string pattern) {
	vector<string> ret;
	if (pattern.empty()) return ret;
	size_t start = 0, index = str.find_first_of(pattern, 0);
	while (index != str.npos) {
		if (start != index)
			ret.push_back(str.substr(start, index - start));
		start = index + 1;
		index = str.find_first_of(pattern, start);
	}
	if (!str.substr(start).empty())
		ret.push_back(str.substr(start));
	return ret;
}

void readKeyWord() {
	int keyWordType = WORDTYPEREAD[analysis]; //
	wordType.push_back(keyWordType);
}

void readString() {
	wordType.push_back(typeOfString);
}

void readNum() {
	wordType.push_back(typeOfNums);
}

void charge(int actionType) {
	if (vectorStatusType[actionType] == keyWordReadFlag) {
		readKeyWord();
	}
	else if (vectorStatusType[actionType] == stringReadFlag) {
		readString();
	}
	else {
		readNum();
	}
	wordComplete.push_back(analysis);
	statusNow = 0;
	analysis = "";

}

void readInAllMessages() {
	ifstream readType("./types.txt");
	ifstream readStatus("./statusChange.txt");
	ifstream readStatusToFunction("./statusToFunction.txt");
	ifstream readWordType("./wordTypeOfStringAndNums.txt");
	//读取各种运行状态的状态
	string getStream;
	readStatusToFunction >> getStream;
	runningFlag = atoi(getStream.c_str());
	readStatusToFunction >> getStream;
	keyWordReadFlag = atoi(getStream.c_str());
	readStatusToFunction >> getStream;
	stringReadFlag = atoi(getStream.c_str());
	readStatusToFunction >> getStream;
	numsReadFlag = atoi(getStream.c_str());

	readWordType >> getStream;
	typeOfString = atoi(getStream.c_str());
	readWordType >> getStream;
	typeOfNums = atoi(getStream.c_str());

	do {
		string singleLine;
		getline(readType, singleLine);
		if (singleLine.empty()) {
			break;
		}
		vector<string> lineList;
		lineList = split(singleLine, "\t");
		WORDTYPEREAD[lineList[0]] = atoi(lineList[1].c_str());
	} while (true);

	do {
		if ((int)vectorReadChar.size() == 28) {
			cout << 28;
		}
		string singleLine;
		getline(readStatus, singleLine);
		if (singleLine.empty()) { //读取到空行，跳出循环
			break;
		}
		vector<string> lineList;
		lineList = split(singleLine, "\t");

		int statusType = atoi(lineList[0].c_str());
		int newNowStatus = atoi(lineList[1].c_str());

		if (statusType == runningFlag) { //新的状态状态具有下一个跳转状态
			string jumpResult = lineList[2];
			int nextStatus = atoi(lineList[3].c_str());

			if (vectorCheck.empty()) { //当前是初始状态，向量中没有条目
				vectorStatusType.push_back(statusType);
				vectorNext.push_back(nextStatus);
				vectorCheck.push_back(newNowStatus);
				vectorReadChar.push_back(jumpResult);
				vectorBase.push_back(0);
			}


			else { //向量里有条目
				//若当前状态与上一个状态不相同，是一个新的状态，base数组需变动
				//若当前状态与上一个状态相同,base数组不需变动，变动其余的
				int previousStatus = vectorCheck.back(); //获取上一个状态
				if (previousStatus != newNowStatus) {
					//当前状态与上一个状态不相同，说明是一个新状态，则新增加一个状态说明此状态的识别情况
					//
					int nextStatusStartBase = (int)vectorCheck.size();  //获得下一个状态的开始位置
					vectorBase.push_back(nextStatusStartBase);
					vectorStatusType.push_back(statusType);
				}
				vectorNext.push_back(nextStatus);
				vectorCheck.push_back(newNowStatus);
				vectorReadChar.push_back(jumpResult);
			}
		}
		else {//新的状态不具有下一个状态,所以再状态类别中新增一行说明本状态的识别情况
			vectorStatusType.push_back(statusType);
			vectorBase.push_back(-1);
		}
	} while (true);
	readType.close();
	readStatus.close();
	readStatusToFunction.close();
}
void readInWholeCodes() {
	/*读入了所有代码*/
	ifstream code("./code.txt");
	stringstream buffer;
	buffer << code.rdbuf();
	wholeCodes = buffer.str();
}
void begin() {
	int cursor = 0;
	while (true)
	{
		if (vectorStatusType[statusNow] == runningFlag) {//如果当前状态为运行状态，则进行读取操作
			reading = wholeCodes.at(cursor);
			if (wholeCodes.at(cursor) == ' '||wholeCodes.at(cursor) == '\n') {//空白跳过
				cursor++;
				continue;
			}
			
			//不是空白
			int searchStartIndex = vectorBase[statusNow]; //当前状态跳转搜索起始位置
			int searchEndIndex;
			if (statusNow + 1 == vectorBase.size()) {//当前状态为列表中最后一个状态，无法从下一个状态获取当前状态的搜索地址end值
				searchEndIndex = vectorNext.size() - 1;
			}
			else {
				int search = 1;
				while (statusNow+search < vectorBase.size() && vectorBase[statusNow + search] == -1) { //意味着下一状态为终结状态
					search++;
				}
				if (statusNow + search >= vectorBase.size()) {
					searchEndIndex = vectorNext.size() - 1;
				}
				else {
					searchEndIndex = vectorBase[statusNow + search] - 1;//end地址为下一状态的起始地址-1
				}
			}

			//根据当前输入与当前状态，确定跳转位置
			string nowWaiting;//用于待匹配的串
			nowWaiting.push_back(wholeCodes.at(cursor));
			int nextStatus = -1;
			for (size_t i = searchStartIndex; i <=searchEndIndex; i++)
			{
				string pattern = "[";
				pattern	+= vectorReadChar[i]; //匹配模式
				pattern += "]";
				regex r(pattern); //装载匹配工具
				if (regex_match(nowWaiting,r)) //如果匹配成功，说明下一个状态在当前行
				{
					nextStatus = vectorNext.at(i); //找到下一个状态
					statusNow = nextStatus;//将当前状态改为下一状态
					
					//注意，以下i==searchEndIndex读入other的情况是带超前搜索的
					if (i == searchEndIndex){
						cursor--;
						reading = "";
					}//以下情况不带超前搜索
					else {
						analysis += reading;
					}	
					break;
				}
			}
			cursor++;
		}
		else //当前并非是运行状态而是终结状态 ，则去判断是进入了哪一个终结状态
		{
			charge(statusNow);
		}
		if (cursor >= wholeCodes.size()) {//运行到代码段末尾，进行跳出，此时所有识别的串及串类别已经保存好
			break;
		}
	}
		

}

void showAllWords() {
	for (int i = 0; i < wordComplete.size(); i++) {
		cout << i << " " << wordComplete.at(i) << "|" << wordType.at(i);
	}
}
int main()
{
	readInAllMessages();
	readInWholeCodes();
	begin();
	
}




