
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
	SparseTable(int size, int maxLev) {}

	int& operator[](int idx) { return mTable[idx][0]; }

	void Assgin(int size, int maxLev) {
		mSize = size;
		mMaxLev = maxLev;
		mTable.assign(size, vector<int>(maxLev, -1e9));
	}

public:
	void Construct() {
		for (int j = 0; j < mMaxLev - 1; j++) {
			for (int i = 0; i < mSize; i++) {
				if (mTable[i][j] < 0) continue;   // -1e9�� ��ĭ
				mTable[i][j + 1] = mTable[mTable[i][j]][j];   // mTable[i][j]�� i�� 2^j�� �̵����� ���� ���. ��, j=1�̸� 2�� �̵�, j=4�� 16�� �̵�
			}
		}
	}

	int MoveUpQuery(int idx, int moveCnt) {
		for (int j = mMaxLev - 1; j >= 0; j--)
			if (moveCnt & (int)1 << j) idx = mTable[idx][j];
		return idx;
	}

	int LcaQuery(int u, int v, int uDep, int vDep) {
		if (u == v) return u;
		if (uDep < vDep) swap(u, v);
		u = MoveUpQuery(u, abs(uDep - vDep));
		if (u != v) {
			for (int j = mMaxLev - 1; j >= 0; j--) {
				// ���� ó���� �Ű澵 �ʿ䰡 ����. depth�� ���� ������ �ϳ��� ������ �Ǹ� �ٸ� �ϳ��� ���� �������� ������ �Ǿ� �ش� ����� �������� �ʴ´�
				if (mTable[u][j] != mTable[v][j]) {
					u = mTable[u][j];
					v = mTable[v][j];
				}
			}
			u = mTable[u][0];   // loop������ lca�� ���� �ڽı��� ���������Ƿ�, ���� ����� ���� �θ� lca�� �ȴ�
		}
		return u;
	}
};


class Tree {
public:
	// Tree
	int mSize, mRoot;
	vector<vector<int>> mAdjList;

	// LCA
	vector<int> mDepth;
	SparseTable mAncestor;


public:
	Tree() {}
	Tree(int size) { Assign(size); }

	void Assign(int size) {
		mSize = size;
		mRoot = 0;
		mAdjList.assign(size, vector<int>());
		mDepth.assign(size, -1);
		mAncestor.Assign(size, (int)ceil(log2(size)) + 1);
	}

	void Add_UndirectedEdge(int u, int v) {
		mAdjList[u].push_back(v);
		mAdjList[v].push_back(u);
	}


public:
	void Dfs(int prev, int curr, int depth) {
		mDepth[curr] = depth;
		mAncestor[curr] = prev;
		for (auto& next : mAdjList[curr]) {
			if (prev == next) continue;
			Dfs(curr, next, depth + 1);
		}
	}

	void Construct() {
		Dfs(-1e9, mRoot, 0);
		mAncestor.Construct();
	}

	int GetLca(int u, int v) { return mAncestor.LcaQuery(u, v, mDepth[u], mDepth[v]); }

	int GetAncestor(int u, int nth) { return mAncestor.MoveUpQuery(u, nth); }
};