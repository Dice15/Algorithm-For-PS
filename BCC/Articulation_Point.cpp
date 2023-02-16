
/**************************************************************************
*   Articulation Point (dfs low link)
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
	int Size, ApCnt;
	vector<vector<int>> Gph;
	vector<bool> Ap;           // 정점이 단절점이면 true
	vector<int> DfsCnt;        // 정점에 dfs방문 순서로 번호 부여
	vector<int> LowLink;       // 정점이 속한 Bcc의 정점중에 가장 작은 DfsCnt

public:
	Graph(int size = 0) { Assign(size); }

	void Assign(int size = 0) {
		Size = size;
		ApCnt = 0;
		Gph.assign(size, vector<int>());
		Ap.assign(size, false);
		DfsCnt.assign(size, 0);
		LowLink.assign(size, 0);
	}

	void Add_UndirectedEdge(int u, int v) {
		Gph[u].push_back(v);
		Gph[v].push_back(u);
	}

public:
	int Dfs3(int curr, int prev, int& cnt) {
		DfsCnt[curr] = LowLink[curr] = ++cnt;
		int child = 0;
		for (auto& next : Gph[curr]) {
			if (next == prev) continue;
			if (DfsCnt[next]) LowLink[curr] = min(LowLink[curr], DfsCnt[next]);
			else {
				child++;
				int low = Dfs3(next, curr, cnt);
				LowLink[curr] = min(LowLink[curr], low);
				if (prev != -1 && DfsCnt[curr] <= low) Ap[curr] = true;
			}
		}
		if (prev == -1)  Ap[curr] = (child >= 2);
		ApCnt += Ap[curr];
		return LowLink[curr];
	}

	void ConstructAp() {
		for (int i = 0, cnt = 0; i < Size; i++)
			if (!DfsCnt[i]) Dfs3(i, -1, cnt);
	}
};