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
using namespace std;

const db eps = 1e-13;

int i, j, N;
int iter;
matrix A, E;
row lambda, rights, norms;
vector < int > ind;
vector < bool > changed;

int sgn(db x)
{
    return (x >= 0) ? 1 : 0;
}

void fillMatrices()
{
    cycle(i, 0, N) cycle(j, 0, N)
        A[i].push_back(N - max(i,j)), E[i].push_back((i == j));
}

void printMatrix(matrix& M)
{
    cycle(i, 0, N)
    {
        cycle(j, 0, N)
            cout << setw(12) << M[i][j] / M[N-1][j];
        cout << endl;
    }
}

int maxind(int k)
{
    int m = (k == N - 1)? 0 : k + 1;
    cycle(i, 0, N)
        if (i != k && abs(A[k][i]) > abs(A[k][m])) m = i;
    return m;
}

int indmaxnorm()
{
    int m = 0;
    cycle(i, 0, N) if (norms[i] > norms[m]) m = i;
    return m;
}

void rotate(int k, int l)
{
    db s, c, theta, tau, t;
    row Lk(N), Ll(N); // Rows & Lines
    cycle (i, 0, N) Lk[i] = A[k][i], Ll[i] = A[l][i];

/*
    if (abs(Lk[k] - Ll[l]) < eps) theta = M_PI/4;
    else theta = atan(2.0 * Lk[l] / (Ll[l] - Lk[k])) / 2.0;
    c = cos(theta); s = sin(theta);
*/

    tau = (Lk[k] - Ll[l]) / (2.0 * Lk[l]);
    t = 1.0 / (tau + sgn(tau) * sqrt(tau * tau + 1));
    c = 1.0 / sqrt(t * t + 1); s = - t * c;
  //  cout << tau << " " << t << " " << c << " " << s << endl;


    // A_new = G * A_old * G^T
    A[k][k] = c * c * Lk[k] - 2 * s * c * Lk[l] + s * s * Ll[l];
    A[l][l] = s * s * Lk[k] + 2 * s * c * Lk[l] + c * c * Ll[l];
    A[k][l] = A[l][k] = (c * c - s * s) * Lk[l] + s * c * (Lk[k] - Ll[l]);
    cycle(i, 0, N) if (i != k && i != l)
    {
        db tk = A[i][k], tl = A[i][l];
        A[k][i] = A[i][k] = c * Lk[i] - s * Ll[i];
        A[l][i] = A[i][l] = s * Lk[i] + c * Ll[i];
        norms[i] += abs(A[i][k]) - abs(tk) + abs(A[l][k]) - abs(tl);
    }
    norms[k] = norms[l] = 0;
    cycle(j, 0, N)
    {
        if (j != k) norms[k] += abs(A[k][j]);
        if (j != l) norms[l] += abs(A[l][j]);
    }
    // E_new = E_old * G^T
    cycle(j, 0, N)
    {
        db ek = E[j][k], el = E[j][l];
        E[j][k] = c * ek - s * el;
        E[j][l] = s * ek + c * el;
    }
}


void Jacobi()
{
    int k, l, m;
    cycle (k, 0, N) ind[k] = maxind(k);
    cycle (i, 0, N) cycle (j, 0, N) if (i != j) norms[i] += abs(A[i][j]);
    do
    {
        ++iter;
        m = 0;
        cycle(k, 1, N - 1)
            if (abs(A[k][ind[k]]) > abs(A[m][ind[m]])) m = k;
        k = m; l = ind[m];

        rotate(k, l);
        ind[k] = maxind(k); ind[l] = maxind(l);
        m = indmaxnorm();
    } while (norms[m] > eps);
}

int main()
{
    cout << "Input N (N > 1)." << endl;
    cin >> N;
    A.resize(N); E.resize(N); ind.resize(N); lambda.resize(N); rights.resize(N); norms.resize(N);
    cycle(i, 0, N) rights[i] = 1.0 / (2 * (1.0 - cos(((2 * i + 1) * M_PI) / (2 * N + 1))));
    fillMatrices();
    Jacobi();

    cout << fixed << setprecision(6);

    cycle(i, 0, N) lambda[i] = A[i][i];
    cycle(i, 0, N)
    {
        cout << setw(12) << lambda[i];
        bool found = 0;
        cycle(j, 0, N) if (abs(rights[j] - lambda[i]) < 1e-10) found = 1;
        cout << ((found)? " +" : " -") << endl;
    }

  //  printMatrix(E);

    cout << endl << iter << endl;
    return 0;
}