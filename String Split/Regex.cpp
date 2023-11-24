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


// 분할 유형
enum SplitType { SPLIT_TYPE_CONTAIN, SPLIT_TYPE_EXCEPT };


// regex를 기반으로 텍스트를 분할하는 함수
vector<string> split(const string& text, SplitType split_type, const regex& format = regex(R"()")) {
    sregex_token_iterator iter, end;
    if (split_type == SPLIT_TYPE_CONTAIN) {   // 포함하는 문자에 따라 분할
        iter = sregex_token_iterator(text.begin(), text.end(), format);
    }
    else if (split_type == SPLIT_TYPE_EXCEPT) {   // 포함하지 않는 문자에 따라 분할
        iter = sregex_token_iterator(text.begin(), text.end(), format, -1);
    }
    return vector<string>(iter, end);
}


int main() {
    // 정수와 연산자를 분리
    string text1 = "100-200*300-500+20";
    for (const auto& s : split(text1, SPLIT_TYPE_CONTAIN, regex(R"((\d+)|(\D))"))) {
        cout << s << '\n';
    }

    // 2차원 벡터에 각각 정수를 저장
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