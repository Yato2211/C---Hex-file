#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define MAXLENGTH  100

unsigned char hex_result[MAXLENGTH];

unsigned int address = 0;

//tuong ung voi cac dia chi tu 0x0000h -> 0xFFFFh
unsigned char data[65536]; 


//Kiem tra dinh dang mo rong cua file 
bool check(char *input_file)
{
	if(strstr(input_file,".hex")) return true;

	if(strstr(input_file,".h86")) return true;

	else return false;
}


//Chuyen doi ki tu he hexa sang ki tu he thap phan
int hextodec(char h)
{
    int num = 0;

	if ((h <= 'f') && ( h >= 'a')) num = h - 'a' + 10;
    
    else if((h <= '9') && (h >= '0')) num = h - '0';
    
    else num = h - 'A' + 10;
    
    return num;
}



//Doc va kiem tra tung dong du lieu
int scan_line(char *line, int nLength)
{
   	unsigned char c1, c2, sum;
	//kiem tra dinh dang ki tu dau tien ":"
	if (line[0] != ':') return 0; 

   	int i = 1, j = 0;

	// -1 di ki tu '\n'
	while( i < nLength-1) 
   	{
    	c1 = hextodec(line[i++]);

      	c2 = hextodec(line[i++]);

      	sum = c2 | (c1 << 4);

      	hex_result[j++] = sum;
   }
   	unsigned  char  byte_count;

    unsigned  int   check_address;

	unsigned  char  record_type;

	//kiem tra dinh dang xem co du cac thanh phan khong
	if(j<5) return 0;
	if(hex_result[0]+5 != j) return 0;

	byte_count = hex_result[0];

	//kiem tra byte count co dung khong
	if (byte_count == 0) return 1; 

	//kiem tra dia chi
	check_address = hex_result[2] | ((unsigned int)hex_result[1] << 8);

	//Khong doc vung nho lon hon 64Kb
	if(check_address > 65535) return 1;

	record_type = hex_result[3];

	//record type
	if (record_type != 0) return 1;

	unsigned int stack = 0;

	while(stack < byte_count)
	{
		//Lay du lieu va luu vao tung o dia chi tuong ung
		data[stack + check_address] = hex_result[stack+4];

		stack = stack + 1;
	}
	if(address < stack + check_address -1)

	//Xac dinh o dia chi cuoi cung chua du kieu cua file
	address = stack + check_address -1;

   	return j;
}


//In du lieu trong file
void hex_print()
{
	unsigned int i = 0;

	int c = 0;
	
	//In du lieu den o chua du lieu cuoi cung
	while(i <= address){

		//In vung 1: dia chi
		printf("%04x  ",i);

		//Xac dinh so o du lieu in ra tren 1 dong
		//1 dong se in ra 16 o du lieu
		// neu khong du thi in den dia chi lon nhat
		int n; 

		int k = 0;

		if (15<address - i) n=15;

		else n=address - i;

		//In vung 2: du lieu cho tung dia chi duoi dang ki tu hexa
		//Toi da 16 o nho tren 1 dong
		for(k = 0; k <= n; k++) printf(" %02x",data[i+k]);
		   
		//In cho du 16 vi tri tren 1 dong cho dung dinh dang
		//neu du lieu dau vao khong du de in tiep
		for(;k<16;k++) printf("   ");
		
		printf("   ");

		//In vung 3: in du lieu duoi dang ki tu ASCII
		for(k = 0; k <= n; k++) printf("%c",data[i+k]);

		printf("\n");

		i = i + 16;

		c=c + 1;
		
		//Kiem tra hien thi du 25 dong
		if(c == 25)
		{
			printf("\nPress any key to continue displaying data");

			printf("\nPress 's' to stop data display\n");

			if(getch() == 's') break;

			if(getch() == 'S') break;

			system("cls");

			c = 0;
		}
	}
}


int main(int argc, char *argv[])
{
	int num;

   	FILE *fptr;

   	char input_file[MAXLENGTH];

	unsigned char input_line[MAXLENGTH];

	int line = 0;

   	if(argc > 1) strcpy(input_file,argv[1]);

    if(!check(input_file))
    {
    	printf("Error:%s could not be opened. ", input_file);

    	exit(1);
	}	
	fptr = fopen(input_file,"r");

	if(fptr == NULL)
   	{
      	printf("Error:%s could not be opened.", input_file);  

      	exit(1);             
   	}

   	//khoi tao cac o dia chi rong chua ki tu FF
   	int i = 0;

   	for(i = 0; i<65536; i++) data[i] = 255;

	while(fgets(input_line, MAXLENGTH, fptr) != NULL)
   	{
		//doc tung dong va kiem tra xem co dung dinh dang khong
    	int check_line = scan_line(input_line, strlen(input_line)); 

    	line = line + 1;

      	if(check_line == 0)
		  	{
      		printf("\nError: line %d is malformed.\n",line);

      		return 0;
	  		}
   	}

   	//In file ra man hinh voi format yeu cau
   	hex_print();
 
   	
   	if(feof(fptr)) printf ("\nRead to the end of the file: %s", input_file);  

   	else printf ("\nError: the line is too long in file : %s", input_file);  

	fclose(fptr);

	return 0;
}

// Coder by: Nguyen Quang Trung.
// MSSV    : 20181796.