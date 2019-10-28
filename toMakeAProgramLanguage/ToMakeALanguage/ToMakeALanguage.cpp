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

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>词法分析器
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
//用于读取关键字
void readKeyWord() {
	int keyWordType = WORDTYPEREAD[analysis]; //
	wordType.push_back(keyWordType);
}
//用于读取字符串
void readString() {
	wordType.push_back(typeOfString);
}
//用于读取数字
void readNum() {
	wordType.push_back(typeOfNums);
}
//判断归约成哪一种元素
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
//读进所有词法分析状态
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
//读进所有输入代码
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
//展示词法分析器结果
void showAllWords() {
	for (int i = 0; i < wordComplete.size(); i++) {
		cout << i << "\t" << wordComplete[i] << "\t|" << wordType[i] << endl;
	}
	cout << endl;
}
//开始进行词法分析器
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

			//否则分析串不为空，则利用此字符使当前状态强行跳转

			//根据当前输入与当前状态，确定下一跳转位置
			int searchStartIndex = vectorBase[statusNow]; //当前状态跳转搜索起始位置
			int searchEndIndex;
			if (statusNow + 1 == vectorBase.size()) {//当前状态为列表中最后一个状态，无法从下一个状态获取当前状态的搜索地址end值
				searchEndIndex = vectorNext.size() - 1;
			}
			else {
				int search = 1;
				while (statusNow + search < vectorBase.size() && vectorBase[statusNow + search] == -1) { //意味着下一状态为终结状态
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
			//用于判断当前跳转是否找到
			bool otherFlag = true;
			//匹配到的跳转信息行
			int matchedIndex = -1;

			for (size_t i = searchStartIndex; i <= searchEndIndex; i++)
			{
				//从初始化的符号转移表中读出符号并装进匹配串
				string pattern = "";
				pattern += vectorReadChar[i];
				regex r(pattern);
				//匹配成功，说明找到状态转移
				if (regex_match(nowWaiting, r))
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

					//没有进行other跳转
					if (i != searchEndIndex) {
						otherFlag = false;
						matchedIndex = i;
					}
					//找到转移状态
					nextStatus = vectorNext.at(i);
					//修改当前状态
					statusNow = nextStatus;
					break;
				}
			}
			//匹配到了非other跳转
			if (otherFlag == false) {
				//将缓冲区加入分析串
				analysis += reading;
			}
			//匹配到other跳转
			else {
				//认为当前搜索位置已经到了给定范围的最后一位
				//意味着是读入了当前状态的一个other
				//也就意味着进行了超前搜索并且进入了终结状态
				//ERROR的情况只有在状态0且没有找到跳转状态的时候
				//说明读入的符并非本语法的允许符号
				//读取非法符号的状态是-1，进行跳跃处理
				if (statusNow == -1) {
					if (reading == " " || reading == "\n" || reading == "\t") {
						string previous = "";
						for (int i = cursor - 3; i < cursor; i++) {
							if (i >= 0)
							{
								if (wholeCodes[i] != ' ' && wholeCodes[i] != '\n' && wholeCodes[i] != '\t') {
									previous += wholeCodes[i];
								}
								else {
									previous += "_";
								}
							}
						}
						string next = "";
						for (int i = cursor + 1; i <= cursor + 3; i++) {
							if (i < wholeCodes.size())
							{
								if (wholeCodes[i] != ' ' && wholeCodes[i] != '\n' && wholeCodes[i] != '\t') {
									next += wholeCodes[i];
								}
								else {
									next += "_";
								}
							}
						}

						cout << "WARNNING:Jumping: << WHITE_SPACE||ENTER||TAB >>" << endl;
						cout << "Near the: << " << previous << "__" << next << " >>" << endl;
					}
					else {
						cout << "ERROR:Reading an unavaliable char: << " << reading << " >>" << endl;
					}

					//当前状态回炉
					statusNow = 0;
				}
				else {
					cursor--;
				}

				//指针回退，当前读入缓冲区清空

				reading = "";
			}
			cursor++;
		}
		//当前并非是运行状态而是终结状态 ，则去判断是进入了哪一个终结状态
		else
		{
			charge(statusNow);
		}
	}

	wordComplete.push_back("-1");
	wordType.push_back(-1);
}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>词法分析器


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>语法分析器
//语法分析器依赖变量

//一级单元记录产生式
//t:char 产生式左部
//tRight:string 产生式右部
//rangeEnd:int 二级列表的状态搜索最末位
struct syntaxTopUnit
{
	string t;
	vector<vector<string>> tRight;
	int rangeEnd = -1;
};
//二级单元记录文法状态
//itemStep:int 项目识别标记
//tComeNum:int 标记此项目来自哪一个产生式
//tCome:string 标记此项目来自于哪一个符号
//waiting:vector<string>标记待识别的字符
struct syntaxSecUnit
{
	int itemStep = -1;
	int tComeNum = -1;
	string tCome = "";
	string waiting = "";
	int tPNum = -1;
};

//项目集记录单元
//closure:vector<int> 当前项目集包含的项目序号
//go:map<string,int> 项目转移路由表
struct closureAndNext
{
	vector<int> closureBase;
	string closureBaseFlag;

	vector<int> closureTotal;
	map<string, int> go;
};

//语法树记录单元
struct treeUnit {
	string thisUnit;
	vector<treeUnit> subTrees;
};

//一级文法记录单元
vector<syntaxTopUnit> tSearchRange;
//二级文法状态搜索索引
vector<syntaxSecUnit> tStatus;
//项目集闭包与后继项目集
vector<closureAndNext> closure;
//搜索范围记录变量
int range = -1;

//终结符非终结符记号
int TFlag = -1;
int VFlag = -1;
map<string, int> VAndT;

//标记一个项目集位置,无限搜索或重复定义
map<string, int> IAndGo;

//FOLLOW集查询
map<string, vector<string>> followMap;

//FIRST集查询
map<string, vector<string>> firstMap;

//SLR1状态转移表
vector<vector<int>> SLRtable;

//字符随机搜索表
map<string, int> VSearch;

//产生式随机检索表
map<string, int> PList;

//标记需要手动解除递归的非终结符
map<string, int> handByV;

//标记需要从分析栈剔除的元素
map<string, int> handByMap;

//语法树根节点
treeUnit treeRoot;

//读取文法信息
void readInAllSyntaxInfo() {
	ifstream readHandBy("./hasRecrusionV.txt");

	string newLine;
	getline(readHandBy, newLine);
	vector<string> lineCut = split(newLine, "\t");
	for (int i = 0; i < lineCut.size(); i++)
	{
		handByV[lineCut[i]] = 1;
	}

	getline(readHandBy,newLine);
	lineCut = split(newLine,"\t");
	for (int i = 0; i < lineCut.size();i++) {
		handByMap[lineCut[i]] = 1;
	}

	ifstream readSyntax("./syntax.txt");
	int numOfV = -1;
	int numOfPNum = 0;
	do {


		string newLine;
		vector<string> lineCut;
		getline(readSyntax, newLine);
		if (newLine.empty()) {
			break;
		}
		lineCut = split(newLine, "\t");

		syntaxTopUnit newSynTopUnit;
		newSynTopUnit.t = lineCut[0];
		newSynTopUnit.rangeEnd = range;
		newSynTopUnit.rangeEnd++;
		range++;


		//当栈为空时，一级索引直接压栈
		if (numOfV == -1) {
			numOfV++;
			tSearchRange.push_back(newSynTopUnit);
			numOfPNum = 0;

		}
		else {
			//栈不为空，则判断是否是跟上一个状态t一样
			//与上一状态不一样
			if (numOfV != -1 && newSynTopUnit.t != tSearchRange.back().t) {
				numOfV++;
				tSearchRange.push_back(newSynTopUnit);
				numOfPNum = 0;

			}
			//与上一个状态t一样，则更新上一个状态的end值
			else {
				tSearchRange.back().rangeEnd = newSynTopUnit.rangeEnd;
				numOfPNum++;
			}
		}



		vector<string> tempTRight;
		for (int i = 2; i < lineCut.size() && lineCut[i] != ""; i++) {

			tempTRight.push_back(lineCut[i]);
			syntaxSecUnit newSynSecUnit;
			newSynSecUnit.tComeNum = numOfV;
			newSynSecUnit.tCome = lineCut[0];
			newSynSecUnit.itemStep = i - 2;
			newSynSecUnit.waiting = lineCut[i];
			newSynSecUnit.tPNum = numOfPNum;
			tStatus.push_back(newSynSecUnit);
			tSearchRange.back().rangeEnd++;
			range++;


		}
		//产生式之一加入左部一级索引中
		tSearchRange.back().tRight.push_back(tempTRight);

		//为每一个产生式加入一个终结状态
		syntaxSecUnit newSynSecUnit;
		newSynSecUnit.itemStep = tStatus[range - 1].itemStep + 1;
		newSynSecUnit.tCome = lineCut[0];
		newSynSecUnit.tComeNum = numOfV;
		newSynSecUnit.tPNum = numOfPNum;
		tStatus.push_back(newSynSecUnit);



	} while (true);

	//读入终结符非终结符标记映射关系
	newLine = "";
	lineCut;
	ifstream readVTFlag("TAndV.txt");
	for (int i = 0; i <= 1; i++) {

		getline(readVTFlag, newLine);
		lineCut = split(newLine, "\t");
		if (lineCut[0] == "T")
		{
			TFlag = atoi(lineCut[1].c_str());
		}
		else {
			VFlag = atoi(lineCut[1].c_str());
		}
	}

	do {
		getline(readVTFlag, newLine);
		if (newLine.empty()) {
			break;
		}
		lineCut = split(newLine, "\t");
		newLine = "";
		VAndT[lineCut[0]] = atoi(lineCut[1].c_str());
	} while (true);
}

//用于1、获取外部向量，构造新的项目集闭包
//2、对新生成的项目集求所有后继项目集
//3、递归实现
void recrusion(int numOfPreviousUnit) {

	closureAndNext newPak = closure[numOfPreviousUnit];

	//标识是否应当访问的数组
	vector<int> visitFlag;
	//标记产生式是否已经添加过
	map<string, int> waitMap;
	//为各状态求得项目集闭包
	//随着size不断增大，可以最终找到所有待识别状态
	for (size_t i = 0; i < newPak.closureTotal.size(); i++)
	{

		//当前状态号
		int statusNow = newPak.closureTotal[i];
		//当前处于未识别状态的非终结符
		string waiting = tStatus[statusNow].waiting;

		//如若当前项目的未识别项目
		//1、是终结符
		//2、为空，不具有后继状态，则不具有求闭包的条件
		//3、已经求过闭包，不用再行求闭包
		//跳过当前步骤
		if (VAndT[waiting] == TFlag || waiting.empty() || waitMap.find(waiting) != waitMap.end()) {
			continue;
		}
		else {
			waitMap[waiting] = 1;
		}

		//从头查找当前等待状态的状态来源 A->C.B,B->.a
		int searchCursor = 0;
		//寻找待加入状态号的首地址
		while (tSearchRange[searchCursor].t != waiting)
		{
			searchCursor++;
		}
		//未识别状态终结符状态的第一个状态加入向量
		int statusWaitingStarting = tSearchRange[searchCursor - 1].rangeEnd + 1;
		int statusWaitingEnding = tSearchRange[searchCursor].rangeEnd;
		for (size_t i = statusWaitingStarting; i <= statusWaitingEnding; i++)
		{
			//获得相同产生式左部的所有右部0号状态产生式
			if (tStatus[i].itemStep == 0)
			{
				newPak.closureTotal.push_back(i);
			}
		}

	}

	//标记一下哪些项目可求其后继项目
	for (size_t i = 0; i < newPak.closureTotal.size(); i++)
	{
		int statusNow = newPak.closureTotal[i];
		string waiting = tStatus[statusNow].waiting;
		if (waiting.empty() || tStatus[statusNow].itemStep == -1) {
			visitFlag.push_back(1);
		}
		else {
			visitFlag.push_back(0);
		}
	}

	int goOnFlag = true;
	//只要含有未求后继的点，就继续搜索
	//本块逻辑如下
	//1、每一次先找到第一个未求后继的点的waiting字符
	//2、求得本列表中所有跟当前waiting字符相同的项目
	//3、所有项目放入一个项目集单元中
	//4、所有项目集单元加上其waiting字符加入当前项目集单元GoMap中
	while (goOnFlag)
	{
		goOnFlag = false;

		//新的项目集
		vector<int> newItemPak;

		//继续搜索的下标
		int goOnSearchIndex;
		string thisTimeWait;
		for (size_t i = 0; i < newPak.closureTotal.size(); i++)
		{
			//当前一项目
			int itemNow = newPak.closureTotal[i];
			//当前项目waiting字符
			string waiting = tStatus[itemNow].waiting;
			//当前点不该访问，则跳过
			//1、当前已经使用过
			//2、当前项目为终结状态
			if (waiting == "" || visitFlag[i] == 1) {
				continue;
			}

			//找到一个可以访问的点，获取为本趟获取的waitingGO项目集
			else {
				int nextItem = itemNow + 1;
				newItemPak.push_back(nextItem);
				thisTimeWait = waiting;
				goOnFlag = true;
				visitFlag[i] = 1;
				goOnSearchIndex = i + 1;
				break;
			}
		}

		//遍历当前项目集中所有状态
		for (goOnSearchIndex; goOnFlag == true && goOnSearchIndex < newPak.closureTotal.size(); goOnSearchIndex++)
		{
			//当前状态号
			int statusNow = newPak.closureTotal[goOnSearchIndex];
			//如果当前状态waiting字符与本趟目标匹配
			//添加进本趟数组中
			if (tStatus[statusNow].waiting == thisTimeWait) {
				visitFlag[goOnSearchIndex] = 1;
				newItemPak.push_back(statusNow + 1);
			}
		}

		//判断本趟搜索是否找到了还未求后继的项目
		if (goOnFlag) {
			//新的项目集闭包单元
			closureAndNext newClosureAndNext;
			newClosureAndNext.closureBase = newItemPak;
			newClosureAndNext.closureTotal = newItemPak;
			newClosureAndNext.closureBaseFlag = "";
			//添加一个标记项目集的字符串
			for (size_t i = 0; i < newClosureAndNext.closureBase.size(); i++)
			{
				newClosureAndNext.closureBaseFlag += to_string(newClosureAndNext.closureBase[i]);
			}

			//判断即将生成的这一项目集是否已经存在
			//此处判断为不存在
			if (IAndGo.find(newClosureAndNext.closureBaseFlag) == IAndGo.end()) {

				//新的项目集闭包单元压入数组
				closure.push_back(newClosureAndNext);
				//新的项目号
				int newItemsNum = closure.size() - 1;
				//新的项目集闭包单元路由信息交给本层项目集闭包单元
				newPak.go[thisTimeWait] = newItemsNum;

				//替换
				closure[numOfPreviousUnit] = newPak;

				//添加一个标记此项目集的路由
				IAndGo[newClosureAndNext.closureBaseFlag] = newItemsNum;

				//交给下一层去生成其自己的项目集闭包
				recrusion(newItemsNum);


			}
			//如果根据基准数据判断的转移状态已经存在
			else {
				newPak.go[thisTimeWait] = IAndGo[newClosureAndNext.closureBaseFlag];

				//替换
				closure[numOfPreviousUnit] = newPak;
			}

		}

	}

	//求得所有后继项目集闭包

}

//构建项目集闭包以及后继项目集
//本算法的构建是
//1、  上层建立一个新的项目集闭包单元，一个新的项目集，并挂入上上层->上层的项目集闭包单元的GOMap中
//1.1、如若是第一层则不会有来自第0层的项目集单元
//2、  本层对项目集求闭包，将项目集闭包并入项目集闭包单元
//2.2、求所有的后继项目，各自建立新的项目集，将读取相同字符的后继项目并入一个项目集
//2.3、将所有的读取字符与后继项目集映射关系填入GOMap中
//3、  调用下一层递归
void createClosureGo() {
	vector<int> startStatus;
	startStatus.push_back(0);
	closureAndNext newPak;
	newPak.closureBase = startStatus;
	newPak.closureTotal = newPak.closureBase;
	newPak.closureBaseFlag += "0";
	closure.push_back(newPak);
	int previousNum = closure.size() - 1;
	recrusion(previousNum);
}

//构建first集
void createFirstAndFollow() {
	ifstream readFirst("./first.txt");
	ifstream readFollow("./follow.txt");
	do
	{
		string newLine;
		vector<string> lineCut;
		getline(readFirst, newLine);
		string tLeft;
		vector<string> vectorFirst;

		if (newLine.empty()) {
			break;
		}
		else {
			lineCut = split(newLine,"\t");
			tLeft = lineCut[0];
		}
		
		for (int i = 2; i < lineCut.size(); i++)
		{
			vectorFirst.push_back(lineCut[i]);
		}
		firstMap[tLeft] = vectorFirst;
	} while (true);
	
	do
	{
		string newLine;
		vector<string> lineCut;
		getline(readFollow, newLine);
		string tLeft;
		vector<string> vectorFollow;

		if (newLine.empty()) {
			break;
		}
		else {
			lineCut = split(newLine, "\t");
			tLeft = lineCut[0];
		}

		for (int i = 2; i < lineCut.size(); i++)
		{
			vectorFollow.push_back(lineCut[i]);
		}
		followMap[tLeft] = vectorFollow;
	} while (true);

	readFirst.close();
	readFollow.close();
}

//构建状态转移表
void SLRactionTable() {
	for (int i = 1; i < tSearchRange.size(); i++) {
		VSearch[tSearchRange[i].t] = i - 1;
	}
	int digitRange = WORDTYPEREAD.size();

	for (int i = 0; i < closure.size(); i++)
	{
		vector<int> newVector;
		SLRtable.push_back(newVector);
		for (int j = 0; j < WORDTYPEREAD.size(); j++)
		{
			SLRtable[i].push_back(-1);
		}
		for (int j = 1; j < tSearchRange.size(); j++)
		{
			SLRtable[i].push_back(-1);
		}
	}

	for (int i = 0; i < closure.size(); i++)
	{
		closureAndNext closureUnit = closure[i];
		for (map<string, int>::iterator iter = closureUnit.go.begin(); iter != closureUnit.go.end(); iter++)
		{
			string first = iter->first;
			int second = iter->second;
			if (VAndT[first] == TFlag) {
				SLRtable[i][atoi(first.c_str())] = second;
			}
			else {
				SLRtable[i][digitRange + VSearch[first]] = second;
			}
		}
	}

	string totalPString = "";
	//此PLIST用于方便的根据产生式来获取其左部量的值下标
	for (int i = tSearchRange.size() - 1; i > -1; i--)
	{

		for (int j = 0; j < tSearchRange[i].tRight.size(); j++)
		{
			totalPString = "";
			for (int k = 0; k < tSearchRange[i].tRight[j].size(); k++)
			{
				totalPString += tSearchRange[i].tRight[j][k];
			}
			PList[totalPString] = i;
		}

	}


}

//打印SLR状态转移表
void showSLRActionTable() {
	cout << "\t";
	for (map<string, int>::iterator iter = WORDTYPEREAD.begin(); iter != WORDTYPEREAD.end(); iter++)
	{
		cout << "(" << iter->first << "," << iter->second << ")" << " ";
	}
	for (int i = 1; i < tSearchRange.size(); i++)
	{
		cout << "(" << tSearchRange[i].t << ")" << " ";
	}
	cout << endl;
	for (int i = 0; i < SLRtable.size(); i++)
	{
		cout << i << " ";
		for (int j = 0; j < SLRtable[i].size(); j++)
		{
			cout << SLRtable[i][j] << " ";
		}
		cout << endl;
	}
}

//开始进行语法分析
void beginSynTax() {
	//归约策略如下：
	//1.if s:then make a new node in this vector
	//2.if r:then make a new node to accept it to their father
	vector<treeUnit> parallelsNodes;
	
	int statusNow = 0;
	bool isAviliable = false;
	//分析状态栈
	vector<int> statusStack;
	//分析字符栈
	vector<string> identifyStrings;
	
	
	
	statusStack.push_back(statusNow);

	//对栈顶的非终结符进行判断
	bool vChargeFlag = false;
	bool removeRecruSwitch = false;
	for (int i = 0; i < wordComplete.size(); i)
	{
		//获取栈顶状态号
		statusNow = statusStack.back();
		
		//获取栈顶字符
		string cursorString = wordComplete[i];
		int cursorType = wordType[i];

		if (cursorType == -1 && statusStack.back() == 1) {
			isAviliable = true;
			break;
		}
		
		
		closureAndNext closureUnit = closure[statusNow];

		//下方要处理的情况有以下3种
		//1、当前栈顶状态为待归约状态
		//2、当前栈顶状态为不归约状态
		//3、当前栈顶为等待读入状态
		//判断当前闭包是否仅含一个终结项目
		//如果仅包含一个终结项目，查看之后的一位符号是否属于其左部产生式的follow集
		//判断一下当前状态unit中是否含有一个终结状态
		bool terminalFlag = false;

		//标记一个终结状态的状态号
		int tIndexFlag;

		//本次有可能需手动归约的左部符号
		//获取当前状态式的左部信息
		string handLeftT;
		for (int i = 0; i < closureUnit.closureTotal.size();i++) {
			if (handByV.find(tStatus[closureUnit.closureTotal[i]].tCome) != handByV.end()) {
				handLeftT = tStatus[closureUnit.closureTotal[i]].tCome;
				break;
			}
		}
		bool handLeftFlag = handLeftT == ""? false:true;
		

		//当归约开关打开，则归约直到栈内仅存一个C与一个T时
		while (removeRecruSwitch && !vChargeFlag)
		{
			string top = identifyStrings.back();
			string top3;
			top3 = identifyStrings.size() >= 3?identifyStrings[identifyStrings.size() - 3] :"";
			//查看末尾3号元素是否是某递归式的启动状态
			if (handByMap.find(top3) != handByMap.end()) {
				treeUnit newUnit;
				newUnit.thisUnit = top;
				vector<treeUnit> vectorTree;
				vectorTree.push_back(parallelsNodes[parallelsNodes.size() - 2]);
				vectorTree.push_back(parallelsNodes[parallelsNodes.size() - 1]);
				newUnit.subTrees = vectorTree;
				parallelsNodes.pop_back();
				parallelsNodes.pop_back();
				parallelsNodes.push_back(newUnit);

				vChargeFlag = true;
				statusStack.pop_back();
				statusStack.pop_back();
				identifyStrings.pop_back();
				identifyStrings.pop_back();
				identifyStrings.push_back(top);
			}
			else {
				removeRecruSwitch = false;
			}
		}

		//判断这个单元是否含有一个终结状态
		for (int i = 0; i < closureUnit.closureTotal.size(); i++) {
			if (tStatus[closureUnit.closureTotal[i]].waiting.empty()) {
				terminalFlag = true;
				tIndexFlag = closureUnit.closureTotal[i];
				break;
			}
		}

		//当前含有一个终结状态
		//去判断下一个读入的字符是否属于Follow集
		if (terminalFlag && vChargeFlag == false) {

			int itemNow = tIndexFlag;
			int tComnNum = tStatus[itemNow].tComeNum;
			int tPNum = tStatus[itemNow].tPNum;
			vector<string> tTotalP = tSearchRange[tComnNum].tRight[tPNum];
			string totalPString = "";

			//判断下一字符是否属于其follow
			string nextCursor = i >= wordComplete.size() ? "" : cursorString;
			int nextCursorType = wordType[i];
			bool findFlag = false;
			vector<string> tempFollow = followMap[tStatus[itemNow].tCome];
			for (int k = 0; k < tempFollow.size(); k++)
			{
				if (to_string(nextCursorType) == tempFollow[k]) {
					findFlag = true;
					break;
				}
			}

			//进行终结、归约状态
			//进行归约的两个条件
			//1、到达了数据尾
			//2、属于其follow集
			if (nextCursor.empty() || findFlag == true) {

				vChargeFlag = true;
				for (int i = 0; i < tTotalP.size(); i++)
				{
					totalPString += tTotalP[i];
				}
				int numOfP = PList[totalPString];
				int sizeOfRight = totalPString.size();
				string left = tSearchRange[numOfP].t;

				//将分析字符栈弹栈 压入新的非终结符
				//根据字符数量弹出对应的状态数量
				treeUnit newUnit;
				vector<treeUnit> vectorTree;
				for (int l = 0; l < tTotalP.size(); l++)
				{
					vectorTree.push_back(parallelsNodes.back());
					parallelsNodes.pop_back();

					identifyStrings.pop_back();
					statusStack.pop_back();
				}
				identifyStrings.push_back(left);

				//反转vectorTree的顺序
				for (int z = 0; z < vectorTree.size()/2;z++) {
					treeUnit temp;
					temp = vectorTree[z];
					vectorTree[z] = vectorTree[(vectorTree.size() -1) - z];
					vectorTree[(vectorTree.size() -1)- z] = temp;
				}
				newUnit.thisUnit = left;
				newUnit.subTrees = vectorTree;
				parallelsNodes.push_back(newUnit);
			}

			//进行移进
			else {
				//得到下一状态
				int newStatus = closureUnit.go[cursorString];
				//新字符新状态压栈
				statusStack.push_back(newStatus);
				identifyStrings.push_back(to_string(cursorType));
			}
		}

		//否则不包含终结项目
		//1、判断当前栈顶是终结符，说明是正常读入，则移动指针进行继续跳转
		//2、当前栈顶是非终结符，说明是超前归约了一步，则先根据当前栈顶进行跳转，再进行移动游标
		else {
			//判断当前栈顶存在一个待处理的读入非终结符操作
			//则做完本次的跳转动作后再行读入下一个字符
			if (vChargeFlag) {
				//获取那个归约出的非终结符
				string charTop = identifyStrings.back();
				closureAndNext  closureNow = closure[statusStack.back()];

				//判断跳转状态是否存在
				int nextStatus;
				if (closureNow.go.find(charTop) != closureNow.go.end()) {
					nextStatus = closureNow.go[charTop];
					statusStack.push_back(nextStatus);
					vChargeFlag = false;
				}

				//跳转状态不存在
				else {
					identifyStrings.pop_back();
					cout << "<<< " << charTop << " at status " << statusStack.back() << " has not route message! >>>" << endl;
					vChargeFlag = false;
				}
			}

			//并没有一个超前归约
			//意味着当前正在读入新的字符，需要判断是否有路由信息
			else {
				//判断当前状态读入一个符号是否有路由信息

				closureAndNext  closureNow = closure[statusStack.back()];
				//判断跳转状态是否存在
				//为了手动解除递归状态
				int nextStatus;
				if (closureNow.go.find(to_string(cursorType)) == closureNow.go.end()) {
					//手动弹出递归状态 
					//并让分析器准备接受一个非终结符
					if (handLeftFlag) {
						treeUnit newUnit;
						newUnit.thisUnit = handLeftT;
						newUnit.subTrees.push_back(parallelsNodes.back());
						parallelsNodes.pop_back();
						parallelsNodes.push_back(newUnit);

						identifyStrings.pop_back();
						statusStack.pop_back();
						identifyStrings.push_back(handLeftT);
						vChargeFlag = true;
						removeRecruSwitch = true;
					}
					else {
						cout << "<<< WARNING：非法字符 " << cursorType << " >>>" << endl;
						i++;
					}
				}

				//存在路由信息，则进行移进
				else {
					//Create a tree node first
					treeUnit newUnit;
					newUnit.thisUnit = to_string(cursorType);
					parallelsNodes.push_back(newUnit);

					closureAndNext  closureNow = closure[statusStack.back()];
					int nextStatus = closureNow.go[to_string(cursorType)];
					statusStack.push_back(nextStatus);
					identifyStrings.push_back(to_string(cursorType));
					i++;
				}

			}
		}
		
	}
	treeRoot.subTrees = parallelsNodes;

	cout << "<<< 语法树生成结束: >>>" << endl<< endl;
	//syntaxDone
	if (isAviliable) {
		cout << "<<< 为符合规格语法 >>>" << endl;
	}
	else {
		cout << "<<< 为不符合规格语法 >>>" << endl;
	}
}

void printTree(int level ,treeUnit treeBegin) {
	for (int i = 0; i < treeBegin.subTrees.size(); i++)
	{
		cout << "<<< LEVEL: "<<level << "| "<< treeBegin.thisUnit << " -> " << treeBegin.subTrees[i].thisUnit << " >>>"<< endl;;
		
	}
	for (int i = 0; i < treeBegin.subTrees.size(); i++)
	{
		printTree(level +1,treeBegin.subTrees[i]);
	}
}

//打印语法树
void showSynTaxTree() {
	treeUnit treeBegin = treeRoot.subTrees[0];
	cout << "<<< LEVEL:-1| R' -> R >>>" << endl;
	for (int i = 0; i < treeBegin.subTrees.size(); i++)
	{
		cout << "<<< LEVEL: 0| " << treeBegin.thisUnit << " -> " << treeBegin.subTrees[i].thisUnit << " >>>" << endl;
		
	}
	for (int i = 0; i < treeBegin.subTrees.size(); i++)
	{
		printTree(1, treeBegin.subTrees[i]);
	}
}

int main()
{
	cout << "<<< DA语言词法分析器开始运行: >>>" << endl << endl;
	readInAllMessages();
	readInWholeCodes();
	begin();
	showAllWords();
	cout << "<<< DA语言词法分析器运行完毕: >>>" << endl<<endl;


	cout << "<<< DA语言语法分分析器开始运行: >>>" << endl <<endl;
	readInAllSyntaxInfo();
	createClosureGo();

	createFirstAndFollow();
	SLRactionTable();
	//	showSLRActionTable();
	beginSynTax();
	cout << endl << "<<< 语法树如下: >>>" << endl << endl;
	showSynTaxTree();
	cout << endl <<"<<< DA语言语法分析器运行完毕: >>>" << endl;

}
