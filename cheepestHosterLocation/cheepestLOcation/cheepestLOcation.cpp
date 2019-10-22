// cheepestLOcation.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
#include <map>
using namespace std;

int numsOfNetPoints = 0;//网络节点数量
int numsOfNetRoads;//链路数量
int numsOfUserPoints;//消费点数量
int priceOfHoster;//服务器价格
int valueNow = 0;//当前价格
int level = 2;//搜索树深度
float percentage = 0.7;//接收概率

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

int main()
{
	ifstream readMe("./valueAndPrice.txt");
	if (!readMe.is_open()) {
		cout << "Read failure" << endl;
	}
	string readySingleLine;
	getline(readMe, readySingleLine);
	vector<string> listLine = split(readySingleLine, " ");
	numsOfNetPoints = atoi(listLine.at(0).c_str());
	numsOfNetRoads = atoi(listLine.at(1).c_str());
	numsOfUserPoints = atoi(listLine.at(2).c_str());

	getline(readMe, readySingleLine);/*读取一行空行*/
	getline(readMe, readySingleLine);
	priceOfHoster = atoi(readySingleLine.c_str());

	getline(readMe, readySingleLine);/*读取一行空行*/

	/*开始初始化部分数组*/
	vector<int> userPointRecord(numsOfUserPoints);//消费点记录
	vector<int> userPointNeeds(numsOfUserPoints);//消费点需求
	vector<vector<int>> matrixOfRoadSize(numsOfNetPoints);
	vector<vector<int>> matrixOfRoadPrice(numsOfNetPoints);
	for (size_t i = 0; i < numsOfNetPoints; i++)
	{
		vector<int> lineVector(numsOfNetPoints);
		matrixOfRoadSize[i] = lineVector;
	}
	for (size_t i = 0; i < numsOfNetPoints; i++)
	{
		vector<int> lineVector(numsOfNetPoints);
		matrixOfRoadPrice[i] = lineVector;
	}
	for (int i = 0; i < numsOfNetPoints; i++) {
		for (int j = 0; j < numsOfNetPoints; j++) {
			matrixOfRoadSize[i][j] = 0;
			matrixOfRoadPrice[i][j] = 0;
		}
	}
	for (int i = 0; i < numsOfUserPoints; i++) {
		userPointRecord[i] = 0;
		userPointNeeds[i] = 0;
	}

	/*开始读入价格容量信息*/
	do {
		getline(readMe, readySingleLine);
		if (readySingleLine.empty()) {
			break;
		}
		else {
			listLine = split(readySingleLine, " ");
			int x = atoi(listLine[0].c_str());
			int y = atoi(listLine[1].c_str());
			int roadSize = atoi(listLine[2].c_str());
			int roadValue = atoi(listLine[3].c_str());
			matrixOfRoadSize[x][y] = roadSize;
			matrixOfRoadSize[y][x] = roadSize;
			matrixOfRoadPrice[x][y] = roadValue;
			matrixOfRoadPrice[y][x] = roadValue;
		}
	} while (true);

	/*读入消费节点*/
	for (int i = 0; i < numsOfUserPoints; i++) {
		getline(readMe, readySingleLine);
		listLine = split(readySingleLine, " ");
		userPointRecord[i] = atoi(listLine[1].c_str());
		userPointNeeds[i] = atoi(listLine[2].c_str());
	}

	valueNow = numsOfUserPoints * priceOfHoster;

	while (true) {
		int newValueNow = 0;
		/*记录某一个候选点具体影响的是哪几个消费点，同时在各行的0位置记录交叉次数*/
		vector<vector<int>> treeRecord;
		/*记录某消费点遍历过那些节点*/
		vector<vector<int>> userRootRecord;
		//记录消费点与服务器点对应关系
		map<int, int> userLinkHoster;

		for (int i = 0; i < numsOfNetPoints; i++) {
			vector<int> newVector;
			/*0位置记录的是被覆盖的次数*/
			treeRecord.push_back(newVector);
		}
		for (int i = 0; i < numsOfNetPoints; i++) {
			vector<int> newVector;
			userRootRecord.push_back(newVector);
		}

		cout << "<<< 准备构建消费点生成树 ..." << endl;
		//构建生成树
		/*深度为level时，对每一消费点进行循环。但是树的信息不必保存，使用一次记录下信息即可删除*/
		for (int eachUserPointCursor = 0; eachUserPointCursor < numsOfUserPoints; eachUserPointCursor++) {
			/*userPointRecord[eachUserPoint] 是每一消费点树根*/
			/*userPointNeeds[eachUserPoint] 是顺序下消费点的需求带宽*/
			//当前网络节点的交叉次数+1
			vector<int> treeCreate;/*用于记录等待向前找点的节点*/
			treeCreate.push_back(userPointRecord[eachUserPointCursor]);/*等待向前找点的树根*/
			vector<vector<int>> tempFlag = matrixOfRoadSize;    /*本矩阵将用于生成树操作，遍历过的点将被置为0*/
			for (int i = 0; i < numsOfNetPoints; i++) {
				for (int j = 0; j < numsOfNetPoints; j++) {
					tempFlag[i][j] = matrixOfRoadPrice[i][j];
				}
			}

			/*消费点为x时，为此消费点的已搜索层数循环*/
			for (int nowLevel = 1; nowLevel < level; nowLevel++) {
				//已搜索层数为x时，等待向前找点的数组
				vector<int> thisTimeSelect;

				/*已搜索层数为x时，为待搜索节点进行循环*/
				for (int eachWaittingCursor = 0; eachWaittingCursor < (int)treeCreate.size(); eachWaittingCursor++) {

					/*正在搜索的节点为x时，为本行的列进行循环*/
					/*treeCreate->at(eachWatting) 是实际待找点的坐标*/
					for (int tempFlagColCursor = 0; tempFlagColCursor < numsOfNetPoints; tempFlagColCursor++) {
						/*当所经点有通路且未访问过时，压栈，并将点设置为已访问*/
						if (tempFlag[treeCreate[eachWaittingCursor]][tempFlagColCursor] > 0) {
							/*访问的下一个节点压栈*/
							thisTimeSelect.push_back(tempFlagColCursor);

							//候选服务器被某一用户节点选中了，此用户节点压栈
							treeRecord[tempFlagColCursor].push_back(userPointRecord[eachUserPointCursor]);
							//这一用户节点访问了某一服务器节点，访问的服务器节点压栈
							userRootRecord[userPointRecord[eachUserPointCursor]].push_back(tempFlagColCursor);
							/*将所有点通往此点的路径设置为已访问*/
							for (int row = 0; row < numsOfNetPoints; row++) {
								tempFlag[row][tempFlagColCursor] = 0;
							}
						}
					}
				}

				treeCreate.clear();
				/*为下一次树的遍历进行清理*/
				for (int col = 0; col < thisTimeSelect.size(); col++) {
					treeCreate.push_back(thisTimeSelect.at(col));
				}
				thisTimeSelect.clear();

			}
		}
		cout << "<<< 消费点树生成完毕 >>>" << endl << endl;
		cout << "<<< 准备按照贪心算法选出候选服务点 ..." << endl;

		//循环直到所有消费点“退出作用域”
		//初始化一个标识还有多少剩余的未退出作用域的点的变量
		int tempNumsOfUserPoints = numsOfUserPoints;
		//标记已经使用过的网络节点
		vector<int> flagOfUsedNetPoint;
		//标记已经退出的消费节点
		map<int, int> flagOfUsedUserPoint;
		for (size_t i = 0; i < numsOfNetPoints; i++)
		{
			flagOfUsedNetPoint.push_back(0);
		}
		//记录已经选中的服务器节点
		vector<int> waittingHosterPoints;
		vector<vector<int>> tempTreeRecord = treeRecord;
		while (tempNumsOfUserPoints > 0)
		{

			int maxIndex = -1;
			int maxValue = -1;

			//找到网络点中交叉最多的点作为服务点候选点,并且此点没有被使用过
			for (int netPointCursor = 0; netPointCursor < numsOfNetPoints; netPointCursor++) {
				if (flagOfUsedNetPoint[netPointCursor] != 0) {
					continue;
				}
				if ((int)tempTreeRecord[netPointCursor].size() > maxValue) {
					maxValue = tempTreeRecord[netPointCursor].size();
					maxIndex = netPointCursor;
				}
			}
			//找到一个交叉最多的点，加入候选点集合
			waittingHosterPoints.push_back(maxIndex);
			//修改相应点访问情况
			flagOfUsedNetPoint[maxIndex] = 1;

			//maxIndex是此时的服务点
			//找到所有此服务点涵盖的消费点
			//下方向量是所有相关消费点号码
			vector<int> tempRecord = tempTreeRecord[maxIndex];
			//将涵盖所有消费点退出作用域
			//将每一个消费点产生的树的影响删除
			//tempRecord是此点影响的所有消费点
			for (int i = 0; i < (int)tempRecord.size(); i++) {
				tempNumsOfUserPoints--;
				flagOfUsedNetPoint[tempRecord[i]] = 1;
				//tempRecord[i]是一个消费点的号码
				//userRootRecord[tempRecord[i]]是一个消费点影响的所有节点
				for (size_t j = 0; j < (int)userRootRecord[tempRecord[i]].size(); j++)
				{
					//userRootRecord[tempRecord[i]][j]是一个消费点影响的一个节点号码
					//将网络点记录矩阵的相应行的此号码的树根编号删除
					int userNow = tempRecord[i];
					//消费点操作的点
					int userOpPointNow = userRootRecord[tempRecord[i]][j];

					//记录消费点与服务点对应关系
					userLinkHoster[userNow] = maxIndex;

					vector<int>::iterator iter;
					for (iter = tempTreeRecord[userOpPointNow].begin(); iter != tempTreeRecord[userOpPointNow].end(); iter++) {
						if (*iter == userNow) {
							break;
						}
					}
					tempTreeRecord[userOpPointNow].erase(iter);
				}
			}
		}
		newValueNow = waittingHosterPoints.size() * priceOfHoster;
		cout << "<<< 贪心算法选择服务点完毕 >>>" << endl << endl;
		cout << "<<< 服务点与消费点对应关系如下：" << endl << endl;
		map<int, int>::iterator iter = userLinkHoster.begin();
		cout << "\t|消费点" << "\t" << "|服务点" << endl;
		for (iter; iter != userLinkHoster.end(); iter++)
		{
			cout << "\t|" << iter->first << "\t|" << iter->second << endl;
		}


		//迪杰斯特拉
		cout << endl << "<<< 即将开始构造所有消费点到其服务点的最短路径以及经过点 ..." << endl;
		//一份用于避免链路超载的备份记录数组
		vector<vector<int>> tempMatrixSize = matrixOfRoadSize;

		//waittingHosterPoints:vector<int>	是待选服务点数组
		//tempMatrixSize:vector<vector<int>>是链路容量数组
		//userLinkHoster:map<int,int>		是消费点与服务点对应表

		//每一消费点进行循环
		//记录每一消费点到其服务点距离

		////////>>>>>>>>>>>>未完待续
		map<int, int> eachUserToItsHosterValue;
		for (map<int, int>::iterator iter = userLinkHoster.begin(); iter != userLinkHoster.end(); iter++)
		{
			//iter->first是一消费点号
			//iter->second是一服务点号
			int nowUser = iter->first;
			int nowHoster = iter->second;

			//迪杰斯特拉算法
			int runningFlag = true;
			//记录迪杰斯特拉生成路径
			vector<int> previous(numsOfNetPoints);
			//记录已搜节点
			vector<int> searched(numsOfNetPoints);
			for (size_t i = 0; i < searched.size(); i++)
			{
				searched[i] = 0;
			}
			for (size_t i = 0; i < numsOfNetPoints; i++)
			{
				if (searched[i] == 0 && tempMatrixSize[nowUser][i] != 0) {
					previous[i] = nowUser;
				}
			}


			while (true)
			{
				//找到本行最小的
				int minValue = 65535;
				int minIndex = 65535;
				for (size_t i = 0; i < numsOfNetPoints; i++)
				{
					if (nowUser == i) {
						continue;
					}

					//找过的就不找了
					if (searched[i] == 0 && tempMatrixSize[nowUser][i] < minValue && tempMatrixSize[nowUser][i] != 0) {

						minValue = tempMatrixSize[nowUser][i];
						minIndex = i;
						searched[i] = 1;
						runningFlag = true;
					}
				}
				if (runningFlag == false) {
					break;
				}
				previous[minIndex] = nowUser;

				//minIndex是此时中转点
				for (size_t i = 0; i < numsOfNetPoints; i++) {
					int oldValue = tempMatrixSize[nowUser][i] == 0 ? 65535 : tempMatrixSize[nowUser][i];
					if (nowUser == i) {
						oldValue = 0;
					}
					int newValueOne = tempMatrixSize[nowUser][minIndex] == 0 ? 65535 : tempMatrixSize[nowUser][minIndex];
					int newValueTwo = tempMatrixSize[minIndex][i] == 0 ? 65535 : tempMatrixSize[minIndex][i];
					if (oldValue > newValueOne + newValueTwo) {
						//替换最小权值
						tempMatrixSize[nowUser][i] = newValueOne + newValueTwo;
						//替换previous数组
						previous[i] = minIndex;
					}
				}
				runningFlag = false;
			}

			cout << "<<< 关于消费点 " << nowUser << " 的寻路已经结束,倒叙路径如下：" << endl;
			cout << "( " << nowHoster << " -> " << previous[nowHoster] << " )" << endl;

			tempMatrixSize[previous[nowHoster]][nowHoster] -= userPointNeeds[nowUser];
			//记录下沿途路径
			int previousIndex = previous[nowHoster];
			while (previousIndex != nowUser)
			{
				cout << "( " << previous[previousIndex] << " -> " << previousIndex << " )" << endl;
				//回溯并且删除已经用掉的链路带宽
				tempMatrixSize[previous[previousIndex]][previousIndex] -= userPointNeeds[nowUser];
				previousIndex = previous[previousIndex];
			}
			int minRoad = tempMatrixSize[nowUser][nowHoster];
			cout << "<<< 找到消费点 " << nowUser << "到服务点 " << nowHoster << " 最小距离：" << minRoad;
			newValueNow += minRoad * userPointNeeds[nowUser];
		}
		//////>>>>>>>>>迪杰斯特拉结束
		return 0;
	}
}


