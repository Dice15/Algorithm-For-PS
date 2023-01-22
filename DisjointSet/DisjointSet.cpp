
/******************************************************
*   Disjoint Set
*   Time Complexity: merge/find O(mlog*n) = O(m)
* 
*   Author : J.H Moon <jqkt15@naver.com>
*   Date : January 01, 2023
*
*   Code Test: https://www.acmicpc.net/problem/1939
*******************************************************/


#include <bits/stdc++.h>
using namespace std;

class DisjointSet {
public:
	int mSetSize;            // 집합들의 크기
	vector<int> mSetTable;   // 속한 집합의 루트를 저장하는 테이블. -1이면 자신이 루트라는 것을 의미


public:
	DisjointSet() {}
	DisjointSet(int size) { Assign(size); }

	void Assign(int size) {
		mSetSize = size;
		mSetTable.assign(size, -1);
	}


public:
	// val이 속한 집합의 번호를 반환한다
	int Find(int u) {
		if (mSetTable[u] < 0) return u;
		return mSetTable[u] = Find(mSetTable[u]);
	}

	// val1이 속한 집합과 val2가 속한 집합을 합친다. 합치는 것을 성공했다면 true를 아니면 false를 반환한다.
	bool Merge(int u, int v) {
		u = Find(u);
		v = Find(v);
		if (u == v) return false;
		mSetTable[u] += mSetTable[v];   // 두 집합의 크기를 합치는 연산
		mSetTable[v] = u;  // val2의 루트를 val1으로 설정함으로써 두 집합을 합침
		return true;
	}
};