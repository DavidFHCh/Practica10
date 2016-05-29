#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "list.h"
#include "strmap.h"

typedef char* string;

// Hashtable with label-address pairs.
StrMap *labels;
// Hashtable with instruction-opcode pairs.
StrMap *opcodes;
// Hashtable with instruction-type pairs.
StrMap *types;

string d_lines, t_lines;

string i_to_b(string in, int n);

/*
	Function to print all key-value pairs in a strmap. Just for debugging purposes.
*/
static void iter(const char *key, const char *value, const void *obj)
{

	printf("key: %s value: %s\n", key, value);
}
/*
	Pouplates the opcodes StrMap with the instruction-opcode pairs.
*/
StrMap *populate_opcodes()
{
	StrMap *ops_map = sm_new(18);
	// sorry mom
	string ops[] = {"lw", "lh", "lb", "sw", "sh", "sb", "add", "addi", "sub", "subi", "and", "andi", "or", "ori", "beqz", "bgtz", "j", "jr"};
	string codes[] = {"000000", "000001", "000010", "000011", "000100", "000101", "000110", "000111", "001000", "001001", "001010", "001011", "001100", "001101", "001110", "001111", "010000", "010001"};
	int i = 0;
	for(i = 0; i < 18; i++)
	{
		sm_put(ops_map, ops[i], codes[i]);
	}

	return ops_map;
}

/*
	Populates the types StrMap with instruction-type pairs.	
*/
StrMap *populate_types()
{
	StrMap *type_map = sm_new(18);
	string ops[] = {"lw", "lh", "lb", "sw", "sh", "sb", "addi", "subi", "andi", "ori", "beqz", "bgtz", "jr", "add", "sub", "and", "or", "j"};
	string typ[] = {"i","r","j"};
	int i;

	for(i = 0; i <= 11; i++)
	{
		sm_put(type_map, ops[i], typ[0]);
	}
	for(; i<=16; i++)
	{
		sm_put(type_map, ops[i], typ[1]);
	}
	sm_put(type_map, "j", "j");
	return type_map;
}

/*
	Gets filename from command line arguments or else expects filename from stdin.
*/
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
		fprintf(stderr, "Something went wong while reading file \'%s\'\n", filename);
		exit(1);
	}
	return file;
}


/*
	Returnes a list of all the tokens read from 'file', delimited by whitespace characters.

*/
list *tokenize(FILE *file)
{
	list *words;
	string line, word, dword, label_line;
	int length, line_count;
	
	words = new_list();
	line_count = 0;		//		line_count actually represents 'memory address' corresponding to the line number times four, ignoring .data and .text directives.
	line = malloc(400); // 400 char limit for each line, because why not.
	labels = sm_new(1000); // Maximum 1000 labels please. Don't be ridiculous.

	// For every line in the file, tokenize it by whitespace delimiter and push tokens into words list.
	while(fgets(line, 400, file) != NULL)
	{
		// Delete newline character.
		length = strlen(line);
		if(line[length - 1 ] == '\n')
			line[--length] = '\0';
		
		// Tokenize with tokens delimited by any whitespace.
		word = strtok(line, " \t");
		// Push every token of the line into the list.
		while(word != NULL)
		{	
			dword = strdup(word);
			
			if(strcmp(dword, ".data") == 0)
				line_count -= 4;
			if(strcmp(dword, ".text") == 0)
				line_count -= 4;

			length = strlen(dword);
			// If the token is a label (ends in ':'), don't push it into the list.
			if(!(dword[length - 1] == ':')){
				push(words, strdup(word));
			}else{
				dword[length - 1] = '\0';
				label_line = malloc(10);
				sprintf(label_line, "%d", line_count);
				sm_put(labels, dword, i_to_b(label_line, 16));
				push(words, dword);
			}
			word = strtok(NULL, " \t");
		}
		line_count += 4;
	}
	
	/*while(size(words))
	{
		printf("%s\n", (char*)pop_first(words));
	}
	printf("%d\n",line_count);
	sm_enum(labels, iter, NULL);*/

	fclose(file); 	//jesus fucking christ
	return words;
}

/*
	returns 1 if last character of l is a colon.
	huehuehuehue.
*/
int is_label(string l)
{
	return (l[strlen(l) -1 ] == ':');
}

/*
	Translates a string representation of a number to a string representation of that number in binary in n bits.
	Implementation: stackoverflow
*/
string i_to_b(string in, int n)
{
	int number = (int)strtol(in, (char**)NULL, 10);
	int bits = n;

    char *str = malloc(bits + 1);
    if(!str) return NULL;
    str[bits] = 0;

    // type punning because signed shift is implementation-defined
    unsigned u = *(unsigned *)&number;
    for(; bits--; u >>= 1)
    	str[bits] = u & 1 ? '1' : '0';

    return str;
}

/*
	Takes a string in the form X(Y) and returns X
*/
string first_imm(string imm)
{
	int length = strlen(imm);
	int i = 0, found = 0;
	string ret = strdup(imm);

	while(i < length && !found)
	{
		if(ret[i] == '(')
		{
			found = 1;
			ret[i] = '\0';
		}
		i++;
	}
	return ret;
}

/*
	Takes a string in the form X(Y) and returns Y
*/
string second_imm(string imm)
{
	int length = strlen(imm);
	int i = 0, found = 0;
	string ret = strdup(imm);

	while(i < length && !found)
	{
		if(ret[i] == '(')
		{
			found = 1;
			ret[i] = '\0';
		}
		i++;
	}

	ret = ret + i;
	i = 0;
	length = strlen(ret);
	found = 0;

	while(i < length && !found)
	{
		if(ret[i] == ')')
		{
			found = 1;
			ret[i] = '\0';
		}
		i++;
	}

	return ret;
}

/*
	Returns 1 if string contains a paren, 0 otherwise.
*/
int contains_paren(string str)
{
	int len = strlen(str);
	int i;
	for(i = 0; i < len; i++)
	{
		if(str[i] == '(' || str[i] == ')')
			return 1;
	}
	return 0;
}

/*
	Translates tokens into their binary form and creates binary instructions from them, returning
	them all inside a list in the order they should be printed (first object in list is first line of object file)
*/
list *translate(list* words)
{
	int data_l;
	int text_l;
	int parse_mode;
	list *lines;
	string word, line, type, opcode;
	string label, directive, data, 
		   first, inst, rs, ors, rd, imm, imm2;
	char buffer[33];
	
	types = populate_types();
	opcodes = populate_opcodes();

	//sm_enum(labels, iter, NULL);
	word = (string)peek_first(words);

	lines = new_list();
	data_l = 0;
	text_l = 0;

	if(strcmp(word, ".data") == 0)
		parse_mode = 1;
	if(strcmp(word, ".text") == 0)
		parse_mode = 0;

	pop_first(words);

	while(parse_mode == 1)
	{
		label = (string)pop_first(words);
		directive = (string)pop_first(words);
		data = (string)pop_first(words);

		if(strcmp(directive, ".word") == 0)
			data_l += 4;
		else if (strcmp(directive, ".byte") == 0)
			data_l++;

		push(lines, strdup(i_to_b(data, 32)));
		if(strcmp((string)peek_first(words), ".text") == 0)
			parse_mode = 0;
	}

	pop_first(words);

	while(parse_mode == 0 && size(words) > 0)
	{
		line = malloc(33);
		opcode = malloc(7);
		imm2 = malloc(17);
		text_l += 4;
		first = (string)peek_first(words);
		if(is_label(first))
			pop_first(words);
		
		inst = (string)pop_first(words);
		type = malloc(2);
		sm_get(types, inst, type, 2);

		if(strcmp(type, "i") == 0)
		{
			//printf("Es una instrucción i!\n");
			rs = (string)pop_first(words);
			rd = (string)pop_first(words);
			if(contains_paren(rd))
			{
				imm = first_imm(rd);
				rd = second_imm(rd);
			}else if(rd[0] == '$'){
				imm = (string)pop_first(words);
			}else{
				imm = rd;
				rd = "$00";
			}
			sm_get(opcodes, inst, opcode, 7);
			if(sm_exists(labels, imm)){
				sm_get(labels, imm, imm2, 17);
				imm = imm2;
			}else{
				imm = i_to_b(imm, 16);
			}

			sprintf(line, "%s%s%s%s", opcode, i_to_b(rs+1, 5), i_to_b(rd+1, 5), imm);
			//printf("linea: %s %s %s %s\n", inst, rs, rd, imm);
			//printf("%s\n", line);
			push(lines, line);
		}else if (strcmp(type, "r") == 0)
		{
			rs = (string)pop_first(words);
			if(strcmp(inst, "jr") == 0){
				ors = "$00";
				rd = "$00";
			}else
			{
				ors = (string)pop_first(words);
				rd = (string)pop_first(words);
			}
			sm_get(opcodes, inst, opcode, 7);
			sprintf(line, "%s%s%s%s%s", opcode, i_to_b(rs+1, 5), i_to_b(ors+1, 5), i_to_b(rd+1, 5), i_to_b("0", 11));
			//printf("%s\n", line);
			push(lines, line);
		}else if (strcmp(type, "j") == 0)
		{
			rs = (string)pop_first(words);
			imm2 = malloc(27);
			sm_get(labels, rs, imm2, 27);
			sm_get(opcodes, inst, opcode, 27);
			sprintf(line, "%s%s%s", opcode, imm2, i_to_b("0",10));
			//printf("%s\n",line);
			push(lines, line);
		}
	}
	string line1 = malloc(33);
	sprintf(line1, "%d",data_l);	
	string line2 = malloc(33);
	sprintf(line2, "%d",text_l);
	
	d_lines = i_to_b(line1, 32);
	t_lines = i_to_b(line2, 32);

	return lines;
}

void print_to_f(list* lines)
{
	FILE *fp;
	string line;
	fp = fopen("b.o","w");
	fprintf(fp, "%s\n", d_lines);
	fprintf(fp, "%s\n", t_lines);
	while(size(lines))
	{
		line = (string)pop_first(lines);
		fprintf(fp, "%s\n", line);
	}

}

/*
	Calls functions in the order required to generate an object file corresponding
	to the input assembly file.
	WARNING: This program only works with well-written assembly files. If 
	assembly file is malformed, the program WILL cause segmentation faults.
	IT WILL!!!
*/
void main(int argc, char *argv[])
{
	print_to_f(translate(tokenize(open_file(get_filename(argc,argv))))); // Functional programming!
	// free_list currently causes a segfault :D
	//free_list(words);
}