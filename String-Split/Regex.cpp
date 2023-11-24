/*********************************************************************************
*   File Name: Regex.cpp
*   Description: Regular expression based string splitter
* 
*   Author : J.H Moon <jqkt15@naver.com>
*   Date : November 25, 2023
* 
*   Time Complexity: unknown
* 
*   Code Test: https://school.programmers.co.kr/learn/courses/30/lessons/67257
*********************************************************************************/

#include <bits/stdc++.h>
#include <regex>
using namespace std;


// ���� ����
enum SplitType { SPLIT_TYPE_CONTAIN, SPLIT_TYPE_EXCEPT };


// regex�� ������� �ؽ�Ʈ�� �����ϴ� �Լ�
vector<string> split(const string& text, SplitType split_type, const regex& format = regex(R"()")) {
    sregex_token_iterator iter, end;
    if (split_type == SPLIT_TYPE_CONTAIN) {   // �����ϴ� ���ڿ� ���� ����
        iter = sregex_token_iterator(text.begin(), text.end(), format);
    }
    else if (split_type == SPLIT_TYPE_EXCEPT) {   // �������� �ʴ� ���ڿ� ���� ����
        iter = sregex_token_iterator(text.begin(), text.end(), format, -1);
    }
    return vector<string>(iter, end);
}


int main() {
    // ������ �����ڸ� �и�
    string text1 = "100-200*300-500+20";
    for (const auto& s : split(text1, SPLIT_TYPE_CONTAIN, regex(R"((\d+)|(\D))"))) {
        cout << s << '\n';
    }

    // 2���� ���Ϳ� ���� ������ ����
    string text2 = "{{2},{2,1},{2,1,3},{222,11,3333,44444}}";
    for (const auto& s : split(text2.substr(1, text2.size() - 2), SPLIT_TYPE_CONTAIN, regex(R"(\{(\d+(,\d+)*)\})"))) {
        cout << s << ": ";
        for (auto& e : split(s, SPLIT_TYPE_CONTAIN, regex(R"(\d+)"))) {
            cout << stoll(e) << ' ';
        }
        cout << '\n';
    }

    return 0;
}