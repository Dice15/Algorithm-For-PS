
/**************************************************************************
*   Strongly Connected (Robert Tarjan)
*   Time Complexity: O(V + E)
*
*   Author: J.H Moon <jqkt15@naver.com>
*   Date: January 28, 2022
*
*   Code Test: https://www.acmicpc.net/problem/26146
***************************************************************************/


#include <bits/stdc++.h>
using namespace std;


class Graph{
public:
	int mSize, mSccCnt;
	vector<vector<int>> mGraph;
	vector<int> mSccNum;              // 정점i는 몇 번 Scc에 속하는가?
	vector<int> mDfsCnt;
	vector<int> mDfsStack;

public:
	Graph(int size = 0) { Assign(size); }

	void Assign(int size = 0) {
		mSize = size;
		mSccCnt = 0;
		mGraph.assign(size, vector<int>());
		mSccNum.assign(size, -1);
		mDfsCnt.assign(size, 0);
		mDfsStack.clear();
	}

	void Add_DirectedEdge(int u, int v) { mGraph[u].push_back(v); }

public:
	int Dfs(int curr, int& cnt) {
		int minCnt = mDfsCnt[curr] = ++cnt;
		mDfsStack.push_back(curr);

		for (auto next : mGraph[curr]) {
			if (mDfsCnt[next] == 0) minCnt = min(minCnt, Dfs(next, cnt));
			else if (mSccNum[next] == -1)  minCnt = min(minCnt, mDfsCnt[next]);
		}

		if (minCnt == mDfsCnt[curr]) {
			while (1) {
				int v = mDfsStack.back(); mDfsStack.pop_back();
				mSccNum[v] = mSccCnt;
				if (v == curr) break;
			}
			mSccCnt++;
		}
		return minCnt;
	}

	void ConstructScc() {
		int cnt = 0;
		for (int i = 0; i < mSize; i++) {
			if (mDfsCnt[i] == 0) Dfs(i, cnt);
		}
	}
};