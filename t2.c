#include "reg52.h"

typedef unsigned char u8;

#define BAUD 115200
#define FOSC 11059200
void t2_init_115200()
{
	// 配置T2用于波特率发生器
	T2CON = 0x34;

	// 使用16位定时器，TH2、TL2同时使用
	// 后续从  TH2 = RCAP2H；TL2 = RCAP2L；
	TH2 = RCAP2H = 0XFF;// 65536 - (FOSC/32/BAUD);
	TL2 = RCAP2L = 0XFD;//(65536 - (FOSC/32/BAUD)) >> 8;
	
	// 启动定时器2
	TR2 = 1;
}

void send_u8(u8 ch)
{
	SBUF=ch;
	while (!TI);
	TI = 0;
}

void uart_init()
{
	// 串口允许发送、接收
	SCON=0x50;
}

void uart_srv() interrupt 4
{
	if (RI)
	{
		RI = 0;
		send_u8(SBUF);
	}
}

void interrupt_init()
{
	// 开启串口中断
	ES=1;

	// 开启总中断
	EA=1;
}

void main()
{
	uart_init();

	t2_init_115200();

	interrupt_init();

	send_u8('c');

	while (1)
	{
	}
}