#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <omp.h>

int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    const int n = 10000, m = 10;
    int arr[n][m];
    int maxRow = -1;
    double startTime;
    int a;

    //заполнение массива
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            arr[i][j] = (-16384) + rand() % (32768);
        }
    }

    //вывод массива
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%6d ", arr[i][j]);
        }
        printf("\n");
    }

    printf("\nВведите число a: ");
    do {
        scanf("%d", &a);
        if (a == 0) printf("Число a должно быть не равно 0. Повторите ввод: ");
    } while (a == 0);

    printf("\nРазмеры массива: %d на %d:\n\n", n, m);

    //вариант алгоритма в обычном режиме
    startTime = omp_get_wtime();
    for (int i = 0; i < n; i++) {
        bool areAllElementsMultipleOfA = true;
        for (int j = 0; j < m; j++) {
            if (arr[i][j] % a != 0) {
                areAllElementsMultipleOfA = false;
                break;
            }
        }
        if (areAllElementsMultipleOfA) {
            maxRow = i;
        }
    }

    if (maxRow != -1) printf("Максимальный номер строки с элементами, кратными числу a: %d\n", maxRow);
	else printf("В массиве нет строк, в которых все элементы кратны числу a\n");
    printf("Стандартное время алгоритма (без OpenMP) в мс = %f\n\n", omp_get_wtime() - startTime);

    //вариант алгоритма в многопоточном режиме
    maxRow = -1;
    startTime = omp_get_wtime();
    #pragma omp parallel for reduction(max : maxRow)
    for (int i = 0; i < n; i++) {
        bool areAllElementsMultipleOfA = true;
        for (int j = 0; j < m; j++) {
            if (arr[i][j] % a != 0) {
                areAllElementsMultipleOfA = false;
                break;
            }
        }

        if (areAllElementsMultipleOfA) {
            maxRow = i;
        }
    }

    if (maxRow != -1) printf("Максимальный номер строки с элементами, кратными числу a: %d\n", maxRow);
    else printf("В массиве нет строк, в которых все элементы кратны числу a\n");
    printf("Время алгоритма с использованием директив распараллеливания (OpenMP) в мс = %f\n", omp_get_wtime() - startTime);

    return 0;
}