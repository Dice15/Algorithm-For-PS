
/******************************************************
*   Segment Tree with Lazy Propagation
*
*   Author : J.H Moon <jqkt15@naver.com>
*   Date : December 30, 2022
*
*   Code Test: https://www.acmicpc.net/problem/10999
*******************************************************/


#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
using namespace std;


template <typename datatype>
class SegmentTree_LazyPropagation
{
public:
    datatype mDefaultVal;

    int mRoot;                     // Ʈ�� ��Ʈ
    int mDepth;                    // Ʈ�� ����
    int mLeafBegin;                // ������� ���� ��ġ (leaf idx is 0-base)
    int mSize;                     // Ʈ�� ũ��
    vector<datatype> mSegTable;    // Ʈ���� ���� �����ϴ� ���̺�
    vector<datatype> mLazyTable;   // �̷��� ������Ʈ�� �����ϴ� ���̺�


public:
    SegmentTree_LazyPropagation() {}
    SegmentTree_LazyPropagation(datatype defVal, int segSize) { Assign(defVal, segSize); }

    datatype& operator [](int leafNum) { return mSegTable[mLeafBegin + leafNum]; }

    void Insert(int leafNum, datatype val) { mSegTable[mLeafBegin + leafNum] = val; }

    void Assign(datatype defVal, int segSize) {
        mDefaultVal = defVal;
        mRoot = 1;
        mDepth = ceil(log2(segSize)) + 1;
        mLeafBegin = 1 << (mDepth - 1);
        mSize = 1 << mDepth;
        mSegTable.assign(mSize, mDefaultVal);
        mLazyTable.assign(mSize, mDefaultVal);
    }


public:
    // ���� �Լ�
    datatype Operation(datatype fst, datatype snd) {
        return fst + snd;
    }

    // ���� ������ �̷��� ������Ʈ ����, ���� ������ ������Ʈ�� �̷�
    void LazyPropagation(int idx, int curL, int curR, datatype val) {
        mLazyTable[idx] += val;
        if (mLazyTable[idx] != mDefaultVal) {
            if (idx < mLeafBegin) {   // ������ �ƴϸ� ���� ������ lazy ����  
                mLazyTable[idx << 1] += mLazyTable[idx];
                mLazyTable[(idx << 1) + 1] += mLazyTable[idx];
            }
            mSegTable[idx] += (curR - curL + 1) * mLazyTable[idx];
            mLazyTable[idx] = 0;
        }
    }

    // ��� ������ ���� ������Ʈ (Lazy Update�� ����� ���Ŀ��� �� �Լ��� ����ؼ��� �� �ȴ�)
    void UpdateAll() {
        for (int idx = mLeafBegin - 1; idx > 0; idx--)
            mSegTable[idx] = Operation(mSegTable[idx << 1], mSegTable[(idx << 1) + 1]);
    }

    // ���� ������Ʈ (Lazy Update)
    datatype UpdateRange(int left, int right, datatype val) {
        if (left > right) swap(left, right);
        return UpdateRange(left, right, val, mRoot, 0, mLeafBegin - 1);
    }
    datatype UpdateRange(int L, int R, datatype val, int idx, int curL, int curR) {
        LazyPropagation(idx, curL, curR, (L <= curL && curR <= R) ? val : mDefaultVal);
        if (curR < L || R < curL) { return mSegTable[idx]; }
        if (L <= curL && curR <= R) { return mSegTable[idx]; }
        int mid = (curL + curR) >> 1;
        return mSegTable[idx] = Operation(UpdateRange(L, R, val, idx << 1, curL, mid), UpdateRange(L, R, val, (idx << 1) + 1, mid + 1, curR));
    }

    // ���� ����
    datatype ProcessQuery(int left, int right) {
        if (left > right) swap(left, right);
        return ProcessQuery(left, right, mRoot, 0, mLeafBegin - 1);
    }
    datatype ProcessQuery(int L, int R, int idx, int curL, int curR) {
        LazyPropagation(idx, curL, curR, mDefaultVal);
        if (curR < L || R < curL) return mDefaultVal;
        if (L <= curL && curR <= R) return mSegTable[idx];
        int mid = (curL + curR) >> 1;
        return Operation(ProcessQuery(L, R, idx << 1, curL, mid), ProcessQuery(L, R, (idx << 1) + 1, mid + 1, curR));
    }
};