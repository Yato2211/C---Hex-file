int checkLine(char* line){                          // Return type of record, -1 if error
    int lineSize = strlen(line);
    if (lineSize>MAX_BYTE_TWICE                           // Validate size of line
        || line[0]>0x31
        || lineSize<MIN_BYTE_TWICE 
        ){
            puts("INVALID RECORD LENGTH");          return -1;
        }                                           
    for (int i=0; i<lineSize;i++){                  // Validate ASCII characters of line
        if (checkASCII(&line[i]) == 0){
            puts("RECORD HAS INVALID CHARACTER(S)");  return -1;
        }              
        else 
            if (line[i] > '9') line[i] = line [i] - 0x07;
    }
    int dataFieldLength = (line[0]-'0')*16 + (line[1]-'0');
    if (dataFieldLength > MAX_DATA_BYTE_TWICE){                // Validate Length of data field
        puts("RECORD HAS INVALID DATA FIELD LENGTH"); return -1;
    }      
    int trueLineSize = 10 + dataFieldLength*2;                 // Validate Length of line
    if (lineSize != trueLineSize){
        puts("RECORD HAS INVALID LENGTH");            return -1;
    }                   
    int type;
    if (line[6] == '0'){                                       // Validate Type of record
        switch (line[7])
        {
        case '0':
            type = 0;
            break;
        case '1':
            type = 1;
            break;
        case '2':
            type = 2;
            break;
        case '4':
            type = 4;
            break;
        case '5':
            type = 5;
            break;
        default:
            type = -1;
            break;
        }
    }   else{
            puts("INVALID RECORD TYPE");              return -1;
        }                                                                   
    return type;
}