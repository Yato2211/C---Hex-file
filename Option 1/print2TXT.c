int print2TXT(char* argv, FILE* hexFile, FILE* TXTfile){
    char TXTname[strlen(argv)+1];                       // Create .TXT file name
    strncpy(TXTname, argv, strlen(argv) - 4);
    TXTname[strlen(argv)-4] = '\0';
    strncat(TXTname,".txt",5);
    TXTfile = fopen(TXTname,"w");
    hexFile = fopen(argv,"r");
    char line[MAX_BYTE_TWICE], getCRLF[10];
    int i=0;
    record lineRecord;
    record* recordPt = &lineRecord;
    lineRecord.Count = 0;
    strcpy(lineRecord.extraAdd,"0000");
    while (feof(hexFile) == 0)
    {
        fscanf(hexFile,":%s",line);
        fgets(getCRLF,10,hexFile);
        lineRecord.Count++;
        int recordType = checkLine(line);
        switch (recordType)
        {
        case 0:
            strcpy( lineRecord.lineIn, line);
            convertLine(recordPt);
            fwrite(lineRecord.lineOut, 1, strlen(lineRecord.lineOut), TXTfile);
            break;
        case 1:
            printf("End Record found at line: %d\nEND PROGRAM", lineRecord.Count);
            fclose(TXTfile);
            fclose(hexFile);
            return 1;
        case 2:
            break;
        case 4:
            for(i=0; i<4; i++){
                lineRecord.extraAdd[i] = line[i+8];
            }
            break;
        case 5:
            break;
        default:
            printf("Incorrect Format at line: %d\nEND PROGRAM", lineRecord.Count);
            fclose(TXTfile);
            fclose(hexFile);
            return -1;
        }
    }
    printf("LINE %d: End of HEX file. End Record not found\nEND PROGRAM",lineRecord.Count);
    fclose(TXTfile);
    fclose(hexFile);
    return 1;
}