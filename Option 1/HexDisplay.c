#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"Headers.h"
FILE* hexFile;
FILE* TXTfile;

int main( int argc, char** argv){
 	if ( argc != 2 ){
		puts("Error: NO INPUT FILE\nEND PROGRAM");
		exit (0);
	}
    int openSuccess = openHexFile(argv[1], hexFile);
    if (openSuccess){
        printf("Press 'S' to print HEXFILE to screen. ");
        printf("Press any other key to print HEXFILE to .TXT.\n");
        if (printSelect())
            print2Screen(argv[1], hexFile);
        else print2TXT(argv[1], hexFile, TXTfile);
    }
return 0;
}