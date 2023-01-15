
/******************************************************
*   Suffix Array(with Counting Sort), Lcp 
*   Time Complexity: Suffix Array: O(NlogN), Lcp: O(N)
*
*   Author : J.H Moon <jqkt15@naver.com>
*   Date : January 16, 2022
*
*   Code Test: https://www.acmicpc.net/problem/13576
*******************************************************/


#include <bits/stdc++.h>
using namespace std;


class SuffixArray {
public:
	int mTextLen;
	string mText;
	vector<int> mSfxArray;      // i��° ������ �ش��ϴ� ���ڿ�(text�� index)�� �����ϰ� �ִ�.
	vector<int> mInvSfxArray;   // ���ڿ�(text�� index)�� ������ �����ϰ� �ִ�. SfxArray�� ���Լ� ����
	vector<int> mLcpArray;      // SfxArray���� � ���ڿ��� �ٷ� �� ���ڿ��� ����� ���λ� �� ���� �� ���� ���̸� ����


public:
	SuffixArray() { }
	SuffixArray(string& text) { Update(text); }

	void Update(string& text) {
		mTextLen = text.size();
		mText = text;
		mSfxArray.assign(mTextLen, 0);
		mInvSfxArray.assign(mTextLen, 0);
		mLcpArray.assign(mTextLen, 0);
		ConstructSFX();
		ConstructLCP();
	}


public:
	void ConstructSFX() {
		int cntRange = max(mTextLen, 255);
		vector<int>& fstSort = mSfxArray, sndSort(mTextLen), ord(mTextLen + 1), newOrd(mTextLen + 1), cnt(cntRange + 1);

		for (int i = 0; i < mTextLen; i++) {
			fstSort[i] = i;
			ord[i] = mText[i];
		}

		for (int len = 1, ordCnt = 1;; len <<= 1) {
			// counting sort: i+len�� ���� ���� ���� �� i�� ���� ����
			fill(cnt.begin(), cnt.end(), 0);
			for (int i = 0; i < mTextLen; i++) cnt[ord[min(i + len, mTextLen)]]++;
			for (int i = 1; i <= cntRange; i++) cnt[i] += cnt[i - 1];
			for (int i = mTextLen - 1; i >= 0; i--) sndSort[--cnt[ord[min(i + len, mTextLen)]]] = i;

			fill(cnt.begin(), cnt.end(), 0);
			for (int i = 0; i < mTextLen; i++) cnt[ord[i]]++;
			for (int i = 1; i <= cntRange; i++) cnt[i] += cnt[i - 1];
			for (int i = mTextLen - 1; i >= 0; i--) fstSort[--cnt[ord[sndSort[i]]]] = sndSort[i];

			if (ordCnt == mTextLen) break;

			// ���ĵ� �����ͷ� i��° ������ ������ �����Ѵ�
			newOrd[fstSort[0]] = ordCnt = 1;
			for (int i = 1; i < mTextLen; i++) {
				if (ord[fstSort[i - 1]] != ord[fstSort[i]] || ord[fstSort[i - 1] + len] != ord[fstSort[i] + len]) {
					ordCnt++;
				}
				newOrd[fstSort[i]] = ordCnt;
			}
			ord = newOrd;
		}
	}

	void ConstructLCP() {
		for (int i = 0; i < mTextLen; i++) mInvSfxArray[mSfxArray[i]] = i;
		for (int i = 0, skip = 0; i < mTextLen; i++) {
			if (mInvSfxArray[i]) {   // inv[i]�� S[i...N]�� ������ ��Ÿ����
				int j = mSfxArray[mInvSfxArray[i] - 1];   // S[sfx[inv[i]-1]...N]�� S[i...N] �ٷ� �� ������ ���ڿ��̰�, j�� Text������ ��ġ�� ��Ÿ��
				while (mText[i + skip] == mText[j + skip]) skip++;
				mLcpArray[mInvSfxArray[i]] = skip;
			}
			// if lcp(S[sfx[inv[i]-1], S[i...N]) > 1 �̸�, lcp(S[sfx[inv[i+1]-1], S[i+1...N]) = lcp(S[sfx[inv[i]-1], S[i...N]) - 1
			// ex lcp(ana, anana) = 3 > 1�̹Ƿ�, lcp(na, nana) = 3 - 1 = 2�̴�.
			skip = max(skip - 1, 0);
		}
	}
};