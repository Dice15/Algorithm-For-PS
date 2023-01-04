
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

    int mRoot;                    // Ʈ�� ��Ʈ
    int mDepth;                   // Ʈ�� ����
    int mLeafBegin;               // ������� ���� ��ġ (leaf idx is 0-base)
    int mSize;                    // Ʈ�� ũ��
    vector<datatype> mSegTable;   // Ʈ���� ���� �����ϴ� ���̺�


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
    // ���� �Լ�
    datatype& Operation(datatype& fst, datatype& snd) {
        return fst < snd ? fst : snd;
    }

    // ��� ������ ���� ������Ʈ
    void UpdateAll() {
        for (int idx = mLeafBegin - 1; idx > 0; idx--) {
            mSegTable[idx] = Operation(mSegTable[idx << 1], mSegTable[(idx << 1) + 1]);
        }
    }

    // Ư�� ������ ���� ������Ʈ
    void Update(int leafNum) {
        int idx = mLeafBegin + leafNum;
        while (idx > mRoot) {
            idx >>= 1;
            mSegTable[idx] = Operation(mSegTable[idx << 1], mSegTable[(idx << 1) + 1]);
        }
    }

    // ���� ���� (param is left <= right)
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