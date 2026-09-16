typedef struct // структура, отвечающая за флаги
{
    int plus;
    int minus;
    int zero;
    int space;
    int hash;
} FLAG;

typedef struct // структура, отвечающая за ширину
{
    char *width;
    int inWidth;
    int posWidth;
} WIDTH;

typedef struct // структура, отвечающая за точность
{
    char *accuracy;
    int inAccuracy;
    int posAccuracy;
} ACC;

typedef struct // общая структура, которая в целом отвечает за 1 спецификатор.
{
    FLAG flag;
    WIDTH wi;
    ACC ac;
    int numbers;
    long long width;
    int point;
    int h;
    int l;
    long long accuracy;
    int symbol;
    int system;
} SPEC;


#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int s21_convert_fract_to_string(double fract_part, int precision,
                                char *fract_str);

// TEST: Удалить при необходимости
char *myItoa(long int number, char *arr, int base, SPEC specif);
void strrev2(char *arr, int start, int end);
//

int takeDigit(char chislo);

int sizeOfNumber(long int copyChisla);
long long converterToLongLong(char *chislo, SPEC *spec, int choose);
int parseSpecificator(SPEC *spec);
SPEC check_specification(char **resume);
void initialize(SPEC *specif);
char *transferStrokiInResultNumberREVERS(SPEC specif, long int chislo);
char *transferStrokiInResultStringREVERS(SPEC specif, char *enter_string);
int s21_sprintf(char *buffer, char *stroka, ...);
char *transferStrokiInResultString(SPEC specif, char *enter_string);
char *transferStrokiInResultFloat(SPEC specif, double chislo);
char *transferStrokiInResultNumber(SPEC specif, long int chislo);
void addNewNumber(SPEC *specif, char symbol, int choose);