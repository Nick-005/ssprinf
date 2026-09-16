#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static double PRECISION = 0.000001;
// static const int MAX_NUMBER_STRING_SIZE = 32;

// TEST: Удалить при необходимости
char *myItoa(int number, char *arr, int base);
char *myDtoa(char *s, double n);
char *myDtoa2(char *s, double n);
// TEST: Удалить при необходимости
void myStrrev(char *arr, int start, int end);
int main(void) {
  // int i;
  // char s[32];
  // double f = 0.914722;
  // myDtoa2(s,f);
  // printf("|%s| - |%f|\n", s , f);
  // double d[] = {0,
  //               42.0,
  //               1234567.89012345,
  //               0.3412,
  //               555555.4214215,
  //               -5234.8888888,
  //               54123.8487372};
  // for (i = 0; i < 7; i++) {
  //   printf("%d: printf: %f, myDtoa: %s\n", i + 1, d[i], myDtoa2(s, d[i]));
  // }
  // printf("max limit for int = |%d|\nmax limit for long int = |%ld|\n",
  // INT_MAX, LONG_MAX);
  printf("--------------ORIGINAL SPRINTF--------------\n");

  // char *prikol2 = "|%d|";
  // int chislo = 897247121;
  // // // buffer = (char *)realloc(buffer, sizeof(char) * 100);
  // // char number = '\"';

  char *prikol = "|%12.4d| |%.10f|";
  char *buffer = malloc(sizeof(char) * 100);
  buffer[0] = '\0';
  sprintf(buffer, prikol,  14, 123.32142 );  // 4294967295
  printf("%s\n", buffer);

  // int a = -112312;
  // unsigned int b = (unsigned int) a;
  // printf("%d\n", b);

  // sprintf(buffer, prikol2, chislo / 32767);
  // printf("|%s|\n", buffer);
  return 0;
}

char *myDtoa(char *s, double n) {
  int digit, m, m1;
  char *c = s;
  int neg = (n < 0);
  if (neg) n = -n;
  // calculate magnitude
  m = log10(n);
  int useExp = (m >= 14 || (neg && m >= 9) || m <= -9);
  if (neg) *(c++) = '-';
  // set up for scientific notation
  if (useExp) {
    if (m < 0) m -= 1.0;
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
    if (m == 0 && n > 0) *(c++) = '.';
    m--;
  }
  if (useExp) {
    *(c++) = 'e';
    if (m1 > 0) {
      *(c++) = '+';
    } else {
      *(c++) = '-';
      m1 = -m1;
    }

    // Преобразование экспоненты
    char expBuffer[10];  // Буфер для хранения строки экспоненты
    int expIndex = 0;
    do {
      expBuffer[expIndex++] = '0' + (m1 % 10);
      m1 /= 10;
    } while (m1 > 0);

    // Реверсирование строки экспоненты
    for (int i = expIndex - 1; i >= 0; i--) {
      *(c++) = expBuffer[i];
    }
  }
  *(c) = '\0';

  return s;
}

char *myDtoa2(char *s, double n) {
  int m;
  double digit;
  char *c = s;
  if (n == 0.0) {
    return "0.000000";
  }

  int neg = (n < 0);
  if (neg) n = -n;
  // calculate magnitude
  m = log10(n);
  printf("|%d|\n", m);
  // int useExp = (m >= 14 || (neg && m >= 9) || m <= -9);
  if (neg) *(c++) = '-';
  // set up for scientific notation
  // if (useExp) {
  //   if (m < 0) m -= 1.0;
  //   n = n / pow(10.0, m);
  //   m1 = m;
  //   m = 0;
  // }
  // if (m < 1.0) {
  //   m = 0;
  // }
  // convert the number

  while (n > PRECISION || m >= 0) {
    // printf(
    //     "\n------------------------------------\nmassiv - |%s|\nchislo - "
    //     "|%f|\ndigit - |%f|\nm - |%d|",
    //     s, n, digit, m);
    double weight = pow(10.0, m);

    if (weight > 0 && !isinf(weight)) {
      digit = floor(n / weight);
      // printf("floor = |%f|\n", n / weight);
      n -= (digit * weight);
      *(c++) = '0' + digit;
    }
    if (m == 0 && n > 0) *(c++) = '.';
    m--;
    // printf(
    //     "\n**********************************\nmassiv - |%s|\nchislo - "
    //     "|%f|\ndigit - |%f|\nm - |%d|\nwidth - "
    //     "|%f|\n------------------------------------\n",
    //     s, n, digit, m, weight);
  }
  // if (useExp) {
  //   *(c++) = 'e';
  //   if (m1 > 0) {
  //     *(c++) = '+';
  //   } else {
  //     *(c++) = '-';
  //     m1 = -m1;
  //   }

  //   // Преобразование экспоненты
  //   char expBuffer[10];  // Буфер для хранения строки экспоненты
  //   int expIndex = 0;
  //   do {
  //     expBuffer[expIndex++] = '0' + (m1 % 10);
  //     m1 /= 10;
  //   } while (m1 > 0);

  //   // Реверсирование строки экспоненты
  //   for (int i = expIndex - 1; i >= 0; i--) {
  //     *(c++) = expBuffer[i];
  //   }
  // }
  *(c) = '\0';

  return s;
}

void myStrrev(char *arr, int start, int end) {
  char temp;

  if (start >= end) return;

  temp = *(arr + start);
  *(arr + start) = *(arr + end);
  *(arr + end) = temp;

  start++;
  end--;
  myStrrev(arr, start, end);
}

char *myItoa(int number, char *arr, int base) {
  int i = 0, r, negative = 0;

  if (number == 0) {
    arr[i] = '0';
    arr[i + 1] = '\0';
    return arr;
  }

  if (number < 0 && base == 10) {
    number *= -1;
    negative = 1;
  }

  while (number != 0) {
    r = number % base;
    arr[i] = (r > 9) ? (r - 10) + 'a' : r + '0';
    i++;
    number /= base;
  }

  if (negative) {
    arr[i] = '-';
    i++;
  }

  myStrrev(arr, 0, i - 1);

  arr[i] = '\0';

  return arr;
}
