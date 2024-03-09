#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctime>
#include <windows.h>

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const int n = 4;
    signed short int arr[n][4];
    
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 4; j++) {
            arr[i][j] = -128 + rand() % 256;
        }
    }

    printf("Исходный массив:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%3d ", arr[i][j]);
        }
        printf("\n");
    }

    signed short int a;
    printf("Введите число a: ");
    do
    {
        scanf("%hd", &a);
        if (a <= 0) printf("Число a должно быть больше 0. Повторите ввод: ");
    } while (a <= 0);

    signed short int remainders[n][4];
    int maxRow;
    __asm {
        //сначала формируем массив остатков
        mov ecx, n ; Счетчик строк
        lea esi, arr ; Указатель на arr
        lea edi, remainders ; Указатель на remainders
        calculateRemainders : push ecx ; Сохранение счетчика строк
        mov ecx, 4 ; Количество элементов в строке
        calculateNextElement : mov ax, word ptr[esi]; Загрузка элемента массива arr[i][j] в регистр AX
        cwd; Преобразование AX в DX:AX
        idiv word ptr[a] ; Деление элемента на число a
        mov word ptr[edi], dx ; Сохранение остатка в массиве remainders[i][j]
        add esi, 2 ; Переход к следующему элементу массива arr
        add edi, 2 ; Переход к следующему элементу массива remainders
        loop calculateNextElement ; Повторить для всех элементов строки
        pop ecx ; Восстановление счетчика строк
        loop calculateRemainders ; Повторить для всех строк, если еще есть

        //теперь ищем максимальный номер строки
        mov ecx, n ; Счетчик строк
        rowcycl : movq mm0, qword ptr[remainders + ecx * 8] ; Загрузка строки remainders[i] в регистр MM0
        pxor mm1, mm1 ; Обнуление регистра MM1
        pcmpeqw mm1, mm0 ; Сравнение элементов строки со строкой нулей
        pmovmskb eax, mm1 ; Сохранение знаковых битов в регистре EAX
        cmp eax, 0xFF ; Проверка, что все элементы строки равны нулю (все биты младшего байта равны 1)
        je end ; Если строка состоит только из элементов, кратных числу a, то выходим
        loop rowcycl ; Повторить для всех строк
        end : mov maxRow, ecx ; Сохранение номера строки
        emms ; Очистка регистров MMX
    }

    if (maxRow) printf("Максимальный номер строки, состоящей только из элементов, кратных числу %d: %d\n", a, maxRow);
    else printf("В массиве нет строки, состоящей только из элементов, кратных числу %d\n", a);

    return 0;
}
