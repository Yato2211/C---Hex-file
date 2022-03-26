#include <iostream>
#include <sstream>
#include <fstream>
#include <array>
#include <stdlib.h>
#include <vector>
#include <iomanip>

#define data_record (0)
#define eof_record (1)
#define extended_segment_addr_record (2)
#define extended_linear_addr_record (4)
#define start_liner_addr_record (5) //MDK-ARM only
#define MAX_ROW 25
#define cmd_line_display_mode 0
#define txt_file_display_mode 1

using namespace std;
typedef enum {
    NO_ERROR = 0,
    FORMAT_ERROR,
    CHECKSUM_ERROR,
    OPEN_FILE_ERROR
}error_t;

string error_line;
int error_line_number;
int is_extended = 0;

error_t displayHexFile(char* file_name);
error_t isHexFile(char* hex_file_name);
error_t checkSum(string line);
error_t checkLineFormat(string line);
int toInt(char* field, int length);

int main(int argc, char** argv)
{
    if (!isHexFile(argv[1]))
    {
        cout << argv[1] << " is not a hex file.";
    }
    else
    {
        error_t res = displayHexFile(argv[1]);
        switch (res)
        {
            case (OPEN_FILE_ERROR):
                cout << argv[1] << " could not be opened.";
                break;
            case (CHECKSUM_ERROR):
                cout << "Error checksum at line " << error_line_number << ": " << error_line;
                break;
            case (FORMAT_ERROR):
                cout << "Error format at line " << error_line_number << ": " << error_line;
                break;
            default: 
                cout << "Display " << argv[1] << " successfully";
                break;
        }
        
    }
    return 0;
}

/**
 * @brief check if it's a hex file
 * @param hex_file_name name of input file
 * @return  true or false
 */
error_t isHexFile(char* hex_file_name)
{
    string tail = "";
    stringstream temp;
    temp << hex_file_name;
    string file_name;
    temp >> file_name;
    for (int outterIndex = 0; outterIndex < file_name.length(); outterIndex++)
    {
        if (file_name[outterIndex] == '.')
        {
            for (int innerIndex = outterIndex + 1; innerIndex < file_name.length() ; innerIndex++)
            {
                tail += file_name[innerIndex];
            }
            break;
        }
    }
    if (tail != "hex")
        return NO_ERROR;
    return OPEN_FILE_ERROR;
}

/**
 * @brief check if checksum of a line is right
 * @param line input line
 * @return  true or false
 */
error_t checkSum(string line)
{
    unsigned int index = 1;
    unsigned long sum = 0;
    char record[2];
    char checkSumField[2];
    while (index < line.length() - 2)
    {
        size_t length = line.copy(record, 2, index);
        record[length] = '\0'; 
        sum += toInt(record, 2);
        index += 2;
    }
    line.copy(checkSumField, 2, index);
    checkSumField[2] = '\0';
    sum = int (((~sum) + 0x01) & 0xFF); 
    if (sum == toInt(checkSumField, 2))
        return NO_ERROR;
    return CHECKSUM_ERROR;
}

/**
 * @brief 
 * @param 
 * @return  
 */
error_t checkLineFormat(string line)
{
    if ((line[0] != ':') || (line.length() < 11))
        return FORMAT_ERROR;
    for (int index = 1; index < line.size(); index++)
    {
        char c = toupper(line[index]);
        if (((c < '0') || (c > '9')) && ((c < 'A') || (c > 'F')))
            return FORMAT_ERROR;
    }
    return NO_ERROR;
}

/**
 * @brief convert hex string into int number
 * @param field input string
 * @param length length of that string
 * @return  converted number
 */
int toInt(char* field, int length)
{   
    int result = 0;
    int exp = 1;
    for (int index = length - 1; index >= 0; index--)
    {
        int digit = toupper(field[index]) - '0';
        if (digit > 9)
            digit -= 7;
        result += digit * exp;
        exp *= 16;
    }
    return result;
}


/**
 * @brief display hex file
 * @param hex_file_name name of input file
 * @return  error_t result
 */
error_t displayHexFile(char* hex_file_name)
{
    fstream hexFile;
    fstream txtFile;
    hexFile.open(hex_file_name, ios_base::in);
    if (!hexFile.is_open())
    {
        return OPEN_FILE_ERROR;
    }

    int line_number = 0;//number of line got from file
    char numOfBytes[3];//numofbytes record in a hex file record
    char lower_addr[5];//lower_addr record in a hex file record
    char higher_addr[5];//higher_addr record in a hex file record
    char recordType[3];//type record in a hex file record
    int addr = 0;
    int end_of_file = 0;
    int compensation = 1;//compensation for space betwwen twpo column when display
    int empty_data = 0xFF;
    int display_mode;
    error_t isHexFileError = NO_ERROR;
    string txt_file_name;
    string line;
    int row = 0;
    vector<int> data;//buffer for storing data
    size_t length;
   
    data.clear();

    while (getline(hexFile, line))
    {
        line_number++;
        if (checkLineFormat(line) == FORMAT_ERROR)
        {
            isHexFileError = FORMAT_ERROR;
            error_line = line;
            error_line_number = line_number;
            break;
        }
        else
        {
            if (checkSum(line))
            {
                isHexFileError = CHECKSUM_ERROR;
                error_line = line;
                error_line_number = line_number;
                break;
            }
            else
            {   
                length = line.copy(numOfBytes, 2, 1);
                numOfBytes[length] = '\0';
                if ((toInt(numOfBytes, 2) * 2) != (line.length() - 11))
                {
                    isHexFileError = FORMAT_ERROR;
                    error_line = line;
                    error_line_number = line_number;
                    break;
                }
                length = line.copy(lower_addr, 4, 3);
                lower_addr[length] = '\0';
                length = line.copy(recordType, 2, 7);
                recordType[length] = '\0'; 
                
                if (toInt(recordType, 2) == extended_linear_addr_record)
                {
                        line.copy(higher_addr, 4, 9);
                        higher_addr[4] = '\0';
                        if (toInt(higher_addr, 4) != 0)
                            is_extended = 1;   
                }
                else
                if (toInt(recordType, 2) == eof_record)
                {
                    end_of_file = 1;
                    break;
                }
                else
                if (toInt(recordType, 2) == extended_segment_addr_record)
                {
                    
                }
                else
                if (toInt(recordType, 2) == data_record)
                {
                    if (is_extended)
                        addr = toInt(higher_addr, 4) * 65536 + toInt(lower_addr, 4);
                    else
                        addr = toInt(lower_addr, 4);
                    if (addr != data.size())
                        for (int index = data.size(); index < addr; index++)
                            data.push_back(empty_data);
                    for (int index = 0; index < toInt(numOfBytes, 2); index++)
                    {
                        char temp[3];
                        line.copy(temp, 2, 9 + (index * 2));
                        temp[2] = '\0';
                        data.push_back(toInt(temp, 2));
                    }
                } 
            }
        }
    }   
    if (isHexFileError)
        return isHexFileError;
    cout << "Choose Mode: \n" 
         << "Mode 1: Display on command-line\n"
         << "Mode 2: Saved in .txt file\n";
    int input_error = 0;
    do 
    {
        cout << "1/2 \n";
        char input;
        cin >> input;
        if ((input == '1') || (input == '2'))
        {
            input_error = 0;
            if (input == '1')
            {
                display_mode = cmd_line_display_mode;
            }
            else
            {
                display_mode = txt_file_display_mode;
            }
        }
        else
        {
            input_error = 1;
            cout << "Syntax error. Just 1 or 2.\n";
        }

    }
    while (input_error);

    if (display_mode == txt_file_display_mode)
    {
        cout << "Get .txt file name: ";
        cin >> txt_file_name;
        txtFile.open(txt_file_name, ios_base::out | ios_base::trunc);
    }
    //if it doesn't have end of file, fill all the remainder with empty_data.

    if ((data.size() > 0) && (data.size() < 0xFFFF) && (isHexFileError == NO_ERROR) && (!end_of_file))
    {
        size_t length = data.size();
        for (int index = length; index <= 0xFFFF; index++)
            data.push_back(empty_data);
    }
    else
    if (data.size() == 0)
    {
        if (display_mode == txt_file_display_mode)
        {
            txtFile << "00000000";
        }
        else
        {
            cout << "00000000" << "\n";
        }
    }
    
                        
    for (int outterIndex = 0; outterIndex < data.size(); outterIndex += 0x10)
    {
        if ((row == MAX_ROW) && (display_mode == cmd_line_display_mode))
        {
            int command_error = 0;
            do
            {
                char command;
                cout << "Do you wanna to continue to print it out? Y/N";
                cin >> command;
                if ((toupper(command) == 'Y') || (toupper(command) == 'N'))
                {
                    if (toupper(command) == 'N')
                        return NO_ERROR;
                    else
                    {
                        system("CLS");
                        row = 0;
                    }
                    command_error = 0;
                }
                else
                {
                    cout << "Syntax Error" << "\n";
                    command_error = 1;
                }
            }while(command_error);
        }

        int data_bytes = 0x10;//bytes in a line

        //display address of first bytes in a line
        if (data.size() < outterIndex + 0x10)
        {
            data_bytes = data.size() - outterIndex;
            compensation = (0x10 - data_bytes) * 3 + 1; 
        }
        char* hexa_addr;
        sprintf(hexa_addr, "%08X", outterIndex);
        if (display_mode == txt_file_display_mode)
        { 
            txtFile << hexa_addr << " ";
        }
        else
        {
            cout << hexa_addr << " ";
        }

        //display 16 bytes in a line
        for (int innerIndex = 0; innerIndex < data_bytes; innerIndex++)
        {
            char* hexa_data;
            sprintf(hexa_data, "%X", data[outterIndex + innerIndex]);
            if (data[outterIndex + innerIndex] >= 0x10)
            {
                if (display_mode == txt_file_display_mode)
                {
                    txtFile << hexa_data << " ";
                }
                else
                {
                    cout << hexa_data << " ";
                }
            }
            else
            {
                
                if (display_mode == txt_file_display_mode)
                {
                    txtFile << "0" << hexa_data << " ";
                }
                else
                {
                    cout << "0" << hexa_data << " ";
                }
            }
                
        }

         //display ascii
        array<char, 16>data_ascii;
        if (display_mode == txt_file_display_mode)
        {
            txtFile << setw(compensation);
        }
        else
        {
            cout << setw(compensation);
        }
        for (int innerIndex = 0; innerIndex < data_bytes; innerIndex++)
        {
            if ((data[outterIndex + innerIndex] >= 32))
            {
                sprintf(data_ascii.data(), "%c", data[outterIndex + innerIndex]);
                if (display_mode == txt_file_display_mode)
                {   
                    txtFile <<data_ascii.data();    
                }
                else
                {
                    cout << data_ascii.data();
                }
            }
            else
            {
                if (display_mode == txt_file_display_mode)
                {   
                    txtFile << ".";    
                }
                else
                {
                    cout<< "."; 
                }
            }
        }
        if (display_mode == txt_file_display_mode)
        {   
            txtFile << "\n";   
        }
        else
        {
            cout << "\n";
        }
        row++;
    }
    hexFile.close();
    txtFile.close();
    return isHexFileError;
}

