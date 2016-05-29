//made by two mad m8, looking for social acceptance. So far Dixego is the only one succesful in that matter.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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
string control_signal;

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
	string ops[] = {"lw", "lh", "lb", "sw", "sh", "sb", "add", "addi", "sub", "subi", "and", "andi", "or", "ori", "beqz", "bgtz", "j", "jr"};
	string codes[] = {"000000", "00001", "000010", "000011", "000100", "000101", "000110", "000111", "001000", "001001", "001010", "001011", "001100", "001101", "001110", "001111", "010000", "010001"};
	int i = 0;
	for(i = 0; i < 18; i++)
	{
		sm_put(ops_map, codes[i], ops[i]);
	}

	return ops_map;
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
	line = malloc(35);
	fgets(line, 35, file);
	data_length = strb_to_i(line,32);
	data = malloc(data_length*sizeof(string));
	fgets(line, 35, file);
	text_length = strb_to_i(line,32);
	text = malloc(text_length*sizeof(string));
	
	while(fgets(line, 35, file) != NULL && datacunt < data_length){
		data[datacunt] = strdup(line);
		datacunt++;
	}
	
	while(fgets(line, 35, file) != NULL && textcunt < text_length){
		text[textcunt] = strdup(line);
		textcunt++;
	}
}

void decode_instruction(string inst_reg){

}

void extreme_foo_part_two()
{
	pc = 0;
	types = populate_types();

	while(pc < text_length){
		inst_reg = text[pc];
		decode_instruction(inst_reg);
	}
}

int main(int argc, char *argv[])
{
	extreme_foo(open_file(get_filename(argc,argv)));
	// end of functional programming :(


	return 0;
}