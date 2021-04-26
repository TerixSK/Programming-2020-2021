#include <stdio.h>
#include <locale.h>
#include <math.h>

enum System
{
    Symbian,
    Embedded_Linux,
    Windows_CE,
    VxWorks,
    QNX
};
struct circle
{
    int radius;
};
union Cardreader
{
    int value;
    struct
    {
        unsigned CardR : 1;
        unsigned SD : 1;
        unsigned CompactFlash : 1;
        unsigned MemoryStick : 1;
    };
};
int main()
{
    setlocale(LC_ALL, "Ru");
    // Задание 1    
    printf("Task 1\n");
    enum System s = Symbian;
    printf("%d\n", s);
    printf("\n");

    // Задание 2
    printf("Task 2\n");
    struct circle r;
    printf("Radius = ");
    scanf_s("%d", &r.radius);
    double pi = 3.141592;
    printf("S = %lf\n\n", pi * pow(r.radius, 2));

    // Задание 3   
    printf("Task 3\n");
    union Cardreader cr; /// Объявление переменной типа объединения
    printf("Введите число в 16-й СС: ");
    // если ввести 3, то в двоичной системе это 0000 0000 0000 0011 
    // должно вывести ON ON OFF OFF - т.к. идём справа налево, берём 3 бита справа (011) и переворачиваем (как бы)
    scanf_s("%x", &cr.value);
    printf("CardR: %s || \ SD: %s || \ CompactFlash: %s || \ MemoryStick: %s\n",
        cr.CardR ? "ON" : "OFF",
        cr.SD ? "ON" : "OFF",
        cr.CompactFlash ? "ON" : "OFF",
        cr.MemoryStick ? "ON" : "OFF");
    return 0;
}