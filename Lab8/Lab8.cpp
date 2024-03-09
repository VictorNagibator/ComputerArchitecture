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

    printf("�������� ������:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%3d ", arr[i][j]);
        }
        printf("\n");
    }

    signed short int a;
    printf("������� ����� a: ");
    do
    {
        scanf("%hd", &a);
        if (a <= 0) printf("����� a ������ ���� ������ 0. ��������� ����: ");
    } while (a <= 0);

    signed short int remainders[n][4];
    int maxRow;
    __asm {
        //������� ��������� ������ ��������
        mov ecx, n ; ������� �����
        lea esi, arr ; ��������� �� arr
        lea edi, remainders ; ��������� �� remainders
        calculateRemainders : push ecx ; ���������� �������� �����
        mov ecx, 4 ; ���������� ��������� � ������
        calculateNextElement : mov ax, word ptr[esi]; �������� �������� ������� arr[i][j] � ������� AX
        cwd; �������������� AX � DX:AX
        idiv word ptr[a] ; ������� �������� �� ����� a
        mov word ptr[edi], dx ; ���������� ������� � ������� remainders[i][j]
        add esi, 2 ; ������� � ���������� �������� ������� arr
        add edi, 2 ; ������� � ���������� �������� ������� remainders
        loop calculateNextElement ; ��������� ��� ���� ��������� ������
        pop ecx ; �������������� �������� �����
        loop calculateRemainders ; ��������� ��� ���� �����, ���� ��� ����

        //������ ���� ������������ ����� ������
        mov ecx, n ; ������� �����
        rowcycl : movq mm0, qword ptr[remainders + ecx * 8] ; �������� ������ remainders[i] � ������� MM0
        pxor mm1, mm1 ; ��������� �������� MM1
        pcmpeqw mm1, mm0 ; ��������� ��������� ������ �� ������� �����
        pmovmskb eax, mm1 ; ���������� �������� ����� � �������� EAX
        cmp eax, 0xFF ; ��������, ��� ��� �������� ������ ����� ���� (��� ���� �������� ����� ����� 1)
        je end ; ���� ������ ������� ������ �� ���������, ������� ����� a, �� �������
        loop rowcycl ; ��������� ��� ���� �����
        end : mov maxRow, ecx ; ���������� ������ ������
        emms ; ������� ��������� MMX
    }

    if (maxRow) printf("������������ ����� ������, ��������� ������ �� ���������, ������� ����� %d: %d\n", a, maxRow);
    else printf("� ������� ��� ������, ��������� ������ �� ���������, ������� ����� %d\n", a);

    return 0;
}
