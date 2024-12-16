#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int main(void)
{
    // int a = 01;
    // int b = a / 10;
    // printf("%d", b);
    double d = 1.1;
    double e = 0;
    double b = modf(d, &e);
    // e = e;
    printf("%f %f", e, b);
    // char *prikol = "%12c ----";
    // char buffer[50];
    // // buffer = (char *)realloc(buffer, sizeof(char) * 100);
    // char number = '\"';
    // sprintf(buffer, prikol, number);
    // printf("|%s|", buffer);
    return 0;
}