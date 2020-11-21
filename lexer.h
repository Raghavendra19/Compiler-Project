/*Group -51
2017A7PS0028P Abhinava Arasada
2017A7PS0092P Vineet Golchha Jain
2017A7PS0123P Raghavendra Maheshwari
2017A7PS0214P Patel Abhishek Bipinkumar
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexerDef.h"

token* createToken();
void flush(char* arr, int size);
char* check_keyword(char curr_arr[]);
FILE* getstream(FILE *fp);
token* getNextToken();