
/**************************************************************************
*   Lca (using Sparse Table)
*   Time Complexity: Construct O(N + NlogN), LcaQuery O(logN)
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
		for (int j = mMaxLev - 1; j >= 0; j--)
			if (moveCnt & (int)1 << j) idx = mTable[idx][j];
		return idx;
	}

	int LcaQuery(int u, int v, int depGap) {
		u = MoveQuery(u, depGap);
		if (u != v) {
			for (int j = mMaxLev - 1; j >= 0; j--) {
				if (mTable[u][j] != mTable[v][j]) {   // 음수 처리는 신경쓸 필요가 없다. depth가 같기 때문에 하나가 음수가 되면 다른 하나도 음수가 된다
					u = mTable[u][j];
					v = mTable[v][j];
				}
			}
			u = mTable[u][0];   // loop문에서 lca바로 아랫 단계까지 진행했으므로 직속 부모를 찾는다
		}
		return u;
	}
};


class Tree {
public:
	int mSize, mRoot;
	vector<vector<int>> mAdjList;
	vector<int> mNodeDepth;
	SparseTable mParent;


public:
	Tree() {}
	Tree(int size) { Assign(size); }

	void Assign(int size) {
		mSize = size;
		mRoot = 0;
		mAdjList.assign(size, vector<int>());
		mNodeDepth.assign(size, -1);
		mParent.Assign(size, (int)ceil(log2(size)) + 1);
	}

	void Add_UndirectedEdge(int u, int v) {
		mAdjList[u].push_back(v);
		mAdjList[v].push_back(u);
	}


public:
	void Construct() {
		Dfs(-1e9, mRoot, 0);
		mParent.Construct();
	}

	void Dfs(int prev, int curr, int depth) {
		mNodeDepth[curr] = depth;
		mParent[curr] = prev;
		for (auto& next : mAdjList[curr]) {
			if (prev == next) continue;
			Dfs(curr, next, depth + 1);
		}
	}

	int GetLca(int u, int v) {   // u와 v의 lca
		if (u == v) return u;
		if (mNodeDepth[u] < mNodeDepth[v]) swap(u, v);
		return mParent.LcaQuery(u, v, mNodeDepth[u] - mNodeDepth[v]);
	}

	int GetAncestor(int u, int nth) { return mParent.MoveQuery(u, nth); }   // nth번째 조상
};