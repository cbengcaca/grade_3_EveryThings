#include <iostream>
#define nums 5
using namespace std;

int main()
{
    int upValue[nums][nums] = {
        {0,0,0,0,0},
        {1,0,2,4,3},
        {4,6,5,2,1},
        {4,4,5,2,1},
        {5,6,8,5,3}
        /*upValue记录的是某一点上方的路径权值
        */
    };

    int leftValue[nums][nums] = {
        {0,3,2,4,0},
        {0,3,2,4,2},
        {0,0,7,3,4},
        {0,3,3,0,2},
        {0,1,3,2,2}
        /*leftValue记录的是某一点左侧记录的路径权值
        */
    };

    int record[nums][nums];
    for (int i=0; i<nums; i++){
        for (int j=0; j<nums; j++){
            record[i][j] = 0;
            /*record记录的是某一点完成最短路径的
            上一跳是向上或者向左
            */
        }
    }

    int bestValue[nums][nums];
    bestValue[0][0] = 0;
    for (int i=1; i<nums; i++){
        bestValue[i][0] = bestValue[i-1][0] + upValue[i][0];
        record[i][0] = 1;
        bestValue[0][i] = bestValue[0][i-1] + leftValue[0][i];
        record[0][i] = -1;
    }

    for (int i=1; i<nums; i++){
        for (int j=i; j<nums; j++){
            int upValueSum = bestValue[i-1][j] + upValue[i][j];
            int leftValueSum = bestValue[i][j-1] + leftValue[i][j];
            if (upValueSum > leftValueSum){ /*最佳路径从上方来*/
                record[i][j] = 1;
                bestValue[i][j] = upValueSum;
            }else{  /*最佳路径从左边来*/
                record[i][j] = -1;
                bestValue[i][j] = leftValueSum;
            }

            upValueSum = bestValue[j-1][i] + upValue[j][i];
            leftValueSum = bestValue[j][i-1] + leftValue[j][i];
            if (upValueSum > leftValueSum){
                record[j][i] = 1;
                bestValue[j][i] = upValueSum;
            }else{
                record[j][i] = -1;
                bestValue[j][i] = leftValueSum;
            }
        }
    }

    cout << "You should follow the road as: " << endl;
    int row = nums-1;
    int col = nums-1;
    while (true){
        cout << "( " << row << " , "<< col <<" )" <<endl;
        if (row == 0 && col == 0){
            break;
        }
        if (record[row][col] == 1){
            row--;
        }else{
            col--;
        }


    }

    return 0;
}
