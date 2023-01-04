
/******************************************************
*   Segment Tree
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