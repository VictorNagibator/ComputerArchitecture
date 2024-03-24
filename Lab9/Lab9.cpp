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

	cout << "������� n: ";
	cin >> n;

	cout << "������� m: ";
	cin >> m;

	cout << "������� a: ";
	cin >> a;

	//��������� ������ ��� ������
	arr = new int* [n];
	for (int i = 0; i < n; i++)
		arr[i] = new int[m];

	//���������� ������� ���������� �������
	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			arr[i][j] = (-16384) + rand() % (32768);
		}
	}

	//����� �������
	//for (int i = 0; i < n; i++) {
	//	for (int j = 0; j < m; j++) {
	//		cout << format("{} ", arr[i][j]);
	//	}
	//	cout << "\n";
	//}


	//������� ��������� � ������� ������
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
		cout << "\n������������ ����� ������ �� ����� ����������, �������� a: " << maxRow << endl;
	}
	else {
		cout << "\n������ �� ����� ����������, �������� a, ���!" << endl;
	}
	cout << "\n����� �������� ���������: " << endTime - startTime << endl;


	//������� ��������� � ������������� ������
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
		cout << "\n������������ ����� ������ �� ����� ����������, �������� a: " << maxRow << endl;
	}
	else {
		cout << "\n������ �� ����� ����������, �������� a, ���!" << endl;
	}
	cout << "\n����� ��������� � �������������� OpenMP: " << endTime - startTime << endl;


	//������������ ������ �� �������
	for (int i = 0; i < n; i++)
		delete[] arr[i];
	delete[] arr;

	return 0;
}
