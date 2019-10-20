#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector>
using namespace std;
int numsOfNetPoints;//网络节点数量
int numsOfNetRoads;//链路数量
int numsOfUserPoints;//消费点数量
int priceOfHoster;//服务器价格
int valueNow = 0;//当前价格
int level = 2;//搜索树深度
float percentage = 0.7;//接收概率

vector<string> split(string str, string pattern){
    vector<string> ret;
    if (pattern.empty()) return ret;
    size_t start = 0, index = str.find_first_of(pattern, 0);
    while (index != str.npos)	{
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
    if(!readMe.is_open()){
        cout <<"Read failure" << endl;
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
    int userPointRecord[numsOfUserPoints];//消费点记录
    int userPointNeeds[numsOfUserPoints];//消费点需求
    int matrixOfRoadSize[numsOfNetPoints][numsOfNetPoints];
    int matrixOfRoadPrice[numsOfNetPoints][numsOfNetPoints];
    for (int i=0; i<numsOfNetPoints; i++){
        for (int j=0; j<numsOfNetPoints; j++){
            matrixOfRoadSize[i][j] = 0;
            matrixOfRoadPrice[i][j] = 0;
        }
    }

    for (int i=0; i<numsOfUserPoints; i++){
        userPointRecord[i] = 0;
        userPointNeeds[i] = 0;
    }

    /*开始读入价格容量信息*/

    do{
        getline(readMe, readySingleLine);
        if (readySingleLine.empty()){
            break;
        }else{
            listLine = split(readySingleLine, " ");
            int x = atoi(listLine.at(0).c_str());
            int y = atoi(listLine.at(1).c_str());
            int roadSize = atoi(listLine.at(2).c_str());
            int roadValue = atoi(listLine.at(3).c_str());
            matrixOfRoadSize[x][y] = roadSize;
            matrixOfRoadPrice[x][y] = roadValue;
        }
    }while(true);

    /*读入消费节点*/
    for (int i=0; i<numsOfUserPoints; i++){
        getline(readMe, readySingleLine);
        listLine = split(readySingleLine," ");
        userPointRecord[i] = atoi(listLine.at(1).c_str());
        userPointNeeds[i] = atoi(listLine.at(2).c_str());
    }

    valueNow = numsOfUserPoints * priceOfHoster;

    while(true){
        vector<vector<int>> treeRecord;/*记录某一个候选点具体影响的是哪几个消费点，同时在各行的0位置记录交叉次数*/
        vector<vector<int>> userRootRecord;/*记录某消费点遍历过那些节点*/
        for(int i=0; i<numsOfNetPoints;i++){
            vector<int> newVector;
            newVector.push_back(0);/*0位置记录的是被覆盖的次数*/
            treeRecord.push_back(newVector);
        }
        for(int i=0; i<numsOfUserPoints;i++){
            vector<int> newVector;
            userRootRecord.push_back(newVector);
        }



        /*深度为level时，对每一消费点进行循环。但是树的信息不必保存，使用一次记录下信息即可删除*/
        for (int eachUserPoint=0; eachUserPoint<numsOfUserPoints; eachUserPoint++){
            /*userPointRecord[eachUserPoint] 是每一消费点树根*/
            /*userPointNeeds[eachUserPoint] 是顺序下消费点的需求带宽*/
            treeRecord.at(userPointRecord[eachUserPoint]).at(0) += 1; /*每一网络节点的交叉次数+1*/
            vector<int> treeCreate;/*用于记录等待向前找点的节点*/
            treeCreate.push_back(userPointRecord[eachUserPoint]);/*等待向前找点的树根*/
            int tempFlag[numsOfNetPoints][numsOfNetPoints];    /*本矩阵将用于生成树操作，遍历过的点将被置为0*/
            for (int i=0; i<numsOfNetPoints; i++){
                for (int j=0; j<numsOfNetPoints; j++){
                    tempFlag[i][j] = matrixOfRoadPrice[i][j];
                }
            }

            /*消费点为x时，为此消费点的已搜索层数循环*/
            for(int nowLevel=1; nowLevel<level; nowLevel++){
                vector<int> thisTimeSelect;

                /*已搜索层数为x时，为待搜索节点进行循环*/
                for(int eachWaitting = 0; eachWaitting<(int)treeCreate.size();eachWaitting++){

                    /*正在搜索的节点为x时，为本行的列进行循环*/
                    /*treeCreate->at(eachWatting) 是实际待找点的坐标*/
                    for(int col=0; col<numsOfNetPoints; col++){
                        if(tempFlag[treeCreate.at(eachWaitting)][col] > 0){  /*当所经点有通路且未访问过时，压栈，并将点设置为已访问*/
                            thisTimeSelect.push_back(col); /*访问的下一个节点压栈*/
                            treeRecord.at(col).at(0) ++; /*将树的影响节点标号++*/
                            treeRecord.at(col).push_back(userPointRecord[eachUserPoint]); /*将树影响的根节点编号进行压栈*/
                            userRootRecord.at(userPointRecord[eachUserPoint]).push_back(col);
                            for(int row=0; row<numsOfNetPoints;row++){/*将所有点通往此点的路径设置为已访问*/
                                tempFlag[row][col] = 0;
                            }
                        }
                    }
                }
                treeCreate.clear();
                /*为下一次树的遍历进行清理*/
                for(int col=0; col<thisTimeSelect.size(); col++){
                    treeCreate.push_back(thisTimeSelect.at(col));
                }
                thisTimeSelect.clear();

            }
        }

        /*迪杰斯特拉*/

    }


    return 0;
}
