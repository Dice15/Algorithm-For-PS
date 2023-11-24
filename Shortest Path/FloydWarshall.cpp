/*********************************************************************************
*   File Name: FloydWarshall.cpp
*   Description: Finding shortest paths in weighted graphs, including those with negative weights.
*
*   Author: J.H Moon <jqkt15@naver.com>
*   Date: November 25, 2023
*
*   Time Complexity: O(n^3)
*
*   Code Test: https://school.programmers.co.kr/learn/courses/30/lessons/67256
*********************************************************************************/

#include <bits/stdc++.h>
using namespace std;


// 플로이드 워셜 알고리즘으로 모든 정점 사이의 최단거리를 계산
void floydWarshall(vector<vector<int>>& dist) {
	int n = dist.size();
	for (int k = 0; k < n; k++)  // 중간 정점 (경유)
		for (int u = 0; u < n; u++)  // 시작 정점
			for (int v = 0; v < n; v++)  // 종료 정점
				dist[u][v] = min(dist[u][v], dist[u][k] + dist[k][v]);
}


int main() {
	int INF = (int)1e9;
	vector<vector<int>> graph = {
		{0, 5, INF, 10},
		{INF, 0, 3, INF},
		{INF, INF, 0, 1},
		{INF, INF, INF, 0}
	};

	floydWarshall(graph);

	for (const auto& row : graph) {
		for (int distance : row) {
			if (distance == INF) cout << "INF ";
			else cout << distance << " ";
		}
		cout << '\n';
	}

	return 0;
}