// Main steps:
// 1. if `s` starts with one of the closed brackets -> find last of its continuous sequence
// 2. main tracked vars: 
// - result: final answer
// - current: input substring, tracked in the current moment
// - stack: aux structure for valid brackets tracking
// 3. Opened brackets push to `current` and `stack`, a-z push to `current` only, when meet closed brackets than:
// - if stack empty: bring longest string
// - if closed bracket not valid via opened one -> find out `current` valid substring and bring longest one
// - else (closed bracket == stack top): just push to current

#include <cassert>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <iostream>

using namespace std;

const string kInfinite{"Infinite"};
const unordered_map<char, char> kDict = {
    {'{', '}'},
    {'(', ')'},
    {'[', ']'}
};
const unordered_set<char> kValues{'}', ')', ']'};

void process_current(string& result, string& current, stack<char>& st) {
    size_t idx{0};
    size_t wrong_idx{0};
    const auto stack_size = st.size();
    while (idx < current.size() && wrong_idx < stack_size) {
        if (kDict.count(current[idx])) {
            ++wrong_idx;
            ++idx;
            st.pop();
        } else {
            ++idx;
        }
    }
    if (current.size()-idx > result.size()) {
        result = current.substr(idx);
    }
    cout << result << ", " << idx << ", " << current << endl;
    current.clear();
}

string process(const string& input) {
    string result;
    string current; 
    stack<char> st;
    for (const auto ch: input) {
        if (kDict.count(ch)) {
            st.push(ch);
            current += ch;
        } else if (kValues.count(ch)) {
            if (st.empty()) {
                if (current.size() > result.size()) {
                    result = current;
                }
                current.clear();
                continue;
            }

            const auto top = st.top();
            if (kDict.at(top) != ch) {
                process_current(result, current, st);
            } else {
                current += ch;
                st.pop();
            }
        } else {
            current += ch;
        }
    }
    if (!current.empty()) {
        process_current(result, current, st);
    }
    return result;
}

bool check_infinite(const std::string& input) {
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

string run(const string& input) {
    if (check_infinite(input)) {
        return kInfinite;
    }
    
    const string double_input = input + input;
    const string result = process(double_input);
    cout << "result: " << result << endl;
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

    // assert(run("]})") == "");
    // assert(run("())") == "()");
    // assert(run("((aa)") == "(aa)");
    // assert(run("[[]") == "[]");
    // assert(run("[[])") == "[]");
    // assert(run("}](){") == "(){}");
    // assert(run("aabbcc)") == "aabbcc");
    // assert(run("(aabbcc") == "aabbcc");
    // assert(run("sh(dh)}") == "sh(dh)");

    // assert(run("[[b[aa]]") == "[b[aa]]");
    // assert(run("[[b[aa]]]]") == "[[b[aa]]]");
    // assert(run("[[b[aa]]}") == "[b[aa]]");
    // assert(run("]h({hh(b})b)[") == "hh");
    // assert(run("]h({h(bb})b)[") == "[]h");
    // assert(run("]h({hdd(b})b)[") == "hdd");
    return 0;
}