typedef struct
{
    int plus;
    int minus;
    int zero;
    int space;
    int hash;
} FLAG;

typedef struct
{
    char *width;
    int inWidth;
    int posWidth;
} WIDTH;

typedef struct
{
    char *accuracy;
    int inAccuracy;
    int posAccuracy;
} ACC;

typedef struct
{
    FLAG flag;
    WIDTH wi;
    ACC ac;
    int numbers;
    long long width;
    int point;
    long long accuracy;
    int symbol;
} SPEC;
