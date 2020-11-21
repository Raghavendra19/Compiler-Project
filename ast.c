#include<stdio.h>
#include<stdlib.h>

#define ROOTNODE 0
#define IDNODE 1
#define MODULENODE 2
#define INPUTPARAMNODE 3
#define OUTPUTPARAMNODE 4
#define ARRAYTYPENODE 5
#define TYPENODE 6
#define RANGENODE 7
#define FORLOOPNODE 8
#define WHILELOOPNODE 9
#define INPUTIONODE 10
#define OUTPUTIONODE 11
#define ARRAYIDNODE 12
#define ASSIGNMENTNODE 13
#define FUNCTIONCALLNODE 14
#define NUMLEAF 15
#define BOOLLEAF 16
#define OPNODE 17
#define DECLARENODE 18
#define CONDITIONALNODE 19
#define CASENODE 20
#define NULLNODE 21

#define INTEGER 1
#define REAL 2
#define BOOLEAN 3

static char* NODETypes[22] = { "ROOTNODE","IDNODE"," MODULENODE","INPUTPARAMNODE"," OUTPUTPARAMNODE"," ARRAYTYPENODE","TYPENODE"," RANGENODE","FORLOOPNODE","WHILELOOPNODE","INPUTIONODE","OUTPUTIONODE","ARRAYIDNODE","ASSIGNMENTNODE","FUNCTIONCALLNODE"," NUMLEAF"," BOOLLEAF","OPNODE"," DECLARENODE ","CONDITIONALNODE","CASENODE","NULLNODE"};

int astCount;

int c = 0;

void printAST(ASTNode* root){
	if(root == NULL){
		return;
	}
	printf("%d %d %s %d\n", c++, root -> type, NODETypes[root -> type], root -> lineno);
	printAST(root -> next);
	printAST(root -> leftmost_child);
	printAST(root -> right);
	return;
}

int traverseAST(ASTNode* root){
	if(root == NULL){
		return 0;
	}
	return 1 + traverseAST(root -> next) + traverseAST(root -> leftmost_child) + traverseAST(root -> right);
}

ASTNode* makeNode(int type1, int isLeaf1, token* lexicalTOKEN1){
    ASTNode* newNode = malloc(sizeof(ASTNode));
    if(isLeaf1 == 0){
        newNode -> type = type1;
        newNode -> isLeaf = isLeaf1;
    }
    else{
        newNode -> type = type1;
        newNode -> isLeaf = 1;
        newNode -> tok = lexicalTOKEN1;
        if(lexicalTOKEN1 != NULL){
        	newNode -> lineno = lexicalTOKEN1 -> line_num;
        }
    }
    return newNode;
}

void addchildren(ASTNode* A, ASTNode* B, ASTNode* C){
	if(B == NULL){
		B = makeNode(NULLNODE, 1, NULL);
	}
    else if(C == NULL){
        C = makeNode(NULLNODE, 1, NULL);
    }
    A -> leftmost_child = B;
    B -> right = C;
    B -> parent = A;
    C -> parent = A;
}

void insert(ASTNode* A, ASTNode* B){
	if(A == NULL){
		return; 	
	}
	//printf("inserting %d in %d", A -> type, A -> type);
    if(B == NULL){
        B = A;
        return;
    }
    ASTNode* temp = malloc(sizeof(ASTNode));
    temp = B;
    while(temp -> next != NULL){
        temp = temp -> next;
    }
    temp -> next = A;
    if(A != NULL){
    	A -> next = NULL;
    }
    //printf("insert finished");
    return;
}

void createAST(tree_node* root){
    if(root == NULL){
        return;
    }
    else{
    	//printf(" here for %d \n", root -> id);
        ASTNode* curr;
        ASTNode* newCurr;
        switch(root -> id){
            case 0 : // program
                createAST(root -> leftmost_child);
                createAST(root -> leftmost_child -> right);
                createAST(root -> leftmost_child -> right -> right);
                createAST(root -> leftmost_child -> right -> right -> right);
                root -> leftmost_child -> syn -> right = root -> leftmost_child -> right -> syn;
                root -> leftmost_child -> right -> syn -> right = root -> leftmost_child -> right -> right -> syn;
                root -> leftmost_child -> right -> right -> syn -> right = root -> leftmost_child -> right -> right -> right -> syn;
                curr = makeNode(ROOTNODE, 0, root -> tok);
                ++astCount;
                curr -> leftmost_child = root -> leftmost_child -> syn;
                curr -> leftmost_child -> parent = curr;
                curr -> leftmost_child -> right -> parent = curr;
                curr -> leftmost_child -> right -> right -> parent = curr;
                curr -> leftmost_child -> right -> right -> right -> parent = curr;
                root -> syn = curr;
                break;
            
            case 1:      // moduledeclarations
                if(strcmp(terms[root -> leftmost_child -> id], "moduleDeclaration") == 0){
                    createAST(root -> leftmost_child);
                    //printf("reached");
                    
                    //printf("reached");
                    root -> leftmost_child -> right -> inh = root -> leftmost_child -> syn;
                    createAST(root -> leftmost_child -> right);
                    insert(root -> inh, root -> leftmost_child -> right -> syn);
                    root -> syn = root -> leftmost_child -> right -> syn;
                }
                else if(strcmp(terms[root -> leftmost_child -> id], "eps") == 0){
                    if(root -> inh == NULL){
                        curr = makeNode(NULLNODE, 1, NULL);
                        root -> inh = curr;
                    }
                    root -> syn = root -> inh;
                    //printf("hello");
                }
                else {
                    printf("should be here %d", 1);
                }
            break;
            case 5:     //moduledeclaration
                if(strcmp(terms[root -> leftmost_child -> id], "DECLARE") == 0){
                    curr = makeNode(IDNODE, 1, root -> leftmost_child -> right -> right -> tok);       // idnode
                    root -> syn = curr;
                }
            break;
            case 2:    // othermodules
                if(strcmp(terms[root -> leftmost_child -> id], "module") == 0){
                    createAST(root -> leftmost_child);
                    root -> leftmost_child -> right -> inh = root -> leftmost_child -> syn;
                    createAST(root -> leftmost_child -> right);
                    insert(root -> inh, root -> leftmost_child -> right -> syn);
                    root -> syn = root -> leftmost_child -> right -> syn;
                }
                else if(strcmp(terms[root -> leftmost_child -> id], "eps") == 0){
                    if(root -> inh == NULL){
                        curr = makeNode(NULLNODE, 1, NULL);
                        root -> inh = curr;
                    }
                    root -> syn = root -> inh;
                }
                else {
                    printf("shouldnt be here 2");
                }
            break;
            case 3:     //drivermodule
                if(strcmp(terms[root -> leftmost_child -> id], "DRIVERDEF") == 0){
                    createAST(root -> leftmost_child -> right -> right -> right -> right);
                    root -> syn = root -> leftmost_child -> right -> right -> right -> right -> syn;
                }
                else{
                    printf("shouldnt be here 3");
                }
            break;
            case 8: // module
                createAST(root -> leftmost_child -> right -> right -> right -> right -> right -> right -> right);
                createAST(root -> leftmost_child -> right -> right -> right -> right -> right -> right -> right -> right -> right -> right);
                createAST(root -> leftmost_child -> right -> right -> right -> right -> right -> right -> right -> right -> right -> right -> right);
                curr = makeNode(IDNODE, 1, root -> leftmost_child -> right -> right -> tok);
                curr -> right = root -> leftmost_child -> right -> right -> right -> right -> right -> right -> right -> syn;
                curr -> right -> right = root -> leftmost_child -> right -> right -> right -> right -> right -> right -> right -> right -> right -> right -> syn;
                curr -> right -> right -> right = root -> leftmost_child -> right -> right -> right -> right -> right -> right -> right -> right -> right -> right -> right -> syn;
                newCurr = makeNode(MODULENODE, 0, NULL);
                newCurr -> leftmost_child = curr;
                curr -> parent = newCurr;
                curr -> right -> parent = newCurr;
                curr -> right -> right -> parent = newCurr;
                curr -> right -> right -> right -> parent = newCurr;
                root -> syn = newCurr;
            break;
            case 12:    //ret
                if(strcmp(terms[root -> leftmost_child -> id], "RETURNS") == 0){
                	createAST(root -> leftmost_child -> right -> right);
                    root -> syn = root -> leftmost_child -> right -> right -> syn;
                }
                else{    
                    curr = makeNode(NULLNODE, 1, NULL);
                    root -> syn = curr;
                }
            break;
            case 9: //input plist
                createAST(root -> leftmost_child -> right -> right);
                curr = makeNode(IDNODE, 1, root -> leftmost_child -> tok);
                curr -> right = root -> leftmost_child -> right -> right -> syn;
                newCurr = makeNode(INPUTPARAMNODE, 0, NULL);
                newCurr -> leftmost_child = curr;
                curr -> parent = newCurr;
                curr -> right -> parent = newCurr;
                root -> leftmost_child -> right -> right -> right -> inh = newCurr;
                createAST(root -> leftmost_child -> right -> right -> right);
                root -> syn = root -> leftmost_child -> right -> right -> right -> syn;
            break;
            case 10: //N1
                if(strcmp(terms[root -> leftmost_child -> id], "COMMA") == 0){
                    createAST(root -> leftmost_child -> right -> right -> right);
                    curr = makeNode(IDNODE, 1, root -> leftmost_child -> right -> tok);
                    curr -> right = root -> leftmost_child -> right -> right -> right -> syn;
                    newCurr = makeNode(INPUTPARAMNODE, 0, NULL);
                    newCurr -> leftmost_child = curr;
                    curr -> parent = newCurr;
                    curr -> right -> parent = newCurr;
                    root -> leftmost_child -> right -> right -> right -> right -> inh = newCurr;
                    createAST(root -> leftmost_child -> right -> right -> right -> right);
                    insert(root -> inh, root -> leftmost_child -> right -> right -> right -> right -> syn);
                    root -> syn = root -> leftmost_child -> right -> right -> right -> right -> syn;
                }
                else{
                    if(root -> inh == NULL){
                        curr = makeNode(NULLNODE, 1, NULL);
                        root -> inh = curr;
                    }
                    root -> syn = root -> inh;
                }
            break;
            case 13: // out plist
                createAST(root -> leftmost_child -> right -> right);
                curr = makeNode(IDNODE, 1, root -> leftmost_child -> tok);
                curr -> right = root -> leftmost_child -> right -> right -> syn;
                newCurr = makeNode(OUTPUTPARAMNODE, 0, NULL);
                newCurr -> leftmost_child = curr;
                curr -> parent = newCurr;
                curr -> right -> parent = newCurr;
                root -> leftmost_child -> right -> right -> right -> inh = newCurr;
                createAST(root -> leftmost_child -> right -> right -> right);
                root -> syn = root -> leftmost_child -> right -> right -> right -> syn;
            break;
            case 11:  //N2
                if(strcmp(terms[root -> leftmost_child -> id], "COMMA") == 0){
                	createAST(root -> leftmost_child -> right -> right -> right);
                    curr = makeNode(IDNODE, 1, root -> leftmost_child -> right -> tok);
                    curr -> right = root -> leftmost_child -> right -> right -> right -> syn;
                    newCurr = makeNode(OUTPUTPARAMNODE, 0, NULL);
                    newCurr -> leftmost_child = curr;
                    curr -> parent = newCurr;
                    curr -> right -> parent = newCurr;
                    root -> leftmost_child -> right -> right -> right -> right -> inh = newCurr;
                    createAST(root -> leftmost_child -> right -> right -> right -> right);
                    insert(root -> inh, root -> leftmost_child -> right -> right -> right -> right -> syn);
                    root -> syn = root -> leftmost_child -> right -> right -> right -> right -> syn;
                }
                else{
                    if(root -> inh == NULL){
                        curr = makeNode(NULLNODE, 1, NULL);
                        root -> inh = curr;
                    }
                    root -> syn = root -> inh;
                }
            break;
            case 14:   //data type
                if(strcmp(terms[root -> leftmost_child -> id], "ARRAY") == 0){
                    createAST(root -> leftmost_child -> right -> right);
                    createAST(root -> leftmost_child -> right -> right -> right -> right -> right);
                    curr = makeNode(ARRAYTYPENODE, 0, NULL);
                    curr -> leftmost_child = root -> leftmost_child -> right -> right -> syn;
                    curr -> leftmost_child -> right = root -> leftmost_child -> right -> right -> right -> right -> right -> syn;
                    root -> syn = curr;
                }
                else if(strcmp(terms[root -> leftmost_child -> id], "INTEGER") == 0){
                    curr = makeNode(TYPENODE, 1, NULL);
                    curr -> typeN = INTEGER;
                    root -> syn = curr;
                }
                else if(strcmp(terms[root -> leftmost_child -> id], "REAL") == 0){
                    curr = makeNode(TYPENODE, 1, NULL);
                    curr -> typeN = REAL;
                    root -> syn = curr;
                }
                else{
                    curr = makeNode(TYPENODE, 1, NULL);
                    curr -> typeN = BOOLEAN;
                    root -> syn = curr;
                }
            break;
            case 16: // range arrays
                createAST(root -> leftmost_child);
                createAST(root -> leftmost_child -> right -> right);
                curr = makeNode(RANGENODE, 0, NULL);
                curr -> leftmost_child = root -> leftmost_child -> syn;
                curr -> leftmost_child -> right = root -> leftmost_child -> right -> right -> syn;
                curr -> leftmost_child -> parent = curr;
                curr -> leftmost_child -> right -> parent = curr;
                root -> syn = curr;
            break;
            case 15:  // type
                if(strcmp(terms[root -> leftmost_child -> id], "INTEGER") == 0){
                    curr = makeNode(TYPENODE, 1, NULL);
                    curr -> typeN = INTEGER;
                    root -> syn = curr;
                }
                else if(strcmp(terms[root -> leftmost_child -> id], "REAL") == 0){
                    curr = makeNode(TYPENODE, 1, NULL);
                    curr -> typeN = REAL;
                    root -> syn = curr;
                }
                else{
                    curr = makeNode(TYPENODE, 1, NULL);
                    curr -> typeN = BOOLEAN;
                    root -> syn = curr;
                }
            break;
            case 7: // moduleDef
                createAST(root -> leftmost_child -> right);
                root -> syn = root -> leftmost_child -> right -> syn;
            break;
            case 18: //statements
                if(strcmp(terms[root -> leftmost_child -> id], "statement") == 0){
                    createAST(root -> leftmost_child);
                    root -> leftmost_child -> right -> inh = root -> leftmost_child -> syn;
                    createAST(root -> leftmost_child -> right);
                    insert(root -> inh, root -> leftmost_child -> right -> syn);
                    root -> syn = root -> leftmost_child -> right -> syn;
                }
                else{
                    if(root -> inh == NULL){
                        curr = makeNode(NULLNODE, 1, NULL);
                        root -> inh = curr;
                    }
                    root -> syn = root -> inh;
                }
            break;
            case 19: //statement
                if(strcmp(terms[root -> leftmost_child -> id], "ioStmt") == 0){
                    createAST(root -> leftmost_child);
                    root -> syn = root -> leftmost_child -> syn;
                }
                else if(strcmp(terms[root -> leftmost_child -> id], "simpleStmt") == 0){
                    createAST(root -> leftmost_child);
                    root -> syn = root -> leftmost_child -> syn;
                }
                else if(strcmp(terms[root -> leftmost_child -> id], "declareStmt") == 0){
                    createAST(root -> leftmost_child);
                    root -> syn = root -> leftmost_child -> syn;
                }
                else if(strcmp(terms[root -> leftmost_child -> id], "conditionalStmt") == 0){
                    createAST(root -> leftmost_child);
                    root -> syn = root -> leftmost_child -> syn;
                }
                else{
                    createAST(root -> leftmost_child);
                    root -> syn = root -> leftmost_child -> syn;
                }
            break;
            case 23: //iterative Stmt
                if(strcmp(terms[root -> leftmost_child -> id], "FOR") == 0){
                    createAST(root -> leftmost_child -> right -> right -> right -> right);
                    createAST(root -> leftmost_child -> right -> right -> right -> right -> right -> right -> right); 
                    curr = makeNode(IDNODE, 1, root -> leftmost_child -> right -> right -> tok);
                    newCurr = makeNode(FORLOOPNODE, 0, NULL);
                    newCurr -> leftmost_child = curr;
                    curr -> right = root -> leftmost_child -> right -> right -> right -> right -> syn;
                    curr -> right -> right = root -> leftmost_child -> right -> right -> right -> right -> right -> right -> right -> syn;
                    curr -> parent = newCurr;
                    curr -> right -> parent = newCurr;
                    curr -> right -> right -> parent = newCurr;
                    root -> syn = newCurr;
                }
                else{
                    createAST(root -> leftmost_child -> right -> right);
                    createAST(root -> leftmost_child -> right -> right -> right -> right -> right);
                    curr = makeNode(WHILELOOPNODE, 0, NULL);
                    curr -> leftmost_child = root -> leftmost_child -> right -> right -> syn;
                    curr -> leftmost_child -> right = root -> leftmost_child -> right -> right -> right -> right -> right -> syn;
                    root -> syn = curr;
                }
            break;
            case 20:  // ioStmt
                if(strcmp(terms[root -> leftmost_child -> id], "GET_VALUE") == 0){
                    curr = makeNode(IDNODE, 1, root -> leftmost_child -> right -> right -> tok);
                    newCurr = makeNode(INPUTIONODE, 0, NULL);
                    newCurr -> leftmost_child = curr;
                    curr -> parent = newCurr;
                    root -> syn = newCurr;
                }
                else{
                    createAST(root -> leftmost_child -> right -> right);
                    curr = makeNode(OUTPUTIONODE, 0, NULL);
                    curr -> leftmost_child = root -> leftmost_child -> right -> right -> syn;
                    root -> leftmost_child -> right -> right -> syn -> parent = curr;
                    root -> syn = curr;
                }
            break;
            case 24:   //var
                if(strcmp(terms[root -> leftmost_child -> id], "var_id_num") == 0){
                    createAST(root -> leftmost_child);
                    root -> syn = root -> leftmost_child -> syn;
                }
                else{
                    createAST(root -> leftmost_child);
                    root -> syn = root -> leftmost_child -> syn;
                }
            break;
            case 26:    //whichId
                if(strcmp(terms[root -> leftmost_child -> id], "SQBO") == 0){
                    createAST(root -> leftmost_child -> right);
                    curr = makeNode(ARRAYIDNODE, 0, NULL);
                    curr -> leftmost_child = root -> inh;
                    curr -> leftmost_child -> right = root -> leftmost_child -> right -> syn;
                    curr -> leftmost_child -> parent = curr;
                    curr -> leftmost_child -> right -> parent = curr;
                    root -> syn = curr;
                }
                else{
                    if(root -> inh == NULL){
                        curr = makeNode(NULLNODE, 1, NULL);
                        root -> inh = curr;
                    }
                    root -> syn = root -> inh;
                }
            break;
            case 21: //simpleStmt
                if(strcmp(terms[root -> leftmost_child -> id], "assignmentStmt") == 0){
                    createAST(root -> leftmost_child);
                    root -> syn = root -> leftmost_child -> syn;    
                }
                else{
                    createAST(root -> leftmost_child);
                    root -> syn = root -> leftmost_child -> syn;
                }
            break;
            case 27:  //assignmentStmt
                curr = makeNode(IDNODE, 1, root -> leftmost_child -> tok);
                root -> leftmost_child -> right -> inh = curr;
                createAST(root -> leftmost_child -> right);
                root -> syn = root -> leftmost_child -> right -> syn;
            break;
            case 29: // whichStmt
                if(strcmp(terms[root -> leftmost_child -> id], "lvalueIDStmt") == 0){
                    root -> leftmost_child -> inh = root -> inh;
                    createAST(root -> leftmost_child);
                    root -> syn = root -> leftmost_child -> syn;
                }
                else{
                    root -> leftmost_child -> inh = root -> inh;
                    createAST(root -> leftmost_child);
                    root -> syn = root -> leftmost_child -> syn;
                }
            break;
            case 32: //lvalueIDStmt
                createAST(root -> leftmost_child -> right);
                curr = makeNode(ASSIGNMENTNODE, 0, NULL);
                curr -> leftmost_child = root -> inh;
                curr -> leftmost_child -> right = root -> leftmost_child -> right -> syn;
                curr -> leftmost_child -> parent = curr;
                curr -> leftmost_child -> right -> parent = curr;
                root  -> syn = curr;
            break;  
            case 33: //lvalueARRStmt
                createAST(root -> leftmost_child -> right);
                createAST(root -> leftmost_child -> right -> right -> right -> right);
                curr = makeNode(ARRAYIDNODE, 0, NULL);
                curr -> leftmost_child = root -> inh;
                curr -> leftmost_child -> right = root -> leftmost_child -> right -> syn;
                curr -> leftmost_child -> parent = curr;
                curr -> leftmost_child -> right -> parent = curr;
                curr -> right = root -> leftmost_child -> right -> right -> right -> right -> syn;
                newCurr = makeNode(ASSIGNMENTNODE, 0, NULL);
                newCurr -> leftmost_child = curr;
                curr -> parent = newCurr;
                curr -> right -> parent = newCurr;
                root -> syn = newCurr;
            break;
            case 36:   //Index
                if(strcmp(terms[root -> leftmost_child -> id], "NUM") == 0){
                    curr = makeNode(NUMLEAF, 1, root -> leftmost_child -> tok);
                    root -> syn = curr;
                }
                else{
                    curr = makeNode(IDNODE, 1, root -> leftmost_child -> tok);
                    root -> syn = curr;
                }
            break;
            case 28:  //moduleReuseStmt
            	root -> leftmost_child -> inh = NULL;
                createAST(root -> leftmost_child);
                createAST(root -> leftmost_child -> right -> right -> right -> right -> right -> right);
                curr = makeNode(IDNODE, 1, root -> leftmost_child -> right -> right -> right -> tok);
                root -> leftmost_child -> syn -> right = curr;
                curr -> right = root -> leftmost_child -> right -> right -> right -> right -> right -> right -> syn;
                newCurr = makeNode(FUNCTIONCALLNODE, 0, NULL);
                newCurr -> leftmost_child = root -> leftmost_child -> syn;
                newCurr -> leftmost_child -> parent = newCurr;
                newCurr -> leftmost_child -> right -> parent = newCurr;
                newCurr -> leftmost_child -> right -> right -> parent = newCurr;
                root -> syn = newCurr;
            break;
            case 37: //optional
                if(strcmp(terms[root -> leftmost_child -> id], "SQBO") == 0){
                    createAST(root -> leftmost_child -> right);
                    root -> syn = root -> leftmost_child -> right -> syn;
                }
                else{
                    if(root -> inh == NULL){
                        curr = makeNode(NULLNODE, 1, NULL);
                        root -> inh = curr;
                    }
                    root -> syn = root -> inh;
                }
            break;
            case 38: //idList
                curr = makeNode(IDNODE, 1, root -> leftmost_child -> tok);
                root -> leftmost_child -> right -> inh = curr;
                createAST(root -> leftmost_child -> right);
                root -> syn = root -> leftmost_child -> right -> syn;
            break;
            case 39:   //N3
                if(strcmp(terms[root -> leftmost_child -> id], "COMMA") == 0){
                    curr = makeNode(IDNODE, 1, root -> leftmost_child -> right -> tok);
                    root -> leftmost_child -> right -> right -> inh = curr;
                    createAST(root -> leftmost_child -> right -> right);
                    insert(root -> inh, root -> leftmost_child -> right -> right -> syn);
                    root -> syn = root -> leftmost_child -> right -> right -> syn;
                }
                else{
                    if(root -> inh == NULL){
                        curr = makeNode(NULLNODE, 1, NULL);
                        root -> inh = curr;
                    }
                    root -> syn = root -> inh;
                }
            break;
            case 34: //expression
                if(strcmp(terms[root -> leftmost_child -> id], "arithmeticOrBooleanExpr") == 0){
                    createAST(root -> leftmost_child);
                    root -> syn = root -> leftmost_child -> syn;
                }
                else{
                    createAST(root -> leftmost_child);
                    root -> syn = root -> leftmost_child -> syn;
                }
            break;
            case 35: //U
                createAST(root -> leftmost_child);
                createAST(root -> leftmost_child -> right);
                
                // addchildren
                addchildren(root -> leftmost_child -> syn, NULL, root -> leftmost_child -> right -> syn);

                root -> syn = root -> leftmost_child -> syn;
            break;
            case 41: //new_NT
                if(strcmp(terms[root -> leftmost_child -> id], "BO") == 0){
                    createAST(root -> leftmost_child -> right);
                    root -> syn = root -> leftmost_child -> right -> syn;
                }
                else{
                    createAST(root -> leftmost_child);
                    root -> syn = root -> leftmost_child -> syn;
                }
            break;
            case 43:    //arithmeticOrBooleanExpr
                createAST(root -> leftmost_child);
                root -> leftmost_child -> right -> inh = root -> leftmost_child -> syn;
                createAST(root -> leftmost_child -> right);
                root -> syn = root -> leftmost_child -> right -> syn;
            break;
            case 51:   //N7
                if(strcmp(terms[root -> leftmost_child -> id], "logicalOp") == 0){
                    createAST(root -> leftmost_child);
                    createAST(root -> leftmost_child -> right);
                    

                    //addchildren
                    addchildren(root -> leftmost_child -> syn, root -> inh, root -> leftmost_child -> right -> syn);
                   

                    root -> leftmost_child -> right -> right -> inh = root -> leftmost_child -> syn;
                    createAST(root -> leftmost_child -> right -> right);
                    root -> syn = root -> leftmost_child -> right -> right -> syn;
                }
                else{
                    if(root -> inh == NULL){
                        curr = makeNode(NULLNODE, 1, NULL);
                        root -> inh = curr;
                    }
                    root -> syn = root -> inh;
                }
            break;
            case 45: //anyterm
                if(strcmp(terms[root -> leftmost_child -> id], "arithmeticExpr") == 0){
                    createAST(root -> leftmost_child);
                    
                    root -> leftmost_child -> right -> inh = root -> leftmost_child -> syn;
                    createAST(root -> leftmost_child -> right);
                    root -> syn = root -> leftmost_child -> right -> syn;
                }
                else{
                    createAST(root -> leftmost_child);
                    root -> syn = root -> leftmost_child -> syn;
                }
            break;
            case 44://boolConstt
                if(strcmp(terms[root -> leftmost_child -> id], "TRUE") == 0){
                    curr = makeNode(BOOLLEAF, 1, root -> leftmost_child -> tok);
                    root -> syn = curr;
                }
                else{
                    curr = makeNode(BOOLLEAF, 1, root -> leftmost_child -> tok);
                    root -> syn = curr;
                }
            break;
            case 52:    //N8
                if(strcmp(terms[root -> leftmost_child -> id], "relationalOp") == 0){
                    createAST(root -> leftmost_child);
                    createAST(root -> leftmost_child -> right);

                    //addchildren
                    addchildren(root -> leftmost_child -> syn, root -> inh, root -> leftmost_child -> right -> syn);

                    root -> syn = root -> leftmost_child -> syn;
                }
                else{
                    if(root -> inh == NULL){
                        curr = makeNode(NULLNODE, 1, NULL);
                        root -> inh = curr;
                    }
                    root -> syn = root -> inh;
                }
            break;
            case 40: //arithmeticExpr
                createAST(root -> leftmost_child);
                
                root -> leftmost_child -> right -> inh = root -> leftmost_child -> syn;
                createAST(root -> leftmost_child -> right);
                root -> syn = root -> leftmost_child -> right -> syn;
            break;
            case 50:  //N4
                if(strcmp(terms[root -> leftmost_child -> id], "op1") == 0){
                    createAST(root -> leftmost_child);
                    createAST(root -> leftmost_child -> right);
                    

                    //addchildren
                    addchildren(root -> leftmost_child -> syn, root -> inh, root -> leftmost_child -> right -> syn);

                    root -> leftmost_child -> right -> right -> inh = root -> leftmost_child -> syn;
                    createAST(root -> leftmost_child -> right -> right);
                    root -> syn = root -> leftmost_child -> right -> right -> syn;
                }
                else{
                    if(root -> inh == NULL){
                        curr = makeNode(NULLNODE, 1, NULL);
                        root -> inh = curr;
                    }
                    root -> syn = root -> inh;
                }
            break;
            case 4: //term
                createAST(root -> leftmost_child);
                
                root -> leftmost_child -> right -> inh = root -> leftmost_child -> syn;
                createAST(root -> leftmost_child -> right);
                root -> syn = root -> leftmost_child -> right -> syn;
            break;
            case 6: //N5
                if(strcmp(terms[root -> leftmost_child -> id], "op2") == 0){
                    createAST(root -> leftmost_child);
                    createAST(root -> leftmost_child -> right);
                    

                    //addchildren
                    addchildren(root -> leftmost_child -> syn, root -> inh, root -> leftmost_child -> right -> syn);

                    root -> leftmost_child -> right -> right -> inh = root -> leftmost_child -> syn;
                    createAST(root -> leftmost_child -> right -> right);
                    root -> syn = root -> leftmost_child -> right -> right -> syn;
                }
                else{
                    if(root -> inh == NULL){
                        curr = makeNode(NULLNODE, 1, NULL);
                        root -> inh = curr;
                    }
                    root -> syn = root -> inh;
                }
            break;
            case 42: // factor
                if(strcmp(terms[root -> leftmost_child -> id], "BO") == 0){
                    createAST(root -> leftmost_child -> right);
                    root -> syn = root -> leftmost_child -> right -> syn;
                }
                else{
                    createAST(root -> leftmost_child);
                    root -> syn = root -> leftmost_child -> syn;
                }
            break;
            case 25: //var_id_num
                if(strcmp(terms[root -> leftmost_child -> id], "ID") == 0){
                    
                    curr = makeNode(IDNODE, 1, root -> leftmost_child -> tok);
                    root -> leftmost_child -> right -> inh = curr;
                    createAST(root -> leftmost_child -> right);
                    root -> syn = root -> leftmost_child -> right -> syn;
                }
                else if(strcmp(terms[root -> leftmost_child -> id],"NUM") == 0){
                    curr = makeNode(NUMLEAF, 1, root -> leftmost_child -> tok);
                    root -> syn = curr;
                }
                else{
                    curr = makeNode(NUMLEAF, 1, root -> leftmost_child -> tok);
                    root -> syn = curr;
                }
            break;
            case 30: //op1
                curr = makeNode(OPNODE, 1, root -> leftmost_child -> tok);
                root -> syn = curr;
            break;
            case 31: //op2
                curr = makeNode(OPNODE, 1, root -> leftmost_child -> tok);
                root -> syn = curr;
            break;
            case 46: //logicalOp
                curr = makeNode(OPNODE, 1, root -> leftmost_child -> tok);
                root -> syn = curr;
            break;
            case 47: //relationalOp
                curr = makeNode(OPNODE, 1, root -> leftmost_child -> tok);
                root -> syn = curr;
            break;
            case 55: //declareStmt
                createAST(root -> leftmost_child -> right);
                createAST(root -> leftmost_child -> right -> right -> right);
                curr = makeNode(DECLARENODE, 0, NULL);
                root -> syn = curr;
            break;
            case 22: //conditionalStmt
                createAST(root -> leftmost_child -> right -> right -> right -> right -> right);
                createAST(root -> leftmost_child -> right -> right -> right -> right -> right -> right);
                curr = makeNode(IDNODE, 1, root -> leftmost_child -> right -> right -> tok);
                curr -> right = root -> leftmost_child -> right -> right -> right -> right -> right -> syn;
                curr -> right -> right = root -> leftmost_child -> right -> right -> right -> right -> right -> right -> syn;
                newCurr = makeNode(CONDITIONALNODE, 0, NULL);
                newCurr -> leftmost_child = curr;
                curr -> parent = newCurr;
                curr -> right -> parent = newCurr;
                curr -> right -> right -> parent = newCurr;
                root -> syn = newCurr;
            break;
            case 48: //caseStmts
                createAST(root -> leftmost_child -> right);
                createAST(root -> leftmost_child -> right -> right -> right);
                
                curr = makeNode(CASENODE, 0, NULL);
                curr -> leftmost_child = root -> leftmost_child -> right -> syn;
                curr -> leftmost_child -> right = root -> leftmost_child -> right -> right -> right -> syn;
                curr -> leftmost_child -> parent = curr;
                curr -> leftmost_child -> right -> parent = curr;
                root -> leftmost_child -> right -> right -> right -> right -> right -> right -> inh = curr;
                createAST(root -> leftmost_child -> right -> right -> right -> right -> right -> right);
                root -> syn = root -> leftmost_child -> right -> right -> right -> right -> right -> right -> syn;
            break;
            case 53: //N9
                if(strcmp(terms[root -> leftmost_child -> id], "CASE") == 0){
                    createAST(root -> leftmost_child -> right);
                    createAST(root -> leftmost_child -> right -> right -> right);
                    
                    curr = makeNode(CASENODE, 0, NULL);
                    curr -> leftmost_child = root -> leftmost_child -> right -> syn;
                    curr -> leftmost_child -> right = root -> leftmost_child -> right -> right -> right -> syn;
                    curr -> leftmost_child -> parent = curr;
                    curr -> leftmost_child -> right -> parent = curr;
                    root -> leftmost_child -> right -> right -> right -> right -> right -> right -> inh = curr;
                    createAST(root -> leftmost_child -> right -> right -> right -> right -> right -> right);
                    insert(root -> inh, root -> leftmost_child -> right -> right -> right -> right -> right -> right -> syn);
                    root -> syn = root -> leftmost_child -> right -> right -> right -> right -> right -> right -> syn;
                }
                else{
                    if(root -> inh == NULL){
                        curr = makeNode(NULLNODE, 1, NULL);
                        root -> inh = curr;
                    }
                    root -> syn = root -> inh;
                }
            break;
            case 54: //value
                if(strcmp(terms[root -> leftmost_child -> id], "NUM") == 0){
                    curr = makeNode(NUMLEAF, 1, root -> leftmost_child -> tok);
                    root -> syn = curr;
                }
                else{
                    curr = makeNode(BOOLLEAF, 1, root -> leftmost_child -> tok);
                    root -> syn = curr;
                }
            break;
            case 49: //Default
                if(strcmp(terms[root -> leftmost_child -> id], "DEFAULT") == 0){
                    createAST(root -> leftmost_child -> right -> right);
                    root -> syn = root -> leftmost_child -> right -> right -> syn;
                }
                else{
                    
                    curr = makeNode(NULLNODE, 1, NULL);
                    root -> syn = curr;
                    
                }
            break;
            case 17: //range
                curr = makeNode(NUMLEAF, 1, root -> leftmost_child -> tok);
                newCurr = makeNode(NUMLEAF, 1, root -> leftmost_child -> right -> right -> tok);
                curr -> right = newCurr;
                ASTNode* temp = makeNode(RANGENODE, 0, NULL);
                temp -> leftmost_child = curr;
                temp -> leftmost_child -> parent = temp;
                newCurr -> parent = temp;
                root -> syn = temp;
            break;
        }
    }
    //printf("done with %d \n", root -> id);
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
    printf("%d", getEnumVal("var_id_num"));
    
   	createAST(temp);
    return 0;
}*/

