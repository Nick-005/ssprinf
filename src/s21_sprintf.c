#include "s21_sprintf.h"
#include <string.h>
void CharAndString(int a, va_list *factor, SPEC specif, char *buffer){
  char *enterr;
  if (a == 99) {
    enterr = malloc(sizeof(char) * 2);
    enterr[0] = (char)va_arg(*factor, int);
    enterr[1] = '\0';
    char *resultSymbol = StringIntoString(specif, enterr);
    int sizeChars = strlen(resultSymbol);
    strncat(buffer, resultSymbol, sizeChars);
    free(enterr);
    free(resultSymbol);
  } else{
    enterr = va_arg(*factor, char *);
    char *resultSymbol =  StringIntoString(specif, enterr);
    int sizeChars = strlen(resultSymbol);
    strncat(buffer, resultSymbol, sizeChars);
    free(resultSymbol);
  }
}

void UnsignedInteger(SPEC specif , va_list *factor , char *buffer){
  unsigned long int num = 0;
  if (specif.l == 1) 
    num = va_arg(*factor, unsigned long int);
  else if (specif.h == 1) 
    num = (unsigned short int)va_arg(*factor, unsigned int);
    else 
    num = va_arg(*factor, unsigned int);
  if (!(num == 0.0 && specif.accuracy == 0 && !(specif.width > 0)))
  {
    char *unsignedInt = (specif.flag.minus == 0 ? NumberIntoString(specif, num): NumberIntoString_REVERS(specif, num));
    int sizeUint = strlen(unsignedInt);
    strncat(buffer, unsignedInt, sizeUint);
    free(unsignedInt);
  }
}

void DecimalOctalHexidecimal(SPEC specif, va_list *factor, char *buffer){
  if (specif.l == 1) { 
    long int dlinnoeChislo = va_arg(*factor, long int);
    if (dlinnoeChislo < 0)
    {
      dlinnoeChislo *= -1;
      specif.sign = -1;
    } else
      specif.sign = 1;  
    char *resultLong = (specif.flag.minus == 0 ? NumberIntoString(specif, dlinnoeChislo) : NumberIntoString_REVERS(specif, dlinnoeChislo));
    int sizeLong = strlen(resultLong);
    strncat(buffer, resultLong, sizeLong);
    free(resultLong);
  } else {
    int chislo = (specif.h == 1 ? (short int)va_arg(*factor, int) : va_arg(*factor, int)); 
    if (chislo < 0)
    {
      chislo *= -1;
      specif.sign = -1;
    } else
      specif.sign = 1;
    char *resultInt = (specif.flag.minus == 0 ? NumberIntoString(specif, chislo) : NumberIntoString_REVERS(specif, chislo));
    
    int sizeShort = strlen(resultInt);
    strncat(buffer, resultInt, sizeShort);
    free(resultInt);
  }
}

int s21_sprintf(char *buffer, char *str, ...) {
  va_list factor;
    buffer[0] = '\0';
    va_start(factor, str);

    for (char *c = str; *c != '\0'; c++) {
        if (*c == '%') {
            SPEC specif;
            // initialize(&specif);
            handle_specifier(&c, &factor, &specif);
            process_specifier_sprintf(specif, &factor, buffer);
            clearMemory(&specif);
        } else {
            strncat(buffer, c, 1);
        }
    }

    strncat(buffer, "\0", 1);
    va_end(factor);
    return 0;
}

void handle_specifier(char **c, va_list *factor, SPEC *specif) {
    *specif = check_specification(c);

    if (specif->wi.star == 1) {
        int width = va_arg(*factor, int);
        if (width < 0) {
            specif->width = width * -1;
            specif->flag.minus = 1;
        } else {
            specif->width = width;
        }
    }

    if (specif->ac.star == 1) {
        int accuracy = va_arg(*factor, int);
        specif->accuracy = (accuracy < 0) ? 6 : accuracy;
    }
}

void process_pointer(va_list *factor, char *buffer) {
    char pointerString[14];
    convertPointerToString(va_arg(*factor, void*), pointerString);
    strncat(buffer, pointerString, 14);
}

void process_float(SPEC specif, va_list *factor, char *buffer) {
    double chisloo = va_arg(*factor, double);
    char *floatInString = FloatIntoString(specif, chisloo);
    size_t sizeFloat = strlen(floatInString);
    strncat(buffer, floatInString, sizeFloat);
    free(floatInString);
}

void process_g_format(SPEC specif, va_list *factor, char *buffer) {
    long double numberG = va_arg(*factor, double);
    char *resultG = DjeiIntoString(specif, numberG);
    size_t sizeG = strlen(resultG);
    strncat(buffer, resultG, sizeG);
    free(resultG);
}

void process_exponent_sprintf(SPEC specif, va_list *factor, char *buffer) {
    double numberExponent = va_arg(*factor, double);
    char *resultExp = ExponentIntoString(specif, numberExponent);
    int sizeExp = strlen(resultExp);
    strncat(buffer, resultExp, sizeExp);
    free(resultExp);
}

void process_specifier_sprintf(SPEC specif, va_list *factor, char *buffer) {
    switch (specif.symbol) {
        case 99:  // char
        case 115: // string (char *)
            CharAndString(specif.symbol, factor, specif, buffer);
            break;
        case 100: // int (d)
            DecimalOctalHexidecimal(specif, factor, buffer);
            break;
        case 101: // e
        case 69:  // E
            process_exponent_sprintf(specif, factor, buffer);
            break;
        case 71:  // g
        case 103: // G
            process_g_format(specif, factor, buffer);
            break;
        case 102: // float
            process_float(specif, factor, buffer);
            break;
        case 117: // unsigned int
        case 111: // octal (o)
        case 120: // hexadecimal (x)
        case 88:  // hexadecimal (X)
            UnsignedInteger(specif, factor, buffer);
            break;
        case 37: // %
            strncat(buffer, "%%", 1);
            break;
        case 112: // pointer
            process_pointer(factor, buffer);
            break;
        default:
            printf("error in switch\n");
            break;
    }
}

char *DjeiIntoString(SPEC specif, long double number){
  int sign =1;
  if (number < 0)
  {
    number = number * -1;
    sign = -1;
  }
  char *resultString;
  unsigned long int integerPart = (unsigned long int) number;
  double countExp = (number == 0.0 ? 0: (double)countOfExponent(number));
  long int precision = specif.accuracy == -1 ? 5 : (specif.accuracy == 0 ? 1 : specif.accuracy); 
  if (countExp >= precision || countExp < -4)
  {
   resultString = ExponentIntoGForString(specif, number);
  }
  else
  {
    long double partAfter = ((number - integerPart));
    int numberToRound = (int)(partAfter* 10);
    if (numberToRound > 5)
      integerPart+=1;
    SPEC empty;
    initialize(&empty);
    char *numberString = NumberIntoString(empty, integerPart);
    char *fractPart = malloc(sizeof(char) * (precision + 1));
  
    s21_g_convert_fract_to_string(partAfter, precision - strlen(numberString), fractPart );
    char *middleWare = malloc(sizeof(char) * (precision + 1 + strlen(numberString)));
    middleWare[0]  ='\0';
    strncat(middleWare, numberString, strlen(numberString));
    strncat(middleWare, fractPart, precision+1 );
    
    resultString = FloatAddFlags(specif, middleWare,strlen(middleWare), sign);
    clearMemory(&empty);
  }
  
  return resultString;
} 

char *ExponentIntoGForString(SPEC specif, long double number) {
    if (number == 0.0) {
        return HandleZeroCaseG(specif);
    }

    int sign, count;
    int _signed = NormalizeNumberG(&number, &count, &sign);

    long int precision = CalculatePrecisionG(specif);
    long int partBefore = (long int)number;
    long double partAfter = number - partBefore;

    if (partBefore + '0' > 57) {
        partBefore -= 9;
        count -= 1;
    }

    char *temp = CreateExponentStringG(specif, partBefore, partAfter, precision, count, _signed);
    char *finalString = FloatAddFlags(specif, temp, strlen(temp), sign);

    free(temp);
    return finalString;
}

char *HandleZeroCaseG(SPEC specif) {
    long int precision = specif.accuracy == -1 ? 6 : specif.accuracy;
    long double rounding = 0.5 * pow(10, -precision);
    long double normalNumber = fabsl(0.0 + 1 * rounding);

    long int partBefore = 0;
    long double partAfter = normalNumber - partBefore;

    char partBeforeString[2];
    partBeforeString[0] = '0';
    partBeforeString[1] = '\0';

    char *partAfterString = malloc(sizeof(char) * (precision + 1 + 4));
    int length = s21_exp_convert_fract_to_string(partAfter, precision, partAfterString);
    partAfterString[length] = (specif.symbol == 'e' ? 'e' : 'E');
    partAfterString[length + 1] = '+';
    partAfterString[length + 2] = '0';
    partAfterString[length + 3] = '0';
    partAfterString[length + 4] = '\0';

    char *temp = malloc(sizeof(char) * (strlen(partBeforeString) + strlen(partAfterString) + 1));
    temp[0] = '\0';
    strncat(temp, partBeforeString, 1);
    strncat(temp, partAfterString, precision + 4 + 2);

    char *finalString = FloatAddFlags(specif, temp, strlen(temp), 1);
    free(temp);
    free(partAfterString);
    return finalString;
}

int NormalizeNumberG(long double *number, int *count, int *sign) {
    *sign = (*number < 0 ? -1 : 1);
    *count = 0;
    int _signed = 0;
    unsigned long int integerPart = (unsigned long int)*number;

    if (integerPart > 0) {
        _signed = 1;
        while (fabsl((double)((int)(*number))) > 9.999999999) {
            *number /= 10;
            (*count)++;
        }
    } else {
        _signed = -1;
        while (fabsl((double)((int)(*number))) < 0.999999999) {
            *number *= 10;
            (*count)++;
        }
    }

    return _signed;
}

long int CalculatePrecisionG(SPEC specif) {
    long int precision = specif.accuracy == -1 ? 6 : (specif.accuracy == 0 ? 1 : specif.accuracy);

    if (specif.symbol == 'g' || specif.symbol == 'G') {
        if (specif.accuracy == 0 || specif.accuracy == 1) {
            precision = 0;
        } else {
            precision -= 1;
        }
    }

    return precision;
}

char *CreateExponentStringG(SPEC specif, long int partBefore, long double partAfter, int precision, int count, int _signed) {
    char partBeforeString[2];
    partBeforeString[0] = partBefore + '0';
    partBeforeString[1] = '\0';

    int countExp = (count > 99 ? 5 : 4);
    char *partAfterString = malloc(sizeof(char) * (precision + 1 + countExp));
    int length = s21_g_convert_fract_to_string(partAfter, precision, partAfterString);

    partAfterString[length - 1] = (specif.symbol == 'g' ? 'e' : 'E');
    partAfterString[length] = (_signed == 1 ? '+' : '-');

    if (count > 99) {
        partAfterString[length + 1] = '0' + count / 100;
        partAfterString[length + 2] = '0' + (count % 100) / 10;
        partAfterString[length + 3] = '0' + count % 10;
        partAfterString[length + 4] = '\0';
    } else {
        partAfterString[length + 1] = '0';
        partAfterString[length + 2] = '0' + count % 10;
        partAfterString[length + 3] = '\0';
    }

    char *temp = malloc(sizeof(char) * (strlen(partBeforeString) + strlen(partAfterString) + 1));
    temp[0] = '\0';
    strncat(temp, partBeforeString, 1);
    strncat(temp, partAfterString, precision + (count > 99 ? 5 : 4) + 2);

    return temp;
}

int countOfExponent(long double number){
  int count = 0;
  unsigned long int integerPart = (unsigned long int) number;
  if (integerPart > 0 )
  {
    while (fabsl((double)((int)(number))) > 9.999999999)
    {
      number /= 10;
      count++;
    }
  } else{
    while (fabsl((double)((int)(number))) < 0.999999999)
    {
      number *= 10;
      count++;
    }
  }
  return count;
}

int s21_g_convert_fract_to_string(double fract_part, int precision,char *fract_str) {
  int fract_len = 0;
  if (precision > 0 && fract_part != 0.) {  
    fract_str[fract_len++] = '.'; 
    for (int i = 0; i < precision ; i++) { 
      fract_part *= 10.0;  
      int digit = (int)fract_part; 
      fract_part -= digit;
      if (digit == 0)
        continue;
      fract_str[fract_len++] = digit + '0';   
        
    }
  } fract_str[fract_len++] = '\0';
  return fract_len;  
}

char *FloatAddSpace_Revers(int length, SPEC specif, int isFlagPlusOrSpace, char *resultString, char *enterString, int plusOrMinus){
  int j = length - 1;
      for (int i = specif.width + isFlagPlusOrSpace; i >= 0 + isFlagPlusOrSpace; i--)
      {
        if (i - isFlagPlusOrSpace >= length)
          resultString[i] = ' ';
        else 
            resultString[i] = enterString[j--];
        
      }
      resultString = FloatPlaceFlagPlusOrSpace(specif, resultString, plusOrMinus, 0, specif.width );
    return resultString;
}

char *FloatAddSpace(SPEC specif, int length, char *resultString, char* enterString, int plusOrMinus){
  int indexOfFirtNumber = 0;
      for (int i = 0; specif.width - i > length; i++) {
        resultString[i] = (specif.flag.zero == 0 ? ' ' : '0');
        indexOfFirtNumber = i +1;
      }
      int j = 0;
      for (int i = indexOfFirtNumber; i <= specif.width; i++)
        resultString[i] = enterString[j++];
      
      resultString = FloatPlaceFlagPlusOrSpace(specif, resultString, plusOrMinus, indexOfFirtNumber-1,specif.width);
  return resultString;
}

char *FloatPlaceFlagPlusOrSpace(SPEC specif, char *resultString, int sign, int index_test, int limit){ 
  int indexOfSign = (specif.flag.zero == 1 ? 0 : index_test);
  if (specif.flag.plus == 1 || sign == -1)
    resultString[indexOfSign] = (sign == -1 ? '-' : '+');
  else if (specif.flag.space == 1)
    resultString[indexOfSign] = (sign == -1 ? '-' : ' ');
  resultString[limit] = '\0';
  return resultString;
}

char *FloatAddFlags(SPEC specif, char *enterString, int length, int plusOrMinus){
  char *resultString;
  int isFlagPlusOrSpace = (specif.flag.plus == 1 || specif.flag.space == 1 ||plusOrMinus == -1 ? 1 : 0 );
  if (specif.width > length)
  {
    resultString = malloc(sizeof(char) * (specif.width + 1 + isFlagPlusOrSpace));
    if (specif.flag.minus == 0)
      resultString = FloatAddSpace(specif, length, resultString, enterString, plusOrMinus); 
    else 
      resultString = FloatAddSpace_Revers(length, specif, isFlagPlusOrSpace, resultString, enterString,plusOrMinus);
  }
  else{
    int j = 0;
    resultString = malloc(sizeof(char) * (length + 1 + isFlagPlusOrSpace));
    for (int i = isFlagPlusOrSpace; i < length + isFlagPlusOrSpace; i++)
      resultString[i] = enterString[j++];
    if (isFlagPlusOrSpace == 1 || plusOrMinus == -1)
      resultString = FloatPlaceFlagPlusOrSpace(specif,resultString, plusOrMinus, 0, length + isFlagPlusOrSpace) ;
    else
      resultString[length + isFlagPlusOrSpace] = '\0';   
  }
  return resultString;
}

char *FloatIntoString(SPEC specif, long double chislo){
  char *endOfArt;
  int precision = specif.accuracy == -1 ? 6 : specif.accuracy;  
  char *partAfter = malloc(sizeof(char) * (precision + 2)); 
  partAfter[0] ='\0';
  memset(partAfter, ' ', precision + 2);
  long double rounding = 0.5 * pow(10, -precision); 
  int sign = chislo < 0 ? -1 : 1;                   
  long double newChislo = fabsl(chislo + sign * rounding);  
  long int decimalInt = (long int)newChislo;                
  long double fractPart = newChislo - decimalInt;           
  SPEC empty;
  initialize(&empty);
  char *partBefore = NumberIntoString(empty, decimalInt);
  clearMemory(&empty);
  int length = s21_convert_fract_to_string(fractPart, precision, partAfter);
  endOfArt = malloc(sizeof(char) * (strlen(partBefore) + 1 + length));
  endOfArt[0] = '\0';
  strncat(endOfArt, partBefore, strlen(partBefore));
  strncat(endOfArt, partAfter, length );
  char *finalString = FloatAddFlags(specif, endOfArt,strlen(partBefore)  + length, sign);
  free(partBefore);
  free(partAfter);
  free(endOfArt);
  return finalString;
}

int s21_convert_fract_to_string(double fract_part, int precision,char *fract_str) {
  int fract_len = 0;
  if (precision > 0) {  
    fract_str[fract_len++] = '.'; 
    for (int i = 0; i < precision; i++) { 
      fract_part *= 10.0;  
      int digit = (int)fract_part; 
      fract_str[fract_len++] = digit + '0';   
      fract_part -= digit;  
    }
  }
  return fract_len;  
}

char *CreateString_Width(SPEC specif, char *enterString, int lengthOfEnterString){
  char *resultString = malloc(sizeof(char) * (specif.width + 1));
      int j = 0;
      for (int i = 0; i < specif.width; i++)
      {
        if (specif.width - i > lengthOfEnterString)
          resultString[i] = (specif.flag.zero == 0 ? ' ' : '0');
        else
          resultString[i] = enterString[j++];
      }
      resultString[specif.width] = '\0';
      
      
      
      
      
    return resultString;
}

char *CreateString_Revers_Width(SPEC specif, char *enterString, int length){
  char *reversString = malloc(sizeof(char) * (specif.width + 1));
  int j = 0;
  for (int i = 0; i < specif.width ; i++)
  {
    if (i < length)
      reversString[i] = enterString[j++]; 
    else
      reversString[i]= ' ';
  }
  reversString[specif.width] = '\0';
  return reversString;
}

char *StringIntoString(SPEC specif, char *enterString){
  char *resultString;
  char *newEnterString = malloc(sizeof(char) * strlen(enterString));
  newEnterString[0] = '\0';
  strncat(newEnterString, enterString, specif.accuracy);
  
  int lengthOfEnterString = strlen(newEnterString);
  if (specif.width > lengthOfEnterString && (specif.accuracy == -1 || specif.accuracy >=lengthOfEnterString))
  {
    if (specif.flag.minus == 0)
      resultString = CreateString_Width(specif, newEnterString, lengthOfEnterString);
    else
      resultString = CreateString_Revers_Width(specif, newEnterString, lengthOfEnterString);
  }
  else{
   resultString = malloc(sizeof(char) * (lengthOfEnterString + 1));
    for (int i = 0; i < lengthOfEnterString; i++)
      resultString[i] = newEnterString[i];
    resultString[lengthOfEnterString] = '\0';
  }
 return resultString;  
}

char *NumberIntoString_REVERS_Width(int isFlagPlusOrSpace, int width, int countOfNumbers, char *resultString, char *numberIntoString, SPEC specif){
  int j = 0;
  for (int i = isFlagPlusOrSpace; i < width + isFlagPlusOrSpace; i++)
  {
    if (i - isFlagPlusOrSpace < countOfNumbers )
      resultString[i] = numberIntoString[j++];
    else 
      resultString[i] = ' ';
  }
  resultString =  placeFlagPlusOrSpace(specif, resultString, 0,width);
  return resultString;
}

char *NumberIntoString_REVERS_WidthAndAccuracy(int isFlagPlusOrSpace, int width, int accuracy , int countOfNumbers, char *resultString, char *numberIntoString, SPEC specif){
  int j = countOfNumbers - 1;
      int maxLimit = (countOfNumbers > accuracy ? countOfNumbers : accuracy); 
      for (int i = width + isFlagPlusOrSpace; i >= 0 + isFlagPlusOrSpace; i--)
      {
        if (i - isFlagPlusOrSpace >= maxLimit)
          resultString[i] = ' ';
        else {
          if (j != -1)
            resultString[i] = numberIntoString[j--];
          else
            resultString[i] = '0';
        }
      }
      resultString = placeFlagPlusOrSpace(specif, resultString, 0, width);
      return resultString;
}

char *NumberIntoString_REVERS_Numbers(int isFlagPlusOrSpace, int countOfNumbers, char *resultString , char *numberIntoString, SPEC specif){
      int j = strlen(numberIntoString) - 1;
      int countOfNum = strlen(numberIntoString);
      for (int i = countOfNum  + isFlagPlusOrSpace; i >= 0 + isFlagPlusOrSpace; i--)
      {
        if (i - isFlagPlusOrSpace >= countOfNum)
          resultString[i] = ' ';
        else 
            resultString[i] = numberIntoString[j--];
      }
      resultString = placeFlagPlusOrSpace(specif, resultString,  0, countOfNum + isFlagPlusOrSpace );
      countOfNumbers = countOfNumbers;
return resultString;
}

char *NumberIntoString_REVERS_Accuracy(int isFlagPlusOrSpace,  int accuracy , int countOfNumbers, char *resultString, char *numberIntoString, SPEC specif){
      int j = countOfNumbers -1;
      int nextStart = accuracy - 1;
      for (int i = accuracy + isFlagPlusOrSpace; j >=0; i--)
      {
        resultString[i-1] = numberIntoString[j--];
        nextStart = i-2;
      }
      for (int i = nextStart; i>= 0; i--)
        resultString[i] = '0';
      resultString = placeFlagPlusOrSpace(specif, resultString,  0, accuracy + isFlagPlusOrSpace );
  return resultString;
}

char *NumberIntoString_REVERS(SPEC specif,unsigned long int chislo){
  char *resultString;
  int countOfNumbers = Unsigned_sizeOfNumber(chislo);
  int isFlagPlusOrSpace = (((specif.flag.plus == 1 || specif.flag.space == 1) || (specif.sign == -1)) && specif.symbol != 'u'? 1 : 0);
  int width = specif.width;
  int accuracy = specif.accuracy;
  char *numberIntoString = malloc(sizeof(char) * (countOfNumbers + 2));
  numberIntoString = myItoa_Unsigned(chislo, numberIntoString, specif.system, specif);
 countOfNumbers = strlen(numberIntoString);
  if (width > countOfNumbers && width > accuracy)
  {
    resultString = malloc(sizeof(char) * (isFlagPlusOrSpace + width + 1)); 
    if (accuracy == -1 || accuracy == 0){
      if (chislo == 0 && accuracy == 0 )
          resultString = NumberIntoString_ZeroAccuracy(specif,  resultString, width);
        else
          resultString = NumberIntoString_REVERS_Width(isFlagPlusOrSpace, width, countOfNumbers, resultString, numberIntoString, specif);
      }
    else 
        resultString = NumberIntoString_REVERS_WidthAndAccuracy(isFlagPlusOrSpace,width, accuracy, countOfNumbers, resultString, numberIntoString, specif);
  }
  else{
    if (countOfNumbers >= accuracy){
      
      resultString = malloc(sizeof(char) * (countOfNumbers + 1 + isFlagPlusOrSpace + 1));
      resultString = NumberIntoString_REVERS_Numbers(isFlagPlusOrSpace, countOfNumbers, resultString, numberIntoString, specif);
    }
    else{
      resultString = malloc(sizeof(char) * (accuracy + 1 + isFlagPlusOrSpace));
      resultString  = NumberIntoString_REVERS_Accuracy(isFlagPlusOrSpace,accuracy,countOfNumbers, resultString, numberIntoString, specif);
    }
  }
  free(numberIntoString);
  return resultString;
}

char *placeFlagPlusOrSpace(SPEC specif, char *resultString,  int index_test, int limit){ 
  if(specif.symbol == 'u'){
    resultString[index_test] = resultString[index_test];
  }else if(specif.flag.hash && specif.symbol != 'd'){
    if (specif.system == 16) {
      resultString[index_test] = '0';
      resultString[index_test+1] = ((char)specif.symbol == 'x'? 'x' : 'X');
    } 
    else if (specif.system == 8) resultString[index_test] = '0';
  }
  else if (specif.flag.plus == 1 || specif.sign == -1)
    resultString[index_test] = ((specif.sign == -1)? '-' : '+');
  else if (specif.flag.space == 1)
    resultString[index_test] = (specif.sign == -1  ? '-' : ' ');
  resultString[limit] = '\0';
  return resultString;
}

char *NumberIntoString_Numbers(int  countOfNumbers, SPEC specif,unsigned long int chislo, int isFlagPlusOrSpace, char *resultString){
  char *numberAsString = malloc(sizeof(char) * (countOfNumbers + 1 + 1));
  numberAsString = myItoa_Unsigned(chislo, numberAsString, specif.system, specif);
  int sizeNumbers = strlen(numberAsString);

  for (int i = isFlagPlusOrSpace; i < sizeNumbers + isFlagPlusOrSpace; i++)
    resultString[i] = numberAsString[i - isFlagPlusOrSpace];
  if (isFlagPlusOrSpace == 1)
    resultString = placeFlagPlusOrSpace(specif, resultString,  0, sizeNumbers + isFlagPlusOrSpace) ;
  else
    resultString[strlen(numberAsString) + isFlagPlusOrSpace] = '\0';
  free(numberAsString);
return resultString;
}

char *NumberIntoString_Accuracy(char *resultString, SPEC specif, int accuracy, int countOfNumbers, unsigned long int chislo, int isFlagPlusOrSpace){
  char *numberAsString = malloc(sizeof(char) * (countOfNumbers + 1));
  numberAsString = myItoa_Unsigned(chislo, numberAsString, specif.system, specif);
  int sizeNumbers = strlen(numberAsString);
  int j =0;
  for (int i = 0; i < accuracy + isFlagPlusOrSpace ; i++)
  {
    if (i < accuracy - sizeNumbers + isFlagPlusOrSpace  )
      resultString[i] = '0';
    else
      resultString[i] = numberAsString[j++];
  }
  resultString = placeFlagPlusOrSpace(specif, resultString,  0, accuracy + isFlagPlusOrSpace);
  resultString[accuracy + isFlagPlusOrSpace] = '\0';
  free(numberAsString);
  return resultString;
}

char *NumberIntoString_Width(SPEC specif, int countOfNumbers,  unsigned long int chislo, char *resultString, int width){
  int indexOfFirtNumber = 0;
  char *numberAsString = malloc(sizeof(char) * (countOfNumbers + 1));
  numberAsString = myItoa_Unsigned(chislo, numberAsString, specif.system, specif);
  int length = strlen(numberAsString);
  for (int i = 0; width - i > length; i++)
  {
    resultString[i] = (specif.flag.zero == 0? ' ' : '0');
    indexOfFirtNumber = i +1;
  }

  int j = 0;
  for (int i = indexOfFirtNumber; i <= width; i++)
    resultString[i] = numberAsString[j++];
  resultString = placeFlagPlusOrSpace(specif, resultString,  indexOfFirtNumber-1, width);
  free(numberAsString);
  return resultString;
}

char *NumberIntoString_ZeroAccuracy(SPEC specif, char *resultString, int width){
  for (int i = 0; i < width; i++)
  {
    resultString[i] = (specif.flag.zero == 0 ? ' ' : '0');
  }
  int index_target = (specif.flag.minus == 1 ? 0 : width-1);
  if( specif.symbol == 'u'){
    if (specif.flag.space == 1)
      resultString[index_target] = ' '; 
  }
  else if (specif.flag.plus == 1 || specif.sign == -1)
  {
    resultString[index_target] = ((specif.sign == -1)? '-' : '+');
  }
  else if (specif.flag.space == 1){
    resultString[index_target] = (specif.sign == -1? '-' : ' ');
  }
  resultString[width ] = '\0';
  return resultString;
}

char *NumberIntoString_WidthAndAccuracy(int accuracy, int width , int countOfNumbers, SPEC specif, char *resultString, unsigned long int chislo, int isFlagPlusOrSpace){
  int indexOfFirtNumber = 0;
    char *numberAsString = malloc(sizeof(char) * (countOfNumbers + 1));
  numberAsString = myItoa_Unsigned(chislo, numberAsString, specif.system, specif);
  int countNumbers = strlen(numberAsString);
  int limitLength = (accuracy > countNumbers ? accuracy : countNumbers);
  for (int i = 0; width - i - isFlagPlusOrSpace > limitLength ; i++)
  {
    resultString[i] = (specif.flag.zero == 0 ? ' ' : '0');
    
    
    indexOfFirtNumber = i + 1;
  }
  int j = 0;
  for (int i = indexOfFirtNumber; i < width + isFlagPlusOrSpace; i++)
  {
    if (width - i > countNumbers)
      resultString[i] = '0';
    
    else
      resultString[i] = numberAsString[j++];
  }
  resultString = placeFlagPlusOrSpace(specif, resultString,  indexOfFirtNumber - 1 + isFlagPlusOrSpace, width+ isFlagPlusOrSpace); 

  free(numberAsString);
  return resultString;
}

char *NumberIntoString(SPEC specif,unsigned long int chislo){
  char *resultString;
  int countOfNumbers = Unsigned_sizeOfNumber(chislo);
  int width = specif.width;
  int isFlagPlusOrSpace = (((specif.flag.plus == 1 || specif.flag.space == 1) && specif.symbol != 'u') || (specif.sign == -1)? (specif.system == 16 ? 2 : 1) : 0);
  int accuracy = specif.accuracy;
  if (width > countOfNumbers && width > accuracy)
  {
    resultString = malloc(sizeof(char) * (width + 1 + isFlagPlusOrSpace));
    if (accuracy <= 0)
    {
      if (chislo == 0 && accuracy == 0 )
        resultString = NumberIntoString_ZeroAccuracy(specif,  resultString, width);
      else
        resultString = NumberIntoString_Width( specif, countOfNumbers, chislo,resultString, width);
    }
    else
      resultString = NumberIntoString_WidthAndAccuracy( accuracy, width ,  countOfNumbers, specif, resultString,  chislo, isFlagPlusOrSpace);
  }
  else{
    if (countOfNumbers >= accuracy){
      resultString = malloc(sizeof(char) * (countOfNumbers + 1 + isFlagPlusOrSpace));
      resultString = NumberIntoString_Numbers(countOfNumbers,  specif, chislo, isFlagPlusOrSpace, resultString);
    }
    else {
      resultString = malloc(sizeof(char) * (accuracy + 1 + isFlagPlusOrSpace));
      resultString = NumberIntoString_Accuracy(resultString, specif, accuracy, countOfNumbers, chislo, isFlagPlusOrSpace);
    }
  }
  return resultString;
}

int s21_exp_convert_fract_to_string(double fract_part, int precision,char *fract_str) {
  int fract_len = 0;
  if (precision > 0) {  
    fract_str[fract_len++] = '.'; 
    for (int i = 0; i < precision; i++) { 
      fract_part *= 10.0;  
      int digit = (int)fract_part; 
      fract_str[fract_len++] = digit + '0';   
      fract_part -= digit;  
    }
  }
  return fract_len;  
}

char *ExponentIntoString(SPEC specif, long double number) {
    if (number == 0.0) {
        return HandleZeroCase(specif);
    }

    int sign, count;
    int _signed = NormalizeNumber(&number, &count, &sign);

    long int precision = specif.accuracy == -1 ? 6 : (specif.accuracy == 0 ? 0 : specif.accuracy);
    long int partBefore;
    long double partAfter;
    CalculateParts(number, sign, precision, &partBefore, &partAfter);

    char *temp = CreateExponentString(specif, partBefore, partAfter, precision, count, _signed);
    char *finalString = FloatAddFlags(specif, temp, strlen(temp), sign);

    free(temp);
    return finalString;
}

char *HandleZeroCase(SPEC specif) {
    long int precision = specif.accuracy == -1 ? 6 : specif.accuracy;
    long double rounding = 0.5 * pow(10, -precision);
    long double normalNumber = fabsl(0.0 + 1 * rounding);

    long int partBefore = 0;
    long double partAfter = normalNumber - partBefore;

    char partBeforeString[2];
    partBeforeString[0] = '0';
    partBeforeString[1] = '\0';

    char *partAfterString = malloc(sizeof(char) * (precision + 1 + 4));
    int length = s21_exp_convert_fract_to_string(partAfter, precision, partAfterString);
    partAfterString[length] = (specif.symbol == 'e' ? 'e' : 'E');
    partAfterString[length + 1] = '+';
    partAfterString[length + 2] = '0';
    partAfterString[length + 3] = '0';
    partAfterString[length + 4] = '\0';

    char *temp = malloc(sizeof(char) * (strlen(partBeforeString) + strlen(partAfterString) + 1));
    temp[0] = '\0';
    strncat(temp, partBeforeString, 1);
    strncat(temp, partAfterString, precision + 4 + 2);

    char *finalString = FloatAddFlags(specif, temp, strlen(temp), 1);
    free(temp);
    free(partAfterString);
    return finalString;
}

int NormalizeNumber(long double *number, int *count, int *sign) {
    *sign = (*number < 0 ? -1 : 1);
    *count = 0;
    int _signed = 0;
    unsigned long int integerPart = (unsigned long int)*number;

    if (integerPart > 0) {
        _signed = 1;
        while (fabsl((double)((int)(*number))) > 9.999999) {
            *number /= 10;
            integerPart = (unsigned long int)*number;
            (*count)++;
        }
    } else {
        _signed = -1;
        while (fabsl((double)((int)(*number))) < 0.999999999) {
            *number *= 10;
            integerPart = (unsigned long int)*number;
            (*count)++;
        }
    }

    return _signed;
}

void CalculateParts(long double number, int sign, long int precision, long int *partBefore, long double *partAfter) {
    long double rounding = (precision == 0 ? 0 : 0.5 * pow(10, -precision));
    long double normalNumber = fabsl(number + sign * rounding);
    *partBefore = (long int)normalNumber;
    *partAfter = normalNumber - *partBefore;

    if (*partBefore + '0' > 57) {
        *partBefore -= 9;
    }
}

char *CreateExponentString(SPEC specif, long int partBefore, long double partAfter, int precision, int count, int _signed) {
    char partBeforeString[2];
    partBeforeString[0] = partBefore + '0';
    partBeforeString[1] = '\0';

    char *partAfterString = malloc(sizeof(char) * (precision + 1 + 4));
    int length = s21_exp_convert_fract_to_string(partAfter, precision, partAfterString);

    partAfterString[length] = (specif.symbol == 'e' ? 'e' : 'E');
    partAfterString[length + 1] = (_signed == 1 ? '+' : '-');
    partAfterString[length + 2] = (count > 9 ? '0' + count / 10 : '0');
    partAfterString[length + 3] = '0' + count % 10;
    partAfterString[length + 4] = '\0';

    char *temp = malloc(sizeof(char) * (strlen(partBeforeString) + strlen(partAfterString) + 1));
    temp[0] = '\0';
    strncat(temp, partBeforeString, 1);
    strncat(temp, partAfterString, precision + (count > 99 ? 5 : 4) + 2);

    return temp;
}

SPEC check_specification(char **resume) {
  SPEC specif;  
  initialize(&specif);
  char *pointer = *resume;
  int flagToExit = 0;
  while (flagToExit == 0) {
    switch (*++pointer){
      case 'd':
      case 'f':
      case 'c':
      case 'o':
      case '%':
      case 's':
      case 'u':
      case 'x':
      case 'X':
      case 'e':
      case 'E':
      case 'g':
      case 'G':
      case 'p':
        if (*pointer == 'x' || *pointer == 'X')
          specif.system = 16;
        else if (*pointer == 'o')
          specif.system = 8;
        flagToExit = 1;
        specif.symbol = *pointer;  
        break;
      case '*':
        if (specif.point == 1) specif.ac.star = 1; 
        else specif.wi.star = 1;
        break;
      case '+':
        specif.flag.plus = 1;  
        break;
      case '-':
        specif.flag.minus = 1;  
        break;
      case '0':
        if (specif.numbers == 0 && specif.point == 0)  
        {
          specif.flag.zero = 1;  
        } else {
          if (specif.point == 0)  
          {
            specif.width =1;  
            addNewNumber(&specif, *pointer,1);  
          } else {
            specif.accuracy =1;  
            addNewNumber(&specif, *pointer,0);  
          }
        }
        break;
      case '.':  
        specif.point = 1;
        break;
      case ' ':
        specif.flag.space = 1;  
        break;
      case '#':
        specif.flag.hash = 1;  
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
        if (specif.point == 0)  
        {
          specif.width =1;  
          addNewNumber(&specif, *pointer, 1);
        } else {
          
          specif.accuracy = 1;  
          addNewNumber(&specif, *pointer, 0);
        }
        specif.numbers += 1;  
        break;
      case 'h':
        specif.h = 1;
        break;
      case 'l':
        specif.l = 1;
        break;
    }
  }
  if (specif.width == 1)
    specif.width = converterToLongLong(specif.wi.width, &specif, 1);
  if (specif.accuracy == 1)
    specif.accuracy = converterToLongLong(specif.ac.accuracy, &specif, 0);
  
  *resume = pointer;
  return specif;
}

long long converterToLongLong(char *chislo, SPEC *spec, int choose) {
  long long result = 0;
  int length = (choose == 1 ? spec->wi.inWidth : spec->ac.inAccuracy);
  int j = 0;
  for (int i = length - 1; i >= 0; i--) {
    int num = takeDigit(chislo[i]);
    if (num != -1) {
      result = result + (num * (j == 0 ? 1 : pow(10, j)));
      j++;
    }
  }
  return result;
}

int takeDigit(char chislo) {
  int num = -1;
  switch (chislo) {
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

void addNewNumber(SPEC *specif, char symbol, int choose) {
  if (choose == 1) {    
    if (specif->wi.inWidth == specif->wi.posWidth) {
      specif->wi.posWidth += 1;
      specif->wi.width =
          realloc(specif->wi.width, sizeof(char) * specif->wi.posWidth);
    }
    specif->wi.width[specif->wi.inWidth++] = symbol;
  } else {    
    if (specif->ac.inAccuracy == specif->ac.posAccuracy) {
      specif->ac.posAccuracy += 1;
      specif->ac.accuracy =
          realloc(specif->ac.accuracy, sizeof(char) * specif->ac.posAccuracy);
    }
    specif->ac.accuracy[specif->ac.inAccuracy++] = symbol;
  }
}

void strrev2(char *arr, int start, int end) {
  char temp;
  if (start >= end) return;
  temp = *(arr + start);
  *(arr + start) = *(arr + end);
  *(arr + end) = temp;
  start++;
  end--;
  strrev2(arr, start, end);
}

char *myItoa_Unsigned(unsigned long int number, char *arr, int base, SPEC specif) {
  int i = 0, r;
  if (number == 0) {
    arr[i] = '0';
    arr[i + 1] = '\0';
    return arr;
  }
  while (number != 0) {
    r = number % base;
    arr[i] = (r > 9) ? (r - 10) + (specif.system == 16 && (char)specif.symbol == 'x' ? 'a' : 'A' ) : r + '0';
    i++;
    number /= base;
  }
  
  strrev2(arr, 0 , i-1);  
  arr[i] = '\0';
    
  return arr;
}

void initialize(SPEC *specif) {
  specif->flag.hash = 0;
  specif->flag.minus = 0;
  specif->flag.plus = 0;
  specif->flag.space = 0;
  specif->flag.zero = 0;
  specif->accuracy = -1;
  specif->point = 0;
  specif->h = 0;
  specif->l = 0;
  specif->symbol = 0;
  specif->width = 0;
  specif->numbers = 0;
  specif->wi.inWidth = 0;
  specif->wi.posWidth = 2;
  specif->wi.width = malloc(sizeof(char) * specif->wi.posWidth);
  specif->ac.inAccuracy = 0;
  specif->ac.posAccuracy = 2;
  specif->ac.accuracy = malloc(sizeof(char) * specif->ac.posAccuracy);
  specif->system = 10;
  specif->sign = 0;
}

int Unsigned_sizeOfNumber(unsigned long int copyChisla) {
  int size = 1;
  int d = 0;
  while (d != -1) {
    d = copyChisla % 10;
    copyChisla /= 10;
    if (copyChisla == 0) {
      d = -1;
    } else
      size++;
  }
  return size;
}

void convertPointerToString(void *pointer, char *string) {
  unsigned long pointerCopy = (unsigned long)pointer;
  char hexDigits[] = "0123456789abcdef";

  
  string[14] = '\0';
  for (int i = 13; i >= 0; i--) {
    string[i] = hexDigits[pointerCopy % 16];  
    pointerCopy /= 16;  
  }

  string[1] = 'x';   
  string[0] = '0'; 
}

void clearMemory(SPEC *specif){
  free(specif->ac.accuracy);
  free(specif->wi.width);
}