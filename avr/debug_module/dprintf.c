/*====================================================================
�t�@�C�����F	dprintf.c
�@�\�T�v�F�V���A���ʐM�@�\���g�p����PC�̃R���\�[����ɕ������o�͂��܂��B
					�V���A���|�[�g�̏������́A�O���ōs���K�v������܂��B
					_rsputc(char c)�́A�g�p����f�o�C�X�ɂ��킹�č쐬���܂��B
          CQ�o�ŕ����...
====================================================================*/
#include "dprintf.h"
#include <stdarg.h>
#include "uart.h"

/* originaly declared at uart.c */
extern void uart_putc(char c);

/*====================================================================
�֐����F	_rsputc
�@�\�F		char�^������1�����R���\�[���o�͂��A0��Ԃ��܂��B
====================================================================*/
int _rsputc(char c)
{
	//�g�p����}�C�R���ɍ��킹�ċL�q���܂�
//    uart_putc(c);
    USART_Transmit((unsigned char)c);
    return 0;
}

/*====================================================================
�֐����F	rsputc
�@�\�F		char�^������1�����R���\�[���o�͂��܂��B
			���s�����̏ꍇ��ASCII�R�[�h��CR���o�͂��܂��B
====================================================================*/
int rsputc(char c)
{
	if(c=='\n')
		_rsputc(0x0d);
	return _rsputc(c);
}

/*====================================================================
�֐����F	rsputs
�@�\�F		�k���|�C���^�ɓ�����܂ŕ�������o�͂��܂��B
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
�֐����F	convx
�@�\�F		�����𕶎���ɕϊ����܂��B
			dat:	�ϊ����鐔��
			base:	�
			clms:	�\�������i�������܂߂čő�7���j
			fill:	�󔒂ɖ��߂�R�[�h
			�߂�l�͕ϊ�����������̐擪�̃|�C���^�ł��B
			������̏I���̓k�������ł��B
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

	buff[7]=0;		/*LSB�̓k������*/
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
�֐����F	dprintf
�@�\�F		printf�̐��������o�[�W�����ł��B
			fmt�F	�\��������
			dprintf�͎��̕ϊ���������T�|�[�g���܂��B
			%c�F	char�^����1����
			%s�F	char*������
			%nx�F	int�^������16�i���\�����܂��B
			%nd�F	int�^������10�i���\�����܂��B
			%nx�A%nd��n�ʒu�ɂ͕\������������܂��Bn��0�Ŏn�܂�ꍇ�A�\�����鐔�̌����\�����ɖ����Ȃ��ꍇ�A�c����O�Ŗ��߂܂��B
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
		//FillChar���`�F�b�N('0')�̂�
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
