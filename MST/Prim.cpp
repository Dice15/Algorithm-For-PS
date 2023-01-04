
/*********************************************************
*   Prim's Algorithm (Minimum Spanning Tree)
* 
*   Time Complexity: O(ElogV)
*
*   Author : J.H Moon <jqkt15@naver.com>
*   Date : January 04, 2023
*
*   Code Test: https://www.acmicpc.net/problem/4792
**********************************************************/


#include <bits/stdc++.h>
using namespace std;


// Prim's Algorithm
class MinimumSpanningTree {
public:
	using datatype = int;

	struct Edge {
		int next;
		datatype weight;
		Edge() :Edge(0, 0) {}
		Edge(int _next, datatype _weight) :next(_next), weight(_weight) {}
	};

	int mVertexCnt;                // 정점의 개수
	vector<vector<Edge>> mGraph;   // 그래프


public:
	MinimumSpanningTree() {}
	MinimumSpanningTree(int vertexCnt) { Assign(vertexCnt); }

	void Assign(int vertexCnt) {
		mVertexCnt = vertexCnt;
		mGraph.assign(vertexCnt, vector<Edge>());
	}

	void Add_UndirectedEdge(int u, int v, datatype weight) {
		mGraph[u].push_back(Edge(v, weight));
		mGraph[v].push_back(Edge(u, weight));
	}


public:
	datatype Prim(int S) {
		datatype result = 0;
		priority_queue<pair<datatype, int>, vector<pair<datatype, int>>, greater<pair<datatype, int>>> pq;
		vector<bool> visited(mVertexCnt, false);

		auto Enque = [&](int next, datatype weight) {
			if (!visited[next]) pq.emplace(weight, next);
		};

		Enque(S, 0);
		while (!pq.empty()) {
			auto [weight, curr] = pq.top(); pq.pop();
			if (visited[curr]) continue;
			visited[curr] = true;
			result += weight;
			for (auto& edge : mGraph[curr]) Enque(edge.next, edge.weight);
		}
		return result;
	}
};