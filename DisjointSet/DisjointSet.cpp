
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