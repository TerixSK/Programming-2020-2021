#include <stdio.h>
#include <string.h>
#include <locale.h>
int main()
{
    setlocale(LC_ALL, "Ru");
    char str1[256];
    char str2[256];
    gets_s(str1);   
    gets_s(str2);
    printf("\n");
    printf("������ ������: %s\n", str1);
    printf("������ ������: %s\n\n", str2);
    //������� 3
    printf("������� 3\n");
    int x = 0;
    // �������� ���� �������� ����� ������ ������ ��� ��� �����
    for (int i = 0, j = 0; str1[i] != '\0' && str2[j] != '\0' && x == 0; i++, j++)
    {
        if (str1[i] > str2[j])
        {
            x++;
        }
        else if (str1[i] < str2[j])
        {
            x--;
        }
    }
    if (x > 0) { printf("������ ������ ������ ������\n\n"); }
    if (x < 0) { printf("������ ������ ������ ������\n\n"); }
    if (x == 0) { printf("������ �����\n\n"); }
    //������� 5
    printf("������� 5\n");
    char str[256];
    strcpy_s(str, str1);
    strcpy_s(str1, str2); // �������� ������ �� ������ ������ �� ������
    printf("str1 = %s\n\n", str1);
    strcpy_s(str1, str); // ����������� �� ��������� ������ � ������ ��� ����������� ������������ �������� ������

    //������� 6
    printf("������� 6\n");
    int n = 6;
    int k = 0; /// �������
    while (str1[k] != '\0') { k++; } /// ����������� ������ �� ������� ����� ������ '\0'
    for (int i = 0; i < n; i++)
    {
        str1[k++] = str2[i]; /// ����������� � ����� 1-� ������ ������ 6 �������� 2-� ������
    }
    str1[k] = '\0';
    printf("str1 = %s\n\n", str1);
    //������� 10
    printf("������� 10\n");
    // ����� ���������� �������� � �������
    int f = 0;
    for (int i = 0; str1[i] != '\0' && !f; i++)
    {
        for (int j = 0; str2[j] != '\0' && !f; j++)
        {
            if (str1[i] == str2[j])
            {
                printf("Character: %c\n\n", str1[i]);
                f = 1;
            }
        }
    }

    //������� 13
    // �������� �� ����� ������ �������, ����������� ����� �� ��������� �������� (������������), �������� �� ������ ������.
    printf("������� 13\n");
    printf("%s", strtok(str1, str2));
    return 0;
}