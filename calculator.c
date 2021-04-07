#include"reg52.h"

int enable = 0;  
#include"input_key.h"
#include"output_1602.h"
#include"calcu_core.h"
#define STM89C52
int main(void){
	double ans;
	LcdIni();
	core_ini();
	Input();
	ans=core_launcher();
	print_ans(ans);
	return 0;
}