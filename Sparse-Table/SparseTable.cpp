
/**************************************************************************
*   Sparse Table
*   Time Complexity: Construct O(NlogL), Query O(logL),  L is 'Max Level'
* 
*   Author: J.H Moon <jqkt15@naver.com>
*   Date: January 21, 2022
*
*   Code Test: https://www.acmicpc.net/problem/17399
***************************************************************************/


#include <bits/stdc++.h>
using namespace std;


class SparseTable {
public:
	int mSize, mMaxLev;
	vector<vector<int>> mTable;


public:
	SparseTable() {}
	SparseTable(int size, int maxLev) { Assign(size, maxLev); }

	int& operator[](int idx) { return mTable[idx][0]; }

	void Assign(int size, int maxLev) {
		mSize = size;
		mMaxLev = maxLev;
		mTable.assign(mSize, vector<int>(mMaxLev, -1e9));
	}


public:
	void Construct() {
		for (int j = 0; j < mMaxLev - 1; j++) {
			for (int i = 0; i < mSize; i++) {
				if (mTable[i][j] < 0) continue;   // -1e9은 빈칸
				mTable[i][j + 1] = mTable[mTable[i][j]][j];   // mTable[i][j]는 i를 2^j번 이동했을 때의 결과. 즉, j=1이면 2번 이동, j=4면 16번 이동
			}
		}
	}

	int MoveQuery(int idx, int moveCnt) {
		for (int j = mMaxLev - 1; j >= 0; j--) {
			if (moveCnt & (int)1 << j) {
				idx = mTable[idx][j];
			}
		}
		return idx;
	}
};
