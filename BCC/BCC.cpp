
/**************************************************************************
*   Biconnected Component (dfs low link)
*   Time Complexity: O(V + E)
*
*   Author: J.H Moon <jqkt15@naver.com>
*   Date: February 17, 2023
*
*   Code Test: https://www.acmicpc.net/problem/11266
***************************************************************************/


#include <bits/stdc++.h>
using namespace std;

class Graph {
public:
	int Size, BccCnt;
	vector<vector<int>> Gph;
	vector<vector<int>> Bcc;   // Bcc에 속한 정점리스트
	vector<vector<int>> Num;   // 정점이 속한 Bcc번호
	vector<int> DfsCnt;        // 정점에 dfs방문 순서로 번호 부여
	vector<int> LowLink;       // 정점이 속한 Bcc의 정점중에 가장 작은 DfsCnt

public:
	Graph(int size = 0) { Assign(size); }

	void Assign(int size = 0) {
		Size = size;
		BccCnt = 0;
		Gph.assign(size, vector<int>());
		Bcc.assign(size, vector<int>());
		Num.assign(size, vector<int>());
		DfsCnt.assign(size, 0);
		LowLink.assign(size, 0);
	}

	void Add_UndirectedEdge(int u, int v) {
		Gph[u].push_back(v);
		Gph[v].push_back(u);
	}

public:
	int Dfs1(int curr, int prev, int& cnt) {
		DfsCnt[curr] = LowLink[curr] = ++cnt;
		for (auto& next : Gph[curr]) {
			if (next == prev) continue;
			LowLink[curr] = min(LowLink[curr], DfsCnt[next] ? DfsCnt[next] : Dfs1(next, curr, cnt));
		}
		return LowLink[curr];
	}

	void AddVertex(int u, int cnt) { Bcc[--cnt].push_back(u); Num[u].push_back(cnt); }
	void Dfs2(int curr, int cnt) {
		if (cnt) AddVertex(curr, cnt);
		for (auto& next : Gph[curr]) {
			if ((int)Num[next].size()) continue;
			if (DfsCnt[curr] <= LowLink[next]) {
				AddVertex(curr, ++BccCnt);
				Dfs2(next, BccCnt);
			}
			else Dfs2(next, cnt);
		}
	}

	void ConstructBcc() {
		for (int i = 0, cnt = 0; i < Size; i++) {
			if (!(int)Num[i].size()) { Dfs1(i, -1, cnt); Dfs2(i, 0); }
		}
	}
};