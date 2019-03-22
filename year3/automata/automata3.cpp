#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <deque>
#include <queue>
#include <string>
#include <utility>
#include <sstream>

using namespace std;

typedef string symbol;
typedef symbol left_part;
typedef deque < symbol > seq;
typedef seq right_part;
typedef deque < seq > Queue;

map < left_part, set < right_part > > Dict;
map < left_part, set < right_part > > Alpha;
set < symbol > Sigma;


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
    R = spl;

    Dict[L].insert(R);
}

void create_alpha() {
    seq emp;
    Alpha["S"].insert(emp);
    for (auto &nt : Dict) {
        for (auto &rule : nt.second) {
            seq alpha = rule;
            while (!alpha.empty()) {
                symbol &top = alpha.front();
                alpha.pop_front();
                if (Sigma.find(top) == Sigma.end())
                    Alpha[top].insert(alpha);
            }
        }
    }
}

set < seq > FIRST(int k, const seq &s) {
    Queue q;
    q.push_back(s);
    set < seq > ans;
    while (!q.empty()) {
        seq &top = q.front();
        bool pure_sigma = true;
        int cur_k = 0;
        seq cur_seq;
        seq top_tmp = top;
        for (auto &c : top) {
            top_tmp.pop_front();
            ++cur_k;
            if (k < cur_k)
                break;
            if (Sigma.find(c) == Sigma.end()) {
                pure_sigma = false;
                for (auto &r : Dict[c]) {
                    seq new_seq = cur_seq;
                    for (auto &e : r)
                        new_seq.push_back(e);
                    for (auto &e : top_tmp)
                        new_seq.push_back(e);
                    q.push_back(new_seq);
                }
                break;
            }
            else
                cur_seq.push_back(c);
        }
        if (pure_sigma)
            ans.insert(cur_seq);
        q.pop_front();
    }
    return ans;
}

bool is_it_LL(int k) {
    bool ans = true;
    for (auto &nt : Dict) {
        if (Alpha.find(nt.first) != Alpha.end())
            for (auto &alpha : Alpha[nt.first]) {
                set < seq > FIRSTs;
                int sum_of_sizes = 0;
                if (!nt.second.empty())
                    for (auto &r : nt.second) {
                        seq s = r;
                        if (!alpha.empty())
                            for (auto &c : alpha)
                                s.push_back(c);
                        set<seq> f = FIRST(k, s);
                        sum_of_sizes += f.size();
                        for (auto &e : f)
                            FIRSTs.insert(e);
                    }
                if (FIRSTs.size() != sum_of_sizes)
                    ans = false;
            }
    }
    return ans;
}

int main() {
    int n, m, k;
    string s;

    cout << "Input the size of Sigma." << endl;
    cin >> m;

    cout << "Input the alphabet of Sigma." << endl;

    for (int i = 0; i < m; ++i) {
        cin >> s;
        Sigma.insert(s);
    }

    cout << "Input the number of rules." << endl;
    cin >> n;

    seq S;
    getline(cin, s);
    for (int i = 0; i < n; ++i) {
        cout << "Input the rule # " << i + 1 << endl;
        // A b* (все символы отделяются пробелами)
        getline(cin, s);
        add_to_dict(s, i + 1);
    }

    create_alpha();

    cout << "Input k." << endl;
    cin >> k;

    if (is_it_LL(k))
        cout << "YES";
    else cout << "NO";
    
    return 0;
}