#include <iostream>
#include <fstream>
#include <queue>
#include <cmath>
#include <chrono>


using namespace std;

void swapTasksWithIds(int id1, int id2, int m, vector<vector<int>> &taskTable, vector<int> &idTable) {
    for(int i=0;i<m;i++) {
        swap(taskTable[id1][i], taskTable[id2][i]);
    }
    swap(idTable[id1], idTable[id2]);
}

void swapTasks(int id1, int id2, int m, vector<vector<int>> &taskTable) {
    for(int i=0;i<m;i++) {
        swap(taskTable[id1][i], taskTable[id2][i]);
    }
}

void countW(int n, int m, vector<vector<int>> taskTable, vector<int> &wTable) {
    int currentSum;
    for(int i=0;i<n;i++) {
        currentSum = 0;
        for(int j=0;j<m;j++) {
            currentSum += taskTable[i][j];
        }
        wTable[i] = currentSum;
    }
}

int countCmax(int n, int m, vector<vector<int>> taskTable) {
    int Cmax;
    vector<vector<int>> cTable(n+1, vector<int> (m+1, 0));
    for(int i=0;i<n+1;i++) {
        for(int j=0;j<m+1;j++) {
            if(i == 0 || j == 0) {
                cTable[i][j] = 0;
            } else {
                cTable[i][j] = max(cTable[i-1][j], cTable[i][j-1]) + taskTable[i-1][j-1];
            }
        }
    }
    Cmax = cTable[n][m];
    cTable.clear();
    cTable.shrink_to_fit();
    return Cmax;
}

int getNextTaskId(int n, vector<int> &wTable) {
    int id = 0, w = 0;
     for(int i=0;i<n;i++) {
        if(w < wTable[i]) {
            w = wTable[i];
            id = i;
        }
    }
    wTable[id] = 0;
    return id;
}

int performNextStep(int stepNumber, int m, vector<vector<int>> taskTable) {
    int Cmax = 0, tmp = 0, whichLoop = 0, loopNum = 0;
    Cmax = countCmax(stepNumber+1, m, taskTable);
    // cout << Cmax << endl;
    for(int i=stepNumber;i>0;i--) {
        loopNum++;
        swapTasks(i, i-1, m, taskTable);
        tmp = countCmax(stepNumber+1, m, taskTable);
        // cout << tmp << endl;
        if(tmp <= Cmax) {
            Cmax = tmp;
            whichLoop = loopNum;
        }
    }
    return whichLoop;
}

void performNeh(int n, int m, vector<vector<int>> taskTable, vector<int> &wTable, vector<int> &idTable) {
    int nextId, loopId, tmp;
    vector<vector<int>> currentTaskTable(n, vector<int> (m, 0));
    nextId = getNextTaskId(n, wTable);
    // cout << nextId + 1 << endl;
    idTable.push_back(nextId);
    for(int i=0;i<m;i++) {
        currentTaskTable[0][i] = taskTable[nextId][i];
    }
    for(int i=1;i<n;i++) {
        nextId = getNextTaskId(n, wTable);
        // cout << nextId + 1 << endl;
        idTable.push_back(nextId);
        for(int j=0;j<m;j++) {
            currentTaskTable[i][j] = taskTable[nextId][j];
        }
        loopId = performNextStep(i, m, currentTaskTable);
        tmp = i;
        for(int j=loopId;j>0;j--) {
            swapTasksWithIds(tmp, tmp-1, m, currentTaskTable, idTable);
            tmp--;
        }
        // cout << endl;
        // for(int j=0;j<idTable.size();j++) {
        //     cout << idTable[j] + 1 << " ";
        // }
        // cout << endl;
        // cout << countCmax(n, m, currentTaskTable) << endl;
        //  for(int j=0;j<i+1;j++) {
        //     for(int g=0;g<m;g++) {
        //         cout << currentTaskTable[j][g] << " " << idTable[j] + 1;
        //     }
        //     cout << endl;
        // }
        // cout << loopId << endl;
    }

    cout << countCmax(n, m, currentTaskTable) << endl;
    for(int i=0;i<n;i++) {
        cout << idTable[i] + 1 << " ";
    }
    cout << endl;
    currentTaskTable.clear();
    currentTaskTable.shrink_to_fit();
}



int main () {
    auto start = chrono::steady_clock::now();
    ifstream data;
    data.open("neh.data.txt");
    int n, m, tmp = 0;
    string str;
    string part1 = "data.", part2 = ":", part3 = "";
    string data1;
    string dataNames[121];
    for(int i = 0; i<121;i++) {
        if(i < 10) {
            part3 = "00" + to_string(i);
        } else if(i < 100 && i >= 10) {
            part3 = "0" + to_string(i);
        } else {
            part3 = to_string(i);
        }
        data1 = part1 + part3 + part2;
        dataNames[i] = data1;
        cout << dataNames[i] << endl;
    }
    for(int i = 0; i<121;i++) {
        while(str != dataNames[i]) {
        // while(str != "data.003:") {
        data >> str;
        }
        data >> str;
        n = stoi(str);
        data >> str;
        m = stoi(str);
        vector<vector<int>> taskTable(n, vector<int> (m, 0));
        vector<int> idTable;
        vector<int> wTable(n,0);
        // load dataset to taskTable
        for(int y=0;y<n;y++) {
            for(int i=0;i<m;i++) {
                data >> taskTable[y][i];
            }
        }
        countW(n, m, taskTable, wTable);
        performNeh(n, m, taskTable, wTable, idTable);
        cout << tmp << endl;
        tmp++;
        taskTable.clear();
        taskTable.shrink_to_fit();
        idTable.clear();
        idTable.shrink_to_fit();
        wTable.clear();
        wTable.shrink_to_fit();
    }
    auto end = chrono::steady_clock::now();
	cout << "Elapsed time in seconds : "
		<< chrono::duration_cast<chrono::seconds>(end - start).count()
		<< " s" << endl;
}