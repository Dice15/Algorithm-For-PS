
/*********************************************************
*   Kruskal's Algorithm (Minimum Spanning Tree)
* 
*   Time Complexity: O(ElogE) + O(Elog*V) = O(ElogE)
*
*   Author : J.H Moon <jqkt15@naver.com>
*   Date : January 01, 2023
*
*   Code Test: https://www.acmicpc.net/problem/1939
**********************************************************/


#include <bits/stdc++.h>
using namespace std;

class DisjointSet {
public:
	int mSetSize;            // 집합들의 크기
	vector<int> mSetTable;   // 속한 집합의 루트를 저장하는 테이블. -1이면 자신이 루트라는 것을 의미


public:
	DisjointSet() {}
	DisjointSet(int setSize) { Assign(setSize); }

	void Assign(int setSize) {
		mSetSize = setSize;
		mSetTable.assign(setSize, -1);
	}


public:
	// val이 속한 집합의 번호를 반환한다
	int Find(int val) {
		if (mSetTable[val] < 0) return val;
		return mSetTable[val] = Find(mSetTable[val]);
	}

	// val1이 속한 집합과 val2가 속한 집합을 합친다. 합치는 것을 성공했다면 true를 아니면 false를 반환한다.
	bool Merge(int val1, int val2) {
		val1 = Find(val1);
		val2 = Find(val2);
		if (val1 == val2) return false;
		mSetTable[val1] += mSetTable[val2];   // 두 집합의 크기를 합치는 연산
		mSetTable[val2] = val1;  // val2의 루트를 val1으로 설정함으로써 두 집합을 합침
		return true;
	}
};


// Kruskal's Algorithm
class MinimumSpanningTree {
public:
	using datatype = int;

	struct Edge {
		int u, v;
		datatype weight;
		Edge() :Edge(0, 0, 0) {}
		Edge(int _u, int _v, datatype _weight) :u(_u), v(_v), weight(_weight) {}
	};

	int mVertexCnt;             // 정점의 개수
	DisjointSet mDisjointSet;   // 두 정점이 같은 컴포넌트에 속해있는지 확인

	int mEdgeCnt;               // 간선의 개수
	vector<Edge> mEdges;        // 간선리스트


public:
	MinimumSpanningTree() {}
	MinimumSpanningTree(int vertexCnt, int edgeCnt = 0) { Assign(vertexCnt, edgeCnt); }

	void Assign(int vertexCnt, int edgeCnt = 0) {
		mVertexCnt = vertexCnt;
		mDisjointSet.Assign(mVertexCnt);
		mEdgeCnt = edgeCnt;
		mEdges.reserve(mEdgeCnt);
	}

	void Add_UndirectedEdge(int u, int v, datatype weight) {
		mEdges.push_back(Edge(u, v, weight));
	}


public:
	datatype Kruskal(int obtEdgeCnt) {
		auto cmp = [&](const Edge& fst, const Edge& snd) {
			return fst.weight < snd.weight;
		};
		sort(mEdges.begin(), mEdges.end(), cmp);

		datatype result = 0;
		for (auto& edge : mEdges) {
			if (mDisjointSet.Merge(edge.u, edge.v)) {
				result += edge.weight;
				if (!(--obtEdgeCnt)) break;
			}
		}
		return result;
	}
};