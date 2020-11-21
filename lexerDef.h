/*Group -51
2017A7PS0028P Abhinava Arasada
2017A7PS0092P Vineet Golchha Jain
2017A7PS0123P Raghavendra Maheshwari
2017A7PS0214P Patel Abhishek Bipinkumar
*/


#include<stdio.h>
#include<stdlib.h>

char* keyword_dict[30] = {"integer","real","boolean","of",                          //keyword dictionary
                "array","start","end","declare","module","driver","program",
                "get_value","print","use","with","parameters","true",
                "false","takes","input","returns","AND","OR","for","in",
                "switch","case","break","default","while"};

char* token_key[30] = {"INTEGER","REAL","BOOLEAN","OF","ARRAY","START",                         //corresponding tokens
                        "END","DECLARE","MODULE","DRIVER","PROGRAM","GET_VALUE",
                        "PRINT","USE","WITH","PARAMETERS","TRUE","FALSE","TAKES","INPUT","RETURNS",
                        "AND","OR","FOR","IN","SWITCH",
                        "CASE","BREAK","DEFAULT","WHILE"};    

struct Token{
    char token_name[13];
    char value[21];
    int line_num;
};



typedef struct Token token;

