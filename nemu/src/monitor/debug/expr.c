#include "nemu.h"
#include <stdlib.h>
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
//#include "elf.h"
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256,
	EQ,NEQ,
	AND,OR,
	HEX_NUM,DECI_NUM,SYMBLE,REG,
	NO,DEREF,MINUS,POSITIVE

	/* TODO: Add more token types */

};

uint32_t get_symbol_value(char* sym); //get a symble value

uint32_t hex_to_i(char *hex)
{/*transform hex to int*/
        int i;
        uint32_t num = 0;
        for(i = 2;hex[i];i ++) {
                if(hex[i] >='a' && hex[i] <= 'f') {
                        num = num *16 + hex[i] - 'a' + 10;
                }
                else if (hex[i] >= 'A' && hex[i] <= 'F') {
                        num = num *16 + hex[i] - 'A' +10;
                }
                else if (hex[i] >= '0' && hex[i] <='9') {
                        num = num *16 + hex[i] - '0';
                }
                else {
			printf("hex_number error !\n");
                        break;
		}
        }
                
        return num;
}



static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"==", EQ},					//equal
	{"-",'-'},					// minus
	{"\\*",'*'},					//multi or point
	{"/",'/'},					//divide
	{"\\(",'('},					//left_p
	{"\\)",')'},					//right_p
	{"0[xX][0-9a-fA-F]+",HEX_NUM},                   //hexadecimal number
	{"[0-9]+",DECI_NUM},				//decimal number
	{"\\$e[abcdsi][xiplh]",REG},			//reg_name
	{"!=",NEQ},					//no equal
	{"&&",AND},					//and 
	{"\\|\\|",OR},					//or
	{"!",NO},					//NO
	{"[a-zA-Z0-9_]+",SYMBLE}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;
	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
 		}
 	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;
bool is_legal;



uint32_t eval(int p,int q);
static bool check_parentheses(int p,int q);
static int find_dominant(int p,int q);
static bool parentheses_is_matched(int p,int q);

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				//Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {
					case NOTYPE:break;		/*ignore spaces*/
					default:
						tokens[nr_token].type=rules[i].token_type;
                                                strncpy(tokens[nr_token].str,substr_start,substr_len);
						tokens[nr_token].str[substr_len]='\0';
                                                nr_token++;	/* panic("please implement me");*/
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

//=======================================================================
void distinguish_op()			/*to distinguish the operator*/
{
	int i;
	for(i = 0; i < nr_token;i++)
                if(tokens[i].type == '*' && (i == 0 || (!(tokens[i-1].type >= HEX_NUM && tokens[i-1].type <= REG) && tokens[i-1].type != 41)))
                        tokens[i].type = DEREF;
                else if(tokens[i].type == '-' && (i == 0 ||(!(tokens[i-1].type >= HEX_NUM && tokens[i-1].type <= REG) && tokens[i-1].type != 41)))
                        tokens[i].type = MINUS;
                else if(tokens[i].type == '+' && (i == 0 ||(!(tokens[i-1].type >= HEX_NUM  &&  tokens[i-1].type <= REG) && tokens[i-1].type != 41)))
                        tokens[i].type = POSITIVE;
}

static bool is_legal1_op(int pos)
{      
	
	if(pos == nr_token-1) {
		printf("the operator %s at position %d is illegal.\n",tokens[pos].str,pos+1);
		return false;
	}
	int type = tokens[pos+1].type;
	if(type == '(' || (type >= HEX_NUM && type <= REG) || (type >= NO && type <= POSITIVE)) 
		return true;
	else {
		printf("the operater %s at position %d is illegal.\n",tokens[pos].str,pos+1);
		return false; 
	}

}

static bool is_legal2_op(int pos)
{
	int type_l = tokens[pos-1].type;
	int type_r = tokens[pos+1].type;
	if( pos == 0 || pos == nr_token-1) {
		printf("the operator %s at position %d is illegal.\n",tokens[pos].str,pos+1);
		return false;
	}
	else {
		if(!(type_l >= HEX_NUM && type_l <= REG) && type_l != ')'){
		 	printf("the operator %s at position %d is illegal.\n",tokens[pos].str,pos+1);
			return false;
		}
		if(!(type_r >= HEX_NUM && type_r <= REG) && type_r != '(' && type_r != POSITIVE && type_r != MINUS && type_r != NO && type_r != DEREF){
			printf("the operator %s at position %d is illegal.\n",tokens[pos].str,pos+1);
			return false;
		}
	}
	return true;
}

static bool is_legal_num(int pos)
{
	if(pos == 0 || pos == nr_token-1)
		return true;
	int type_l = tokens[pos-1].type;
	int type_r = tokens[pos+1].type;
	if(!(type_l >= HEX_NUM && type_l <= REG) && type_l != ')') {
		if(!(type_r >= HEX_NUM && type_r <= REG) && type_r != '(' && !(type_r >= NO && type_r <= POSITIVE)) {
			return true;
		}
		else { 
			printf("the operator %s at position %d is illegal.\n",tokens[pos].str,pos+1);
                        return false;
		}
			
	}
	else {
		printf("the operator %s at position %d is illegal.\n",tokens[pos].str,pos+1);
		return false;
	}	
}

static void is_legal_expr()
{       
	is_legal = true;
	if(!parentheses_is_matched(0,nr_token-1)) {
		printf("bracket not matched.\n");
		is_legal = false;
	}
	int pos;
        int type = 0;
	for(pos = 0; pos < nr_token; pos ++) {
		type = tokens[pos].type;
		if(type >= HEX_NUM && type <= REG) {
			if(!is_legal_num(pos))
				is_legal = false;
		}
		else if(type >=NO && type <= POSITIVE){
			if(!is_legal1_op(pos))
				is_legal = false;
		}
		else if(type != '(' && type != ')') {
			if(!is_legal2_op(pos))
				is_legal = false;
		}			
	}

}
//=======================================================================
uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	/*panic("please implement me");*/
	distinguish_op();

	/*judge expression*/

	is_legal_expr();
	if(is_legal)
		return eval(0,nr_token-1);
	else { 
		printf("Bad expression.\n");
		return 0;
	}
}

static bool parentheses_is_matched(int p,int q)
{
	int pos;
	int level=0;             /*flag of parenteses*/
	for(pos = p;pos <= q;pos++) {
		if(tokens[pos].type == '(')
			level++;
		else if(tokens[pos].type == ')')
			level--;
		if(level < 0)
			return false;
	}
	if(level == 0)
		return true;
	else
		return false;
}

static bool check_parentheses(int p,int q)
{
	if(tokens[p].type == '(' && tokens[q].type == ')') 
		return parentheses_is_matched(p+1,q-1);
	else
		return false;
}

static int find_dominant(int p,int q)
{
	int pos;
	int level = 0; //level of parentheses
	int op_pos[6];      //initial the op_pos
	for(pos =0;pos < 6;pos ++)
		op_pos[pos]=-1;
	int sum = p+q;
	for(pos = p;pos <= q ;pos++) {
		if(tokens[pos].type == '(') ++level;
		else if(tokens[pos].type == ')') --level;
		
		if(level == 0) { //not in a pair of parentheses		
			if(tokens[pos].type == OR)
				op_pos[0] = pos;
			else if(tokens[pos].type == AND)
				op_pos[1] = pos;
			else if(tokens[pos].type == EQ || tokens[pos].type == NEQ)
				op_pos[2] = pos;
			else if(tokens[pos].type == '+' || tokens[pos].type == '-')
				op_pos[3] = pos;
			else if(tokens[pos].type == '*' || tokens[pos].type == '/')
				op_pos[4] = pos;
			else if(tokens[sum-pos].type >= NO && tokens[sum-pos].type <=POSITIVE )
				op_pos[5] = sum - pos;
		}
	}
	
	
	for(pos = 0;pos < 6;pos ++) {
		if(op_pos[pos] != -1)
			return op_pos[pos];
	}
	//printf("%d\n",op_pos[3]);
	return -1;//dominant operator not found
}

uint32_t eval(int p,int q)
{
	if(p > q) {
		return 0;
	}
	else if(p == q) {
		if(tokens[p].type == HEX_NUM)
			return hex_to_i(tokens[p].str);
		else if(tokens[p].type == DECI_NUM)
			return atoi(tokens[p].str);
		else if(tokens[p].type == REG) {
			char * reg_name = tokens[p].str+1;
			int i;
			for( i = 0;i < 8;i++)
				if(strcmp(reg_name,regsl[i])==0)
				 	return cpu.gpr[i]._32;

			if(strcmp(reg_name,"eip") == 0)
					return cpu.eip;

			panic("At function 'eval' reg match failed !\n");
		//	return 0;
		}
		else {
			return get_symbol_value(tokens[p].str);
	
		}

		   
	}
	else if(check_parentheses(p,q) == true) {
		/*The expression is surrounded by a matched pair of parentheses.*/
		/*If that is th case ,just throw away the parenthese.*/
		return eval(p+1,q-1);
	}
	else {
		int op_pos  =  find_dominant(p,q);
		uint32_t val1 = eval(p,op_pos - 1);
		uint32_t val2 = eval(op_pos+1,q);
		switch(tokens[op_pos].type) {
			case '+':return val1 + val2;
			case '-':return val1 - val2;
			case '*':return val1 * val2;
			case '/':return val1 / val2;
			case NO:return !val2;
			case POSITIVE: return val2;
			case MINUS:return -val2;
			case DEREF:return swaddr_read(val2,4,R_DS);
			case EQ:return val1 == val2;
			case NEQ:return val1 != val2;
			case AND:return val1 && val2;
			case OR:return val1 || val2;
			default:assert(0);
		}
	}
}
	
