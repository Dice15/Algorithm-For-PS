
/**************************************************************************
*   Articulation Bridge (dfs low link)
*   Time Complexity: O(V + E)
*
*   Author: J.H Moon <jqkt15@naver.com>
*   Date: February 17, 2023
*
*   Code Test: https://www.acmicpc.net/problem/11400
***************************************************************************/


#include <bits/stdc++.h>
using namespace std;

class Graph {
public:
	int Size, AbCnt;
	vector<vector<int>> Gph;
	vector<pair<int, int>> Ab;   // 단절선 리스트
	vector<int> DfsCnt;          // 정점에 dfs방문 순서로 번호 부여
	vector<int> LowLink;         // 정점이 속한 Bcc의 정점중에 가장 작은 DfsCnt

public:
	Graph(int size = 0) { Assign(size); }

	void Assign(int size = 0) {
		Size = size;
		AbCnt = 0;
		Gph.assign(size, vector<int>());
		Ab.clear();
		DfsCnt.assign(size, 0);
		LowLink.assign(size, 0);
	}

	void Add_UndirectedEdge(int u, int v) {
		Gph[u].push_back(v);
		Gph[v].push_back(u);
	}

public:
	int Dfs4(int curr, int prev, int& cnt) {
		DfsCnt[curr] = LowLink[curr] = ++cnt;
		for (auto& next : Gph[curr]) {
			if (next == prev) continue;
			if (DfsCnt[next]) LowLink[curr] = min(LowLink[curr], DfsCnt[next]);
			else {
				int low = Dfs4(next, curr, cnt);
				LowLink[curr] = min(LowLink[curr], low);
				if (DfsCnt[curr] < low) { Ab.push_back({ min(curr, next), max(curr, next) }); AbCnt++; }
			}
		}
		return LowLink[curr];
	}

	void ConstructAb() {
		for (int i = 0, cnt = 0; i < Size; i++)
			if (!DfsCnt[i]) Dfs4(i, -1, cnt);
	}
};