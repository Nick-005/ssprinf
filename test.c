#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// TEST: Удалить при необходимости
char *itoa(int number, char *arr, int base);
void strrev(char *arr, int start, int end);
int main(void)
{

    int a = 1199;
    char buffer[100];
    buffer[0] = 'f';
    buffer[1] = 'a';
    buffer[2] = 'K';
    buffer[3] = 'I';
    buffer[4] = 'D';
    buffer[5] = 's';
    int i =6;
    char *number = malloc(100 * sizeof(char));
    // number[0] = 'f';
    int j = 0;
    itoa(a,number,10);
    for (; i < 6 + 4 + 1; i++)
    {
        buffer[i] = number[j++];
    }
    
    printf("|%s|\n",buffer);
    // int a = 01;
    // int b = a / 10;
    // printf("%d", b);
    // double d = 1.1;
    // double e = 0;
    // double b = modf(d, &e);
    // e = e;
    // printf("%f %f", e, b);
    // char *prikol = "%12c ----";
    // char buffer[50];
    // // buffer = (char *)realloc(buffer, sizeof(char) * 100);
    // char number = '\"';
    // sprintf(buffer, prikol, number);
    // printf("|%s|", buffer);
    return 0;
}


void strrev(char *arr, int start, int end)
{
    char temp;

    if (start >= end)
        return;

    temp = *(arr + start);
    *(arr + start) = *(arr + end);
    *(arr + end) = temp;

    start++;
    end--;
    strrev(arr, start, end);
}

char *itoa(int number, char *arr, int base)
{
    int i = 0, r, negative = 0;

    if (number == 0)
    {
        arr[i] = '0';
        arr[i + 1] = '\0';
        return arr;
    }

    if (number < 0 && base == 10)
    {
        number *= -1;
        negative = 1;
    }

    while (number != 0)
    {
        r = number % base;
        arr[i] = (r > 9) ? (r - 10) + 'a' : r + '0';
        i++;
        number /= base;
    }

    if (negative)
    {
        arr[i] = '-';
        i++;
    }

    strrev(arr, 0, i - 1);

    arr[i] = '\0';

    return arr;
}
