int print2Screen(char* argv, FILE* hexFile){
    hexFile = fopen(argv,"r");
    int screenCount=0;
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
            printf("%2d",++screenCount);
            lineRecord.lineOut[MAX_LINE_OUT - 2] = '\0';
            puts(lineRecord.lineOut);
            if (screenCount == 25){
                puts("OUT OF SCREEN (25 LINES).");
                puts("Press 'S' to continue printing/ Press any key to stop.");
                if (printSelect()){
                    screenCount = 0;
                    //clrscr();
                    system("cls");
                }
                else {
                    printf("PRINTING STOPED AT LINE: %d\n",lineRecord.Count);
                    puts("END PROGRAM");
                    fclose(hexFile);
                    return 1;
                }
            }
            break;
        case 1:
            printf("End Record found at line: %d\nEND PROGRAM", lineRecord.Count);
            fclose(hexFile);
            return 1;
        case 2:
            printf("LINE %d -RECORD FOUND: Extended Segment Address\n",lineRecord.Count);
            break;
        case 4:
            for(i=0; i<4; i++){
                lineRecord.extraAdd[i] = line[i+8];
            }
            break;
        case 5:
            printf("LINE %d -RECORD FOUND: Linear address record (MDK-ARM only)\n",lineRecord.Count);
            break;
        default:
            printf("Incorrect Format at line: %d\nEND PROGRAM", lineRecord.Count);
            fclose(hexFile);
            return -1;
        }
    }
    printf("LINE %d: End of HEX file. End Record not found\nEND PROGRAM",lineRecord.Count);
    fclose(hexFile);
    return 1;
}
