#include <iostream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

string eps = "eps";

uint32_t find_operation(string expr, char x) {
    int diff = 0;
    uint32_t ind = 0;
    for (auto &c : expr) {
        if (c == '(') ++diff;
        if (c == ')') --diff;
        if (diff == 0 && c == x)
            return ind;
        ++ind;
    }
    return ind;
}

class Edge {
public:
    Edge(string expr_, uint32_t from_, uint32_t to_) {
        from = from_; to = to_; expr = expr_;
        if (expr_[0] == '(' && expr_[expr_.size() - 1] == ')') {
            string expr1 = expr_.substr(1, expr_.size() - 2);

            bool correct = true;
            int diff = 0;
            for (auto &c : expr1) {
                if (c == '(') ++diff;
                if (c == ')') --diff;
                if (diff < 0)
                    correct = false;
            }
            correct = diff == 0;

            if (correct) expr = expr1;
        }
        if (expr.empty()) expr = eps;
    }

    uint32_t from, to;
    string expr;
};

queue < Edge > q;
vector < Edge > final;
uint32_t last = 1;

void process() {
    Edge e = q.front();
    uint32_t
            found_or   = find_operation(e.expr, '|'),
            found_and  = find_operation(e.expr, ' '),
            found_star = find_operation(e.expr, '*');

    if (found_or < e.expr.size()) {
        string s_l = e.expr.substr(0, found_or);
        string s_r = e.expr.substr(1 + found_or, e.expr.size() - found_or - 1);
        Edge e_l(s_l, e.from, e.to), e_r(s_r, e.from, e.to);
        q.push(e_l); q.push(e_r);
    }
    else if (found_and < e.expr.size()) {
        string s_l = e.expr.substr(0, found_and);
        string s_r = e.expr.substr(1 + found_and, e.expr.size() - found_and - 1);
        ++last;
        Edge e_l(s_l, e.from, last), e_r(s_r, last, e.to);
        q.push(e_l); q.push(e_r);
    }
    else if (found_star < e.expr.size()) {
        string s_m = e.expr.substr(0, found_star);
        ++last;
        Edge e_l(eps, e.from, last), e_m(s_m, last, last), e_r(eps, last, e.to);
        q.push(e_l); q.push(e_m); q.push(e_r);
    } else
        final.push_back(e);
    q.pop();
}

int main() {

    string re;
    getline(cin, re);
    q.push(Edge(re, 0, last));
    while (!q.empty())
        process();

    cout << "q0 is a start, q1 is a finish" << endl;
    for (auto & e : final)
        cout << "q" << e.from << " " << e.expr << " -> q" << e.to << endl;

    return 0;
}