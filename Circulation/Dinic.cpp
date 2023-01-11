
/******************************************************
*   Dinic (Circulation)
*   Time Complexity: O(V^2 * E)
*
*   Author : J.H Moon <jqkt15@naver.com>
*   Date : January 11, 2022
*
*   Code Test: https://www.acmicpc.net/problem/13161
*******************************************************/


#include <bits/stdc++.h>
using namespace std;


class FlowGraph
{
public:
	using datatype = long long;

	struct Edge {
		int next;
		datatype capacity;
		int reverse;
		Edge(int _next, datatype _cap, int _rev) :next(_next), capacity(_cap), reverse(_rev) {}
	};

	const datatype INF = 2e9;         // int(=1e9) long long(=1e18 or 2e18)

	int mSource, mSink, mGrapgSize;   // �ҽ�, ��ũ, �׷����� ũ��(=������ ����) 
	vector<vector<Edge>> mGraph;      // �׷���
	vector<int> mLevel;               // ���� ����
	vector<int> mWork;                // ���� ������ ã�� ��, �ߺ� ��� ã�� ���� ���� ����


public:
	FlowGraph() { Assign(0); }
	FlowGraph(int graphSize) { Assign(graphSize); }

	void Assign(int graphSize) {
		mGrapgSize = graphSize;
		mGraph.assign(graphSize, vector<Edge>());
		mLevel.assign(graphSize, 0);
		mWork.assign(graphSize, 0);
	}

	void Add_DirectedEdge(int u, int v, datatype capacity) {
		mGraph[u].push_back(Edge(v, capacity, (int)mGraph[v].size()));
		mGraph[v].push_back(Edge(u, 0, (int)mGraph[u].size() - 1));
	}


public:
	// ���� �׷���
	bool Bfs() {
		fill(mLevel.begin(), mLevel.end(), 0);
		fill(mWork.begin(), mWork.end(), 0);
		queue<int> q;

		auto Enque = [&](int next, int level) {
			mLevel[next] = level;
			q.push(next);
		};

		Enque(mSource, 1);
		while (!q.empty()) {
			int curr = q.front(); q.pop();
			for (auto& edge : mGraph[curr]) {
				if (edge.capacity > 0 && !mLevel[edge.next]) Enque(edge.next, mLevel[curr] + 1);
			}
		}
		return mLevel[mSink] > 0;
	}


	// ���� ���� ã�� ������ �긲
	datatype Dfs(int curr, datatype minFlow) {
		if (curr == mSink) return minFlow;
		for (; mWork[curr] < mGraph[curr].size(); mWork[curr]++) {
			Edge edge = mGraph[curr][mWork[curr]];
			if (edge.capacity > 0 && mLevel[edge.next] == mLevel[curr] + 1) {
				datatype canFlow = Dfs(edge.next, min(minFlow, edge.capacity));
				if (canFlow) {
					mGraph[curr][mWork[curr]].capacity -= canFlow;
					mGraph[edge.next][edge.reverse].capacity += canFlow;
					return canFlow;
				}
			}
		}
		return 0;
	}

	// �ִ����� ��ȯ.
	// �ּ��� Ȱ��: �и��� �� ���� ������Ʈ�� ������ Bfs�Լ��� �������� �� Level�� �ο����η� �Ǻ��� �� �ִ�.
	datatype Dinic(int source, int sink) {
		mSource = source;
		mSink = sink;
		datatype totalFlow = 0;
		while (Bfs()) {
			datatype canFlow;
			while ((canFlow = Dfs(mSource, INF))) totalFlow += canFlow;
		}
		return totalFlow;
	}
};