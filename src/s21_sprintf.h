#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
// #include "s21_string.h"

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
    int star;
} WIDTH;

typedef struct // структура, отвечающая за точность
{
    char *accuracy;
    int inAccuracy;
    int posAccuracy;
    int star;
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
    int sign;
} SPEC;

void convertPointerToString(void *pointer, char *string);
// ---------------------------------------------------------
char *HandleZeroCaseG(SPEC specif);
int NormalizeNumberG(long double *number, int *count, int *sign);
long int CalculatePrecisionG(SPEC specif);
char *CreateExponentStringG(SPEC specif, long int partBefore, long double partAfter, int precision, int count, int _signed);


char *CreateExponentString(SPEC specif, long int partBefore, long double partAfter, int precision, int count, int _signed);
void CalculateParts(long double number, int sign, long int precision, long int *partBefore, long double *partAfter);
int NormalizeNumber(long double *number, int *count, int *sign);
char *HandleZeroCase(SPEC specif);
// ---------------------------------------------------------
int countOfExponent(long double number);

char *DjeiIntoString(SPEC specif, long double number);
char *ExponentIntoGForString(SPEC specif, long double number);
int s21_g_convert_fract_to_string(double fract_part, int precision,char *fract_str);
// ---------------------------------------------------------
char *ExponentIntoString(SPEC specif, long double number);
int s21_exp_convert_fract_to_string(double fract_part, int precision,char *fract_str);
// ---------------------------------------------------------
void CharAndString(int a, va_list *factor, SPEC specif, char *buffer);
void UnsignedInteger(SPEC specif , va_list *factor , char *buffer);
void DecimalOctalHexidecimal(SPEC specif, va_list *factor, char *buffer);
// ---------------------------------------------------------
char* NumberIntoString_REVERS(SPEC specif,unsigned long int chislo);

char *NumberIntoString_REVERS_Width(int isFlagPlusOrSpace, int width, int countOfNumbers, char *resultString, char *numberIntoString, SPEC specif);
char *NumberIntoString_REVERS_WidthAndAccuracy(int isFlagPlusOrSpace, int width, int accuracy , int countOfNumbers, char *resultString, char *numberIntoString, SPEC specif);
char *NumberIntoString_REVERS_Numbers(int isFlagPlusOrSpace, int countOfNumbers, char *resultString , char *numberIntoString, SPEC specif);
char *NumberIntoString_REVERS_Accuracy(int isFlagPlusOrSpace,  int accuracy , int countOfNumbers, char *resultString, char *numberIntoString, SPEC specif);
// ---------------------------------------------------------
char *UnsignedIntIntoString(SPEC specif, unsigned long int chislo);
char *Unsigned_placeFlagPlusOrSpace(SPEC specif, char *resultString, int index_test, int limit);
int Unsigned_sizeOfNumber(unsigned long int copyChisla);
// ---------------------------------------------------------
char *CreateString_Revers_Width(SPEC specif, char *enterString, int length);
char *CreateString_Width(SPEC specif, char *enterString, int lengthOfEnterString);
char *StringIntoString(SPEC specif, char *enterString);
// ---------------------------------------------------------
char *FloatIntoString(SPEC specif, long double chislo);
char *FloatAddFlags(SPEC specif, char *enterString, int length, int plusOrMinus);
char *FloatPlaceFlagPlusOrSpace(SPEC specif, char *resultString, int sign, int index_test, int limit);

char *FloatAddSpace(SPEC specif, int length, char *resultString, char* enterString, int plusOrMinus);
char *FloatAddSpace_Revers(int length, SPEC specif, int isFlagPlusOrSpace, char *resultString, char *enterString, int plusOrMinus);
// ---------------------------------------------------------
char *myItoa_Unsigned(unsigned long int number, char *arr, int base, SPEC specif);
char *placeFlagPlusOrSpace(SPEC specif, char *resultString, int index_test, int limit);


char *NumberIntoString_ZeroAccuracy(SPEC specif,  char *resultString, int width);

char *NumberIntoString(SPEC specif,unsigned long int chislo);

char *NumberIntoString_Accuracy(char *resultString, SPEC specif, int accuracy, int countOfNumbers,unsigned long int chislo, int isFlagPlusOrSpace);
char *NumberIntoString_Numbers(int  countOfNumbers, SPEC specif,unsigned long int chislo, int isFlagPlusOrSpace, char *resultString);
char *NumberIntoString_Width(SPEC specif, int countOfNumbers,unsigned long int chislo, char *resultString, int width);
char *NumberIntoString_WidthAndAccuracy(int accuracy, int width , int countOfNumbers, SPEC specif, char *resultString,unsigned long int chislo, int isFlagPlusOrSpace);

// ---------------------------------------------------------

int s21_convert_fract_to_string(double fract_part, int precision, char *fract_str);
char *myItoa(long int number, char *arr, int base, SPEC specif);
void strrev2(char *arr, int start, int end);
int takeDigit(char chislo);
int sizeOfNumber(long int copyChisla);
long long converterToLongLong(char *chislo, SPEC *spec, int choose);
int parseSpecificator(SPEC *spec);
int s21_sprintf(char *buffer, char *stroka, ...);
void handle_specifier(char **c, va_list *factor, SPEC *specif);
void append_char_to_buffer(char *buffer, char c);
void process_specifier_sprintf(SPEC specif, va_list *factor, char *buffer);
void process_exponent_sprintf(SPEC specif, va_list *factor, char *buffer);
void process_g_format(SPEC specif, va_list *factor, char *buffer);
void process_float(SPEC specif, va_list *factor, char *buffer);
void process_pointer(va_list *factor, char *buffer);
SPEC check_specification(char **resume);
void initialize(SPEC *specif);
void clearMemory(SPEC *specif);
char *transferStrokiInResultNumberREVERS(SPEC specif, long int chislo);
char *transferStrokiInResultStringREVERS(SPEC specif, char *enter_string);
char *transferStrokiInResultString(SPEC specif, char *enter_string);
char *transferStrokiInResultFloat(SPEC specif, double chislo);
char *transferStrokiInResultNumber(SPEC specif, long int chislo);

void addNewNumber(SPEC *specif, char symbol, int choose);