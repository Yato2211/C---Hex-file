#define MAX_BYTE_TWICE 42
#define MIN_BYTE_TWICE 10
#define MAX_DATA_BYTE_TWICE 16
#define STANDARD_HEX_FILE_SIZE_TWICE 65536*2
#define MAX_LINE_OUT 76
#define clrscr() printf("\e[1;1H\e[2J")
typedef struct Record
{
    int Count;
    char extraAdd[4];
    char lineIn[MAX_BYTE_TWICE];
    char lineOut[MAX_LINE_OUT];
} record;

int checkASCII(char* lineChar);
int checkLine(char* line);
int openHexFile(char* argv, FILE* hexFile);
int printSelect(void);
int print2Screen(char* argv, FILE* hexFile);
int print2TXT(char* argv, FILE* hexFile, FILE* TXTfile);
void convertLine(record* lineRecord);

#include"checkASCII.c"
#include"checkLine.c"
#include"openHexFile.c"
#include"printSelect.c"
#include"print2Screen.c"
#include"print2TXT.c"
#include"convertLine.c"