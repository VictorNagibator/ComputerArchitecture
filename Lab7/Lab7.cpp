#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"

int main()
{
	//задание 1
	//Y = a - b + (c * d)^2
    int a, b, c, d, y;

    printf("Input A, B, C, D: ");
    scanf("%d %d %d %d", &a, &b, &c, &d);
    __asm 
    {
		mov eax, c
		imul eax, d
		imul eax
		add eax, a
		sub eax, b
		mov y, eax
    }
    printf("Result: %d\n\n", y);

	//задание 2
	//Y = A / B + C * A, если B <> 0; Y = A * A + C, если B = 0
	printf("Input A, B, C: ");
	scanf("%d %d %d", &a, &b, &c);
	__asm
	{
		mov eax, a
		cmp b, 0
		jnz notzero
		imul eax
		add eax, c
		jmp end
		notzero: cdq
		idiv b
		mov ebx, eax
		mov eax, c
		imul a
		add eax, ebx
		end: mov y, eax
	}
	printf("Result: %d", y);

	//задание 3
	//Y = X^N / 2^N
	int x, n, y;

	//с предусловием
	printf("Input X, N: ");
	scanf("%d %d", &x, &n);
	__asm
	{
		mov ecx, n
		mov eax, 1
		mov ebx, 1
		cycle: cmp ecx, 0
		jle end
		imul eax, x
		imul ebx, 2
		dec cx
		jmp cycle
		end: cdq
		idiv ebx
		mov y, eax
	}
	printf("Result: %d", y);

	//с постусловием
	printf("Input X, N: ");
	scanf("%d %d", &x, &n);
	__asm
	{
		mov ecx, n
		mov eax, 1
		mov ebx, 1
		cmp ecx, 0
		jle end
		cycle: imul eax, x
		imul ebx, 2
		dec cx
		cmp ecx, 0
		ja cycle
		end: cdq
		idiv ebx
		mov y, eax
	}
	printf("Result: %d", y);

	//цикл с счетчиком
	printf("Input X, N: ");
	scanf("%d %d", &x, &n);
	__asm
	{
		mov ecx, n
		mov eax, 1
		mov ebx, 1
		cycle: imul eax, x
		imul ebx, 2
		loop cycle
		cdq
		idiv ebx
		mov y, eax
	}
	printf("Result: %d", y);

	return 0;
}