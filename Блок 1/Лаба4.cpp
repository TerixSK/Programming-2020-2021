#include <stdio.h>
#include <math.h>

int main(void)
{
    // ������� 1
    int left_v = 33;
    int right_v = 88;
    int x;
    scanf_s("%d", &x);
    int result = left_v <= x && x <= right_v; //���������� ������� �������� �
    printf("%d\n", result); // ��������� ������� 1 ���� ����� ���������� � ����� ���������

    // ������� 2
    int z;
    scanf_s("%d", &z);
    z = z >> 19;   // ������� ������� �� 20 ����
    printf("%d", z);

    return 0;
}