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
SPEC check_specification(char *pointer);
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
    /*
        Важная пометка!
        У меня работает пока что только следующие вариации:
        1. Спецификатор 'd' с флагами  '+','-','0'(ну и вроде как ' '). Так же ему можно задавать любую ширину.
        2. Спецификатор 'c' с флагами  '+','-','0'(ну и вроде как ' '). Так же ему можно задавать любую ширину.

            Точность я не сделал. Если точность подразумивает большее кол-во цифр, чем кол-во цифр переданного нам числа,
        то ВСЕГДА слева от него появляются символ '0', такое кол-во раз, НА сколько точность больше,
        чем кол-во цифр переданного нам числа.

        Например: %6.4d, а число нам передали 13, то будет - "  0013".- т.к. точность больше на 2 чем переданное нам число.
                  %5.2d, а число нам передали 13, то будет - "   13". - т.к. точность совпадает с кол-вом цифр в переданном нам числе.
    */

    char *stroka = "%1d"; // строка, в которой у нас есть спецификаторы. Её передаем в s21_sprintf. Можете ее изменять и тестить мои штуки
    char buffer[100];     // массив, куда в итоге будет заноситься результат, но я пока не заношу туда его, а просто вывожу на экран результат функций
                          // так что пока он немного безполезен.

    printf("\nOriginal stroka = |%s|\n\n", stroka); // просто вывожу для показа, какая у нас строка
    s21_sprintf(buffer, stroka, 1234567);           // вызываю метод, где происходит вся логика.

    return 0;
}

int s21_sprintf(char *buffer, char *stroka, ...)
{

    va_list factor;                 // переменная, которая будет нужна для получения неопределенных параметров
    buffer = buffer;                // переменная, которая будет нужна для вывода результата, но пока она не используется
    va_start(factor, stroka);       // отвечает за то, что начали работать с неопределенными параметрами
    for (char *c = stroka; *c; c++) // цикл, который будет проходить по каждому символу в строке
    {
        if (*c == '%')
        {
            // printf("new\n");
            SPEC specif = check_specification(c); // полностью парсит все флаги, ширину, точность и сам символ (спецификатор)
            /*
               c = 0
               d = 1
               f = 2
               s = 3
           */

            int a = parseSpecificator(specif); // в переменной "а" будет лежать тип данных, который нужно вывести
            switch (a)
            {
            case 0:                           // char
                int ch = va_arg(factor, int); // получаем неопределенный параметр типа инт, потому что char нельзя. Можно только "char *"
                // в зависимости от флага '-' мы выбираем функцию для создания строки
                char *result = (specif.flag.minus == 0 ? transferStrokiInResultChar(specif, ch) : transferStrokiInResultCharREVERS(specif, ch));
                printf("result string is = |%s|\nWhere char is = |%c|\nAnd width was = |%lld|\n", result, ch, specif.width);
                break;
            case 1:                               // int
                int chislo = va_arg(factor, int); // получаем неопределенный параметр
                // в зависимости от флага '-' мы выбираем функцию для создания строки
                char *resultInt = (specif.flag.minus == 0 ? transferStrokiInResultInt(specif, chislo) : transferStrokiInResultIntREVERS(specif, chislo));
                printf("result string is = |%s|\nWhere number is = |%d|\nAnd width was = |%lld|\n", resultInt, chislo, specif.width);
                break;
            case 2:                                      // float
                double chisloo = va_arg(factor, double); // получаем неопределенный параметр
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

// моя функция по преобразование числа в строку и возвращает строку (вроде нигде не используется, но пока что оставляю е)
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

// тут будет реализация перевода числа с плавующей точкей в строку
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
    if (specif.width > 1) // если ширина, которую нам передали, больше чем 1, то значит мы должны дополнить вывод пробелами или 0
    {
        // в переменной length (ПОКА ЧТО) целочисленного типа будет лежать ширина, которую мы передали. (т.е. в случае %10.2d - будет число 10)
        int length = (int)specif.width;                   // почему я назвал её "длина", хотя она отвечает за "ширину" - я не знаю. Надо бы исправить, но боюсь что-то сломать.
        stroka = (char *)malloc((length) * sizeof(char)); // выделяем память строке, которую в последствии передадим как результат функции
        for (int i = length; i >= 0; i--)                 // идём уже с конца, т.к. включён флаг '-'
        {
            if (i >= 1) // если не первый символ, то в зависимости от флага '0' и флага '-', мы вставляем нужный символ.
                stroka[i] = ((specif.flag.minus == 1 && specif.flag.zero == 1) || specif.flag.zero == 0 ? ' ' : '0');
            else
                stroka[i] = symbol;
        }
        stroka[length] = '\0';
    }
    else
    {
        stroka = (char *)malloc(2 * sizeof(char)); // если нету ширины, то нам надо вернуть строку, в которой только символ и \0
        stroka[0] = (char)symbol;
        stroka[1] = '\0';
    }
    return stroka;
}

// ПРЕОБРАЗОВАНИЕ ПЕРЕДОВАЕМОГО ЗНАЧЕНИЯ ЧИСЛА В СТРОКУ     !!!РЕВЕРС!!!
char *transferStrokiInResultIntREVERS(SPEC specif, int chislo)
{
    char *stroka;
    int size = sizeOfNumber(chislo); // получает кол-во цифр в переданном нам числе
                                     // в переменной length (ПОКА ЧТО) целочисленного типа будет лежать ширина, которую мы передали. (т.е. в случае %10.2d - будет число 10)
    int length = (int)specif.width;  // почему я назвал её "длина", хотя она отвечает за "ширину" - я не знаю. Надо бы исправить, но боюсь что-то сломать.
    if (length > size)
    {
        stroka = (char *)malloc((length) * sizeof(char));
        for (int i = length; i >= 0; i--)
        {
            if (i >= size)
                stroka[i] = ((specif.flag.minus == 1 && specif.flag.zero == 1) || specif.flag.zero == 0 ? ' ' : '0');
            else
            {

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
                break; // не придумал пока что лучше выхода из for, чем чилловый break);
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
    // в переменной length (ПОКА ЧТО) целочисленного типа будет лежать ширина, которую мы передали. (т.е. в случае %10.2d - будет число 10)
    int length = (int)specif.width; // почему я назвал её "длина", хотя она отвечает за "ширину" - я не знаю. Надо бы исправить, но боюсь что-то сломать.
    if (length > size)
    {
        stroka = (char *)malloc((length) * sizeof(char));
        for (int i = 0; i < length; i++)
        {
            // если разность ширины, которую нам передали, и индекса текующего элемента больше чем кол-во цифр в нашем числе, то мы выводим "дополняющий" символ
            if (length - i > size) // т.е. по факту тут проверяется на кол-во оставшегося места в строке, чтобы нам хватило для вмещения туда самого числа.
                stroka[i] = (specif.flag.zero == 0 ? ' ' : '0');
            else
            {
                char *numberInString = malloc(sizeof(char) * (size + 1)); // массив, куда нам присвоится наше число в виде строки
                numberInString = myItoa(chislo, numberInString, 10, specif);
                // переменная, отвечающая за правильную индексацию массива 'numberInString', но по идее можно просто 'j - i'.
                int k = 0;
                // Если флаг '+' есть и число положительное, то нам надо добавить место под еще один символ для знака числа.
                // Если число было отрицательным, то из функции myItoa вернётся строка уже со знаком '-'
                if (!(specif.flag.plus == 0 && chislo >= 0))
                    i -= 1;
                for (int j = i; j <= length; j++)
                    stroka[j] = numberInString[k++];
                break;
            }
        }
        stroka[length] = '\0';
    }
    else
    {
        stroka = malloc((size + 2) * sizeof(char));
        char *numberInString = malloc(sizeof(char) * (size + 1));
        numberInString = myItoa(chislo, numberInString, 10, specif);
        int k = size;
        for (int j = size; j >= 0; j--)
            stroka[j] = numberInString[k--];

        // stroka[size] = '\0';
        // stroka = malloc((size) * sizeof(char));
        // int copyChisla = chislo;
        // for (int j = size - 1; j >= 0; j--)
        // {
        //     int d = copyChisla % 10;
        //     stroka[j] = takeChar(d);
        //     copyChisla /= 10;
        // }
        // stroka[size] = '\0';
    }
    return stroka;
}

// ПРЕОБРАЗОВАНИЕ ПЕРЕДОВАЕМОГО ЗНАЧЕНИЯ СИМВОЛА В СТРОКУ
char *transferStrokiInResultChar(SPEC specif, int symbol)
{
    char *stroka;
    if (specif.width > 1) // если ширина, которую нам передали, больше чем 1, то значит мы должны дополнить вывод пробелами или 0
    {
        // в переменной length (ПОКА ЧТО) целочисленного типа будет лежать ширина, которую мы передали. (т.е. в случае %10.2d - будет число 10)
        int length = (int)specif.width;                   // почему я назвал её "длина", хотя она отвечает за "ширину" - я не знаю. Надо бы исправить, но боюсь что-то сломать.
        stroka = (char *)malloc((length) * sizeof(char)); // выделяем память строке, которую в последствии передадим как результат функции
        for (int i = 0; i < length; i++)
        {
            if (i + 1 != length) // если не последний символ, то в зависимости от флага 0, мы вставляем нужный символ.
                stroka[i] = (specif.flag.zero == 0 ? ' ' : '0');
            else
                stroka[i] = symbol; // иначе закидываем туда наш полученный символ.
        }
        stroka[length] = '\0'; // закрываем строку.
    }
    else
    {
        stroka = (char *)malloc(2 * sizeof(char)); // если нету ширины, то нам надо вернуть строку, в которой только символ и \0
        stroka[0] = (char)symbol;
        stroka[1] = '\0';
    }
    return stroka;
}

// функция для проверки следующих символов после '%'
// %[флаги][ширина][.точность][длина]спецификатор
SPEC check_specification(char *pointer)
{
    SPEC specif;         // объявление экземпляра структуры
    initialize(&specif); // и её инициализация значений по умолчанию (мб можно по-другому инициализировать, но я не стал пока над этим думать)
    // test = test;        // ну типо переменная, через которую можно получать неопределенное кол-во параметров. Но я её решил не использовать здесь.
    int flagToExit = 0; // ну типо чтобы не делать бесконечный цикл
    while (flagToExit == 0)
    {

        switch (*++pointer) // проверка следующего символа. Цикл начинается с символа, который идёт сразу после %
        {
        // Спецификаторы
        case 'd':
        case 'f':
        case 'c':
        case '%':
            flagToExit = 1;
            specif.symbol = *pointer; // заношу в структуру ЦЕЛОЧИСЛЕННОЕ представление символа
            break;
        // Флаги
        case '+':
            specif.flag.plus = 1; // помечаем, что нам передали флаг '+'
            break;
        case '-':
            specif.flag.minus = 1; // помечаем, что нам передали флаг '-'
            break;
        case '0':
            if (specif.numbers == 0) // если у нас еще не было цифр, то это флаг '0', а не часть ширины или точности
            {
                specif.flag.zero = 1; // помечаем, что нам передали флаг '0'
            }
            else
            {
                if (specif.point == 0) // если мы еще не встретили знак '.' -> мы еще не начали записывать точность -> эти цифры идут в ширину
                {
                    specif.width = 1;                   // помечаем, что у нам передали хоть какую-то ширину.
                    addNewNumber(&specif, *pointer, 1); // передаем это число (*pointer), чтобы добавить его в динамический массив.
                }
                else
                {
                    specif.accuracy = 1;                // помечаем, что у нам передали хоть какую-то точность.
                    addNewNumber(&specif, *pointer, 0); // передаем это число (*pointer), чтобы добавить его в динамический массив.
                }
            }

            break;
        case '.': // помечаем, что у нам передали флаг '.'
            specif.point = 1;
            break;
        case ' ':
            specif.flag.space = 1; // помечаем, что у нам передали флаг ' '
            break;
        case '#':
            specif.flag.hash = 1; // помечаем, что у нам передали флаг #
            break;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            if (specif.point == 0) // если мы еще не встретили знак '.' -> мы еще не начали записывать точность -> эти цифры идут в ширину
            {
                specif.width = 1; // помечаем, что у нам передали хоть какую-то ширину.
                addNewNumber(&specif, *pointer, 1);
            }
            else
            {
                specif.accuracy = 1; // помечаем, что у нам передали хоть какую-то точность.
                addNewNumber(&specif, *pointer, 0);
            }
            specif.numbers += 1; // увеличиваем количество переданных нам цифр. Как минимум нужна для проверки на флага '0'.
            break;

        default:
            break;
        }
    }

    if (specif.width == 1) // если нам передали какую-то ширину, то мы забираем из динамического массива это число
    {                      // и превращаем его в тип long long, т.к. кто-то сказал, что максимальная ширина и точность может вмещать в себя не больше 21 символа. (но это не точно)
        specif.width = converterToLongLong(specif.wi.width, &specif, 1);
    }
    if (specif.accuracy == 1) // аналогично как с шириной, но тут уже с точностью
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
    switch (spec.symbol) // в зависимости от ЦЕЛОЧИСЛЕННОГО представления символа спецификатора мы выдём ему какое-то число, которое я сам придумал.
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

    // Если число отрицательное и система счисления - десятичная
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

    strrev2(arr, 0, i - 1); // функция для "переворота"/"разворота" строки

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

// функция, которая вычисляет количество цифр в числе(размер числа). Вроде работает правильно.
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