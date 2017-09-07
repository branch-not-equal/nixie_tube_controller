/*====================================================================
ファイル名：	dprintf.c
機能概要：シリアル通信機能を使用してPCのコンソール上に文字を出力します。
					シリアルポートの初期化は、外部で行う必要があります。
					_rsputc(char c)は、使用するデバイスにあわせて作成します。
          CQ出版物より...
====================================================================*/
#include "dprintf.h"
#include <stdarg.h>
#include "uart.h"

/* originaly declared at uart.c */
extern void uart_putc(char c);

/*====================================================================
関数名：	_rsputc
機能：		char型文字を1文字コンソール出力し、0を返します。
====================================================================*/
int _rsputc(char c)
{
	//使用するマイコンに合わせて記述します
//    uart_putc(c);
    USART_Transmit((unsigned char)c);
    return 0;
}

/*====================================================================
関数名：	rsputc
機能：		char型文字を1文字コンソール出力します。
			改行文字の場合はASCIIコードのCRを出力します。
====================================================================*/
int rsputc(char c)
{
	if(c=='\n')
		_rsputc(0x0d);
	return _rsputc(c);
}

/*====================================================================
関数名：	rsputs
機能：		ヌルポインタに当たるまで文字列を出力します。
====================================================================*/
char *rsputs(char *str)
{
	while(*str){
		rsputc(*str);
		str++;
	}
	return str;
}

/*====================================================================
関数名：	convx
機能：		数字を文字列に変換します。
			dat:	変換する数字
			base:	基数
			clms:	表示桁数（符号を含めて最大7桁）
			fill:	空白に埋めるコード
			戻り値は変換した文字列の先頭のポインタです。
			文字列の終わりはヌル文字です。
====================================================================*/
char * convx(int dat,int base,int clms,char fill)
{
	const char *dspc="0123456789ABCDEF";
	static char buff[8];
	int cnt;
	int ip=7;
	int d;
	char sgn;

	if(dat<0){
		sgn='-';
		dat=-dat;
	}else{
		sgn=' ';
	}

	buff[7]=0;		/*LSBはヌル文字*/
	do{
		ip--;
		d=dat % base;
		buff[ip]=dspc[d];
		dat/=base;
	}while(dat && ip);
	cnt=clms-(7-ip);
	while((cnt>0)&&(ip>0)){
		ip--;
		buff[ip]=fill;
		cnt--;
	}

	if(sgn=='-'){
		ip--;
		buff[ip]=sgn;
	}
	return &buff[ip];
}

/*====================================================================
関数名：	dprintf
機能：		printfの整数制限バージョンです。
			fmt：	表示文字列
			dprintfは次の変換文字列をサポートします。
			%c：	char型文字1文字
			%s：	char*文字列
			%nx：	int型数字を16進数表示します。
			%nd：	int型数字を10進数表示します。
			%nx、%ndのn位置には表示桁数が入ります。nが0で始まる場合、表示する数の桁が表示桁に満たない場合、残りを０で埋めます。
====================================================================*/
void dprintf(const char *fmt, ...)
{
	va_list ap;
	char *p,*sval,c;
	char fillchr;
	char clms;
	int ival;
	
	va_start(ap,fmt);
	for(p=(char*)fmt;*p;p++){
		if(*p!='%'){
			rsputc(*p);
			continue;
		}
		p++;
		fillchr=' ';
		clms=0;
		//FillCharをチェック('0')のみ
		if(*p=='0'){
			fillchr='0';
			p++;
		}
		while((*p>='0')&&(*p<='9')){
			clms=clms*10+(*p-'0');
			p++;
		}
		switch(*p){
		case 'c':
		case 'C':
			c=va_arg(ap,int);
			rsputc(c);
			break;
		case 'd':
		case 'D':
			ival=va_arg(ap,int);
			rsputi(ival,clms,fillchr);
			break;
		case 'x':
		case 'X':
			ival=va_arg(ap,int);
			rsputx(ival,clms,fillchr);
			break;
		case 's':
		case 'S':
			for(sval=va_arg(ap,char *);*sval; sval++)
				rsputc(*sval);
			break;
		default:
			rsputc(*p);
		}
	}
	va_end(ap);
}


void dbitprint(long data, int len)
{
    while (len-- != 0){
        dprintf("%d", ((data >> len) & 0x01));
        if ((len % 4) == 0) dprintf(" ");
    }
}
