
#include <stdio.h>
#include <stdlib.h>
#include "stdbool.h"
#include "lexer.c"

#define TOTAL_RULES 103
#define TERMINALS 62
#define NON_TERMINALS 56
#define TERMS 118
#define EPLS 117
#define STACK_SIZE 50

typedef struct astNode{
	int type;
    int isLeaf;
    struct astNode* parent;
    struct astNode* leftmost_child;
    struct astNode* right;
    struct astNode* next;    // if it is part of a linked list, for example variable declarations etc.
	int typeN; // only if it is a typenode.
    token* tok; // if is leaf
    int lineno;
} ASTNode;

typedef struct Parse_Tree{				//ADT for the parse tree
	int isTerminal;
	int id;
	int isTraversed;
	token* tok;							// used only if leaf
	struct Parse_Tree *leftmost_child;
	struct Parse_Tree *left;
	struct Parse_Tree *right;
	struct Parse_Tree *parent;
	int isleaf;
	ASTNode* syn;
	ASTNode* inh;
} tree_node;

typedef struct Stack{
	int top;
	int capacity;
	int* arr;
} stack;

typedef struct term_struct{
    char name[20];
    int flag;
    struct term_struct *next;
} term_node;

term_node* linked_list[TOTAL_RULES];

typedef struct firstfollow{
    int fftable[TERMS][2][TERMS];
} FirstAndFollow ;


//to handle all the terms

static char* terms[118] = { 
	"program","moduleDeclarations","otherModules","driverModule","term","moduleDeclaration","N5","moduleDef","module","input_plist","N1","N2","ret","output_plist","dataType","type","range_arrays","range","statements","statement","ioStmt","simpleStmt","conditionalStmt","iterativeStmt","var","var_id_num","whichId","assignmentStmt","moduleReuseStmt","whichStmt","op1","op2","lvalueIDStmt","lvalueARRStmt","expression","U","Index","optional","idList","N3","arithmeticExpr","new_NT","factor","arithmeticOrBooleanExpr","boolConstt","AnyTerm","logicalOp","relationalOp","caseStmts","Default","N4","N7","N8","N9","value","declareStmt","$","INTEGER","REAL","BOOLEAN","OF","ARRAY","START","END","DECLARE","MODULE","DRIVER","PROGRAM","RECORD","TAGGED","UNION","GET_VALUE","PRINT","USE","WITH","PARAMETERS","TRUE","FALSE","TAKES","INPUT","RETURNS","AND","OR","FOR","IN","SWITCH","CASE","BREAK","DEFAULT","WHILE","DRIVERDEF","DEF","LE","LT","DRIVERENDDEF","ENDDEF","GE","GT","COLON","NE","EQ","PLUS","MINUS","DIV","RANGEOP","SEMICOL","COMMA","SQBO","SQBC","BO","BC","COMMENTMARK","MUL","ID","NUM","ASSIGNOP","RNUM","eps"};

int G[103][20];

typedef struct table{
int parsetable[56][61];
}Table;

Table T;

FirstAndFollow * F;

