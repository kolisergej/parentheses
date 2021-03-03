#include <cassert>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_set>
#include <unordered_map>

using namespace std;

const string kInfinite{"Infinite"};
const unordered_map<char, char> kDict = {
    {'{', '}'},
    {'(', ')'},
    {'[', ']'}
};
const unordered_set<char> kValues{'}', ')', ']'};

bool check_infinite(const string& input) {
    size_t first_open_idx = 0;
    while (first_open_idx < input.size() && kValues.count(input[first_open_idx])) {
        ++first_open_idx;
    }
    stack<char> st;
    const auto check_ch = [&st, &input](size_t i) {
        const auto ch = input[i];
        if (kDict.count(input[i])) {
            st.push(ch);
            return true;
        }
        if (!kValues.count(ch)) {
            return true;
        }
        if (!st.empty() && kDict.at(st.top()) == ch) {
            st.pop();
            return true;
        }
        return false;
    };

    for (size_t i = first_open_idx; i < input.size(); ++i) {
        if (!check_ch(i)) {
            return false;
        }
    }
    for (size_t i = 0; i < first_open_idx; ++i) {
        if (!check_ch(i)) {
            return false;
        }
    }
    return st.empty();
}

string process_letters_only(const string& input) {
    string result;
    int start{-1};
    size_t current_length{0};
    for (size_t i = 0; i < input.size(); i++) {
        const auto ch = input[i];
        if (kDict.count(ch) || kValues.count(ch)) {
            if (start != -1 && current_length > result.size()) {
                result = input.substr(start, current_length);
            }
            current_length = 0;
            start = -1;
            continue;
        }
        if (start == -1) {
            start = i;
        }
        ++current_length;
    }
    if (start != -1 && current_length > result.size()) {
        result = input.substr(start, current_length);
    }
    return result;
}

string process_with_parrs(const string& input) {
    int start{-1};
    int end{-1};
    stack<pair<char, int>> st;
    for (int i = 0; i < input.size(); ++i) {
        const auto ch = input[i];
        if (kDict.count(ch)) {
            st.push(make_pair(ch, i));
        } else if (kValues.count(ch)) {
            if (st.empty()) {
                continue;
            }
            const auto top = st.top();
            if (kDict.count(top.first) && kDict.at(top.first) == ch) {
                st.pop();
                int aux = 0;
                while (!st.empty()) {
                    if (kValues.count(st.top().first) || kDict.count(st.top().first)) {
                        break;
                    }
                    ++aux;
                    st.pop();
                }
                if (end == top.second - aux) {
                    end = i+1;
                } else if (top.second < start) {
                    start = top.second;
                    end = i+1;
                } else if (end - start < (i - top.second) + aux) {
                    start = top.second-aux;
                    end = i+1;
                }
            } else {
                stack<pair<char, int>>().swap(st);
            }
        } else if (!st.empty()) {
            auto top = st.top();
            if (kValues.count(top.first) || !kDict.count(top.first)) {
                st.push(make_pair(ch, i));
            }
        } else {
            st.push(make_pair(ch, i));
        }
        // cout << start << ", " << end << endl;
    }
    if (start != -1 && end != -1) {
        return input.substr(start, end-start);
    }
    return "";
}

string run(const string& input) {
    if (check_infinite(input)) {
        return kInfinite;
    }
    
    const string double_input = input + input;
    const string letters_only_max = process_letters_only(double_input);
    const string max_with_parrs = process_with_parrs(double_input);
    const string result = (letters_only_max.size() > max_with_parrs.size()) ? letters_only_max : max_with_parrs;
    // cout << "result: " << result << endl;
    return result;
}

int main() {
    // assert(run("") == kInfinite);
    // assert(run("aabbcc") == kInfinite);
    // assert(run("[]") == kInfinite);
    // assert(run("(){}") == kInfinite);
    // assert(run("]h({hdb}b)[") == kInfinite);
    // assert(run("[[b[aa]]]") == kInfinite);
    // assert(run("]][[b[aa]") == kInfinite);
    assert(run("aaaa)((aaa)") == kInfinite);

    assert(run("]})") == "");
    assert(run("())") == "()");
    assert(run("((aa)") == "(aa)");
    assert(run("[[]") == "[]");
    assert(run("[[])") == "[]");
    assert(run("}](){") == "(){}");

    assert(run("aa)((aaaaa") == "(aaaaaaa)");
    assert(run("((aaa)((aa)") == "(aaa)");
    assert(run("((aaa)((aaa)") == "(aaa)");
    assert(run("aaaaaaa)") == "aaaaaaa");
    assert(run("(aaaaaaa") == "aaaaaaa");
    assert(run("((aa)(aa)") == "(aa)(aa)");
    assert(run("(aa(aa)a(aa)a))") == "(aa(aa)a(aa)a)");
    assert(run("((aa(aa)a(aa)a)") == "(aa(aa)a(aa)a)");

    assert(run("(aa)aa(aa))") == "(aa)aa(aa)");
    assert(run("(aa(aa)aa(aa)))") == "(aa(aa)aa(aa))");
    // run("]h({hhh(b})b)[");
    // run("]h({h(bb})b)[");
    // run("]h({hdd(b})b)[");

    assert(run("((aa)((aa)") == "(aa)");
    assert(run("((aa)(aa)((aa)") == "(aa)(aa)");
    assert(run("(aa))(aa))") == "(aa)");
    assert(run("(aba))(aa))") == "(aba)");
    assert(run("(aa)(aa))") == "(aa)(aa)");
    assert(run("((a{a})))") == "((a{a}))");

    assert(run("aabbcc)") == "aabbcc");
    assert(run("(aabbcc") == "aabbcc");
    assert(run("sh(dh)}") == "sh(dh)");

    assert(run("[[b[aa]]") == "[b[aa]]");
    assert(run("[[b[aa]]]]") == "[[b[aa]]]");
    assert(run("[[b[aa]]}") == "[b[aa]]");
    // assert(run("]h({hhh(b})b)[") == "hhh");
    // assert(run("]h({h(bb})b)[") == "[]h");
    // assert(run("]h({hdd(b})b)[") == "hdd");
    return 0;
}