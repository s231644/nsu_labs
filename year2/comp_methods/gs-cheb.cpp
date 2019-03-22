#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>
#include <cassert>
#include <ctime>

#define USE_MATH_DEFINES
#define db double
#define row vector < db >
#define mp make_pair
#define cycle(it, from, to) for (it = from; it < to; ++it)

using namespace std;

const db eps = 100;
const long long MAX_TIME_LIMIT = 1e9;
const long long MAX_ITER_LIMIT = 1e8 / 4;
long long iterGS, iterCh;

int i, j, k, m, N;
int l = 14; // Chebyshev's parameter, we need p = 2^l-size tree
int p = 1 << l;
bool found = 0;
db M_lower, M_upper;
long long Binomial[2010][2010];
db A[1000][1000];
vector < int > Theta (p << 1);
vector < db > t (p << 1), tao (p << 1);
vector < vector < int > > Tree(p << 1);

void fillBin()
{
    cycle(i, 0, 2 * N + 1)
        Binomial[0][i] = Binomial[i][0] = 1;
    cycle(i, 1, 2 * N + 1) cycle(j, 1, 2 * N + 1)
        Binomial[i][j] = Binomial[i - 1][j] + Binomial[i][j - 1];
}

long long lambda(int ind)
{
    long long ans = Binomial[k][N - 1] * Binomial[k][N + k];
    return (ind % 2)? -ans: ans;
}

void fillTree(int levels)
{
    cycle(j, 1, levels + 1)
    {
        int size_of_tree = 1 << (j - 1); // before this
        cycle(i, 0, size_of_tree)
        {
            int tmp = (size_of_tree << 1) - 1 - i;
            Tree[i].push_back(tmp);
        }
    }
}

void DFS(int v_cur, int &step)
{
    Theta[step] = (v_cur << 1) + 1;
    t[step] = cos((Theta[step] * M_PI) / (2.0 * p));
    tao[step] = 2.0 / ((M_upper + M_lower) - (M_upper - M_lower) * t[step]);

    ++step;
    int i1;
    cycle(i1, 0, Tree[v_cur].size())
    {
        int child = Tree[v_cur][Tree[v_cur].size() - 1 - i1];
        DFS(child, step);
    }
}

void fillMatrix()
{
    cycle(i, 0, N) cycle(j, 0, N)
        A[i][j] = 1.0 / (i + j + 1);
}

void printMatrix()
{
    cycle(i, 0, N) {
        cycle(j, 0, N)
            cout << setw(8) << A[i][j];
        cout << endl;
    }
}

void real_fight(row &b)
{
    cycle(i, 0, N)
        b[i] = (i % 2)? -sqrt(i + 1): sqrt(i + 1);
}

void transform(row &b)
{
    db b_new = (N % 2)? 1.0 * b[0] / N: -1.0 * b[0] / N;
    cycle(i, 1, N)
        b_new += lambda(i) * b[i];
    b[0] = b_new;
}

void GS(row &right_part, row &x)
{
    row x_new = x;
    cycle(i, 0, N) {
        db S = right_part[i];
        cycle(j, 0, N) if (j != i)
            S -= A[i][j] * x_new[j];
        x_new[i] = S / A[i][i];
    }
    x = x_new;
    ++iterGS;
}

void Cheb(row &right_part, row &x, int k)
{
    row x_new = x;
    cycle(i, 0, N)
    {
        db S = right_part[i];
        cycle(j, 0, N)
            S -= A[i][j] * x[j];
        x_new[i] += tao[k] * S;
        assert(abs(x_new[i]) < 1e15);
    }
    x = x_new;
}

db residual(row &right_part, row &x)
{
    db ans = 0;
    cycle(i, 0, N)
    {
        db S = -right_part[i];
        cycle(j, 0, N)
            S += A[i][j] * x[j];
        ans += abs(S);
    }
    return ans;

}

int main()
{
    cout << "Input N (N > 1)." << endl;
    cin >> N;
    stringstream ss, ssL;
    ss << N;
    string file_name = "Ch_" + ss.str();
    ssL << l; file_name += "_l=" + ssL.str();
    file_name += ".txt";

    ofstream fout (file_name);

    fillMatrix();
    fillBin();
    row b(N), x_GS(N); real_fight(b);
    transform(b);
    db res; long long border = 10000;

    fout << fixed << setprecision(10);

/*

    // Gauss–Seidel method

    long long t_start = clock();

    while (!found)
    {
        GS(b, x_GS);
        res = residual(b, x_GS);
        if (iterGS < 10000 || (iterGS % border == 0))
        fout << iterGS << "   " << res << endl;
        if (iterGS >= 100 * border && iterGS < 1e8) border *= 100;
        if (res < eps || iterGS >= MAX_ITER_LIMIT) found = 1;
    }
    long long t_current = clock();
    fout << endl <<  "epsilon:  " << eps << endl;
    fout << "time(sec):   " << (db)(t_current - t_start) / CLOCKS_PER_SEC << endl;
    fout << "residual:   " << res << endl;
    fout << "iterations:  " << iterGS << endl;
  //  cout << "G-S: " << setw(20) << check_diff(b, x_GS) << endl;
//    cycle(i, 0, N)
//        cout << setw(20) << x_GS[i] << endl;
*/


    // Chebyshev method

    M_lower = 0; M_upper = 0;
    cycle(i, 0, N) M_upper += A[i][i];
    fillTree(l);
    int STEP = 0;
    DFS(0, STEP);
    row x_Ch(N); found = 0;
    long long t_start = clock();

    while (!found)
    {
        for (k = 0; k < p; ++k)
        {
            ++iterCh;
            Cheb(b, x_Ch, k);
            res = residual(b, x_Ch);
            if (iterCh < 10000 || (iterCh % border == 0))
            fout << iterCh << "   " << res << endl;
            if (iterCh >= 100 * border && iterCh < 1e8) border *= 100;
            if (res < eps || iterCh >= MAX_ITER_LIMIT) found = 1;
        }

    }
    long long t_current = clock();
    fout << endl <<  "epsilon:  " << eps << endl;
    fout << "time(sec):   " << (db)(t_current - t_start) / CLOCKS_PER_SEC << endl;
    fout << "residual:   " << res << endl;
    fout << "iterations:  " << iterCh << endl;
    cout << "iterations:  " << iterCh << endl;
//    cycle(i, 0, N)
//        cout << setw(20) << x_Ch[i] << endl;
//   cout << "Che: " << setw(20) << check_diff(b, x_Ch) << endl;
//   cout << "min_diff: " << DIFF << endl;

   // printMatrix();
   // cout << Matrix_norm(2);

    fout.close();
    return 0;
}