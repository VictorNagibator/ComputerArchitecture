#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <omp_llvm.h>

int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	const int n = 10000, m = 10;
	int arr[n][m];
	double startTime;
	int maxRow = 0;

	//заполнение массива
	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			arr[i][j] = (-16384) + rand() % (32768);
		}
	}

	printf("Максимальное количество потоков: %d\n\n", omp_get_max_threads());
	printf("Исходный массив %d на %d:\n", n, m);
	//вывод массива
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			printf("%6d ", arr[i][j]);
		}
		printf("\n");
	}

	int a;
	printf("Введите число a: ");
	do
	{
		scanf("%d", &a);
		if (a == 0) printf("Число a должно быть не равно 0. Повторите ввод: ");
	} while (a == 0);

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

	printf("\nМаксимальный номер строки с элементами, кратными числу a: %d\n", maxRow);
	printf("Стандартное время алгоритма (без OpenMP) в мс = %f\n\n", omp_get_wtime() - startTime);


	//вариант алгоритма в многопоточном режиме
	omp_set_num_threads(12);
	maxRow = 0;
	startTime = omp_get_wtime();
	#pragma omp parallel for shared(maxRow)
	for (int i = 0; i < n; i++) {
		bool areAllElementsMultipleOfA = true;
		for (int j = 0; j < m; j++) {
			if (arr[i][j] % a != 0) {
				areAllElementsMultipleOfA = false;
				break;
			}
		}
		if (areAllElementsMultipleOfA) {
			#pragma omp critical
			{
				if (i > maxRow) {
					maxRow = i;
				}
			}
		}
	}

	printf("Максимальный номер строки с элементами, кратными числу a: %d\n", maxRow);
	printf("Время алгоритма с использованием директив распараллеливания в мс = %f\n", omp_get_wtime() - startTime);
	
	return 0;
}
