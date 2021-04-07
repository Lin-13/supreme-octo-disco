#include "reg52.h"
#define matric_keyboard P1
#define INPUT_KEY.H

sbit key1=P3^1;
sbit key2=P3^0;
sbit key3=P3^2;
sbit key4=P3^3;
char getchar_from_keyboard(void);
int get_a_key(void);
void delay(int);
char key_list1[20]={
	'\0','#','(',')',
	'0','.','=','-',
	'1','2','3','+',
	'4','5','6','/',
	'7','8','9','*'
};

void delay(int i){
	while(i){
		i--;
	}
}
//返回能够入栈的字符
char getchar_from_keyboard(void){
	char ret;
	int key;
	while((key=get_a_key())==0)
		continue;
		ret=key_list1[key-1];
		return ret;
}	
/***********************************
*1~4,  独立按键，从左至右；
*5~20，矩阵按键,从下至上，从左至右；
*0，   输入错误；
***********************************/
int get_a_key(void){
		matric_keyboard=0xf0;
		while(key1||key2||key3||key4||(matric_keyboard==0xf0)) 
			continue;
		if(key1==0){
			delay(100);
			if(key1==0)
				return 1;
				else return 0;
		}
		if(key2==0){
			delay(100);
			if(key2==0)
				return 2;
			else return 0;
		}
		if(key3==0){
			delay(100);
			if(key3==0)
				return 3;
			else return 0;
		}
		if(key4==0){
			delay(100);
			if(key4==0)
				return 4;
			else return 0;
		}
		if(matric_keyboard!=0xf0){
				int row;
				delay(100);
				if(matric_keyboard==0xf0){
					return 0;
				}
				else{
					if(matric_keyboard==0x70){
						row=4;
					}
					else if(matric_keyboard==0xb0){
						row=3;
					}
					else if(matric_keyboard==0xd0){
						row=2;
					}
					else if(matric_keyboard==0xe0){
						row=1;
					}
					else return 0;
					
					matric_keyboard=0x0f;
					if(matric_keyboard==0x07){
						 return 4*row+1;
					}
					else if(matric_keyboard==0x0b){
						return 4*row+2;
					}
					else if(matric_keyboard==0x0d){
						return 4*row+3;
					}
					else if(matric_keyboard==0x0e){
						return 4*row+4;
					}
					else return 0;
				}
			}
		return 0;
}			