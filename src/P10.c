//made by two mad m8s, looking for social acceptance. So far Dixego is the only one succesful in that matter.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "strmap.h"

typedef char* string;

string *data;
string *text;
int data_length;
int text_length;
int pc;
int registro[32];
int RAM[1024];
string inst_reg;
StrMap *types;
StrMap *ctrl_signals;
string control_signal;

//Here be global variables for actually executing de stuff
int regd, regs1, regs2;
int immediate, address;

void execute1(string op);
void execute2(string op);
void execute3(string op);

string voltea_bits(string str)
{
    int i = 0;
    int len = strlen(str);
    string nstr = malloc(len);
    int j;
    for(j=0; i < len; i++){
        nstr[i] = (str[i] == '0') ? '1' : '0';
    }
    return nstr;
}

string sumale_uno(string str)
{
    int uno = 1;
    int i = strlen(str) -1;
    while(i>=0)
    {
        if(*(str+i) == '1' && uno)
        {
            *(str+i) = '0';
        }else if(*(str+i) == '0' && uno)
        {
            *(str+i) = '1';
            uno = 0; // No tengas miedo;
        }
        i--;
    }
    return str;
}

int b_to_i(string str)
{
    int len = strlen(str);
    int i = len-1, num = 0;
    float in;
    while(i>=0)
    {

        num += (*(str+i) == '1')? pow(2.0, (len-i-1)*1.0) : 0.0;
        i--;
    }
    return num;
} 

// Toma los últimos n caracteres de una cadena y los convierte a un entero (cadena debe estar en complemento a dos)
int strb_to_i(string str, int n)
{
    int strl = strlen(str);
    int i = 0;
    int neg = 0;
    int res;
    string newstring;

    if(strl < n)
    {
        n = strl;
    }
    newstring = str+(strl-n);
    newstring = strdup(newstring);
    if(*newstring == '1')
    {
        neg = 1;
        newstring = voltea_bits(newstring);
        newstring = sumale_uno(newstring);
    }

    res = b_to_i(newstring);
    if(neg)
        res = res*(-1);
    return res;
}

StrMap *populate_types()
{
	StrMap *ops_map = sm_new(18);
	// sorry mom
			// clearly you can see this was Diego's idea.
	string codes[] = {"000000", "00001", "000010", "000011", "000100", "000101", "000110", "000111", "001000", "001001", "001010", "001011", "001100", "001101", "001110", "001111", "010000", "010001"};
	string ops[] = {"i", "i", "i", "i", "i", "i", "r", "i", "r", "i", "r", "i", "r", "i", "i", "i", "j", "r"};
	int i = 0;
	for(i = 0; i < 18; i++)
	{
		sm_put(ops_map, codes[i], ops[i]);
	}

	return ops_map;
}

StrMap *populate_ctrl_signals()
{
	StrMap *signals_map = sm_new(18);
	string codes[] = {"000000", "00001", "000010", "000011", "000100", "000101", "000110", "000111", "001000", "001001", "001010", "001011", "001100", "001101", "001110", "001111", "010000", "010001"};
	string signals[] = {"1100100", "1100100", "1100100", "0100010", "0100010", "0100010", "1110001", "1100001","1110001", "1100001", "1110001", "1100001","1110001","1100001", "1111111", "0101000","0011000", "0101000"};
	int i = 0; 
	for(i = 0; i < 18; i++)
	{
		sm_put(signals_map, codes[i], signals[i]);
	}

	return signals_map;
}

string get_filename(int argc, char *argv[])
{
	string filename = malloc(100); // Arbitrary 100 char limit to file size because why not.
	// If there's less than 2 arguments, read a filename from stdin.
	if(argc < 2)
	{
		printf("Type the name of an assembly file.\n");
		scanf("%s", filename);
	}
	// Else, first argument is the filename
	else
	{
		filename = argv[1];
	}
	return filename;
}


/*
	Tries to read file 'filename' in read mode. If it doesn't exist or something goes otherwise wrong,
	exits the prigram with exit code 1.
*/
FILE *open_file(string filename)
{
	FILE *file;
	file = fopen(filename,"r");

	// If file is not found, exit program with exit code 1.
	if(!file)
	{
		fprintf(stderr, "Something went wrong while reading file \'%s\'\n", filename);
		exit(1);
	}
	return file;
}

int extreme_foo(FILE *file)
{
	string line;
	int datacunt = 0,textcunt = 0;
	line = malloc(33);
	fgets(line, 33, file);
	data_length = strb_to_i(line,33);
	data = malloc(data_length*sizeof(string));
	fgets(line, 33, file);
	text_length = strb_to_i(line,33);
	text = malloc(text_length*sizeof(string));
	
	while(fgets(line, 33, file) != NULL && datacunt < data_length){
		data[datacunt] = strdup(line);
		datacunt++;
	}
	
	while(fgets(line, 33, file) != NULL && textcunt < text_length){
		text[textcunt] = strdup(line);
		textcunt++;
	}
}

void extreme_execution()
{
	string op = malloc(5);
	string tipo = malloc(1);
	for(int i = 0; i < 6; i++)
	{
		op[i] = inst_reg[i];
	}
	//shit shit fuck fuck fuck
	sm_get(types,op, tipo, 1);
	if(strcmp(tipo,"i") == 0)
	{
		execute1(op);
	}else if(strcmp(tipo, "r") ==0)
	{
		execute2(op);
	}else if(strcmp(tipo, "j") == 0)
	{
		execute3(op);
	}
}
void execute1(string op)
{
	pc++;
	int res;
	if(strcmp(op,"000000") == 0) //lw
	{
		registro[regd] = RAM[strb_to_i(data[immediate + regs1],32)];
	}
	else if(strcmp(op,"000001") == 0) //lh
	{
		registro[regd] = RAM[strb_to_i(data[immediate + regs1],32)];
	}
	else if(strcmp(op,"000010") == 0) //lb
	{
		registro[regd] = RAM[strb_to_i(data[immediate + regs1],32)];
	}
	else if(strcmp(op,"000011") == 0) //sw
	{
		RAM[regd] = registro[strb_to_i(data[immediate + regs1],32)];
	}
	else if(strcmp(op,"000100") == 0) //sh
	{
		RAM[regd] = registro[strb_to_i(data[immediate + regs1],32)];
	}
	else if(strcmp(op,"000111") == 0) //addi
	{
		res = registro[regs1] + immediate;
	}
	else if(strcmp(op,"001001") == 0) //subi
	{
		res = registro[regs1] - immediate;
	}
	else if(strcmp(op,"001011") == 0) //andi
	{
		res = registro[regs1] & immediate;
	}
	else if(strcmp(op,"001101") == 0) //ori
	{
		res = registro[regs1] | immediate;
	}
	else if(strcmp(op,"001101") == 0) //beqz
	{
		if(registro[regd] == 0)
			pc = immediate;
	}
	else if(strcmp(op,"001111") == 0) //bgtz
	{
		if(registro[regd] > 0)
			pc = immediate;
	}

	if(control_signal[0] == '1')
		registro[regd] = res;	
}

void execute2(string op)
{
	pc++;
	int res;
	if(strcmp(op,"000110") == 0) //add
	{
		res = registro[regs1] + registro[regs2];
	}else if(strcmp(op, "001000") == 0) //sub
	{
		res = registro[regs1] - registro[regs2];
	}else if(strcmp(op, "001010") == 0) //and
	{
		res = registro[regs1] & registro[regs2];
	}else if(strcmp(op, "001100") == 0) //or
	{
		res = registro[regs1] | registro[regs2];
	}else if(strcmp(op, "010001") == 0) //jr
	{
		pc = registro[regd];
	}

	if(control_signal[0] == '1')
		registro[regd] = res;
}

void execute3(string op)
{
	if(strcmp(op,"010000") == 0) //j
	{
		pc = address;
	}
}


void decode_instruction(string inst_reg){
	string op = malloc(6);
	string tipo = malloc(1);
	string sign = malloc(7);
	control_signal = malloc(7);
	string rd   = malloc(5),
		   rs1  = malloc(5), 
		   rs2  = malloc(5), 
		   im   = malloc(16),
		   desp = malloc(26);
	int i;

	//[1,1,0,1,0,0,
	//	1,1,1,0,0,
	//	0,1,1,0,0,
	//	0,0,0,0,0,0,0,1,1,1,0,1,0,1,0,1]

	for(i = 0; i < 6; i++)
	{
		op[i] = inst_reg[i];
	}
	sm_get(types,op,tipo,1);

	if(strcmp(tipo,"i") == 0)
	{
		sm_get(ctrl_signals,op,control_signal,7);		// en teoria esto deberia regresar las señales de control.

		for(i = 0; i < 6; i++)
			rd[i] = inst_reg[i+6];
		for(i = 0; i < 6; i++)
			rs1[i] = inst_reg[i+11];
		for(i = 0; i < 16; i++)
			im[i] = inst_reg[i+16];

		regd = strb_to_i(rd, 5);
		regs1= strb_to_i(rs1, 5);
		immediate = strb_to_i(im, 16);
	}
	else if(strcmp(tipo,"j") == 0)
	{
		sm_get(ctrl_signals,op,control_signal,7);     // We divide this in cases for clarity  when following the datapath.
		for(i = 0; i <26; i++)
			desp[i] = inst_reg[i+6];
		address = strb_to_i(desp, 26);

	}
	else if(strcmp(tipo,"r") == 0)
	{
		sm_get(ctrl_signals,op,control_signal,7);		// if you don't like it, why r u reading it???
		for(i = 0; i < 6; i++)
			rd[i] = inst_reg[i+6];
		for(i = 0; i < 6; i++)
			rs1[i] = inst_reg[i+11];
		for(i = 0; i < 6; i++)
			rs2[i] = inst_reg[i+16];
		regd = strb_to_i(rd, 5);
		regs1= strb_to_i(rs1, 5);
		regs2= strb_to_i(rs2, 5);
	}
	else
	{
		printf("Something terrible has happened, and it is your fault.");
		exit(1);
	}
}

void extreme_foo_part_two()
{
	pc = 0;
	types = populate_types();

	while(pc < text_length){
		inst_reg = text[pc];
		printf("%s\n", data[0]);
		decode_instruction(inst_reg);
		extreme_execution();
	}
}



int main(int argc, char *argv[])
{
	extreme_foo(open_file(get_filename(argc,argv)));
	extreme_foo_part_two();
	// end of functional programming :(


	return 0;
}