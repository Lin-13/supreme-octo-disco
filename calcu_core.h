#define STACKSIZE 128
#define LISTSIZE 22
#define FXMAX 5   //函数自变量最大数量
#define CALCU_CORE.H

/****************************
*栈及指针
****************************/
xdata char stack[STACKSIZE];
char* ptr_cal = stack;  // 将符号转变成算式的指针 
char* stack_maxend = stack + STACKSIZE;
char* stack_end;

/*****************************
*数学定义
******************************/
const double PI = 3.141592654;

/************************
*合法的符号列表
*************************/
char inputlist[LISTSIZE] = 
{ '0','1','2','3','4','5','6','7','8','9','.','+','-','*','/','(',')','=','s','c',','};
/*************************
*函数列表
**************************/
double CalBracket(void); //处理括号,返回括号里的值 
double calcu_to_calcu(int level);
double Calcu(int level) reentrant;
double trans(void);
void Input(void);
int inlist(void);



double core_launcher(void);
void core_ini(void);
double Power(double, double);

#ifdef FUNCTION_MODE
double functionlist();
double sin(double*, char);
double cos(double*, char);
double power(double* p_begin, char num);
double ln(double*,char);
#endif
void core_ini(void){
	int i=0;
	while(i<STACKSIZE){
		stack[i]=' ';
		i++;
	}

}
double core_launcher(void) {
	double ans=0;
	ptr_cal = stack;
	ans = Calcu(0);
	//print_ans(ans);
	return ans;
}
double CalBracket(void) {
	 
	double ret = 0;   //括号里的计算值 
	ptr_cal++;    //光标右移
	switch (*ptr_cal)
	{
	case ')': break;
	default: ret = Calcu(0);
		break;
	}
	ptr_cal++;
	return ret;
}
double calcu_to_calcu(int level){
	double ret;
	ret=Calcu(level);
	return ret;
}
double Calcu(int level)reentrant { //level=0表示由calbracket调用 ,保证加减运算从左往右进行 ,1表示由自身调用;level=-1,函数；level=1，加法；level=2，乘法
	double ret = 0;
	char cal = *ptr_cal;
	while (1) {
		if (*ptr_cal == '(') {
			ret = CalBracket();
		}
		else if (*ptr_cal == ')') break;
		else if ((*ptr_cal >= '0' && *ptr_cal <= '9') || *ptr_cal == '.') {
			ret = trans();

		}
		else if (*ptr_cal == '+') {
			if (level <= 0) {
				ptr_cal++;
				ret += calcu_to_calcu(1);
			}
			if (level >= 1) return ret;
		}
		else if (*ptr_cal == '-') {
			if (level <= 0) {
				ptr_cal++;
				ret -= calcu_to_calcu(1);
			}
			if (level >= 1) return ret;
		}
		else if (*ptr_cal == '*') {
			ptr_cal++;
			if (*ptr_cal == '(') ret *= CalBracket();
			else if (level >= 2) return ret;
			else ret *= calcu_to_calcu(2);
		}
		else if (*ptr_cal == '/') {
			ptr_cal++;
			if (*ptr_cal == '(') ret /= CalBracket();
			else if (level >= 2) return ret;
			else ret /= calcu_to_calcu(2);
		}
		else if (*ptr_cal == '=') break;
		else if (*ptr_cal == ',') {
			if (level == -1) {
				return ret;
				ptr_cal++;
			}
			else {
				enable = 100+level;
				return ret;
			}
		}
		else {
#ifdef FUNCTION_MODE
			ret = functionlist();
#else enable=1;
#endif
		}
		if (enable != 0) break;
	}
	return ret;
}
double trans(void) {
	double ret_int = 0;
	double ret_dec = 0;
	while (*ptr_cal >= '0' && *ptr_cal <= '9') {
		ret_int = ret_int * 10 + *ptr_cal-'0';
		ptr_cal++;
	}
	if (*ptr_cal == '.') {
		ptr_cal++;
		while (*ptr_cal >= '0' && *ptr_cal <= '9') {
			ret_dec = ret_dec * 10 + *ptr_cal-'0';
			ptr_cal++;
		}
		while (ret_dec >= 1) {
			ret_dec = ret_dec / 10;
		}
	}
	if (*ptr_cal == '.') enable = 200;  //当还有.时为异常情况 

	return ret_int + ret_dec;
}
#ifdef FUNCTION_MODE
double functionlist(void) {
	double ret=0;
	double (*p_fx)(double* p_begin, char);
	double* p_begin=0,*p_end=0;
	char para_num=0;
	double x[FXMAX];
	int i;
	
	switch (*ptr_cal) {
		case 's': p_fx = sin;
			break;
		case 'c':p_fx = cos;
			break;
		case 'P':p_fx=Power;
		default:
			enable = 300;
			return 0;
	}
	ptr_cal++;
	if (*ptr_cal != '(') {
		enable = 301;
		return 0;
	}
	ptr_cal++;//指向括号后的字符

	
	p_begin = x;
	p_end=x;
	for (i = 0; i < FXMAX; i++) {
		*p_end = Calcu(-1);
		para_num++;
		if (*ptr_cal == ')') break;  //碰到）时停止
	}
	while (*ptr_cal != ')') ptr_cal++;//若参数超过最大值，忽略后面输入的参数，使ptr_cal指向最近的）
	ptr_cal++;
	ret = (*p_fx)(p_begin,para_num);
	return ret;
}

double sin(double* p_begin, char num) {
	
	double x1 = *p_begin;
	double ret = 0;
	double a[24] = {-1/2585201673884976640000.0,0,1/5090942171709440000.0,0,-1/121645100408832000.0,0,1/355687428096000.0,0, -1/1307674368000,0,1/6227020800.0,0,-1/39916800.0,0,1/362880.0,0,-1/5040.0,0,1/120.0,0,-1/6.0,0,1,0 };
	double* a_ptr = a, * a_end = a+24;
	double EPS = 0.000001;
	if (num!=1) {
		enable = 30101;
	}
	
	if (x1 >= EPS) {
		while (x1 >= 2 * PI) x1 = x1 - 2 * PI;
	}
	if (x1 <= -EPS) {
		while (x1 <= -2 * PI) x1 = x1 + 2 * PI;
	}
	if (x1 <= EPS && x1 >= -EPS) return 0;
	//printf("%f\n", x1);
	while (a_ptr < a_end) {
		ret = ret * x1 + *a_ptr;
		a_ptr++;
	}
	return ret;
}
double cos(double* p_begin, char num) {
	
	double x1 = *p_begin;
	double ret = 0;
	double a[25] = {1/620448401733239439360000.0,0,-1/1124000727777607680000.0,0,1/2432902008176640000.0,0,-1/6402373705728000.0,0,1/20922789888000.0,0,-1/87178291200.0,0,1/479001600.0,0,-1/3628800.0,0,1 / 40320.0,0,-1 / 720.0,0,1 / 24.0,0,-1 / 2.0,0,1 };//数量过少，至少需要24为才能达到有效精度
	double* a_ptr = a, * a_end = a + 25;
	double EPS = 0.000001;
	if (num!=1) {
		enable = 30201;
	}
	if (x1 >= EPS) {
		while (x1 >= 2 * PI) x1 = x1 - 2 * PI;
	}
	if (x1 <= -EPS) {
		while (x1 <= -2 * PI) x1 = x1 + 2 * PI;
	}
	if (x1 <= EPS && x1 >= -EPS) return 1;
	while (a_ptr < a_end) {
		ret = ret * x1 + *a_ptr;
		a_ptr++;
	}
	return ret;
}
double power(double* p_begin, char num) {
	
	double a=*p_begin;
	double b=*(p_begin+1);
	int x = (int)a;
	double r = 1;
	int y = (int)b;
	int i;
	if(num!=2){
		enable=30301;
		return 0;
	}
	for (i = 0; i < y; i++) {
		r = r * x;
	}
	return r;
}

double ln(double* p_begin, char num) {
	double x1;
	double ret = 0;
	unsigned long b=0;
	double EPS = 0.000001;
	if (num!=1) {
		enable = 30201;
	}
	
	x1 = *p_begin;
	
	if (x1 <= EPS) {
		return 0;
	}
	
	while(x1 > 10) {
		x1 /= 10;
		b++;
	}
//	hh[a[10]={,-1/4.0,1/3.0,-1/2.0,1,0}
}
#endif
double power_2(double a,double b){
	int x = (int)a;
	double r = 1;
	int y = (int)b;
	int i;
	for (i = 0; i < y; i++) {
		r = r * x;
	}
	return r;
}
void Input(void) {
	char* ptr_edi = stack;
	int i=0;
	while (ptr_edi < stack_maxend) {
		*ptr_edi = getchar_from_keyboard();
		if (*ptr_edi == '=')
			break;
		if(i<16) PrintEqu(stack);
		else PrintEqu(stack+i-15);
		i++;
		ptr_edi++;
	}
	stack_end = ptr_edi+1;
}
int Check(void) {
	if (inlist()) return 1;
	else return 0;
}
int inlist(void) {
	int i;
	char* ptr = stack;
	while (ptr <stack_end) {
		int found = 0;
		for (i = 0; i < LISTSIZE; i++)
			if (*ptr == inputlist[i]) found = 1;
		if (found == 0) return 0;
		ptr++;
	}
	return 1;
}

