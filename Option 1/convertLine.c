void convertLine(record* lineRecord){           // Returns pointer to converted lineRecord
    int i=0;
    int hexValue;
    lineRecord->lineOut[0] = '\0';
    strncat(lineRecord->lineOut,"|",1);                          // '|' and ' ' format display appearance
    strncat(lineRecord->lineOut,lineRecord->extraAdd,4);
    strncat(lineRecord->lineOut,(lineRecord->lineIn + 2),4);     // Add Address
    int dataFieldLength = (lineRecord->lineIn[0]-'0')*16 + (lineRecord->lineIn[1]-'0');
    for (i=0; i< dataFieldLength; i++){                         // Add Hex Data field
        strncat(lineRecord->lineOut," ",1);
        strncat(lineRecord->lineOut,(lineRecord->lineIn + 8 +i*2),2);
    }
    if (dataFieldLength < 16){                                   // Formatting appearance
        for (i=0; i<(16 - dataFieldLength);i++){
            strncat(lineRecord->lineOut,"   ",3);
        }
    }
    strncat(lineRecord->lineOut,"|",1);
    char ASCIIchar[1];
    for (i=0; i<dataFieldLength;i++){                           // Add ASCII Data
        ASCIIchar[0] = (lineRecord->lineIn[i*2+8]-'0')*16 + (lineRecord->lineIn[i*2+9]-'0');
        strncat(lineRecord->lineOut,ASCIIchar,1);
    }
    if (dataFieldLength < 16){                                   // Formatting appearance
        for (i=0; i<((16 - dataFieldLength));i++){
            strncat(lineRecord->lineOut," ",1);
        }
    }
    strncat(lineRecord->lineOut,"\n",1);
    for (i=0; i<76; i++){
        if ((lineRecord->lineOut[i] > 0x39) && (lineRecord->lineOut[i] < 0x41)){
            lineRecord->lineOut[i] = lineRecord->lineOut[i] + 0x07; 
        }
    }
}
