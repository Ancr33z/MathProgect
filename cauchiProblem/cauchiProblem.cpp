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

int i, N, h = 1, t, j, x0 = 0, q = 1;


//------------------------------------------------Функция мэйн начало-------------------------------------------------------------
int main()
{
    setlocale(LC_ALL, "rus");

    cin >> N; // Ввод размеров верхней границы интеграла 



    F = new double[N]; // Массив для подсчёта Ф 
    R = new double[N]; // Массив для подсчёта R 
    Y = new double[N]; // Массив для подсчёта Y

    //----------------------------- Нахождение Ф функции с записью в массив
    long double sumForF = NULL;

    F[0] = -x0 * q + f(0);

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

    long double sumForR = NULL;

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
         //cout << '\n' << F[j] << ' ';
    }

    //----------------------------- Нахождение приближенного результата 

    cout << "Result:" << endl;
    //for (i = 0; i < N; i++) {
    //    cout << R[i] << '\n';
    //}
    //cout << '\n';
    //for (i = 0; i < N; i++) {
    //    cout << F[i] << '\n';
    //}
    


    //cout << '\n';
    for (i = 0; i < N; i++) {
        printf("%.7f\n", Y[i]);
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
    long double sumForYApprox = 0, CloseY = 0;

    if (n == 1) {
        for (i = 0; i < limiеОfApproximation; i++)
        {
            sumForYApprox = 0;

            if (i == 0) {
                CloseY = R[0] * Y[0] + F[0];
            }
            else {
                for (j = 0; j < n; j++)
                    if (j == 0)
                        sumForYApprox += (Y[0] * R[1]) / 2;
                    //else if (j == n - 1) {  // это условние не выполнено ни разу 
                    //    sumForYApprox += (R[1] * CloseY) / 2;
                    //}
                    else {
                        sumForYApprox += Y[n - j];
                    }
                CloseY = j * sumForYApprox + F[n];
                // CloseY работает неправильно потому что насчитывает себя рекурсивно из
                // за чего нужно добавить новую переменную и присваивать ей значение предыдущей функции
            }
        }
    }
    else {
        for (i = 0; i < limiеОfApproximation; i++)
        {
            sumForYApprox = 0;
            if (i == 0) {
                for (j = 0; j < n; j++)
                    if (j == 0)
                        sumForYApprox += (Y[n - 1] * R[j]) / 2;
                    else{
                        sumForYApprox += Y[n - j];
                    }
                CloseY = j * sumForYApprox + n * R[1] * Y[n - j] + F[n];


            }
            else {
                sumForYApprox = NULL;
                for (j = 0; j < n; j++) {

                    //    if (j == 0)
                    //        sumForYApprox += (Y[n - 1] * R[j]) / 2;
                    //    else if (j == n) {
                    //        sumForYApprox += (R[0] * CloseY) / 2;
                    //        break;
                    //    }else {
                    //        sumForYApprox += Y[n - j];
                    //    }
                    //CloseY = j * sumForYApprox + F[j];

                    if (sumForYApprox == NULL) {
                        sumForYApprox += ((Y[1] * R[0]) / 2 + (Y[0] * R[1]) / 2 );

                    }
                    else {
                        sumForYApprox += ((Y[n - j] * R[j - 1]) / 2 + (Y[n - j - 1] * R[j]) / 2 );
                        //cout << ((Y[n - j] * R[j - 1]) / 2 + (Y[n - j - 1] * R[j]) / 2) << " ";

                    }
                }
                CloseY = sumForYApprox + F[n];
            }
        }
    }
    return CloseY;
};


//---------------------------------------------------OLD VERSION---------------------------------------------

//function main()
//% Запрос ввода размера верхней границы интеграла
//N = input('Введите размеры верхней границы интеграла: ');
//
//% Инициализация массивов
//F = zeros(1, N);
//R = zeros(1, N);
//Y = zeros(1, N);
//
//% Константы
//h = 1;
//x0 = 0;
//q = 1;
//limitOfApproximation = 4;
//
//% Нахождение Ф функции с записью в массив
//sumForF = NaN;
//F(1) = -x0 * q + f(0);
//
//for j = 2:N
//if isnan(sumForF)
//sumForF = k1(0) / 2 + k1(1) / 2;
//F(2) = -x0 * q + f(1) + x0 * h * h * sumForF;
//else
//sumForF = sumForF + k1(j - 1) / 2 + k1(j - 2) / 2;
//F(j) = -x0 * q + f(j - 1) + x0 * h * h * sumForF;
//end
//end
//
//% Нахождение Ядра с записью в массив
//sumForR = NaN;
//R(1) = -q;
//R(2) = h / 2 * (k1(1) + k1(0)) - q;
//
//for j = 3:N
//if isnan(sumForR)
//sumForR = (k1(0) / 2) + k1(1) + (k1(2) / 2);
//R(j) = h * sumForR - q;
//else
//sumForR = sumForR + k1(j - 1) / 2 + k1(j - 2) / 2;
//R(j) = h * sumForR - q;
//end
//end
//
//% Нахождение приближенного значения
//Y(1) = F(1);
//for j = 2:N
//Y(j) = ApproxValue(j, Y, F, R, limitOfApproximation) + F(j);
//end
//
//% Вывод результатов
//disp('Result:');
//for i = 1:N
//fprintf('%.7f\n', Y(i));
//end
//
//% Построение графика
//figure;
//plot(1:N, Y, '-o');
//title('Значения Y');
//xlabel('Индекс');
//ylabel('Y');
//grid on;
//end
//
//function val = f(t)
//val = cos(t) / 2 - sin(t) / 2 - exp(-t) / 2;
//end
//
//function val = k1(n)
//val = sin(n);
//end
//
//function CloseY = ApproxValue(n, Y, F, R, limitOfApproximation)
//sumForYApprox = 0;
//CloseY = 0;
//
//if n == 2
//for i = 1:limitOfApproximation
//sumForYApprox = 0;
//
//if i == 1
//CloseY = R(1) * Y(1) + F(1);
//else
//for j = 1:n - 1
//if j == 1
//sumForYApprox = sumForYApprox + (Y(1) * R(2)) / 2;
//else
//sumForYApprox = sumForYApprox + Y(n - j + 1);
//end
//end
//CloseY = sumForYApprox + F(n);
//end
//end
//else
//for i = 1:limitOfApproximation
//sumForYApprox = 0;
//if i == 1
//for j = 1:n - 1
//if j == 1
//sumForYApprox = sumForYApprox + (Y(n - 1) * R(j)) / 2;
//else
//sumForYApprox = sumForYApprox + Y(n - j + 1);
//end
//end
//CloseY = sumForYApprox + n * R(2) * Y(n - 1) + F(n);
//else
//sumForYApprox = NaN;
//for j = 1:n - 1
//if isnan(sumForYApprox)
//sumForYApprox = ((Y(2) * R(1)) / 2 + (Y(1) * R(2)) / 2);
//else
//sumForYApprox = sumForYApprox + ((Y(n - j + 1) * R(j - 1)) / 2 + (Y(n - j) * R(j)) / 2);
//end
//end
//CloseY = sumForYApprox + F(n);
//end
//end
//end
//end
//
//% Запуск основной функции
//main();



//-------------------------------- LAST VERSION -----------------------------------

//function main()
//% Запрос ввода размера верхней границы интеграла
//T = input('Введите размеры верхней границы интеграла: ');
//N = input('Количество тчек разбиения: ');
//
//% Инициализация массивов
//F = zeros(1, N);
//R = zeros(1, N);
//Y = zeros(1, N);
//
//% Константы
//h = T / N;
//x0 = 0;
//q = 1;
//limitOfApproximation = 4;
//
//% Нахождение Ф функции с записью в массив
//sumForF = NaN;
//F(1) = -x0 * q + f(0);
//
//for j = 2:N
//if isnan(sumForF)
//sumForF = k1(0) / 2 + k1(h) / 2;
//F(2) = -x0 * q + f(h) + x0 * h * h * sumForF;
//else
//sumForF = sumForF + k1((j - 1) * h) / 2 + k1((j - 2) * h) / 2;
//F(j) = -x0 * q + f((j - 1) * h) + x0 * h * h * sumForF;
//end
//end
//
//% Нахождение Ядра с записью в массив
//sumForR = NaN;
//R(1) = -q;
//R(2) = h / 2 * (k1(h) + k1(0)) - q;
//
//for j = 3:N
//if isnan(sumForR)
//sumForR = (k1(0) / 2) + k1(h) + (k1(2 * h) / 2);
//R(j) = h * sumForR - q;
//else
//sumForR = sumForR + k1((j - 1) * h) / 2 + k1((j - 2) * h) / 2;
//R(j) = h * sumForR - q;
//end
//end
//
//% Нахождение приближенного значения
//Y(1) = F(1);
//for j = 2:N
//Y(j) = ApproxValue(j, Y, F, R, limitOfApproximation) + F(j);
//end
//
//% Вывод результатов
//disp('Result:');
//for i = 1:N
//fprintf('%.7f\n', Y(i));
//end
//
//% Построение графика
//figure;
//plot(1:N, Y, '-b');
//title('Значения Y');
//xlabel('Тао');
//ylabel('Y');
//grid on;
//end
//
//function val = f(t)
//val = cos(t) / 2 - sin(t) / 2 - exp(-t) / 2;
//end
//
//function val = k1(n)
//val = sin(n);
//end
//
//function CloseY = ApproxValue(n, Y, F, R, limitOfApproximation)
//sumForYApprox = 0;
//CloseY = 0;
//
//if n == 2
//for i = 1:limitOfApproximation
//sumForYApprox = 0;
//
//if i == 1
//CloseY = R(1) * Y(1) + F(1);
//else
//for j = 1:n - 1
//if j == 1
//sumForYApprox = sumForYApprox + (Y(1) * R(2)) / 2;
//else
//sumForYApprox = sumForYApprox + Y(n - j + 1);
//end
//end
//CloseY = sumForYApprox + F(n);
//end
//end
//else
//for i = 1:limitOfApproximation
//sumForYApprox = 0;
//if i == 1
//for j = 1:n - 1
//if j == 1
//sumForYApprox = sumForYApprox + (Y(n - 1) * R(j)) / 2;
//else
//sumForYApprox = sumForYApprox + Y(n - j + 1);
//end
//end
//CloseY = sumForYApprox + n * R(2) * Y(n - 1) + F(n);
//else
//sumForYApprox = NaN;
//for j = 1:n - 1
//if isnan(sumForYApprox)
//sumForYApprox = ((Y(2) * R(1)) / 2 + (Y(1) * R(2)) / 2);
//else
//sumForYApprox = sumForYApprox + ((Y(n - j + 1) * R(j - 1)) / 2 + (Y(n - j) * R(j)) / 2);
//end
//end
//CloseY = sumForYApprox + F(n);
//end
//end
//end
//end
//
//% Запуск основной функции
//main();