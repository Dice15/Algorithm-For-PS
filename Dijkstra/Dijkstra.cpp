
/******************************************************
*   Dijkstra
*
*   Author : J.H Moon <jqkt15@naver.com>
*   Date : December 31, 2022
*
*   Code Test: https://www.acmicpc.net/problem/5719
*******************************************************/


#include <iostream>
#include <vector>
#include <queue>
using namespace std;


class WeightGraph
{
public:
	using datatype = int;

	struct Edge {
		int next;
		datatype weight;
		Edge(int _next, datatype _weight) :next(_next), weight(_weight) {}
	};

	const datatype INF = 1e9;         // int(=1e9) long long(=1e18 or 2e18)

	int mSource, mSink, mGraphSize;   // 그래프의 시작점, 그래프의 끝점, 그래프의 크기 (=정점의 개수)
	vector<vector<Edge>> mGraph;      // 그래프
	vector<datatype> mDist;           // 최단경로 (RevGraph)


public:
	WeightGraph() { Assign(0); }
	WeightGraph(int graphSize) { Assign(graphSize); }

	vector<Edge>& operator[](int idx) { return mGraph[idx]; }

	void Assign(int graphSize) {
		mGraphSize = graphSize;
		mGraph.assign(graphSize, vector<Edge>());
	}

	void Add_DirectedEdge(int u, int v, datatype weight) {
		mGraph[u].push_back(Edge(v, weight));
	}


public:
	int Dijkstra(int source, int sink) {
		mSource = source;
		mSink = sink;
		mDist.assign(mGraphSize, INF);

		priority_queue<pair<datatype, int>, vector<pair<datatype, int>>, greater<pair<datatype, int>>> pq;

		auto Enque = [&](int next, datatype pathWeight) {
			if (mDist[next] > pathWeight) {
				mDist[next] = pathWeight;
				pq.emplace(pathWeight, next);
			}
		};

		Enque(mSource, 0);
		while (!pq.empty()) {
			auto [pathWeight, curr] = pq.top(); pq.pop();
			if (mDist[curr] != pathWeight) continue;
			for (auto& edge : mGraph[curr]) Enque(edge.next, edge.weight + pathWeight);
		}

		return mDist[mSink];
	}
};