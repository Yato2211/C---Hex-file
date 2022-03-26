int openHexFile(char* argv, FILE* hexFile){   
    hexFile = fopen(argv,"r");
    char line[MAX_BYTE_TWICE], getCRLF[10];
    fscanf(hexFile,":%s",line);
    fgets(getCRLF,10,hexFile);
    if (checkLine(line)==-1) {
        printf("Error: %s COULD NOT BE OPENED\n",argv);
        fclose(hexFile);
        return 0;
    }    
    else{
        printf("Message: %s OPENED SUCCESSFULLY\n",argv);
        fclose(hexFile);
        return 1;
    }
}