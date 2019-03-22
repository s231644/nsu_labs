#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <deque>
#include <queue>
#include <string>
#include <utility>
#include <sstream>

#define left_cycle for (int i = 0; i < q.now_parsing.size(); ++i)
#define right_cycle for (int i = q.now_parsing.size() - 1; i >= 0; --i)

using namespace std;

typedef string symbol;
typedef symbol left_part;
typedef deque < symbol > seq;
typedef pair < seq, int > right_part;

struct query {
public:
    seq to_parse, now_parsing;
    string cur_states;
    int length = 0;

    query(seq to_p, seq now_p, string cur_st, int len) {
        to_parse = to_p, now_parsing = now_p, cur_states = cur_st, length = len;
    }

    query(const query &q) {
        to_parse = q.to_parse, now_parsing = q.now_parsing, cur_states = q.cur_states, length = q.length;
    }

    query() = default;
};

typedef deque < query > Queue;

map < left_part, set < right_part > > Dict;

seq split(string const &text)
{
    istringstream is(text);

    string word;
    seq vs;
    while (is >> word) vs.push_back(word);

    return vs;
}

void add_to_dict(string const &rule, int i) {
    left_part L;
    right_part R;
    seq spl = split(rule);

    L = spl.front();
    spl.pop_front();
    R = make_pair(spl, i);

    Dict[L].insert(R);
}

void BFS(Queue &Q, int type, vector < string > &ans) {
    query q(Q.front());
    Q.pop_front();

    if (q.length > 20 || !ans[type].empty())
        return;

    if (!q.to_parse.empty() && !q.now_parsing.empty() && q.to_parse.front() == q.now_parsing.front()) {
        query q_new(q);
        q_new.to_parse.pop_front(), q_new.now_parsing.pop_front();
        Q.push_front(q_new);
    }

    if (q.to_parse.empty()) {
        if (q.now_parsing.empty()) {
            ans[type] = q.cur_states;
            return;
        }
        else
            return;
    }
    if (q.now_parsing.empty())
        return;

    if (Dict.find(q.now_parsing[0]) == Dict.end() && q.now_parsing[0] != q.to_parse[0])
        return;

    if (!type) {
        left_cycle {
            if (Dict.find(q.now_parsing[i]) != Dict.end()) {
                for (auto &elem : Dict[q.now_parsing[i]]) {
                    seq new_parsing = elem.first;
                    for (int j = i - 1; j >= 0; --j)
                        new_parsing.push_front(q.now_parsing[j]);
                    for (int j = i + 1; j < q.now_parsing.size(); ++j)
                        new_parsing.push_back(q.now_parsing[j]);
                    query new_q;
                    new_q.to_parse = q.to_parse;
                    new_q.now_parsing = new_parsing;
                    new_q.cur_states = q.cur_states +  " " + to_string(elem.second);
                    new_q.length = q.length + 1;
                    Q.push_back(new_q);
                }
                break;
            }
        }

    } else {
        right_cycle {
            if (Dict.find(q.now_parsing[i]) != Dict.end()) {
                for (auto &elem : Dict[q.now_parsing[i]]) {
                    seq new_parsing = elem.first;
                    for (int j = i - 1; j >= 0; --j)
                        new_parsing.push_front(q.now_parsing[j]);
                    for (int j = i + 1; j < q.now_parsing.size(); ++j)
                        new_parsing.push_back(q.now_parsing[j]);
                    query new_q;
                    new_q.to_parse = q.to_parse;
                    new_q.now_parsing = new_parsing;
                    new_q.cur_states = q.cur_states +  " " + to_string(elem.second);
                    new_q.length = q.length + 1;
                    Q.push_back(new_q);
                }
                break;
            }
        }
    }
}


int main() {
    int n, m;
    cout << "Input the number of rules." << endl;
    cin >> n;

    string s;
    seq S;
    getline(cin, s);
    for (int i = 0; i < n; ++i) {
        cout << "Input the rule # " << i + 1 << endl;
        // A b* (все символы отделяются пробелами)
        getline(cin, s);
        add_to_dict(s, i + 1);
    }

    cout << "Input S." << endl;
    getline(cin, s);
    S = split(s);

    cout << "Input the number of queries." << endl;
    cin >> m;
    getline(cin, s);
    for (int i = 0; i < m; ++i) {
        cout << "Input the query # " << i + 1 << endl;
        getline(cin, s);

        vector < string > ans(2);
        query top(split(s), S, "", 0);
        Queue Q_left, Q_right;
        Q_left.push_back(top), Q_right.push_back(top);
        while (!Q_left.empty()) {
            BFS(Q_left, 0, ans);
        }

        while (!Q_right.empty()) {
            BFS(Q_right, 1, ans);
        }

        cout << ans[0] << " & " << ans[1] << endl;
    }

    return 0;
}