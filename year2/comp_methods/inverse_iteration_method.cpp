#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>

#define USE_MATH_DEFINES
#define db double
#define row vector < db >
#define matrix vector < row >
#define cycle(it, from, to) for (it = from; it < to; ++it)
#define pb push_back
using namespace std;

const db eps = 1e-12;

int i, j, k, N;
int iter;
matrix A, B, E, Q, R;
row lambdas, rights, SINs, COSs, start;
db cur_shift;
int finding;

void fillMatrices()
{
    row tmp(N);
    cycle(i, 0, N)
        A.pb(tmp), B.pb(tmp), Q.pb(tmp), R.pb(tmp);
    cycle(i, 0, N) cycle(j, 0, N)
        A[i][j] = N - max(i,j), B[i][j] = N - max(i,j);
}

void fillstart()
{
    cycle(i, 0, N)
        SINs[i] = sin(i), COSs[i] = cos(i);
    cycle(i, 0, N)
    {
        start[i] = COSs[i];
        cycle(j, i + 1, N) start[i] *= SINs[j];
    }
}

void fillQR()
{
    cycle(i, 0, N) cycle(j, 0, N) R[i][j] = B[i][j], Q[i][j] = (i == j);
}

void printMatrix(matrix &M)
{
    cycle(i, 0, M.size())
    {
        cycle(j, 0, M[i].size())
            cout << setw(12) << M[i][j];
        cout << endl;
    }
}

void QR_decomposition()
{
    fillQR();
    cycle(i, 0, N - 1)
    {
        db R_in = 0;
        cycle(j, i + 1, N) R_in += R[j][i] * R[j][i];

        if (R_in > eps)
        {
            R_in += R[i][i] * R[i][i];
            row u(N - i); db u_norm = 0;

            u[0] = (R[i][i] >= 0)? R[i][i] + sqrt(R_in) : R[i][i] - sqrt(R_in);
            u_norm += u[0] * u[0];
            cycle(j, i + 1, N)
            {
                u[j - i] = R[j][i]; u_norm += u[j - i] * u[j - i];
            }

            cycle(k, i, N)
            {
                db scalar_product = 0;
                cycle(j, i, N) scalar_product += u[j - i] * R[j][k];
                cycle(j, i, N) R[j][k] -= 2.0 * u[j - i] * scalar_product / u_norm;
            }
            cycle(k, 0, N)
            {
                db scalar_product = 0;
                cycle(j, i, N) scalar_product += u[j - i] * Q[j][k];
                cycle(j, i, N) Q[j][k] -= 2.0 * u[j - i] * scalar_product / u_norm;
            }
        }
    }
    cycle(i, 0, N) cycle(j, i + 1, N) swap(Q[i][j], Q[j][i]);
}

row solve_with_QR(row &b)
{
    row z(N), x(N);
    cycle(i, 0, N) cycle(j, 0, N) z[i] += Q[j][i] * b[j];
    cycle(i, 0, N)
    {
        x[N - 1 - i] = z[N - 1 - i];
        cycle(k, 0, i) x[N - 1 - i] -= R[N - 1 - i][N - 1 - k] * x[N - 1 - k];
        x[N - 1 - i] /= R[N - 1 - i][N - 1 - i];
    }
    return x;
}

void shift(db s) // to the left
{
    cycle(i, 0, N) B[i][i] = A[i][i] - s;
    cur_shift = s;
}

db sc_prod(row &v, row &w)
{
    db ans = 0;
    cycle (i, 0, N) ans += v[i] * w[i];
    return ans;
}

row normalize(row &v)
{
    db norm = sqrt(sc_prod(v, v));
    row ans(N);
    cycle (i, 0, N) ans[i] = v[i] / norm;
    return ans;
}

bool is_found(db x)
{
    bool ans = 0;
    cycle(i, 0, finding) if (abs(lambdas[i] - x) < 1e-7) ans = 1;
    return ans;
}

pair < db, row > eigenVV()
{
    int count = 0;
    row x_k(N), x_k1(N), y_k1(N);
    x_k = start;
    db la_old = -1, la_new = 0;
    QR_decomposition();
    while (abs(la_new - la_old)  >=  eps)
    {
        la_old = la_new;
        y_k1 = solve_with_QR(x_k);
        la_new = sc_prod(x_k, y_k1) / sc_prod(y_k1, y_k1);
        x_k1 = normalize(y_k1);
        x_k = x_k1;
        ++count;
    }
    return make_pair(la_new, x_k);
}

void search_lambdas(db l, db r)
{
    db m = (sqrt(2) * l + r) / (sqrt(2) + 1);
    shift(m);
    pair < db, row > result = eigenVV();
    db la = result.first + cur_shift;
    if (!is_found(la))
    {
        lambdas.pb(la); E.pb(result.second);
        cout << setw(3) << ++finding << "  E-value: " << setw(16) << la << "   [" << setw(14) << l << "; " << setw(14) << r << "]   " << endl;
        search_lambdas(l, min(la, m));
        search_lambdas(max(la, m), r);
    }
}


int main()
{
    cout << "Input N (N > 1)." << endl;
    cin >> N; assert(N > 1);

    rights.resize(N); start.resize(N); SINs.resize(N); COSs.resize(N);
    cycle(i, 0, N) rights[i] = 1.0 / (2 * (1.0 - cos(((2 * i + 1) * M_PI) / (2 * N + 1))));
    fillstart(); fillMatrices();
    cout << fixed << setprecision(9);

    search_lambdas(0, (N + 1.0) * N / 2.0);
  //  printMatrix(E);

    cycle(i, 0, lambdas.size())
    {
        bool sear = 0;
        cycle(j, 0, N) if (abs(lambdas[j] - rights[i]) < 1e-8) sear = 1;
        cout << ((sear)? "+" : "-");
    }

    cout << endl;
    return 0;
}