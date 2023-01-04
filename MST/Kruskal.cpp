
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
	int mSetSize;            // ���յ��� ũ��
	vector<int> mSetTable;   // ���� ������ ��Ʈ�� �����ϴ� ���̺�. -1�̸� �ڽ��� ��Ʈ��� ���� �ǹ�


public:
	DisjointSet() {}
	DisjointSet(int setSize) { Assign(setSize); }

	void Assign(int setSize) {
		mSetSize = setSize;
		mSetTable.assign(setSize, -1);
	}


public:
	// val�� ���� ������ ��ȣ�� ��ȯ�Ѵ�
	int Find(int val) {
		if (mSetTable[val] < 0) return val;
		return mSetTable[val] = Find(mSetTable[val]);
	}

	// val1�� ���� ���հ� val2�� ���� ������ ��ģ��. ��ġ�� ���� �����ߴٸ� true�� �ƴϸ� false�� ��ȯ�Ѵ�.
	bool Merge(int val1, int val2) {
		val1 = Find(val1);
		val2 = Find(val2);
		if (val1 == val2) return false;
		mSetTable[val1] += mSetTable[val2];   // �� ������ ũ�⸦ ��ġ�� ����
		mSetTable[val2] = val1;  // val2�� ��Ʈ�� val1���� ���������ν� �� ������ ��ħ
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

	int mVertexCnt;             // ������ ����
	DisjointSet mDisjointSet;   // �� ������ ���� ������Ʈ�� �����ִ��� Ȯ��

	int mEdgeCnt;               // ������ ����
	vector<Edge> mEdges;        // ��������Ʈ


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