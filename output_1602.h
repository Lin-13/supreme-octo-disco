#define LCD_DB P0
#define OUTPUT_1602.H

sbit rs=P2^6;	//rs=0，输入指令，1，输入数据
sbit rw=P2^5;	//0写入lcd，1读取
sbit E=P2^7;	//1时读取信息，1->0（下降沿）执行指令
unsigned char p_ddram=0x00;
struct cgchar{
	unsigned char ch_p;
	unsigned char model[8];
};
const struct cgchar E_cgch={
		0x00,
		{0x0f,0x08,0x0f,0x08,0x0f,0x00,0x00,0x00}
};
int Check(void);
void PrintEqu(char*);
void print_ans(double);
void Delay1ms(unsigned int);
void LcdIni(void);
void WriteCom(unsigned char);
void WriteData(char);
//void LCD_Print_roll(char ch);
void LCD_Print_line1(char ch);
void CGRAM_new(unsigned char[8],unsigned char);
void double_to_char(double,char[16]);
void strncpy(char*,char*,int);
double power_2(double,double);

void PrintEqu(char* ptr){
	unsigned char i=0;
	for(i=0;i<16;i++){
		WriteCom(0x80+i-1);
		WriteData(*ptr++);
	}
}	
void strncpy(char*a,char*b,int n){
	int i=0;
		while((i<n)&&(a[i]!=0)&&(a[i]!=0)){
			a[i]=b[i];
			i++;
		}
}
void print_ans(double ans) {
	char ans_char[16]={' '};
	char* ptr=ans_char;
	unsigned char i=0;
	if(Check==0||enable!=0) {
		strncpy(ans_char,"error!",6);
		ans_char[6]=' ';
	}
	else double_to_char(ans,ans_char);
	
	WriteCom(0x0c);						//关闭光标
	for(i=0;i<16;i++){
		WriteCom(0x80+0x40+i-1);//第二行
		WriteData(*ptr++);
	}
	return;
}
void double_to_char(double ans,char ans_char[16]){
	double EPS=0.000000001;
	int cnt=0;
	char point_front[16];
	int b=0;//记录指数
	double a=ans;
	int a_col=16;
	int a_in=0;
	
	if(ans<-EPS){
		ans_char[a_in]='-';
		ans=0-ans;
		a_in++;
	}
	if(ans>=power_2(10,100)){
		strncpy(ans_char,"mathmatic error",15);
		ans_char[15]=' ';
		return;
	}
	else if(ans>=power_2(10,16)){
		while((a=ans/10)>=10){ 
			b++;
			ans=a;
		}
		a_col=13;
		ans_char[a_in]='0'+(int)a;
		a_in++;
		ans_char[a_in]='.';
		a_in++;
		while(a_in<a_col){
			ans_char[a_in]=(int)(10*a)%10+'0';
			a*=10;
			a_in++;
		}
		ans_char[13]=0x00;
		ans_char[14]='0'+b/10;
		ans_char[15]='0'+b%10;
	}
	else{
		a=ans;
		if(ans<1){
			 ans_char[a_in]='0';
			 a_in++;
		}
		while(a>=1){
			point_front[cnt]=(int)a%10;
			cnt++;
			a/=10;
		}

		while(cnt>0){
			ans_char[a_in]=point_front[cnt-1]+'0';
			a_in++;
			cnt--;
		}
		ans_char[a_in++]='.';//保存小数点前推送的字符
		
		a=ans;
		while(a_in<16){
			a*=10;
			ans_char[a_in]=(int)a%10+'0';
		}
	}
}	
void LcdIni(void){
	p_ddram=0x00;
	CGRAM_new(E_cgch.model,E_cgch.ch_p);
	
	WriteCom(0x01);//清屏
	WriteCom(0x0f);//显示开，光标开。有闪烁
	WriteCom(0x06);//设置为光标右移，显示屏不移动
	WriteCom(0x38);//8位数据总线，显示两行，5*7点阵
	WriteCom(0x80);//设定下一个要存入数据的DDRAM的地址（7位）
	
}
/***********************************
void LCD_Print_roll(char ch){
	
		WriteData(ch);
		p_ddram++;
		if(p_ddram>15) {
			WriteCom(0x18);
		}
		
		WriteCom(0x80+p_ddram);
		Delay1ms(200);
}
***************************************/
void CGRAM_new(unsigned char model[8],unsigned char p_cgram){//p_cgram从0到7
	char i=0,j=0;
	unsigned char com=0x40;		//初始化指令
	p_cgram&=0x07;								//使地址始终在0和7之内
	com+=(p_cgram<<3);
	
	while(i<8){
		WriteCom(com);
		WriteData(model[i]);
		com++;
		i++;
	}
}
		
void Delay1ms(unsigned int c) //误差0us
{
	int a,b;
	for (; c>0; c--)
	{
		for (b=199;b>0;b--)
		{
			for(a=1;a>0;a--);
		}
	}
}

void WriteCom(unsigned char com){
	rs=0;
	rw=0;
	E=0;
	LCD_DB=com;
	Delay1ms(1);
	E=1;
	Delay1ms(5);
	E=0;
}

void WriteData(char dat){
	rs=1;
	rw=0;
	E=0;
	LCD_DB=dat;
	Delay1ms(1);
	E=1;
	Delay1ms(5);
	E=0;
}