#include <iostream>
#include <fstream>
#define c(x) for (x = 0; x < 3; ++x)
#define a 6
#define b 7
#define m_itP(i) if (p[i] == 0) P[i] = a; else if (p[i] == 1) P[i] = b; else P[i] = 0;
#define m_itQ(i) if (q[i] == 0) Q[i] = a; else if (q[i] == 1) Q[i] = b; else Q[i] = 0;

using namespace std;
ofstream fout("res2.txt");

int s(int x)
{
    if (x == a)
        return b;
    else if (x == b)
        return a;
    else
        return x+1;
}

int sum[5][5]    = {{-1, -1,  a, -1, -1},
                    {-1, -1,  b, -1, -1},
                    { b, -1,  0,  1,  2},
                    {-1, -1,  1,  2,  3},
                    {-1, -1,  2,  3,  4}};

int mult[5][5]    = {{-1, -1,  0, -1, -1},
                     {-1, -1,  0, -1, -1},
                     {-1, -1,  0,  0,  0},
                     {-1, -1,  0,  1,  2},
                     {-1, -1,  0,  2,  4}};

bool try_fill()
{
    bool ok = 1;
    for (int x = 0; x < 4; ++x)
    {
        int x_t, y_t, sy, xy;
        if (x == 0) x_t = a;
        else if (x == 1) x_t = b;
        else x_t = x - 2;
        // "настоящее значение x, а не табличное"

        for (int y = 0; y < 4; ++y)
        {
            if (y == 0)
            {
                y_t = a;
                sy = 1;
            }
            else if (y == 1)
            {
                y_t = b;
                sy = 0;
            }
            else
            {
                y_t = y - 2;
                sy = y_t + 1;
            }

            // есть табличные и настоящие значения
             if (sy < 4)
             {
                  if ( sum[x][sy] != s(sum[x][y]) ) ok = 0; // Q5
             }


            if (mult[x][y] == a)
                xy = 0;
            else if (mult[x][y] == b)
                xy = 1;
            else
                xy = mult[x][y] + 2;

            if (sy < 4 && xy < 4)
            {
                if ( mult[x][sy] != sum[xy][x] ) ok = 0; // Q7
            }


        }
    }

    return ok;
}

void check()
{
    for (int x = 0; x < 4; ++x)
    {
        int x_t, y_t, sy, xy;
        if (x == 0) x_t = a;
        else if (x == 1) x_t = b;
        else x_t = x - 2;
        // "настоящее значение x, а не табличное"

        for (int y = 0; y < 4; ++y)
        {
            if (y == 0)
            {
                y_t = a;
                sy = 1;
            }
            else if (y == 1)
            {
                y_t = b;
                sy = 0;
            }
            else
            {
                y_t = y - 2;
                sy = y+1;
            }

            // есть табличные и настоящие значения

            fout << x << " " << y << "     ";
             if (sy < 4)
             {
                  //if ( sum[x][sy] != s(sum[x][y]) ) ok = 0; // Q5
                 fout << "x + s(y) = " << sum[x][sy] << "   ";
                 fout << "s(x + y) = " << s(sum[x][y]) << "   ";
             }


            if (mult[x][y] == a)
                xy = 0;
            else if (mult[x][y] == b)
                xy = 1;
            else
                xy = mult[x][y] + 2;

            if (sy < 4 && xy < 4)
            {
                fout << "x * s(y) = " << mult[x][sy] << "   ";
                fout << "x * y + x = " << sum[xy][x] << "   ";
            }
            fout << endl;
        }
    }
}


int main()
{

    int count = 0;
    int p[13], q[14]; // iterators
    double P[13], Q[14]; // iterators
    int Gl1, Gl2;
    c(p[0]) c(p[1]) c(p[2]) c(p[3]) c(p[4]) c(p[5]) c(p[6]) c(p[7]) c(p[8])// c(p[9]) c(p[10]) c(p[11]) c(p[12])
    c(q[0]) c(q[1]) c(q[2]) c(q[3]) c(q[4]) c(q[5]) c(q[6]) c(q[7]) c(q[8]) c(q[9])// c(q[10]) c(q[11]) c(q[12]) c(q[13])
    {
        for (Gl1 = 0; Gl1 < 9; ++Gl1) m_itP(Gl1);
        for (Gl2 = 0; Gl2 < 10; ++Gl2) m_itQ(Gl2);
        sum[0][0] = P[0]; sum[0][1] = P[1]; sum[0][3] = P[2];
        sum[1][0] = P[3]; sum[1][1] = P[4]; sum[1][3] = P[5];
        sum[2][1] = P[6];
        sum[3][0] = P[7]; sum[3][1] = P[8];

     //   sum[0][4] = P[9]; sum[1][4] = P[10];
     //   sum[4][0] = P[11]; sum[4][1] = P[12];


        mult[0][0] = Q[0]; mult[0][1] = Q[1]; mult[0][3] = Q[2];
        mult[1][0] = Q[3]; mult[1][1] = Q[4]; mult[1][3] = Q[5];
        mult[2][0] = Q[6]; mult[2][1] = Q[7];
        mult[3][0] = Q[8]; mult[3][1] = Q[9];

      //  mult[0][4] = Q[10]; mult[1][4] = Q[11];
      //  mult[4][0] = Q[12]; mult[4][1] = Q[13];

        //таблицы заполнены
        cout << p[0] << p[1] << p[2] << p[3] << p[4] << p[5] << p[6] << p[7] << p[8] << endl;
        bool now = try_fill();
        if (now)
        {
            ++count;
            fout << "Sum table:" << endl;
            for (int i = 0; i < 5; ++i)
            {
                for (int j = 0; j < 5; ++j)
                    if (sum[i][j] == a)
                       fout << 'a' << "   ";
                else if (sum[i][j] == b)
                        fout << 'b' << "   ";
                else fout << sum[i][j] << "   ";
                fout << endl;
            }

            fout << endl;

            fout << "Mult table:" << endl;
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                    if (mult[i][j] == a)
                       fout << 'a' << "   ";
                else if (mult[i][j] == b)
                        fout << 'b' << "   ";
                else fout << mult[i][j] << "   ";
                fout << endl;
            }

            check();


            fout << "---------------------------------------" << endl;

        }
    }

    fout << count << endl;

    return 0;
}