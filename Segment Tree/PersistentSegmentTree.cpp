
/**************************************************************************
*   Persistent Segment Tree
*   Time Complexity: Update(add new tree) O(logN), Query O(logN)
*
*   Author: J.H Moon <jqkt15@naver.com>
*   Date: January 25, 2022
*
*   Code Test: https://www.acmicpc.net/problem/2042
***************************************************************************/


#include <bits/stdc++.h>
using namespace std;


class PersistentSegmentTree {
public:
	using datatype = long long;

	struct Node {
		Node* son[2];
		datatype value;
		Node(datatype _val = 0, Node* _l = NULL, Node* _r = NULL) { value = _val; son[0] = _l; son[1] = _r; }
	};

	int mSize, mLevel;
	vector<Node*> mRoot;           // 0 ~ mLevel 번째 루트
	vector<datatype> mLeafTable;   // 0번 세그먼트 트리의 리프

public:
	PersistentSegmentTree(int size = 0, int levCnt = 1) { Assign(size, levCnt); }

	datatype& operator[](int idx) { return mLeafTable[idx]; }

	void Assign(int size, int levCnt = 1) {
		mSize = 1 << (int)ceil(log2(size));
		mLevel = levCnt;
		mRoot.reserve(mLevel);
		mLeafTable.assign(size, 0);
	}

public:
	datatype Operation(datatype fst, datatype snd) { return fst + snd; }

	// 0번 세그먼트 트리 구성
	void Construct() { mRoot.push_back(CreateBaseTree(0, mSize - 1)); }
	Node* CreateBaseTree(int currL, int currR) {
		if ((int)mLeafTable.size() <= currL) return new Node();
		if (currL == currR) return new Node(mLeafTable[currL]);
		int mid = (currL + currR) >> 1;
		Node* newNode = new Node();
		newNode->son[0] = CreateBaseTree(currL, mid);
		newNode->son[1] = CreateBaseTree(mid + 1, currR);
		newNode->value = Operation(newNode->son[0]->value, newNode->son[1]->value);
		return newNode;
	}

	// 데이터 업데이트 및 새 트리를 구성
	void Update(int idx, datatype value) { mRoot.push_back(CreateNewTree(idx, value, mRoot.back(), 0, mSize - 1)); }
	Node* CreateNewTree(int idx, datatype value, Node* node, int currL, int currR) {
		if (idx < currL || currR < idx) return node;
		if (currL == currR) return new Node(value);   // 문제에 따라 갱신 or 누적
		int mid = (currL + currR) >> 1;
		Node* newNode = new Node();
		newNode->son[0] = CreateNewTree(idx, value, node->son[0], currL, mid);
		newNode->son[1] = CreateNewTree(idx, value, node->son[1], mid + 1, currR);
		newNode->value = Operation(newNode->son[0]->value, newNode->son[1]->value);
		return newNode;
	}

	// lev번째 트리에서 쿼리 수행
	datatype Query(int left, int right, int lev = -1) {
		if (left > right) swap(left, right);
		return SearchSegment(left, right, lev == -1 ? mRoot.back() : mRoot[lev], 0, mSize - 1);
	}
	datatype SearchSegment(int obtL, int obtR, Node* node, int currL, int currR) {
		if (obtR < currL || currR < obtL) return 0;
		if (obtL <= currL && currR <= obtR) return node->value;
		int mid = (currL + currR) >> 1;
		return Operation(SearchSegment(obtL, obtR, node->son[0], currL, mid), SearchSegment(obtL, obtR, node->son[1], mid + 1, currR));
	}
};