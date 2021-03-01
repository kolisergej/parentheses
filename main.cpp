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

void process(const string& input, size_t start, size_t end, string& result, string& current, stack<char>& st) {
    for (size_t i = start; i < end; ++i) {
        const auto ch = input[i];
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
}

string run(const string& input) {
    size_t first_close_idx = 0;
    while (first_close_idx < input.size() && kValues.count(input[first_close_idx])) {
        ++first_close_idx;
    }
    string result;
    string current;
    stack<char> st;
    process(input, first_close_idx, input.size(), result, current, st);
    if (first_close_idx > 0) {
        process(input, 0, first_close_idx, result, current, st);
    }

    if (!current.empty()) {
        process_current(result, current, st);
    }

    result = (result.size() == input.size()) ? "Infinite" : result;
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
    // assert(run("(()") == "()");
    // assert(run("[[]") == "[]");
    // assert(run("[[])") == "[]");
    // assert(run("}](){") == "(){}");
    // assert(run("aabbcc)") == "aabbcc");
    // assert(run("(aabbcc") == "aabbcc");
    // assert(run("sh(dh)}") == "sh(dh)");

    // assert(run("[[b[aa]]") == "[b[aa]]");
    // assert(run("[[b[aa]]]]") == "[[b[aa]]]");
    // assert(run("[[b[aa]]}") == "[b[aa]]");
    assert(run("]h({h(b})b)[") == "[]");
    assert(run("]h({hdd(b})b)[") == "hdd");
    return 0;
}