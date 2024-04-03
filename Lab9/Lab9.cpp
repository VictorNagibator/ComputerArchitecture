#include <iostream>
#include <format>
#include <windows.h>
#include <omp_llvm.h>

using namespace std;

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	int** arr;
	int n, m;
	int maxRow = -1;
	int a;
	double startTime, endTime;

	cout << "Введите n: ";
	cin >> n;

	cout << "Введите m: ";
	cin >> m;

	cout << "Введите a: ";
	cin >> a;

	//выделение памяти под массив
	arr = new int* [n];
	for (int i = 0; i < n; i++)
		arr[i] = new int[m];

	//заполнение массива случайными числами
	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			arr[i][j] = (-16384) + rand() % (32768);
		}
	}

	//вывод массива
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << format("{:6d} ", arr[i][j]);
		}
		cout << "\n";
	}

	//вариант алгоритма в обычном режиме
	startTime = omp_get_wtime();
	for (int i = n - 1; i >= 0; i--)
	{
		bool isMultiple = true;
		for (int j = 0; j < m; j++)
		{
			if (arr[i][j] % a != 0)
			{
				isMultiple = false;
				break;
			}
		}

		if (isMultiple)
		{
			maxRow = i;
			break;
		}
	}
	
	endTime = omp_get_wtime();

	if (maxRow != -1) {
		cout << "\nМаксимальный номер строки со всеми элементами, кратными a: " << maxRow << endl;
	}
	else {
		cout << "\nСтроки со всеми элементами, кратными a, нет!" << endl;
	}
	cout << "\nВремя обычного алгоритма: " << endTime - startTime << endl;


	//вариант алгоритма в многопоточном режиме
	maxRow = -1;
	omp_set_num_threads(6);
	startTime = omp_get_wtime();
	#pragma omp parallel for reduction(max : maxRow) 
	for (int i = 0; i < n; i++)
	{
		bool isMultiple = true;
		for (int j = 0; j < m; j++)
		{
			if (arr[i][j] % a != 0)
			{
				isMultiple = false;
				break;
			}
		}

		if (isMultiple)
		{
			maxRow = i;
		}
	}

	endTime = omp_get_wtime();

	if (maxRow != -1) {
		cout << "\nМаксимальный номер строки со всеми элементами, кратными a: " << maxRow << endl;
	}
	else {
		cout << "\nСтроки со всеми элементами, кратными a, нет!" << endl;
	}
	cout << "\nВремя алгоритма с использованием OpenMP: " << endTime - startTime << endl;


	//освобождение памяти от массива
	for (int i = 0; i < n; i++)
		delete[] arr[i];
	delete[] arr;

	return 0;
}
