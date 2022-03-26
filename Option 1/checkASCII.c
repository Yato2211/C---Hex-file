int checkASCII(char* lineChar){                     // Returns 0 if not an ASCII character
    if ((*lineChar >= 0x30) && (*lineChar <= 0x39)) return 1;
    if ((*lineChar >= 0x41) && (*lineChar <= 0x46)) return 1;
    if ((*lineChar >= 0x61) && (*lineChar <= 0x66)){
        *lineChar = *lineChar - 0x20;                return 1;
    };
    return 0;    
}