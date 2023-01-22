
/**************************************************************************************************
*   Heavy Light Decomposition
*   Query Time Complexity: O(logN * logN), logN개의 chain이 존재 -> logN개의 구간 쿼리로 처리 가능
* 
*   Author: J.H Moon <jqkt15@naver.com>
*   Date: January 23, 2022
*
*   Code Test: https://www.acmicpc.net/problem/2927
***************************************************************************************************/

#include <bits/stdc++.h>
using namespace std;


class SegmentTree {  // 필요에 따라 Lazy Propagation 적용
public:
	using datatype = int;

	int mRoot, mSize, mDepth, mLeafBegin;
	vector<datatype> mSegTable;

public:
	SegmentTree() {}
	SegmentTree(int size) { Assgin(size); }

	datatype& operator[](int idx) { return mSegTable[mLeafBegin + idx]; }

	void Assgin(int size) {
		mRoot = 1;
		mDepth = (int)ceil(log2(size)) + 1;
		mLeafBegin = 1 << (mDepth - 1);
		mSize = (int)1 << mDepth;
		mSegTable.assign(mSize, 0);
	}

public:
	datatype Operation(datatype fst, datatype snd) { return fst + snd; }

	void UpdateAll() {
		for (int i = mLeafBegin - 1; i > 0; i--)
			mSegTable[i] = Operation(mSegTable[i << 1], mSegTable[(i << 1) + 1]);
	}

	void Update(int idx, datatype value) {
		mSegTable[mLeafBegin + idx] = value;
		idx = (mLeafBegin + idx) >> 1;
		while (idx) {
			mSegTable[idx] = Operation(mSegTable[idx << 1], mSegTable[(idx << 1) + 1]);
			idx >>= 1;
		}
	}

	datatype Query(int obtL, int obtR) {
		if (obtL > obtR) swap(obtL, obtR);
		return SearchRange(obtL, obtR, mRoot, 0, mLeafBegin - 1);
	}
	datatype SearchRange(int obtL, int obtR, int idx, int currL, int currR) {
		if (currR < obtL || obtR < currL) return 0;
		if (obtL <= currL && currR <= obtR) return mSegTable[idx];
		int mid = (currL + currR) >> 1;
		return Operation(SearchRange(obtL, obtR, idx << 1, currL, mid), SearchRange(obtL, obtR, (idx << 1) + 1, mid + 1, currR));
	}
};


class HldTree {
public:
	using datatype = int;
	using ipair = pair<int, int>;

	// Tree
	int mSize, mRoot;
	vector<vector<int>> mAdjList;   // 트리
	vector<datatype> mNodeWeight;   // 노드 가중치
	vector<int> mParent;            // 노드의 부모
	vector<int> mDepth;             // 노드의 깊이

	// HLD
	int mDfsCnt;
	vector<int> mSubSize;        // 서브트리 크기
	vector<int> mChainTop;       // 노드가 속한 체인의 가장 윗 정점
	vector<ipair> mDfsOrder;     // 노드 방문 순서 {in, out}

	// Range Qeury
	SegmentTree segTree;

public:
	HldTree() {}
	HldTree(int size) { Assign(size); }

	void Assign(int size) {
		mSize = size;
		mRoot = 0;
		mAdjList.assign(size, vector<int>());
		mNodeWeight.assign(size, 0);
		mParent.assign(size, -1e9);
		mDepth.assign(size, -1e9);

		mDfsCnt = -1;
		mSubSize.assign(size, 0);
		mChainTop.assign(size, -1e9);
		mDfsOrder.assign(size, ipair(-1e9, -1e9));

		segTree.Assgin(size);
	}

	void Add_UndirectedEdge(int u, int v) {
		mAdjList[u].push_back(v);
		mAdjList[v].push_back(u);
	}

public:
	// 서브트리의 크기, 노드의 깊이, 직속 부모를 구한다
	void Dfs(int prev, int curr, int depth) {
		mSubSize[curr] = 1;
		mDepth[curr] = depth;
		mParent[curr] = prev;
		for (auto& next : mAdjList[curr]) {
			if (prev == next) continue;
			Dfs(curr, next, depth + 1);
			mSubSize[curr] += mSubSize[next];
			// 서브트리가 가장 큰 것이 맨 앞으로(단, 맨 앞이 prev면 일단 swap)
			if (mAdjList[curr][0] == prev || mSubSize[next] > mSubSize[mAdjList[curr][0]]) swap(next, mAdjList[curr][0]);
		}
	}

	// Heavy Light Decomposition
	void Decomposition(int prev, int curr) {
		mDfsOrder[curr].first = ++mDfsCnt;
		for (auto& next : mAdjList[curr]) {
			if (prev == next) continue;
			// 맨 앞 노드면 부모 체인에 포함, 아니면 새로운 체인 생성 
			mChainTop[next] = next == mAdjList[curr][0] ? mChainTop[curr] : next;
			Decomposition(curr, next);
		}
		mDfsOrder[curr].second = mDfsCnt;
	}

	void Construct() {
		for (int i = 0; i < mSize; i++) {   // forest 구조 고려 (dfs, decomposition을 각 트리에 대해 진행해야함)
			if (mChainTop[i] != -1e9) continue;
			Dfs(-1, i, 0);
			mChainTop[i] = i;
			Decomposition(-1, i);
		}
		for (int i = 0; i < mSize; i++) segTree[mDfsOrder[i].first] = mNodeWeight[i];
		segTree.UpdateAll();
	}

	void UpdateWeight(int u, int value) {
		segTree.Update(mDfsOrder[u].first, mNodeWeight[u] = value);
	}

	int Query(int u, int v) {
		int res = 0;
		while (mChainTop[u] ^ mChainTop[v]) {   // 두 노드가 같은 체인에 포함될 때 까지 조상으로 올라감
			if (mDepth[mChainTop[u]] < mDepth[mChainTop[v]]) swap(u, v);
			int uTop = mChainTop[u];
			res += segTree.Query(mDfsOrder[uTop].first, mDfsOrder[u].first);
			u = mParent[uTop];
		}
		if (mDepth[u] > mDepth[v]) swap(u, v);
		res += segTree.Query(mDfsOrder[u].first, mDfsOrder[v].first);
		return res;
	}
};