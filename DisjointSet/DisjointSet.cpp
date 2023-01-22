
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
	int mSetSize;            // ���յ��� ũ��
	vector<int> mSetTable;   // ���� ������ ��Ʈ�� �����ϴ� ���̺�. -1�̸� �ڽ��� ��Ʈ��� ���� �ǹ�


public:
	DisjointSet() {}
	DisjointSet(int size) { Assign(size); }

	void Assign(int size) {
		mSetSize = size;
		mSetTable.assign(size, -1);
	}


public:
	// val�� ���� ������ ��ȣ�� ��ȯ�Ѵ�
	int Find(int u) {
		if (mSetTable[u] < 0) return u;
		return mSetTable[u] = Find(mSetTable[u]);
	}

	// val1�� ���� ���հ� val2�� ���� ������ ��ģ��. ��ġ�� ���� �����ߴٸ� true�� �ƴϸ� false�� ��ȯ�Ѵ�.
	bool Merge(int u, int v) {
		u = Find(u);
		v = Find(v);
		if (u == v) return false;
		mSetTable[u] += mSetTable[v];   // �� ������ ũ�⸦ ��ġ�� ����
		mSetTable[v] = u;  // val2�� ��Ʈ�� val1���� ���������ν� �� ������ ��ħ
		return true;
	}
};