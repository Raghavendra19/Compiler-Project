/*Group -51
2017A7PS0028P Abhinava Arasada
2017A7PS0092P Vineet Golchha Jain
2017A7PS0123P Raghavendra Maheshwari
2017A7PS0214P Patel Abhishek Bipinkumar
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

static int lineno = 1;
static int end = 0;
int state = 0;
char Buff[4000];
FILE *fp1;

void removeComments(char *testcaseFile, char *cleanFile){
    FILE* fp2 = fopen(testcaseFile, "r");
    FILE* fp3 = fopen(cleanFile, "w");
    int flag = 0;
    do{
        char c = fgetc(fp2);
        if(feof(fp2)){
            break;
        }
        if(c != '*'){
            if(flag == 1){
                fputc('*', fp3);
                flag = 0;
            }
        }

        if(c == '\n'){
            fputc(c, fp3);
            continue;
        }
        if(c == '*' && flag == 0){
            flag = 1;
        }
        else if(c == '*' && flag == 1){
            flag = 2;
        }
        else if(c == '*' && flag == 2){
            flag = 3;
        }
        else if(c == '*' && flag == 3){
            flag = 4;
        }
        if(flag == 0){
            fputc(c, fp3);
        }

        if(flag == 4){
            flag = 0;
        }
    }while(1);
    fclose(fp2);
    fclose(fp3);
    return;
}


token* createToken(char arr[], char brr[], int linenum){
    token* tok = (token *)malloc(sizeof(token));
    strcpy(tok -> token_name, arr);
    strcpy(tok -> value, brr);
    tok -> line_num = linenum; 
    return tok;
}

void flush(char* arr, int size){
    int i;
    for(i = 0 ; i < size ; ++i){
        arr[i] = '\0';
    }
    return;
}

char* check_keyword(char curr_arr[]){           // checks if given array contains a keyword and if so returns token_key, else returns "notKeyword"
    char * ret;
    ret = (char *)malloc(sizeof(char)*30);
    int i;
    for(i = 0 ; i < 30 ; ++i){
        if(strcmp(curr_arr, keyword_dict[i]) == 0){
            strcpy(ret, token_key[i]);
            return ret;
        }
    }
    strcpy(ret, "notKeyword");
    return ret;                                           
}

FILE* getstream(FILE *fp1){  //the input stream method, reads buffsize data into buffer B and returns file with advanced pointer 
	int i;
    int buffsize = 3998;
    flush(Buff, 3998);
	if(fp1 == NULL){
		printf("Invalid File name..\n");
		exit(0);
	}
	if(fread(Buff,(size_t)buffsize,1,fp1)){
        return fp1;
	}
	return NULL;
}

FILE* reloadBuffer(){
    fseek( fp1, 0, SEEK_CUR);
    fp1 = getstream(fp1);
    Buff[3998] = '~';
    return fp1;
}

void check(){
    if(end == 3998){
        fp1 = reloadBuffer();
        end = 0;
    }
}

int tok_count = 0;

token* getNextToken(){
    if(Buff[end] == 0){
        tok_count++;
        return createToken("FINISH", "$", lineno);
    }
    char ch;
    char arr[21];
    char brr[100];
    int currIndex = 0;
    flush(arr, 21);
    while(1){
        ch = Buff[end];
        if(ch == '\0'){
            break;
        }
        switch(state){ 
            case 0:
                if(ch == 0){
                    tok_count++;
                    return createToken("FINISH", "$", lineno);
                }
                if(ch == '+'){
                    ++end;
                    check();
                    tok_count++;
                    return createToken("PLUS", "+", lineno);
                }
                else if(ch == '-'){
                    ++end;
                    check();
                    tok_count++;
                    return createToken("MINUS", "-", lineno);
                }
                else if(ch == '/'){
                    ++end;
                    check();
                    tok_count++;
                    return createToken("DIV", "/", lineno);
                }
                else if(ch == ';'){
                    ++end;
                    check();
                    tok_count++;
                    return createToken("SEMICOL", ";", lineno);
                }
                else if(ch == ','){
                    ++end;
                    check();
                    tok_count++;
                    return createToken("COMMA", ",", lineno);
                }
                else if(ch == '['){
                    ++end;
                    check();
                    tok_count++;
                    return createToken("SQBO", "[", lineno);
                }
                else if(ch == ']'){
                    ++end;
                    check();
                    tok_count++;
                    return createToken("SQBC", "]", lineno);
                }
                else if(ch == '('){
                    ++end;
                    check();
                    tok_count++;
                    return createToken("BO", "(", lineno);
                }
                else if(ch == ')'){
                    ++end;
                    check();
                    tok_count++;
                    return createToken("BC", ")", lineno);
                }
                else if(ch == ' ' || ch == '\t' || ch == '\n'){
                    ++end;
                    check();
                    if(ch == '\n'){
                        ++lineno;
                    }
                    continue;
                }
                else if(ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z'){
                    state = 1;
                    arr[0] = ch;
                    currIndex = 1;
                    ++end;
                    check();
                    continue;
                }
                else if(ch == '<'){
                    state = 2;
                    arr[0] = ch;
                    currIndex = 1;
                    ++end;
                    check();
                    continue;
                }
                else if(ch == '>'){
                    state = 3;
                    arr[0] = ch;
                    currIndex = 1;
                    ++end;
                    check();
                    continue;
                }
                else if(ch == '='){
                    state = 4;
                    arr[0] = ch;
                    currIndex = 1;
                    ++end;
                    check();
                    continue;
                }
                else if(ch == '!'){
                    state = 5;
                    arr[0] = ch;
                    currIndex = 1;
                    ++end;
                    check();
                    continue;
                }
                else if(ch == '.'){
                    state = 6;
                    arr[0] = ch;
                    currIndex = 1;
                    ++end;
                    check();
                    continue;
                }
                else if(ch == ':'){
                    state = 7;
                    arr[0] = ch;
                    currIndex = 1;
                    ++end;
                    check();
                    continue;
                }
                else if(ch == '*'){
                    state = 8;
                    arr[0] = ch;
                    currIndex = 1;
                    ++end;
                    check();
                    continue;
                }
                else if(ch >= '0' && ch <= '9'){
                    state = 11;
                    brr[0] = ch;
                    currIndex = 1;
                    ++end;
                    check();
                    continue;
                }
                else{
                	if(Buff[end] == 0){
                		return createToken("FINISH", "$", lineno);
                	}
                    printf("Unknown char %c, printing next one", ch);
                    ++end;
                    state = 0;
                    continue;
                }
            break;
            case 1:
                while(Buff[end] >= 'a' && Buff[end] <= 'z' || Buff[end] >= 'A' && Buff[end] <= 'Z' || Buff[end] == '_' || Buff[end] >= '0' && Buff[end] <= '9'){
                    arr[currIndex] = Buff[end];
                    ++currIndex;
                    if(currIndex > 21){
                        arr[21] = '\0';
                        printf("Error because identifier %s(first 20 letters) length is too long", arr);
                        break;
                    }
                    ++end;
                    check();
                }
                arr[currIndex] = '\0';
                state = 0;
                if(strcmp(check_keyword(arr), "notKeyword") != 0){
                    return createToken(check_keyword(arr), arr, lineno);
                }
                return createToken("ID", arr, lineno);
            break;
            case 2:
                if(Buff[end] == '<'){
                    ++end;
                    check();
                    state = 17;
                    break;
                }
                else if(Buff[end] == '='){
                    ++end;
                    check();
                    tok_count++;
                    state = 0;
                    return createToken("LE", "<=", lineno);
                }
                else{
                    tok_count++;
                    state = 0;
                    return createToken("LT","<", lineno);
                }
            break;
            case 3:
                if(Buff[end] == '>'){
                    ++end;
                    check();
                    state = 18;
                    break;
                }
                else if(Buff[end] == '='){
                    ++end;
                    check();
                    tok_count++;
                    state = 0;
                    return createToken("GE", ">=", lineno);
                }
                else{
                    tok_count++;
                    state = 0;
                    return createToken("GT",">", lineno);
                }
            break;
            case 4:
                if(Buff[end] == '='){
                    ++end;
                    check();
                    ++tok_count;
                    state = 0;
                    return createToken("EQ", "==", lineno);
                }
                else{
                    printf("= is not a valid symbol in %d", lineno);
                    state = 0;
                    return createToken("Invalid", "=", lineno);
                }
            break;
            case 5:
                if(Buff[end] == '='){
                    ++end;
                    check();
                    ++tok_count;
                    state = 0;
                    return createToken("NE", "!=", lineno);
                }
                else{
                    printf("! is not a valid symbol");
                    state = 0;
                    return createToken("Invalid", "!", lineno);
                } 
            break;
            case 6:
                if(Buff[end] == '.'){
                    ++end;
                    check();
                    ++tok_count;
                    state = 0;
                    return createToken("RANGEOP", "..", lineno);
                }
                else{
                    printf(". is not a valid symbol");
                    state = 0;
                    return createToken("Invalid", ".", lineno);
                } 
            break;
            case 7:
                if(Buff[end] == '='){
                    ++end;
                    check();
                    ++tok_count;
                    state = 0;
                    return createToken("ASSIGNOP", ":=", lineno);
                }
                else{
                    ++tok_count;
                    state = 0;
                    return createToken("COLON", ":", lineno);
                } 
            break;
            case 8:
                if(Buff[end] == '*'){
                    ++end;
                    check();
                    state = 9;
                    continue;
                }
                else{
                    ++tok_count;
                    state = 0;
                    return createToken("MUL", "*", lineno);
                }
            break;
            case 9:
                if(Buff[end] != '*'){
                    while(Buff[end] != '*'){
                        if(Buff[end] == '\n'){
                            lineno++;
                        }
                        ++end;
                        check();
                    }                    
                }
                ++end;
                check();
                state = 10;
            break;
            case 10:
                if(Buff[end] != '*'){
                    if(Buff[end] == '\n'){
                        lineno++;
                    }
                    ++end;
                    check();
                    state = 9;
                }
                else{
                    ++end;
                    check();
                    //++tok_count;
                    state = 0;
                    return getNextToken();
                    //return NULL;//createToken("COMMENT MARK", "**", lineno);
                }
            break;
            case 11:
                while(currIndex < 100 && Buff[end] >= '0' && Buff[end] <= '9'){
                    brr[currIndex] = Buff[end];
                    ++end;
                    check();
                    ++currIndex;
                }
                if(currIndex == 100){
                    printf("Overflow error");
                    break;
                }
                else if(Buff[end] == '.'){
                    brr[currIndex] = '.';
                    ++currIndex;
                    ++end;
                    check();
                    state = 12;
                }
                else{
                    state = 0;
                    brr[currIndex] = '\0';
                    ++tok_count;
                    return createToken("NUM", brr, lineno);
                }
            break;
            case 12:
                if(Buff[end] == '.'){
                    state = 0;
                    --currIndex;
                    --end;
                    brr[currIndex] = '\0';
                    ++tok_count;
                    return createToken("NUM", brr, lineno);
                    
                }
                else if(Buff[end] >= '0' && Buff[end] <= '9'){
                    brr[currIndex] = Buff[end];
                    ++end;
                    check();
                    ++currIndex;
                    state = 13;
                }
                else{
                    printf("Not a valid num");
                    state = 0;
                }
            break;
            case 13:
                while(currIndex < 100 && Buff[end] >= '0' && Buff[end] <= '9'){
                    brr[currIndex] = Buff[end];
                    ++end;
                    check();
                    ++currIndex;
                }
                if(currIndex == 100){
                    printf("Overflow error");
                    state = 0;
                    break;
                }
                else if(Buff[end] == 'E' || Buff[end] == 'e'){
                    state = 14;
                    brr[currIndex] = Buff[end];
                    ++currIndex;
                    ++end;
                    check();
                }
                else{
                    brr[currIndex] = '\0';
                    state = 0;
                    ++tok_count;
                    return createToken("RNUM", brr, lineno);
                }
            break;
            case 14:
                if(Buff[end] == '+' || Buff[end] == '-'){
                    brr[currIndex] = Buff[end];
                    ++currIndex;
                    ++end;
                    check();
                    state = 15;
                }
                else if(Buff[end] >= '0' && Buff[end] <= '9'){
                    brr[currIndex] = '+';
                    ++currIndex;
                    brr[currIndex] = Buff[end];
                    ++currIndex;
                    ++end;
                    check();
                    state = 16;
                }
                else{
                    printf("Invalid Number");
                    state = 0;
                    break;
                }
            break;
            case 15:
                if(Buff[end] >= '0' && Buff[end] <= '9'){
                    state = 16;
                    brr[currIndex] = Buff[end];
                    ++currIndex;
                    ++end; 
                    check();
                }
                else{
                    printf("Not a valid num");
                    state = 0;
                    break;
                }
            break;
            case 16:
                while(currIndex < 100 && Buff[end] >= '0' && Buff[end] <= '9'){
                    brr[currIndex] = Buff[end];
                    ++end;
                    check();
                    ++currIndex;
                }
                if(currIndex == 100){
                    printf("Overflow error");
                    state = 0;
                    break;
                }
                else{
                    state = 0;
                    brr[currIndex] = '\0';
                    ++tok_count;
                    return createToken("RNUM", brr, lineno);
                }
            break;
            case 17:
                if(ch == '<'){
                    ++end;
                    check();
                    ++tok_count;
                    state = 0;
                    return createToken("DRIVERDEF", "<<<", lineno);
                    break;
                }
                else{
                    state = 0;
                    return createToken("DEF", "<<", lineno);
                    break;
                }
            break;
            case 18:
                if(ch == '>'){
                    ++end;
                    check();
                    ++tok_count;
                    state = 0;
                    return createToken("DRIVERENDDEF", ">>>", lineno);
                    break;
                }
                else{
                    state = 0;
                    return createToken("ENDDEF", "<<", lineno);
                    break;
                }
            break;
        }
    }
}




