#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// #include <string.h>
static double PRECISION = 0.00000000000001;
// static const int MAX_NUMBER_STRING_SIZE = 32;

// TEST: Удалить при необходимости
char *myItoa(int number, char *arr, int base);
char * myDtoa(char *s, double n);
// TEST: Удалить при необходимости
void myStrrev(char *arr, int start, int end);
int main(void)
{

    // int i;
    char s[32];
    // double d[] = {
    //     0.0,
    //     42.0,
    //     1234567.89012345,
    //     0.000000000000018,
    //     555555.55555555555555555,
    //     -888888888888888.8888888,
    //     111111111111111111111111.2222222222
    // };
    // for (i = 0; i < 7; i++) {
    //     printf("%d: printf: %.14g, myDtoa: %s\n", i+1, d[i], myDtoa(s, d[i]));
    // }
    myDtoa(s, 123.112323);
    printf("|%s|\n", s);


    // int a = 1199;
    // char buffer[100];
    // buffer[0] = 'f';
    // buffer[1] = 'a';
    // buffer[2] = 'K';
    // buffer[3] = 'I';
    // buffer[4] = 'D';
    // buffer[5] = 's';
    // int i =6;
    // char *number = malloc(100 * sizeof(char));
    // // number[0] = 'f';
    // int j = 0;
    // myItoa(a,number,10);
    // for (; i < 6 + 4 + 1; i++)
    // {
    //     buffer[i] = number[j++];
    // }
    
    // printf("|%s|\n",buffer);
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

char * myDtoa(char *s, double n) {
        int digit, m, m1;
        char *c = s;
        int neg = (n < 0);
        if (neg)
            n = -n;
        // calculate magnitude
        m = log10(n);
        int useExp = (m >= 14 || (neg && m >= 9) || m <= -9);
        if (neg)
            *(c++) = '-';
        // set up for scientific notation
        if (useExp) {
            if (m < 0)
               m -= 1.0;
            n = n / pow(10.0, m);
            m1 = m;
            m = 0;
        }
        if (m < 1.0) {
            m = 0;
        }
        // convert the number
        while (n > PRECISION || m >= 0) {
            double weight = pow(10.0, m);
            if (weight > 0 && !isinf(weight)) {
                digit = floor(n / weight);
                n -= (digit * weight);
                *(c++) = '0' + digit;
            }
            if (m == 0 && n > 0)
                *(c++) = '.';
            m--;
        }
        if (useExp) {
            // convert the exponent
            int i, j;
            *(c++) = 'e';
            if (m1 > 0) {
                *(c++) = '+';
            } else {
                *(c++) = '-';
                m1 = -m1;
            }
            m = 0;
            while (m1 > 0) {
                *(c++) = '0' + m1 % 10;
                m1 /= 10;
                m++;
            }
            c -= m;
            for (i = 0, j = m-1; i<j; i++, j--) {
                // swap without temporary
                c[i] ^= c[j];
                c[j] ^= c[i];
                c[i] ^= c[j];
            }
            c += m;
        }
        *(c) = '\0';
    
    return s;
}

void myStrrev(char *arr, int start, int end)
{
    char temp;

    if (start >= end)
        return;

    temp = *(arr + start);
    *(arr + start) = *(arr + end);
    *(arr + end) = temp;

    start++;
    end--;
    myStrrev(arr, start, end);
}

char *myItoa(int number, char *arr, int base)
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

    myStrrev(arr, 0, i - 1);

    arr[i] = '\0';

    return arr;
}
