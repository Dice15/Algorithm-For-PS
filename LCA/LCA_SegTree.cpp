
/******************************************************
*   LCA (using Segment Tree)
*
*   Author: J.H Moon <jqkt15@naver.com>
*   Date: December 30, 2022
*
*   Code Test: https://www.acmicpc.net/problem/11438
*******************************************************/


#include <bits/stdc++.h>
using namespace std;


template <typename datatype>
class SegmentTree
{
public:
    datatype mDefaultVal;

    int mRoot;                    // 트리 루트
    int mDepth;                   // 트리 깊이
    int mLeafBegin;               // 리프노드 시작 위치 (leaf idx is 0-base)
    int mSize;                    // 트리 크기
    vector<datatype> mSegTable;   // 트리의 값을 저장하는 테이블


public:
    SegmentTree() {}
    SegmentTree(datatype defVal, int segSize) { Assign(defVal, segSize); }

    datatype& operator [](int leafNum) { return mSegTable[mLeafBegin + leafNum]; }

    void Insert(int leafNum, datatype val) { mSegTable[mLeafBegin + leafNum] = val; }

    void Assign(datatype defVal, int segSize) {
        mDefaultVal = defVal;
        mRoot = 1;
        mDepth = ceil(log2(segSize)) + 1;
        mLeafBegin = 1 << (mDepth - 1);
        mSize = 1 << mDepth;
        mSegTable.assign(mSize, mDefaultVal);
    }


public:
    // 연산 함수
    datatype& Operation(datatype& fst, datatype& snd) {
        return fst < snd ? fst : snd;
    }

    // 모든 리프에 대해 업데이트
    void UpdateAll() {
        for (int idx = mLeafBegin - 1; idx > 0; idx--) {
            mSegTable[idx] = Operation(mSegTable[idx << 1], mSegTable[(idx << 1) + 1]);
        }
    }

    // 특정 리프에 대해 업데이트
    void Update(int leafNum) {
        int idx = mLeafBegin + leafNum;
        while (idx > mRoot) {
            idx >>= 1;
            mSegTable[idx] = Operation(mSegTable[idx << 1], mSegTable[(idx << 1) + 1]);
        }
    }

    // 쿼리 수행 (param is left <= right)
    datatype& ProcessQuery(int left, int right) {
        if (left > right) swap(left, right);
        return SearchRange(left, right, mRoot, 0, mLeafBegin - 1);
    }
    datatype& SearchRange(int L, int R, int idx, int curL, int curR) {
        if (R < curL || curR < L) return mDefaultVal;
        if (L <= curL && curR <= R) return mSegTable[idx];
        int mid = (curL + curR) >> 1;
        return Operation(SearchRange(L, R, idx << 1, curL, mid), SearchRange(L, R, (idx << 1) + 1, mid + 1, curR));
    }
};



class Tree
{
public:
    using ipair = pair<int, int>;

    int mSize;                      // 트리 크기
    int mRoot;                      // 트리 루트
    vector<vector<int>> mTreeAdj;   // 트리의 연결 상태

    SegmentTree<ipair> mSegTree;    // LCA를 쿼리를 수행할 세그트리
    int mLeafCnt;                   // 리프 카운팅
    vector<int> mLeafIdx;           // 트리의 노드가 세그트리의 몇 번째 리프에 있는지 저장


public:
    Tree() {}
    Tree(int treeSize) { Assign(treeSize); }

    void AddUndiredtedEdge(int u, int v) {
        mTreeAdj[u].push_back(v);
        mTreeAdj[v].push_back(u);
    }

    void Assign(int treeSize) {
        mSize = treeSize;
        mRoot = 0;
        mTreeAdj.assign(mSize, vector<int>());

        mSegTree.Assign(ipair(1e8, 1e8), mSize * 2);
        mLeafCnt = 0;
        mLeafIdx.assign(mSize * 2, -1);
    }


public:
    // root를 기준으로 lca segment tree구성
    void ConstructLca(int root) {
        Dfs(-1, mRoot = root, 0);
        mSegTree.UpdateAll();
    }
    void Dfs(int parent, int node, int depth) {
        mSegTree.Insert(mLeafIdx[node] = mLeafCnt++, ipair(depth, node));
        for (auto& son : mTreeAdj[node]) {
            if (parent == son) continue;
            Dfs(node, son, depth + 1);
            mSegTree.Insert(mLeafCnt++, ipair(depth, node));
        }
    }

    ipair GetNode(int u) { return mSegTree[mLeafIdx[u]]; }

    ipair Lca(int u, int v) { return mSegTree.ProcessQuery(mLeafIdx[u], mLeafIdx[v]); }
};
