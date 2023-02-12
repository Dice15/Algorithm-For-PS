
/******************************************************************
*   Minimum Cost Circulation (Johnson's Algorithm, SPFA, Dijkstra)
*   Time Complexity: O(ElogV * F)
*
*   Author : J.H Moon <jqkt15@naver.com>
*   Date : February 13, 2023
*
*   Code Test: https://www.acmicpc.net/problem/12959
******************************************************************/


#include <bits/stdc++.h>
using namespace std;


class FlowGraph
{
public:
	using datatype = long long;

	struct Edge {
		int next;
		datatype capacity, weight;
		int reverse;
		Edge(int _next, datatype _cap, datatype _wgt, int _rev) :next(_next), capacity(_cap), weight(_wgt), reverse(_rev) {}
	};

	const datatype INF = 1e18;             // int(=1e9) long long(=1e18 or 2e18)

	int Size;                              // 그래프의 크기(=정점의 개수) 
	vector<vector<Edge>> AdjList;          // 그래프

	vector<datatype> Potential, NewDist;   // 경로비용
	vector<int> Prev, EdgeIdx;             // 증가경로 (사용된 정점, 간선)


public:
	FlowGraph(int size = 0) { Assign(size); }

	void Assign(int size) {
		Size = size;
		AdjList.assign(size, vector<Edge>());
		Potential.assign(size, INF);
		NewDist.assign(size, INF);
		Prev.assign(size, INF);
		EdgeIdx.assign(size, INF);
	}

	void AddDirectedEdge(int u, int v, datatype capacity, datatype weight) {
		AdjList[u].push_back(Edge(v, capacity, weight, (int)AdjList[v].size()));
		AdjList[v].push_back(Edge(u, 0, -weight, (int)AdjList[u].size() - 1));
	}

public:
	bool Spfa(int source, int sink) {
		fill(Potential.begin(), Potential.end(), INF);
		queue<int> q;
		vector<bool> inque(Size, false);
		bool res = false;

		auto Enque = [&](int next, int weight) {
			Potential[next] = weight;
			if (!inque[next]) { inque[next] = true; q.push(next); }
		};

		Enque(source, 0);
		while (!q.empty()) {
			int curr = q.front(); q.pop(); inque[curr] = false;
			if (curr == sink) res = true;
			for (int i = 0; i < (int)AdjList[curr].size(); i++) {
				Edge edge = AdjList[curr][i];
				if (edge.capacity > 0 && Potential[edge.next] > Potential[curr] + edge.weight) {
					Prev[edge.next] = curr;
					EdgeIdx[edge.next] = i;
					Enque(edge.next, Potential[curr] + edge.weight);
				}
			}
		}
		return res;
	}

	bool Dijkstra(int source, int sink) {
		fill(NewDist.begin(), NewDist.end(), INF);
		priority_queue<pair<datatype, int>, vector<pair<datatype, int>>, greater<pair<datatype, int>>> pq;
		bool res = false;

		auto Enque = [&](int next, int weight) {
			NewDist[next] = weight;
			pq.emplace(weight, next);
		};

		Enque(source, 0);
		while (!pq.empty()) {
			auto [weight, curr] = pq.top(); pq.pop();
			if (NewDist[curr] != weight) continue;
			if (curr == sink) res = true;
			for (int i = 0; i < (int)AdjList[curr].size(); i++) {
				Edge edge = AdjList[curr][i];
				datatype newWeight = edge.weight + Potential[curr] - Potential[edge.next];
				if (edge.capacity > 0 && NewDist[edge.next] > NewDist[curr] + newWeight) {
					Prev[edge.next] = curr;
					EdgeIdx[edge.next] = i;
					Enque(edge.next, NewDist[curr] + newWeight);
				}
			}
		}
		return res;
	}

	pair<datatype, datatype> Mcmf(int source, int sink) {
		if (!Spfa(source, sink)) return { -INF, -INF };
		auto res = pair<datatype, datatype>(0, 0);
		while (Dijkstra(source, sink)) {
			for (int i = 0; i < Size; i++) Potential[i] += min(INF, NewDist[i]);
			datatype minCap = INF;
			for (int v = sink; v != source; v = Prev[v]) minCap = min(minCap, AdjList[Prev[v]][EdgeIdx[v]].capacity);
			for (int v = sink; v != source; v = Prev[v]) {
				int rev = AdjList[Prev[v]][EdgeIdx[v]].reverse;
				AdjList[Prev[v]][EdgeIdx[v]].capacity -= minCap;
				AdjList[v][rev].capacity += minCap;
			}
			res.first += minCap * (Potential[sink] - Potential[source]);
			res.second += minCap;
		}
		return res;
	}
};