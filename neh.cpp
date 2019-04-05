#include <iostream>
#include <fstream>
#include <queue>
#include <cmath>
#include <chrono>


using namespace std;

int taskTable[500][20];
int currentTaskTable[500][20];
int nextStepTable[500][20];
int cTable[501][21];
int idTable[500];
int wTable[500];

void swapTasksWithIds(int id1, int id2, int m) {
    for(int i=0;i<m;i++) {
        swap(currentTaskTable[id1][i], currentTaskTable[id2][i]);
    }
    swap(idTable[id1], idTable[id2]);
}

void swapTasks(int id1, int id2, int m) {
    for(int i=0;i<m;i++) {
        swap(nextStepTable[id1][i], nextStepTable[id2][i]);
    }
}

void countW(int n, int m) {
    int currentSum;
    for(int i=0;i<n;i++) {
        currentSum = 0;
        for(int j=0;j<m;j++) {
            currentSum += taskTable[i][j];
        }
        wTable[i] = currentSum;
    }
}

int countCmax(int n, int m, int tmpTable[500][20]) {
    int Cmax;
    for(int i=0;i<n+1;i++) {
        for(int j=0;j<m+1;j++) {
            if(i == 0 || j == 0) {
                cTable[i][j] = 0;
            } else {
                cTable[i][j] = max(cTable[i-1][j], cTable[i][j-1]) + tmpTable[i-1][j-1];
            }
        }
    }
    Cmax = cTable[n][m];
    return Cmax;
}

int getNextTaskId(int n) {
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

int performNextStep(int stepNumber, int m) {
    for(int i = 0;i<stepNumber+1;i++) {
        for(int j=0;j<m;j++) {
            nextStepTable[i][j] = currentTaskTable[i][j];
        }
    }
    int Cmax = 0, tmp = 0, whichLoop = 0, loopNum = 0;
    Cmax = countCmax(stepNumber+1, m, nextStepTable);
    for(int i=stepNumber;i>0;i--) {
        loopNum++;
        swapTasks(i, i-1, m);
        tmp = countCmax(stepNumber+1, m, nextStepTable);
        if(tmp <= Cmax) {
            Cmax = tmp;
            whichLoop = loopNum;
        }
    }
    return whichLoop;
}

void performNeh(int n, int m) {
    int nextId, loopId, tmp;
    nextId = getNextTaskId(n);
    idTable[0] = nextId;
    for(int i=0;i<m;i++) {
        currentTaskTable[0][i] = taskTable[nextId][i];
    }
    for(int i=1;i<n;i++) {
        nextId = getNextTaskId(n);
        idTable[i] = nextId;
        for(int j=0;j<m;j++) {
            currentTaskTable[i][j] = taskTable[nextId][j];
        }
        loopId = performNextStep(i, m);
        tmp = i;
        for(int j=loopId;j>0;j--) {
            swapTasksWithIds(tmp, tmp-1, m);
            tmp--;
        }
    }

    cout << countCmax(n, m, currentTaskTable) << endl;
    for(int i=0;i<n;i++) {
        cout << idTable[i] + 1 << " ";
    }
    cout << endl;
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
        // while(str != "data.002:") {
        data >> str;
        }
        data >> str;
        n = stoi(str);
        data >> str;
        m = stoi(str);
        for(int y=0;y<n;y++) {
            for(int i=0;i<m;i++) {
                data >> taskTable[y][i];
            }
        }
        countW(n, m);
        performNeh(n, m);
        cout << tmp << endl;
        tmp++;
    }
    auto end = chrono::steady_clock::now();
	cout << "Elapsed time in seconds : "
		<< chrono::duration_cast<chrono::seconds>(end - start).count()
		<< " s" << endl;
}