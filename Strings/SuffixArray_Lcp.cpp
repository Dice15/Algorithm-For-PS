
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
	vector<int> mSfxArray;      // i번째 순위에 해당하는 문자열(text의 index)을 저장하고 있다.
	vector<int> mInvSfxArray;   // 문자열(text의 index)의 순위를 저장하고 있다. SfxArray와 역함수 관계
	vector<int> mLcpArray;      // SfxArray에서 어떤 문자열과 바로 앞 문자열의 공통된 접두사 중 가장 긴 것의 길이를 저장


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
			// counting sort: i+len에 대해 먼저 정렬 후 i에 대해 정렬
			fill(cnt.begin(), cnt.end(), 0);
			for (int i = 0; i < mTextLen; i++) cnt[ord[min(i + len, mTextLen)]]++;
			for (int i = 1; i <= cntRange; i++) cnt[i] += cnt[i - 1];
			for (int i = mTextLen - 1; i >= 0; i--) sndSort[--cnt[ord[min(i + len, mTextLen)]]] = i;

			fill(cnt.begin(), cnt.end(), 0);
			for (int i = 0; i < mTextLen; i++) cnt[ord[i]]++;
			for (int i = 1; i <= cntRange; i++) cnt[i] += cnt[i - 1];
			for (int i = mTextLen - 1; i >= 0; i--) fstSort[--cnt[ord[sndSort[i]]]] = sndSort[i];

			if (ordCnt == mTextLen) break;

			// 정렬된 데이터로 i번째 데이터 순위를 갱신한다
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
			if (mInvSfxArray[i]) {   // inv[i]는 S[i...N]의 순위를 나타낸다
				int j = mSfxArray[mInvSfxArray[i] - 1];   // S[sfx[inv[i]-1]...N]은 S[i...N] 바로 앞 순위의 문자열이고, j는 Text에서의 위치를 나타냄
				while (mText[i + skip] == mText[j + skip]) skip++;
				mLcpArray[mInvSfxArray[i]] = skip;
			}
			// if lcp(S[sfx[inv[i]-1], S[i...N]) > 1 이면, lcp(S[sfx[inv[i+1]-1], S[i+1...N]) = lcp(S[sfx[inv[i]-1], S[i...N]) - 1
			// ex lcp(ana, anana) = 3 > 1이므로, lcp(na, nana) = 3 - 1 = 2이다.
			skip = max(skip - 1, 0);
		}
	}
};