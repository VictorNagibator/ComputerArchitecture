#include <mpi.h>
#include <iostream>
#include <format>

using namespace std;

int main(int *argc, char** argv) {
    int** arr;
    int n, m;
    int maxRow = -1;
    int a;
    double startTime, endTime;

    MPI_Init(argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        cout << "Input n: ";
        cin >> n;

        cout << "Input m: ";
        cin >> m;

        cout << "Input a: ";
        cin >> a;
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&a, 1, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    //выделение памяти под массив
    arr = new int* [n];
    for (int i = 0; i < n; i++)
        arr[i] = new int[m];

    //заполнение массива случайными числами
    srand(time(NULL) + rank);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            arr[i][j] = (-16384) + rand() % (32768);
        }
    }

    //вывод массива
    //if (rank == 0) {
    //    for (int i = 0; i < n; i++) {
    //       for (int j = 0; j < m; j++) {
    //            cout << format("{:6d} ", arr[i][j]);
    //       }
    //       cout << "\n";
    //    }
    //}

    int localMaxRow = -1;
    //синхронизация процессов перед самим алгоритмом
    MPI_Barrier(MPI_COMM_WORLD);

    startTime = MPI_Wtime();

    for (int i = rank; i < n; i += size) {
        bool divisible = true;
        for (int j = 0; j < m; ++j) {
            if (arr[i][j] % a != 0) {
                divisible = false;
                break;
            }
        }

        if (divisible) {
            localMaxRow = i;
        }
    }

    endTime = MPI_Wtime();

    int globalMaxRow;
    //с помощью редукции выбираем строку с максимальным номером
    MPI_Reduce(&localMaxRow, &globalMaxRow, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        if (globalMaxRow == -1) {
            cout << endl << "No row is divisible by " << a << endl;
        }
        else {
            cout << endl << "Max row divisible by " << a << " found at index " << globalMaxRow << endl;
        }

        cout << "Time: " << endTime - startTime << endl;
    }

    //освобождение памяти от массива
    for (int i = 0; i < n; i++)
        delete[] arr[i];
    delete[] arr;

    MPI_Finalize();
    return 0;
}
