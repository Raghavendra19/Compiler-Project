/*Group -51
2017A7PS0028P Abhinava Arasada
2017A7PS0092P Vineet Golchha Jain
2017A7PS0123P Raghavendra Maheshwari
2017A7PS0214P Patel Abhishek Bipinkumar
*/



#include<stdio.h>
#include "stdbool.h"
#include "parserDef.h"
#include<time.h>
#include"ast.c"

int sizeP = 0;

FirstAndFollow* initialize(FirstAndFollow * F){
    int i = 56;
    for(i = 56 ; i < 118 ; ++i){
        F->fftable[i][0][0] = i;
        F->fftable[i][0][1] = -1;
        F->fftable[i][1][0] = -1;
    }
    return F;
}

int flag0 = 0;

int flagp = 0;

int firstmap[118] = {0};
int followmap[118] = {0};

int canEpsilon(int non_term){
    int flag = 0;
    int a = 0;
    if(non_term == 117){
        return 1;
    }
    else if(non_term > 55){
        return 0;
    }
    else{
        int i;
        for(i = 0 ; i < 103 ; ++i){
            if(G[i][0] == non_term){
                int j;
                for(j = 1 ; j < 20 ; ++j){
                    if(G[i][j] == -1){
                        a = 1;
                        break;
                    }
                    else if(G[i][j] == G[i][0]){
                        a = 0;
                        break;
                    }
                    if(!canEpsilon(G[i][j])){
                        a = 0;
                        break;
                    }
                }
            }
            if(a == 1){
                flag = 1;
                break;
            }
            a = 0;
        }
        if(flag == 1){
            return 1;
        }
    }
    return 0;
} 

void display(tree_node *node,char* outfile)
{
	FILE* fp4 = fopen(outfile, "a");
    printf(" %s ", terms[node -> id]);
    
    
    if(node->isleaf==1 && node ->tok != NULL)
    printf("%s ",(node->tok)->value);
    else
    printf("%s ","____");
    if(node -> tok != NULL){
        printf("%d ",((node->tok)->line_num));
        printf("%s ",((node->tok)->token_name));
        if(strcmp((node->tok)->token_name,"NUM")==0||strcmp(node -> tok -> token_name, "RNUM") == 0)
            printf("%s ",(node->tok)->value);
    }
        
    if(node->parent==NULL){
        printf("%s ","ROOT");
    }
    
    else{
        printf("<");
        printf("%s ",terms[node -> parent -> id]);
        printf(">");
    }
    

    if(node->isleaf==1)
    printf("%s ","yes");
    else
    printf("%s ","no");

    printf("%s ",terms[node->id]);

    printf("%s ","\n");
    
    if(node -> id == 0){
        flag0 = 1;
    }
    
    
    return;
}


void fakePrint(tree_node* node, char* filename){
    tree_node* temp = (tree_node *)malloc(sizeof(tree_node));
    if(node != NULL && node -> isTraversed == flagp){
        fakePrint(node -> leftmost_child, filename);
		display(node, filename);
        temp = node -> leftmost_child;
        while(temp != NULL){
            fakePrint(temp, filename);
            temp = temp -> right;
        }
        node -> isTraversed = 1 - flagp;
    }
    return;
}

bool isNonTerminal(int i ){
    if ( i >= 1 && i<= NON_TERMINALS) 
    {
        return true ;
    }
    else
        return false ;
}

FirstAndFollow * first ( int non_termi_no, FirstAndFollow * F, int  G[][20]){
    if(firstmap[non_termi_no] == 1){
           // printf("returning from table") ;
            return F ;
    }
    else{
        if(non_termi_no == 0){
            F->fftable[0][0][0] = 64 ;
            F->fftable[0][0][1] = 91 ;
            F->fftable[0][0][2] = 90 ;
            F->fftable[0][0][3] = -1 ;
            return F ;
        }
        else if(isNonTerminal(non_termi_no)){
            //printf("Non Termi %d\n",non_termi_no);
                //printf("ENTERS");
                int count = 0 ;
                int j;
                for (j = 0 ; j < TOTAL_RULES ; j++)
                {
                    if (G[j][0] == non_termi_no ) { // Rule starting with this Non terminal
                        //printf("Rule No %d and Non termi = %d\n",j,non_termi_no);
                        int k = 1 ;
                        while (G[j][k] != -1){
                            //printf("G[%d][%d] = %d\n",j,k, G[j][k]);
                            if ( isNonTerminal(G[j][k]) ){
                                if (canEpsilon(G[j][k])){
                                    int p = k ;
                                    while(G[j][p] != -1 && canEpsilon(G[j][p])){
                                        //printf("COOL \n");
                                        F = first(G[j][p],F,G);
                                        //printf("first of %d",G[j][p]) ;
                                        int l = 0;
                                        while(F->fftable[G[j][p]][0][l] != -1){
                                            if(F->fftable[G[j][p]][0][l] != EPLS){
                                                //printf("For Non Termi %d Count %d , %d\n",non_termi_no,count,F->fftable[G[j][p]][0][l]);
                                                F->fftable[non_termi_no][0][count++] = F->fftable[G[j][p]][0][l] ;
                                            }
                                            l++ ;
                                        }
                                        F->fftable[non_termi_no][0][count] = -1 ;
                                        p++ ;
                                        if(G[j][p] == -1 ){
                                            F->fftable[non_termi_no][0][count++] = EPLS ;
                                            F->fftable[non_termi_no][0][count] = -1 ;
                                        }
                                        if(G[j][p] != -1 && !canEpsilon(G[j][p])){
                                            F = first(G[j][p], F, G);
                                            int y=0 ;
                                            while(F->fftable[G[j][p]][0][y] != -1){
                                                F->fftable[non_termi_no][0][count++] = F->fftable[G[j][p]][0][y] ;
                                                y++ ;
                                            }
                                            F->fftable[non_termi_no][0][count] = -1 ;
                                        }
                                    }
                                }
                                else {
                                    //printf("SMOOTH INDIRECT \n");
                                    F = first(G[j][k], F, G);
                                    int z = 0 ;
                                    while (F->fftable[G[j][k]][0][z] != -1){
                                        F->fftable[non_termi_no][0][count++] = F->fftable[G[j][k]][0][z] ;
                                        //printf("first of %d %d ",G[j][k],first_arr[z]);
                                        //printf("\n");
                                        z++ ;
                                    }
                                    F->fftable[non_termi_no][0][count] =-1 ; 
                                    break ;
                                }
                            }
                            else if (G[j][k] == EPLS){
                                //printf("Epsilon Rule\n");
                                F->fftable[non_termi_no][0][count++] = EPLS ;
                                F->fftable[non_termi_no][0][count] = -1 ;
                                break ;
                            }
                            else { 
                                //printf("DIRECT\n");         //Case when A->bB first one is a terminal 
                                F->fftable[non_termi_no][0][count++] = G[j][k] ;
                                //printf("%d \n",G[j][k]);
                                F->fftable[non_termi_no][0][count] = -1 ;
                                break ;
                            }
                            k++ ;
                        }
                    }
                } 
                firstmap[non_termi_no] = 1 ;
                return F ;
            }
            else {
                F->fftable[non_termi_no][0][0] = non_termi_no ;
                F->fftable[non_termi_no][0][1] = -1 ;
                return F ;
            }
        }

}
FirstAndFollow * follow(int non_termi_follow, int G[][20], FirstAndFollow * F) {
    if (non_termi_follow == 2){
        F->fftable[2][1][0] = 90 ;
        F->fftable[2][1][1] = 56 ;
        F->fftable[2][1][2] = -1 ;
        return F ;
    }
    else if (non_termi_follow == 8){
        F->fftable[8][1][0] = 90 ;
        F->fftable[8][1][1] = 56 ;
        F->fftable[8][1][2] = 91 ;
        F->fftable[8][1][3] = -1 ;
        return F ;
    }
    else if (followmap[non_termi_follow] == 0 ){
        //printf("NON Termi follow %d\n", non_termi_follow);
        int count = 0 ;
        //printf("Entered 1") ;
        if (non_termi_follow == 0){
            //printf("WORKING BASE CASE");
            F->fftable[0][1][count++] = 56 ;
            F->fftable[0][1][count] = -1 ;
            return F ;
        }
        int i;
        for (i = 0 ; i < TOTAL_RULES ; i++)
        {
            int j = 1 ; 
            while(G[i][j] != -1){
                int gadbad = 0 ;
                if (G[i][j] == non_termi_follow){
                   // printf("Rule no %d Term no %d\n",i,j) ;
                   // printf("%d\n", G[i][0]);
                    int k = j+1 ;
                   // printf("%d ",G[i][k]);
                   //F = first(G[i][k], F, G);
                   // printTestCase(F->fftable[27][0]);
                        while(G[i][k] != -1 && canEpsilon(G[i][k])){
                            //printf("Traversing term no %d",k);
                            int l = 0 ;
                            while(F->fftable[G[i][k]][0][l] != -1){
                                if (F->fftable[G[i][k]][0][l] != EPLS){
                                    //printf("First of %d : %d ",G[i][k], F->fftable[G[i][k]][0][l]) ;
                                    F->fftable[non_termi_follow][1][count++] = F->fftable[G[i][k]][0][l] ;
                                    F->fftable[non_termi_follow][1][count] = -1 ;
                                }
                                l++ ;
                            }
                            k++ ;
                        }
                        if (G[i][k] != -1 && !canEpsilon(G[i][k])){
                            int m = 0 ;
                            while(F->fftable[G[i][k]][0][m] != -1)
                            {
                                F->fftable[non_termi_follow][1][count++] = F->fftable[G[i][k]][0][m] ;
                                F->fftable[non_termi_follow][1][count] = -1 ;
                                m++ ;
                            }
                            break ;
                        }
                        else{
                            if(non_termi_follow != G[i][0]){
                               // printf("Ghusgya %d", G[i][0]);
                                F = follow(G[i][0], G, F) ;
                               // printf("Baahar aa gya %d\n", G[i][0]);
                                int n = 0 ;
                                while(F->fftable[G[i][0]][1][n] != -1){
                                    F->fftable[non_termi_follow][1][count++] = F->fftable[G[i][0]][1][n];
                                    F->fftable[non_termi_follow][1][count] = -1 ;
                                    n++ ;
                                }
                            }
                            else{
                                gadbad = 1 ;
                            }
                        }
                }
                if(gadbad == 1){
                    break ;
                }
                j++ ;
            }
        }
        followmap[non_termi_follow] = 1 ;
        return F ;
    }
    else {
        return F ;
    }
}

void removeDuplicates(int  * a){
    int n = 0 ;
    while (a[n] != -1){
        n++ ;
    }
    n++ ;
    int i;
    for(i = 0; i < n; i++)
    {
        int j;
        for(j = i+1; j < n; )
        {
            if(a[j] == a[i])
            {
                int k;
                for(k = j; k < n; k++)
                {
                    a[k] = a[k+1];
                }
                n--;
            }
            else
            {
                j++;
            }
        }
    }
}
FirstAndFollow * ComputeFirstAndFollowSets( int  G[][20], FirstAndFollow * F) {
     int * ar ;
    FirstAndFollow * ret = (FirstAndFollow *)malloc(sizeof(FirstAndFollow)) ;
    int i;
    for (i = 0 ; i <= TERMS ; i++){
            F = first(i,F,G);
           // removeDuplicates(F->fftable[i][0]);
            //printf("First  of %d :",i);
            int trav = 0 ;
            while(F->fftable[i][0][trav] != -1 ){
                ret->fftable[i][0][trav] = F->fftable[i][0][trav] ;
                trav++ ;
            }
            ret->fftable[i][0][trav] = -1 ;
            //printTestCase(ret->fftable[i][0]);
    }
    //ret = first(56,F,G);
    int j;
    for (j = 0 ; j <= NON_TERMINALS ; j++){
        ret = follow(j, G, ret);
        removeDuplicates(ret->fftable[j][1]) ;
    }
    ret = initialize(ret);
    return ret;
}


void move(tree_node* currn){
    while(currn -> right == NULL){
        currn = currn -> parent;
    }
    currn = currn -> right;
}

stack* createStack(int size){
    stack* s;
    s = (stack *)malloc(sizeof(stack));
    s -> top = -1;
    s -> capacity = size;
    s -> arr = (int *)malloc(size * sizeof(int));
    return s;
}

tree_node* createTreeNode(int ide, token* toke){
    tree_node* newt = (tree_node *)malloc(sizeof(tree_node));
    newt -> id = ide;
    newt -> tok = toke;
    if(ide > 55){
        newt -> isTerminal = 1;
        newt -> isleaf = 1;
    }
    else{
        newt -> isTerminal = 0;
        newt -> isleaf = 0;
    }
    newt -> right = NULL;
    newt ->isTraversed = 0;
    sizeP++;
    return newt;
}

int isFull(stack* s){
    if(s -> top == s -> capacity - 1){
        return 1;
    }
    return 0;
}

int isEmpty(stack* s){
    if(s -> top == -1){
        return 1;
    }
    return 0;
}

void push(stack* s, int a){
    if(isFull(s)){
        printf("The stack is FULL");
        return;
    }
    s -> top ++;
    s -> arr[s -> top] = a;
    return;
}

void pop(stack* s){
    if(isEmpty(s)){
        printf("stack is EMPTY");
        return;
    }
    s -> top--;
    return;
}

int top(stack* s){
    if(isEmpty(s)){
        printf("Stack is EMPTY");
    }
    return s -> arr[s -> top];
}

term_node* createNewNode(char arr[], int flags){
    term_node* newNode;
    newNode = (term_node *)malloc(sizeof(term_node));
    strcpy(newNode -> name, arr);
    newNode -> flag = flags;
}

int isTerminal(term_node* temp){
    return temp -> flag;
}

int getEnumVal(char * arr){
    int i;
    for(i = 0 ; i < 118 ; ++i){
        if(strcmp(arr, terms[i]) == 0){
            return i;
        }
    }
    return -1;
}


void getGrammar(char * filename){
    int check = 0;
    char buff[5000], ch;
    //flush(buff, 5000);
    FILE *fp;
    fp = fopen(filename, "r");
    int i = 0;
    while((ch = fgetc(fp)) != EOF){
        buff[i] = ch;
        ++i;
        if(i == 5000){
            printf("Grammar file is too large.");
            return;
        }
    }
    buff[i] = '\0';
    fclose(fp);
    char curr[30];
    int j;

    int rule_num = 0;

    for(j = 0 ; j < i ; ++j){
        check = 0;
        int num_inside = 1;
        if(buff[j] == '\0'){
            return;
        }
        else if(buff[j] == '<'){
            ++j;
            int k = 0;
            while(buff[j] != '>'){
                curr[k] = buff[j];
                ++k;
                ++j;
            }
            ++j;
            ++j;
            ++j;
            ++j;
            ++j;
            curr[k] = '\0';
            G[rule_num][0] = getEnumVal(curr);
            linked_list[rule_num] = createNewNode(curr, 0);
            term_node* currn = linked_list[rule_num];
            currn -> next = NULL;
            k = 0;
            while(buff[j] != '\n'){
                if(buff[j] == ' '){
                    curr[k] = '\0';
                    G[rule_num][num_inside] = getEnumVal(curr);
                    currn -> next = createNewNode(curr, check);
                    currn = currn -> next;
                    currn -> next = NULL;
                    ++num_inside;
                    k = 0;
                    ++j;
                    continue;
                }
                else if(buff[j] =='-' || buff[j] == '>' || buff[j] == '<'){
                    if(buff[j] == '<'){
                        check = 1;
                    }
                    else if(buff[j] == '>'){
                        check = 0;
                    }
                    ++j;
                    continue;
                }
                curr[k] = buff[j];
                ++k;
                ++j;
            }
            curr[k] = 0;
            G[rule_num][num_inside] = getEnumVal(curr);
            currn -> next = createNewNode(curr, check);
            currn = currn -> next;
            currn -> next = NULL;
            ++num_inside;
            G[rule_num][num_inside] = -1;
            int rando;
            //for(rando = 0 ; rando <= num_inside ;  rando++){
            //    printf(" %d ", G[rule_num][rando]);
            //}
            //printf("\n");
            ++rule_num;
        }
    }
    return;
}


tree_node* parseInputSourceCode(char *testcaseFile){
	getGrammar("Grammar.txt");
    int iterations = 0;

    FILE *fp;
    fp = fopen(testcaseFile, "r");
    fp = getstream(fp);

    token* currtok;
    currtok = (token *)malloc(sizeof(token));
    currtok = getNextToken();
    if(currtok == NULL || strcmp(currtok -> token_name, "Invalid") == 0 || strcmp(currtok -> token_name, "COMMENT MARK") == 0){
        while(currtok != NULL && strcmp(currtok -> token_name, "COMMENT MARK") == 0){
                currtok = getNextToken();
        }
    }
    if(currtok == NULL){
    	return NULL;
    }

    if(strcmp(currtok -> token_name, "Invalid") == 0){
    	while(strcmp(currtok -> token_name, "Invalid") == 0){
    		currtok = getNextToken();
    	}
    }

    stack* s;
    s = createStack(1000);
    push(s, -1);
    push(s, 0);

    tree_node* root;
    root = createTreeNode(0, NULL);
    tree_node* currn = (tree_node *)malloc(sizeof(tree_node));
    currn = root;
    currn -> parent = NULL;


    while((currtok != NULL && strcmp(currtok -> value, "$") != 0 )&& top(s) != -1){
        ++iterations;
        if(iterations == 1000){
            break;
        }
        if(top(s) > 55){
            if(top(s) == 117){
                pop(s);
                currn -> left = NULL;
                currn -> leftmost_child = NULL;
                currn -> right = NULL;
                while(currn -> right == NULL){
                    currn = currn -> parent;
                }
                currn = currn -> right;
                continue;
            }
            else if(top(s) == getEnumVal(currtok -> token_name)){
                currn -> tok = (token *)malloc(sizeof(token));
                currn -> tok = currtok;
                pop(s);
                while(currn -> right == NULL){
                    currn = currn -> parent;
                }
                currn = currn -> right; 
                currtok = getNextToken();
                if(currtok == NULL){
                	break;
                }
                if(strcmp(currtok -> token_name, "Invalid") == 0){
    				while(strcmp(currtok -> token_name, "Invalid") == 0){
    					currtok = getNextToken();
    				}
    			}

                if(strcmp(currtok -> token_name, "COMMENT MARK") == 0){
                    while(strcmp(currtok -> token_name, "COMMENT MARK") == 0){
                        currtok = getNextToken();
                        if(currtok == NULL){
                        	break;
                        }
                        if(strcmp(currtok -> token_name, "Invalid") == 0){
    						while(strcmp(currtok -> token_name, "Invalid") == 0){
    							currtok = getNextToken();
    						}
    					}
                    }
                    if(currtok == NULL){
                    	break;	
                    }
                    if(strcmp(currtok -> token_name, "Invalid") == 0){
    					while(strcmp(currtok -> token_name, "Invalid") == 0){
    						currtok = getNextToken();
    					}
    				}
                }
                continue;
            }
            else{
                printf("Expected token %s but received token %s \n", terms[top(s)], currtok -> token_name);
                while(strcmp(terms[top(s)], currtok -> token_name) != 0){
                	currtok = getNextToken();
                	if(currtok == NULL){
                		return root;
                	}
                }
                continue;
            }
        }
        else{
            int curr_grammar_rule = T.parsetable[top(s)][getEnumVal(currtok -> token_name) - 56];
            //printf("%d %s %s\n", curr_grammar_rule, terms[top(s)], terms[getEnumVal(currtok -> token_name) ]);
            if(curr_grammar_rule == -1 || curr_grammar_rule == -2){
            	while(top(s) != -1 && T.parsetable[top(s)][getEnumVal(currtok -> token_name) - 56] == -1){
            		printf("%s doesnt lie in the first or follow sets of %s, so popping out %s from the stack \n", currtok -> token_name, terms[top(s)], terms[top(s)]);
            		if(strcmp(terms[top(s)], "program") == 0){
            			printf("Stack is empty \n" );
            			return NULL;
            		}
            		pop(s);
            		printf("Top of stack after popping is %d \n", top(s));
            	}
            	if(top(s) == -1){
					printf("stack is empty, terminating. \n");
					break;            		
            	}
            	else if(top(s) == -2){
            		pop(s);
            		continue;
            	}
            }
            int j; 
            pop(s);
            for(j = 1 ; j < 20 ; ++j){
                if(G[curr_grammar_rule][j] == -1){
                    currn -> right = NULL;
                    break;
                }
                //tree_node* newNode = createTreeNode(G[curr_grammar_rule][j], NULL);
                if(j == 1){
                    currn -> leftmost_child = createTreeNode(G[curr_grammar_rule][j], NULL);
                    currn -> leftmost_child -> parent = currn;
                    currn = currn -> leftmost_child;
                    currn -> left = NULL;
                }
                else{
                    //printf("hello");
                    currn -> right = createTreeNode(G[curr_grammar_rule][j], NULL);
                    currn -> right -> left = currn;
                    currn = currn -> right;
                    currn -> parent = currn -> left -> parent;
                    
                    //printf(" %d \n", currn -> parent -> id);
                    
                }
                if(currn -> id > 55){
                    currn -> isTerminal = 1;
                    currn -> isleaf = 1;
                }
                else{
                    currn -> isTerminal = 0;
                    currn -> isleaf = 0;
                }
            }
            while(currn -> left != NULL){
                push(s, currn -> id);
                currn = currn -> left;
            }
                push(s, currn -> id);
        }
    }
    
    if(top (s) != -1 && canEpsilon(top(s))){
		currn -> leftmost_child = createTreeNode(117, NULL);
		currn -> leftmost_child -> parent = currn;
		currn = currn -> leftmost_child;
		currn -> left = NULL;
		currn -> right = NULL;
		currn -> leftmost_child = NULL;
	}

    /*if(strcmp(currtok -> token_name, "FINISH") != 0){
        while(strcmp(currtok -> token_name, "FINISH") != 0){
            getNextToken();
        }
    }*/
    return root;
}



void printParseTree(tree_node *PT, char *outfile)
{
    fakePrint(PT, outfile);
    flagp = 1 - flagp;
    return;
}



void printTestCase(int * arr){
    int pt = 0 ;
    //printf("First:  ");
    while(arr[pt] != -1){
        printf("%d ",arr[pt]);
        pt++ ;
    }
}

void CreateParseTable()
{
    int flag;
    int i;
    for(i=0;i<56;++i)
    {
        int j;
        for(j=0;j<61;++j)
        T.parsetable[i][j]=-1;
    }
	for(i=0;i<103;++i)
	{
	int j;
	flag=0;
		for(j=1;G[i][j]!=-1 && flag==0 ;++j)
		{
		flag=1;
            int k;
			for(k=0;F->fftable[G[i][j]][0][k]!=-1;k++)
			{
				if(F->fftable[G[i][j]][0][k]>=57 && F->fftable[G[i][j]][0][k]<117)
				{
				T.parsetable[G[i][0]][F->fftable[G[i][j]][0][k]-56]=i;
				}
				else if(F->fftable[G[i][j]][0][k]==117)
				{
				flag=0;
				}
			}
		}
			if(flag==0 && G[i][j] == -1)
			{
                int k;
				for(k=0;F->fftable[G[i][0]][1][k]!=-1;++k)
				{
				T.parsetable[G[i][0]][F->fftable[G[i][0]][1][k]-56]=i;
				}
			}
	}
	for(i=0;i<56;++i)
	{
        int j;
		for(j=0;F->fftable[i][1][j]!=-1;++j)
		{
		if(T.parsetable[i][F->fftable[i][1][j]-56]==-1)
			T.parsetable[i][F->fftable[i][1][j]-56]=-2;									
		}
	}
}


/*int main(){
    getGrammar("Grammar.txt");
    F = (FirstAndFollow *)malloc(sizeof(FirstAndFollow));
    F = ComputeFirstAndFollowSets(G,F);
    CreateParseTable();
    fp1=fopen("frapon.txt","w");
    tree_node* temp;
    temp = parseInputSourceCode("t4.txt");
    fclose(fp1);
    printParseTree(temp , "ramon.txt");
    return 0;
}*/

void removeCommentsprintonconsole(char *testcaseFile){
    FILE* fp2 = fopen(testcaseFile, "r");
    
    int flag = 0;
    do{
        char c = fgetc(fp2);
        if(feof(fp2)){
            break;
        }
        if(c != '*'){
            if(flag == 1){
                printf("%c",'*');
                flag = 0;
            }
        }

        if(c == '\n'){
            printf("%c",'\n');
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
            printf("%c",c);
        }

        if(flag == 4){
            flag = 0;
        }
    }while(1);
    fclose(fp2);
    return;
}


int main (int argc,char **argv)
{   
    getGrammar("Grammar.txt");
    printf("FIRST and FOLLOW set automated \n");
    printf("Both lexical and syntax analysis modules implemented");
    int choice;
	clock_t start, end;
	double total_CPU_time,total_CPU_time_in_seconds;
    while(1){
		printf("Please Select Option\n");
		printf("***********************\n");
		printf("1. Press 1 to print the list of tokens on the console along with lexeme and line number. \n");
		printf("2. Press 2 to only parse the input file and print corresponding errors and parse tree. \n");
		printf("3. Press 3 to create and print the abstract syntax tree. \n");
		printf("4. Press 4 to to display the total allocated memory. \n");
		printf("5. Press 0 for exit. \n");
		printf("\n");    
		scanf("%d", &choice);
		token* temp;
		tree_node* temp2;
		FILE* rando;
        switch(choice){
 			case 0: printf("Exiting!\n");
					return 0;

            case 1: 
                    fp1=fopen(argv[1],"r");
                    fp1=getstream(fp1);
                    temp=getNextToken();
                    while(temp !=NULL && strcmp(temp -> token_name,"FINISH")!=0)
                    {
                        printf("%d",temp->line_num);
                        printf("%s","    ");
                        printf("%s",temp->value);
                        printf("%s","    ");
                        printf("%s",temp->token_name);
						printf("\n");
                        temp = getNextToken();

                    }

                    return 0;

            case 2: getGrammar("Grammar.txt");
    				F = (FirstAndFollow *)malloc(sizeof(FirstAndFollow));
    				F = ComputeFirstAndFollowSets(G,F);
    				CreateParseTable();
    				//rando = fopen(argv[2], "w");
    				//fclose(rando);
    				temp2 = parseInputSourceCode(argv[1]);
    				printParseTree(temp2 , NULL);
    				return 0;
            case 3: 
                    getGrammar("Grammar.txt");
    				F = (FirstAndFollow *)malloc(sizeof(FirstAndFollow));
    				F = ComputeFirstAndFollowSets(G,F);
    				CreateParseTable();
    				//rando = fopen(argv[2], "w");
    				//fclose(rando);
    				temp2 = parseInputSourceCode(argv[1]);
    				createAST(temp2);
    				printf("AST is printed in the following order starting from the root node. First current node is printed then print function is recursively called on next(if it is part of a linked list), then on leftmost child and finally on right sibling, similar to a dfs. \n");
    				printf("For each node we print serial number, node type(int), node type(string), and line num (if the ast node corresponds to a token). \n");
    				printAST(temp2 -> syn);
                    return 0;
            case 4:
					getGrammar("Grammar.txt");
    				F = (FirstAndFollow *)malloc(sizeof(FirstAndFollow));
    				F = ComputeFirstAndFollowSets(G,F);
    				CreateParseTable();
    				//rando = fopen(argv[2], "w");
    				//fclose(rando);
    				sizeP = 0;
    				temp2 = parseInputSourceCode(argv[1]);
    				createAST(temp2);
    				int count = traverseAST(temp2 -> syn);
    				printf("Number of nodes in AST : %d \n" , count);
    				ASTNode a;
    				printf("Size of each ASTNode : %ld bytes\n", sizeof(a));
    				printf("Total allocated memory to the AST is : %ld bytes \n \n", sizeof(a)*count);
    				printf("Number of node in the parse tree : %d bytes \n", sizeP);
    				printf("size of each parse tree node : %ld bytes \n", sizeof(*temp2));
    				printf("Total allocated memory to the Parse Tree is : %ld bytes \n \n", sizeof(*temp2)*sizeP);
       				printf("Therefore compression percentage is %f \n \n", ((float)sizeof(*temp2)*sizeP - sizeof(a)*count)/(sizeof(*temp2)*sizeP)*100);
                    return 0;
        }
    }
    return 0;
}
