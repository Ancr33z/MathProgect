#include <iostream>
#include <algorithm>

using namespace std;

void mexFunction()
{}

double f(int t)
{
    return cos(t) / 2 - sin(t) / 2 - exp(-t) / 2;
};

double k1(int n)
{
    return sin(n);
};

const unsigned short limiеОfApproximation = 4;

long double ApproxValue(int h);

double* F = new double; // Массив для подсчёта Ф 
double* R = new double; // Массив для подсчёта R 
double* Y = new double; // Массив для подсчёта Y


//------------------------------------------------Функция мэйн начало-------------------------------------------------------------
int main()
{
    setlocale(LC_ALL, "rus");

    int i, N, h = 1, t, j, x0 = 0, q = 1;
    cin >> N; // Ввод размеров верхней границы интеграла 


    F = new double[N]; // Массив для подсчёта Ф 
    R = new double[N]; // Массив для подсчёта R 
    Y = new double[N]; // Массив для подсчёта Y

    //----------------------------- Нахождение Ф функции с записью в массив
    long double sumForF = NULL;

    for (j = 1; j < N; j++) {
        if (sumForF == NULL) {
            sumForF = k1(0) / 2 + k1(1) / 2;
            F[1] = -x0 * q + f(1) + x0 * h * h * sumForF;
            
        }
        sumForF += k1(j) / 2 + k1(j - 1) / 2;
        F[j] = -x0 * q + f(j) + x0 * h * h * sumForF;
    }

    //-----------------------------

    //----------------------------- Нахождение Ядра с записью в массив

    double sumForR = 0;

    R[0] = -q;
    R[1] = h / 2 * (k1(1) + k1(0)) - q;

    for (j = 2; j <= N; j++) {
        if (sumForR == NULL) {
            sumForR = (k1(0) / 2) + k1(1) + (k1(2) / 2);
            R[j] = h * sumForR - q;
            continue;
        }
        else {
            sumForR += k1(j) / 2 + k1(j - 1) / 2;
            R[j] = h * sumForR - q;
        }
    }
    //-----------------------------

    //----------------------------- Нахождение приближенного значения

    Y[0] = F[0];
    for (j = 1; j < N; j++)
    {
        Y[j] = ApproxValue(j) + F[j];
    }




    //----------------------------- Нахождение приближенного результата 

    cout << "Result:" << endl;

    for (i = 0; i < N; i++) {
        cout << Y[i] << ' ';
    }

    system("pause");

    return 0;

    delete[] F;
}
//------------------------------------------------Функция мэйн конец-------------------------------------------------------------

//Y[0] = F[0];
//for (j = 1; j < N; j++)
//{
//    Y[j] = ApproxValue(j) + F[j];
//}

long double ApproxValue(int n)
{
    long double sumForYApprox = 0, CloseY;
    int i, j;

    if (n == 1) {
        for (i = 0; i < limiеОfApproximation; i++)
        {

            if (i == 0) {
                CloseY = R[0] * Y[0] + F[n];
            }
            else {
                for (j = 0; j < n; j++)
                    if (j == 0)
                        sumForYApprox += (Y[0] * R[1]) / 2;
                    else if (j == n) {
                        sumForYApprox += (R[0] * CloseY) / 2;
                        break;
                    }
                    else
                        sumForYApprox += Y[j - i];
                CloseY = j * sumForYApprox + F[j];
            }
        }
    }
    else {
        for (i = 0; i < limiеОfApproximation; i++)
        {

            if (i == 0) {
                for (j = 0; j < n; j++)
                    if (j == 0)
                        sumForYApprox += (Y[n - j] * R[j]) / 2;
                    else
                        sumForYApprox += Y[j - i];
                CloseY = j * sumForYApprox + n * R[1] * Y[n - j] + F[j];
            }
            else {
                for (j = 0; j < n; j++)
                    if (j == 0)
                        sumForYApprox += (Y[n - j] * R[j]) / 2;
                    else if (j == n) {
                        sumForYApprox += (R[0] * CloseY) / 2;
                        break;
                    }
                    else
                        sumForYApprox += Y[j - i];
                CloseY = j * sumForYApprox + F[j];
            }
        }
    }
    return CloseY;
};