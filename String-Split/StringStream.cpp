/*********************************************************************************
*   File Name: StringStream.cpp
*   Description: stringstream-based delimiter string splitter.
*
*   Author: J.H Moon <jqkt15@naver.com>
*   Date: November 25, 2023
*
*   Time Complexity: O(n)
*
*   Code Test: https://school.programmers.co.kr/learn/courses/30/lessons/67257
*********************************************************************************/

#include <bits/stdc++.h>
using namespace std;


// stringstream�� ������� �ؽ�Ʈ�� �����ϴ� �Լ�
vector<string> split(const string& text, char delimiter) {
	vector<string> result;
	stringstream ss(text);
	string item;
	while (getline(ss, item, delimiter)) {
		result.push_back(item);
	}
	return result;
}


int main() {
	// ��ǥ�� �������� �и�
	string text1 = "{22,1111,3333,4444}";
	for (const auto& s : split(text1.substr(1, text1.size() - 2), ',')) {
		cout << stoll(s) << '\n';
	}

	return 0;
}