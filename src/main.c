// #include "struct.h"
#include "s21_sprintf.h"
#include <stdio.h>


int main(void) {
    
  char *original =      "|%-.9d| |%.10f|"; // оригинал 
  char buffer_original[100];
  buffer_original[0] = '\0';
  
  
  char *s21_original =        "|%- .9d| |%.10f|"; // моя реализация
  char buffer_s21[100];
  buffer_s21[0] = '\0';

  s21_sprintf(buffer_s21, s21_original, 14, 123.32142);     // моя реализация
  sprintf(buffer_original, original, 14, 123.32142);  // оригинальный метод
  
  printf("\n-------------MY SPRINTF-------------\n%s\n\n-------------ORIGINAL SPRINTF-------------\n%s\n\n", buffer_s21, buffer_original);
  return 0;
}
