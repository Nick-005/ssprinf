/*
    1.  scanf отправляет введенные данные в массив чаров.

    2.  C помощью цикла и функции isdigit (из ctype.h)
        проверить каждый элемент массива, является ли он цифрой.
        Можно ввести еще одно условие - разделитель (точка или запятая)
        для вещественных чисел.

    3.  Если не цифра или не разделитель, прерываем цикл и говорим,
        что введено не число.
        Если же все элементы массива ОК и достигнут нуль-терминирующий элемент,
        делаем с ним, что нужно: переводим в int, float и т.д.
        по своему усмотрению.

*/
#include "struct.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <math.h>

// TEST: Удалить при необходимости
char *myItoa(int number, char *arr, int base, SPEC specif);
void strrev2(char *arr, int start, int end);
//

int takeDigit(char chislo);
char takeChar(int chislo);

int sizeOfNumber(int chislo);

long long converterToLongLong(char *chislo, SPEC *spec, int choose);
int parseSpecificator(SPEC spec);
int s21_sprintf(char *buffer, char *stroka, ...);
SPEC check_specification(char *pointer, va_list *test);
void initialize(SPEC *specif);

char *converterIntToString(int chislo);

char *transferStrokiInResultCharREVERS(SPEC specif, int symbol);
char *transferStrokiInResultIntREVERS(SPEC specif, int chislo);

char *transferStrokiInResultFloat(double chislo);
char *transferStrokiInResultChar(SPEC specif, int symbol);
char *transferStrokiInResultInt(SPEC specif, int chislo);

void addNewNumber(SPEC *specif, char symbol, int choose);
int main(void)
{

    char *stroka = "%+-10d";
    char buffer[100];
    printf("Original stroka = |%s|\ns\n", stroka);
    s21_sprintf(buffer, stroka, 12);

    return 0;
}

int s21_sprintf(char *buffer, char *stroka, ...)
{

    va_list factor;
    buffer = buffer;
    va_start(factor, stroka);
    for (char *c = stroka; *c; c++)
    {
        if (*c == '%')
        {
            // printf("new\n");
            SPEC specif = check_specification(c, &factor);
            /*
               c = 0
               d = 1
               f = 2
               s = 3
           */

            int a = parseSpecificator(specif);
            switch (a)
            {
            case 0: // char
                int ch = va_arg(factor, int);
                char *result = (specif.flag.minus == 0 ? transferStrokiInResultChar(specif, ch) : transferStrokiInResultCharREVERS(specif, ch));
                printf("result string is = |%s|\nWhere char is = |%c|\nAnd width was = |%lld|\n", result, ch, specif.width);
                break;
            case 1: // int
                int chislo = va_arg(factor, int);

                char *resultInt = (specif.flag.minus == 0 ? transferStrokiInResultInt(specif, chislo) : transferStrokiInResultIntREVERS(specif, chislo));
                printf("result string is = |%s|\nWhere number is = |%d|\nAnd width was = |%lld|\n", resultInt, chislo, specif.width);

                break;
            case 2: // float
                double chisloo = va_arg(factor, double);
                // printf("%f", chisloo);
                transferStrokiInResultFloat(chisloo);
                break;
            // case 3: // string

            //     break;
            default:
                break;
            }
        }
    }
    return 0;
}

char *converterIntToString(int chislo)
{
    char *stroka = malloc(sizeof(char));

    int size = sizeOfNumber(chislo);
    int copyChisla = chislo;
    stroka = realloc(stroka, sizeof(char) * size);
    for (int i = size - 2; i >= 0; i--)
    {
        int d = copyChisla % 10;
        stroka[i] = takeChar(d);
        copyChisla /= 10;
    }
    stroka[size - 1] = '\0';
    return stroka;
}

char *transferStrokiInResultFloat(double chislo)
{
    double cheloe;
    double drob = modf(chislo, &cheloe);
    printf("%f %f", cheloe, drob);
    return "as";
}

// ПРЕОБРАЗОВАНИЕ ПЕРЕДОВАЕМОГО ЗНАЧЕНИЯ СИМВОЛА В СТРОКУ   !!!РЕВЕРС!!!
char *transferStrokiInResultCharREVERS(SPEC specif, int symbol)
{
    char *stroka;
    if (specif.width > 0 && specif.width > 1)
    {
        int length = (int)specif.width;
        stroka = (char *)malloc((length) * sizeof(char));
        for (int i = length; i >= 0; i--)
        {
            if (i >= 1)
                stroka[i] = (specif.flag.zero == 0 ? ' ' : '0');
            else
                stroka[i] = symbol;
        }
        stroka[length] = '\0';
    }
    else
    {
        stroka = (char *)malloc(2 * sizeof(char));
        stroka[0] = (char)symbol;
        stroka[1] = '\0';
    }
    return stroka;
}

// ПРЕОБРАЗОВАНИЕ ПЕРЕДОВАЕМОГО ЗНАЧЕНИЯ ЧИСЛА В СТРОКУ     !!!РЕВЕРС!!!
char *transferStrokiInResultIntREVERS(SPEC specif, int chislo)
{
    char *stroka;
    int size = sizeOfNumber(chislo);
    int length = (int)specif.width;
    if (length > size)
    {
        stroka = (char *)malloc((length) * sizeof(char));
        for (int i = length; i >= 0; i--)
        {
            if (i >= size)
                stroka[i] = (specif.flag.zero == 0 ? ' ' : '0');
            else
            {
                //  Рабочий код
                // int copyChisla = chislo;
                // for (int j = i; j >= 0; j--)
                // {
                //     int d = copyChisla % 10;
                //     stroka[j] = takeChar(d);
                //     copyChisla /= 10;
                // }
                // break;

                // Тестирую вот этот, т.к. более удобная реализация
                char *numberInString = malloc(sizeof(char) * (size + 1));
                numberInString = myItoa(chislo, numberInString, 10, specif);
                int k;
                if (specif.flag.plus == 0 && chislo >= 0)
                    k = size - 1;
                else
                {
                    k = size;
                    i += 1;
                }
                for (int j = i; j >= 0; j--)
                    stroka[j] = numberInString[k--];
                break;
            }
        }
        stroka[length] = '\0';
    }
    else
    {
        stroka = malloc((size) * sizeof(char));
        char *numberInString = malloc(sizeof(char) * (size + 1));
        numberInString = myItoa(chislo, numberInString, 10, specif);
        int k = 0;
        for (int j = 0; j < size + 1; j++)
            stroka[j] = numberInString[k++];
    }
    return stroka;
}

// ПРЕОБРАЗОВАНИЕ ПЕРЕДОВАЕМОГО ЗНАЧЕНИЯ ЧИСЛА В СТРОКУ
char *transferStrokiInResultInt(SPEC specif, int chislo)
{
    char *stroka;
    int size = sizeOfNumber(chislo);
    int length = (int)specif.width;
    if (length > size)
    {
        stroka = (char *)malloc((length) * sizeof(char));
        for (int i = 0; i < length; i++)
        {
            if (length - i > size)
                stroka[i] = (specif.flag.zero == 0 ? ' ' : '0');
            else
            {
                char *numberInString = malloc(sizeof(char) * (size + 1));
                numberInString = myItoa(chislo, numberInString, 10, specif);
                int k;
                if (specif.flag.plus == 0 && chislo >= 0)
                    k = 0;
                else
                {
                    k = 0; // k = size;
                    i -= 1;
                }
                for (int j = i; j <= length; j++)
                    stroka[j] = numberInString[k++];
                break;

                // int copyChisla = chislo;
                // for (int j = length - 1; j >= length - size; j--)
                // {
                //     int d = copyChisla % 10;
                //     stroka[j] = takeChar(d);
                //     copyChisla /= 10;
                // }
                // break;
            }
        }
        stroka[length] = '\0';
    }
    else
    {
        stroka = malloc((size) * sizeof(char));
        int copyChisla = chislo;
        for (int j = size - 1; j >= 0; j--)
        {
            int d = copyChisla % 10;
            stroka[j] = takeChar(d);
            copyChisla /= 10;
        }
        stroka[size] = '\0';
    }
    return stroka;
}

// ПРЕОБРАЗОВАНИЕ ПЕРЕДОВАЕМОГО ЗНАЧЕНИЯ СИМВОЛА В СТРОКУ
char *transferStrokiInResultChar(SPEC specif, int symbol)
{
    char *stroka;
    if (specif.width > 0 && specif.width > 1)
    {
        int length = (int)specif.width;
        stroka = (char *)malloc((length) * sizeof(char));
        for (int i = 0; i < length; i++)
        {
            if (i + 1 != length)
                stroka[i] = (specif.flag.zero == 0 ? ' ' : '0');
            else
                stroka[i] = symbol;
        }
        stroka[length] = '\0';
    }
    else
    {
        stroka = (char *)malloc(2 * sizeof(char));
        stroka[0] = (char)symbol;
        stroka[1] = '\0';
    }
    return stroka;
}

// функция для проверки следующих символов после '%'
// %[флаги][ширина][.точность][длина]спецификатор
SPEC check_specification(char *pointer, va_list *test)
{
    SPEC specif;
    initialize(&specif);
    test = test;
    int flagToExit = 0;
    while (flagToExit == 0)
    {

        switch (*++pointer)
        {
        // Спецификаторы
        case 'd':
        case 'f':
        case 'c':
        case '%':
            flagToExit = 1;
            specif.symbol = *pointer;
            break;
        // Флаги
        case '+':
            specif.flag.plus = 1;
            break;
        case '-':
            specif.flag.minus = 1;
            break;
        case '0':
            if (specif.numbers == 0)
            {
                specif.flag.zero = 1;
            }
            else
            {
                if (specif.point == 0)
                {
                    specif.width = 1;
                    addNewNumber(&specif, *pointer, 1);
                }
                else
                {
                    specif.accuracy = 1;
                    addNewNumber(&specif, *pointer, 0);
                }
            }

            break;
        case ' ':
            specif.flag.space = 1;
            break;
        case '#':
            specif.flag.hash = 1;
            break;
        // ширина
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if (specif.point == 0)
            {
                specif.width = 1;
                addNewNumber(&specif, *pointer, 1);
            }
            else
            {
                specif.accuracy = 1;
                addNewNumber(&specif, *pointer, 0);
            }
            specif.numbers += 1;
            break;
        case '.':
            specif.point = 1;
            break;
        default:
            break;
        }
    }

    if (specif.width == 1)
    {
        specif.width = converterToLongLong(specif.wi.width, &specif, 1);
    }
    if (specif.accuracy == 1)
    {
        specif.accuracy = converterToLongLong(specif.ac.accuracy, &specif, 0);
    }

    return specif;
}

// переводит строку в число типа long long
long long converterToLongLong(char *chislo, SPEC *spec, int choose)
{
    long long result = 0;
    int length = (choose == 1 ? spec->wi.inWidth : spec->ac.inAccuracy);
    int j = 0;
    for (int i = length - 1; i >= 0; i--)
    {
        int num = takeDigit(chislo[i]);
        if (num != -1)
        {
            result = result + (num * (j == 0 ? 1 : pow(10, j)));
            j++;
        }
    }
    return result;
}

// переводит цифру из char в int
int takeDigit(char chislo)
{
    int num = -1;
    switch (chislo)
    {
    case 48:
        num = 0;
        break;
    case 49:
        num = 1;
        break;
    case 50:
        num = 2;
        break;
    case 51:
        num = 3;
        break;
    case 52:
        num = 4;
        break;
    case 53:
        num = 5;
        break;
    case 54:
        num = 6;
        break;
    case 55:
        num = 7;
        break;
    case 56:
        num = 8;
        break;
    case 57:
        num = 9;
        break;
    }
    return num;
}

// переводит цифру из int в char
char takeChar(int chislo)
{
    char num = '-';
    switch (chislo)
    {
    case 0:
        num = 48;
        break;
    case 1:
        num = 49;
        break;
    case 2:
        num = 50;
        break;
    case 3:
        num = 51;
        break;
    case 4:
        num = 52;
        break;
    case 5:
        num = 53;
        break;
    case 6:
        num = 54;
        break;
    case 7:
        num = 55;
        break;
    case 8:
        num = 56;
        break;
    case 9:
        num = 57;
        break;
    }
    return num;
}

// возвращает номер спецификатора (порядок нумерации я сам придумал)
int parseSpecificator(SPEC spec)
{
    /*
       c = 0
       d = 1
       f = 2
       s = 3
   */
    int otvet = -1;
    switch (spec.symbol)
    {
    case 99:
        otvet = 0;
        break;
    case 100:
        otvet = 1;
        break;
    case 102:
        otvet = 2;
        break;
    case 115:
        otvet = 3;
        break;
    default:
        break;
    }
    return otvet;
}

// добавляет в динамический массив ПОЦИФЕРНО ширину или точность
void addNewNumber(SPEC *specif, char symbol, int choose)
{
    if (choose == 1)
    {
        // ширина
        if (specif->wi.inWidth == specif->wi.posWidth)
        {
            specif->wi.posWidth += 1;
            specif->wi.width = realloc(specif->wi.width, sizeof(char) * specif->wi.posWidth);
        }
        specif->wi.width[specif->wi.inWidth++] = symbol;
    }
    else
    {
        // точность
        if (specif->ac.inAccuracy == specif->ac.posAccuracy)
        {
            specif->ac.posAccuracy += 1;
            specif->ac.accuracy = realloc(specif->ac.accuracy, sizeof(char) * specif->ac.posAccuracy);
        }
        specif->ac.accuracy[specif->ac.inAccuracy++] = symbol;
    }
}

// ну штука для работы ITOA
void strrev2(char *arr, int start, int end)
{
    char temp;

    if (start >= end)
        return;

    temp = *(arr + start);
    *(arr + start) = *(arr + end);
    *(arr + end) = temp;

    start++;
    end--;
    strrev2(arr, start, end);
}

// функция для перевода числа в строку
char *myItoa(int number, char *arr, int base, SPEC specif)
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
    else if (negative == 0 && specif.flag.plus == 1)
    {
        arr[i] = '+';
        i++;
    }

    strrev2(arr, 0, i - 1);

    arr[i] = '\0';

    return arr;
}
// функция, которая задаёт значения по умолчанию
void initialize(SPEC *specif)
{
    specif->flag.hash = 0;
    specif->flag.minus = 0;
    specif->flag.plus = 0;
    specif->flag.space = 0;
    specif->flag.zero = 0;
    specif->accuracy = 0;
    specif->point = 0;
    specif->symbol = 0;
    specif->width = 0;
    specif->numbers = 0;
    specif->wi.inWidth = 0;
    specif->wi.posWidth = 1;
    specif->wi.width = malloc(sizeof(char));
    specif->ac.inAccuracy = 0;
    specif->ac.posAccuracy = 1;
    specif->ac.accuracy = malloc(sizeof(char));
}

// функция, которая вычисляет количество цифр в числе(размер числа)
int sizeOfNumber(int copyChisla)
{
    int size = 1;
    int d = 0;
    while (d != -1)
    {

        d = copyChisla % 10;
        copyChisla /= 10;

        if (copyChisla == 0)
        {
            d = -1;
        }
        else
            size++;
    }
    return size;
}