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
	
	//为代码尾部人为增加一个换行符
	//用于利用尾部的超前搜索
	//强行将代码尾的并不会进入识别动作的非关键字字符串识别
	wholeCodes.append("\n");
}
void begin() {
	int cursor = 0;
	while (true)
	{
		if (cursor >= wholeCodes.size()) {
			break;
		}
		if (vectorStatusType[statusNow] == runningFlag) {//当前状态为运行状态，继续运行
			reading = wholeCodes.at(cursor);
			//分析串中为空，则此字符可以废弃
			if (reading == " " || reading == "\n") {
				if (analysis == "") {
					cursor++;
					continue;
				}
			}
			//否则分析串不为空，则利用此字符使当前状态强行跳转

			//根据当前输入与当前状态，确定下一跳转位置
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

			string nowWaiting;//当前从缓冲区内读入的字符
			nowWaiting.push_back(wholeCodes.at(cursor));
			int nextStatus = -1;
			for (size_t i = searchStartIndex; i <=searchEndIndex; i++)
			{
				//从初始化的符号转移表中读出符号并装进匹配串
				string pattern = "[";
				pattern	+= vectorReadChar[i]; 
				pattern += "]";
				regex r(pattern); 
				//匹配成功，说明找到状态转移
				if (regex_match(nowWaiting,r)) 
				{
					//匹配成功意味着3种情况
					//1.匹配到了关键字，并未进行超前搜索
					//	当前状态因阅读情况将转入关键字识别动作
					//	当前阅读情况将被读入分析串
					//2.匹配到了当前状态的一个other转移符号
					//	意味着当前阅读的结果不会加入到分析串中
					//	因为当前状态不会因为当前阅读而进行转移
					//	所以当前分析串会进行识别动作，当前阅读状态会被丢弃
					//	进行了超前搜索，指针将回退
					//3.匹配到了一个非终结状态的中间状态
					//	意味着下一状态仍然是识别状态，不会进行任何的识别动作
					//	当前阅读情况将读入分析串

					//找到转移状态
					nextStatus = vectorNext.at(i); 
					//修改当前状态
					statusNow = nextStatus;
					
					//认为当前搜索位置已经到了给定范围的最后一位
					//意味着是读入了当前状态的一个other
					//也就意味着进行了超前搜索并且进入了终结状态
					if (i == searchEndIndex){
						//指针回退，当前读入缓冲区清空
						cursor--;
						reading = "";
					}
					//没有进入终结状态
					else {
						analysis += reading;
					}	
					break;
				}
			}
			cursor++;
		}
		//当前并非是运行状态而是终结状态 ，则去判断是进入了哪一个终结状态
		else 
		{

			charge(statusNow);
		}
	}
}

void showAllWords() {
	cout << "DA语言词法分析器运行完毕：" << endl;
	for (int i = 0; i < wordComplete.size(); i++) {
		cout << i << " " << wordComplete[i] << "|" << wordType[i] << endl;
	}
}
int main()
{
	readInAllMessages();
	readInWholeCodes();
	begin();
	showAllWords();
}




