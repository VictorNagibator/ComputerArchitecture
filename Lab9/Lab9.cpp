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

	//���������� �������
	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			arr[i][j] = (-16384) + rand() % (32768);
		}
	}

	printf("������������ ���������� �������: %d\n\n", omp_get_max_threads());
	printf("�������� ������ %d �� %d:\n", n, m);
	//����� �������
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			printf("%6d ", arr[i][j]);
		}
		printf("\n");
	}

	int a;
	printf("������� ����� a: ");
	do
	{
		scanf("%d", &a);
		if (a == 0) printf("����� a ������ ���� �� ����� 0. ��������� ����: ");
	} while (a == 0);

	//������� ��������� � ������� ������
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

	printf("\n������������ ����� ������ � ����������, �������� ����� a: %d\n", maxRow);
	printf("����������� ����� ��������� (��� OpenMP) � �� = %f\n\n", omp_get_wtime() - startTime);


	//������� ��������� � ������������� ������
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

	printf("������������ ����� ������ � ����������, �������� ����� a: %d\n", maxRow);
	printf("����� ��������� � �������������� �������� ����������������� � �� = %f\n", omp_get_wtime() - startTime);
	
	return 0;
}
