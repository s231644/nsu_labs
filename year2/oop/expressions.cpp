#include <iostream>
#include <string>
#include <stack>
#include <cctype>

using namespace std;

class Expression
{
public:

    virtual Expression *diff() = 0;
    virtual void print() = 0;
};

class Number: public Expression
{
public:
    int num;
    Number(int n)
    {
        num = n;
    }
    Expression *diff()
    {
        return new Number(0);
    }
    void print()
    {
        cout << num;
    }

};

class Variable: public Expression
{
public:
    char var;
    Variable(char v)
    {
        var = v;
    }
    Expression *diff()
    {
        return new Number(1);
    }
    void print()
    {
        cout << var;
    }
};

class Add: public Expression
{
public:
    Expression *e1, *e2;
    Add(Expression *E1, Expression *E2): e1(E1), e2(E2) {}
    Expression *diff()
    {
        return new Add(e1->diff(),e2->diff());
    }
    void print()
    {
        cout << "("; e1->print(); cout << "+"; e2->print(); cout << ")";
    }
};

class Sub: public Expression
{
public:
    Expression *e1, *e2;
    Sub(Expression *E1, Expression *E2): e1(E1), e2(E2) {}
    Expression *diff()
    {
        return new Sub(e1->diff(),e2->diff());
    }
    void print()
    {
        cout << "("; e1->print(); cout << "-"; e2->print(); cout << ")";
    }
};

class Mult: public Expression
{
public:
    Expression *e1, *e2;
    Mult(Expression *E1, Expression *E2): e1(E1), e2(E2) {}
    Expression *diff()
    {
        return new Add(new Mult(e1->diff(),e2), new Mult(e1, e2->diff()));
    }
    void print()
    {
        cout << "("; e1->print(); cout << "*"; e2->print(); cout << ")";
    }
};

class Res: public Expression
{
public:
    Expression *e1, *e2;
    Res(Expression *E1, Expression *E2): e1(E1), e2(E2) {}
    Expression *diff()
    {
        return new Res(new Sub(new Mult(e1->diff(),e2), new Mult(e1, e2->diff())), new Mult(e2, e2));
    }
    void print()
    {
        cout << "("; e1->print(); cout << "/"; e2->print(); cout << ")";
    }
};

bool issign(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/')
        return 1;
    else return 0;
}

Expression *read_expression(string s)
{
    Expression *e1, *e2, *res;
    stack < Expression *> S;
    stack < char > signs;
    for (int i = 0; i < s.size(); ++i)
    {
        if (s[i] == '(')
            continue;

        else if (isalpha(s[i]))
            S.push(new Variable(s[i]));

        else if (isdigit(s[i]))
            S.push(new Number((int)s[i]-48));
        else if (issign(s[i]))
            signs.push(s[i]);

        else // (s[i] == ')')
        {
            e2 = S.top(); S.pop();
            e1 = S.top(); S.pop();
            if (signs.top() == '+')
                res = new Add(e1, e2);
            else if (signs.top() == '-')
                res = new Sub(e1, e2);
            else if (signs.top() == '*')
                res = new Mult(e1, e2);
            else if (signs.top() == '/')
                res = new Res(e1, e2);
            signs.pop();
            S.push(res);
        }
    }
    return res;
}

int main()
{
    int k = 9;
    Number *e = new Number(k);
    Variable *x = new Variable('s');
    e->print(); cout << endl;
    Expression *f = e->diff();
    f->print(); cout << endl;
    x->print(); cout << endl;

    Expression *add = new Add(f, x);
    Expression *mult = new Mult(add, x);
    add->print(); cout << endl;
    mult->print(); cout << endl;

    Expression *mult2 = mult->diff();
    mult2->print(); cout << endl;

    string to_read = "((x+1)*(x+(x/3)))";
   // cin >> to_read;
    Expression *stroka = read_expression(to_read);

    return 0;
}