#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cfloat>

#define db double
#define row vector < db >
#define eps 0.0001

using namespace std;

int i, j, k, m;

db sq_root(db x)
{
    db l = 0.0, r = 1.0 + x;
    while (abs(r - l) > 1e-13)
    {
        db m = (r + l) / 2.0;
        if (m * m > x) r = m;
        else l = m;
    }
    return l;
}

struct Matrix
{
    int size;
    vector < row > V; // values
};

Matrix createMatrix(int N)
{
    Matrix tmpM;
    tmpM.size = N;
    row tmp_v(N);

    for (i = 0; i < N; ++i)
        tmpM.V.push_back(tmp_v);

    return tmpM;
}

Matrix fillMatrix(int N, db a)
{
    Matrix tmpM = createMatrix(N);

    for (i = 0; i < N; ++i)
        for (j = 0; j < N; ++j)
            if (i < j) tmpM.V[i][j] = a + 1;
            else if (i == N - 1 && j == 0) tmpM.V[i][j] = a - 1;
            else tmpM.V[i][j] = a;

    return tmpM;
}

Matrix fillX(int N, db a)
{
    Matrix tmpM = createMatrix(N);
    for (i = 1; i < N; ++i)
    {
        tmpM.V[i][i] = -1;
        tmpM.V[i][i - 1] = 1;
    }
    tmpM.V[0][0] = a; tmpM.V[0][N - 1] = -(a + 1);
    tmpM.V[N - 1][0] = -a; tmpM.V[N - 1][N - 1] = a;

    return tmpM;
}

db check_difference(Matrix &M1, Matrix &M2)
{
    db diff = 0;
    int N = M1.size;
    for (i = 0; i < N; ++i)
        for (j = 0; j < N; ++j)
            diff += abs(M1.V[i][j] - M2.V[i][j]);
    return diff;
}

void printMatrix(Matrix M)
{
    int N = M.size;
    for (i = 0; i < N; ++i)
    {
        for (j = 0; j < N; ++j)
            cout << setw(16) << M.V[i][j];
        cout << endl;
    }
    cout << endl;
}

void LU_decomposition(Matrix &A, Matrix &L, Matrix &U)
{
    int N = A.size;
    for (i = 0; i < N; ++i)
        for (j = i; j < N; ++j)
        {
            db S1 = 0, S2 = 0;

            for (k = 0; k < i; ++k)
                S1 += L.V[i][k] * U.V[k][j];

            U.V[i][j] = A.V[i][j] - S1;

            for (k = 0; k < i; ++k)
                S2 += L.V[j][k] * U.V[k][i];

            L.V[j][i] = (A.V[j][i] - S2) / U.V[i][i];
        }
}

void solve_via_LU(Matrix &L, Matrix &U, Matrix &X, Matrix &Y)
{
    int N = L.size;
    for (j = 0; j < N; ++j)
    {
        for (i = 0; i < N; ++i)
        {
            db Sy = 0;

            for (k = 0; k < i; ++k)
                Sy += L.V[i][k] * Y.V[k][j];

            Y.V[i][j] = -Sy;
            if (i == j) Y.V[i][j] += 1;
        }
        for (i = N - 1; i >= 0; --i)
        {
            db Sx = 0;

            for (k = i + 1; k < N; ++k)
                Sx += U.V[i][k] * X.V[k][j];

            X.V[i][j] = (Y.V[i][j] - Sx)/U.V[i][i];
        }
    }
}

void QR_decomposition(Matrix &Q, Matrix &R)
{
    int N = R.size;
    for (i = 0; i < N - 1; ++i)
    {
        int h = N - i;
        db R_in = 0;
        for (j = i + 1; j < N; ++j)
            R_in += R.V[j][i] * R.V[j][i];

        if (R_in > eps)
        {
            R_in += R.V[i][i] * R.V[i][i];

            // computing u
            row u(h); db u_norm = 0;

            if (R.V[i][i] >= 0)
//                u[0] = R.V[i][i] - sqrt(R_in);
  //          else u[0] = R.V[i][i] + sqrt(R_in);
                           u[0] = R.V[i][i] - sq_root(R_in);
                      else u[0] = R.V[i][i] + sq_root(R_in);

            u_norm += u[0] * u[0];

            for (j = i + 1; j < N; ++j)
            {
                u[j - i] = R.V[j][i];
                u_norm += u[j - i] * u[j - i];
            }
            // ok with u

            // multiplying by a reflection matrix H
            for (m = i; m < N; ++m)
            {
                db scalar_product = 0;
                for (j = i; j < N; ++j)
                   scalar_product += u[j - i] * R.V[j][m];
                for (j = i; j < N; ++j)
                    R.V[j][m] -= 2.0 * u[j - i] * scalar_product / u_norm;
            }

            for (m = 0; m < N; ++m)
            {
                db scalar_product = 0;
                for (j = i; j < N; ++j)
                   scalar_product += u[j - i] * Q.V[j][m];
                for (j = i; j < N; ++j)
                    Q.V[j][m] -= 2.0 * u[j - i] * scalar_product / u_norm;
            }
        }
    }
    for (i = 0; i < N; ++i)
        for (j = i + 1; j < N; ++j)
            swap(Q.V[i][j], Q.V[j][i]);
}

void solve_via_QR(Matrix &Q, Matrix &R, Matrix &X, Matrix &Y)
{
    int N = Q.size;
    for (j = 0; j < N; ++j)
        for (i = 0; i < N; ++i)
            Y.V[i][j] = Q.V[j][i];

    for (j = 0; j < N; ++j)
    {
        for (i = N - 1; i >= 0; --i)
        {
            db Sx = 0;

            for (k = i + 1; k < N; ++k)
                Sx += R.V[i][k] * X.V[k][j];

            X.V[i][j] = (Y.V[i][j] - Sx) / R.V[i][i];
        }
    }
}

int main()
{
    int N = 0; db a;
    cout << "Input N and a (N > 0, a != 0)." << endl;
    cout << fixed << setprecision(10);
    cin >> N >> a;
    a = 10;
    for (N = 4; N < 513; N *= 2)
    {
        Matrix A = fillMatrix(N, a);
        Matrix X1 = createMatrix(N), Y1 = createMatrix(N), L = createMatrix(N), U = createMatrix(N);
        Matrix X2 = createMatrix(N), Y2 = createMatrix(N), Q = createMatrix(N), R = A;
        for (i = 0; i < N; ++i) Q.V[i][i] = 1;

        LU_decomposition(A, L, U); solve_via_LU(L, U, X1, Y1);
        QR_decomposition(Q, R); solve_via_QR(Q, R, X2, Y2);
     //   printMatrix(A);
     //   printMatrix(Q);
     //   printMatrix(R);
     //   printMatrix(X1);
     //   printMatrix(X2);

        Matrix Right = fillX(N, a);
        db n1 = check_difference(Right, X1), n2 = check_difference(Right, X2);
        cout << fixed << setprecision(14);
        cout << n1 << "     " << n2 << endl;

    }


    return 0;
}