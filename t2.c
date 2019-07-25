#include "reg52.h"

typedef unsigned char u8;

#define BAUD 115200
#define FOSC 11059200
void t2_init_115200()
{
	// ����T2���ڲ����ʷ�����
	T2CON = 0x34;

	// ʹ��16λ��ʱ����TH2��TL2ͬʱʹ��
	// ������  TH2 = RCAP2H��TL2 = RCAP2L��
	TH2 = RCAP2H = 0XFF;// 65536 - (FOSC/32/BAUD);
	TL2 = RCAP2L = 0XFD;//(65536 - (FOSC/32/BAUD)) >> 8;
	
	// ������ʱ��2
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
	// ���������͡�����
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
	// ���������ж�
	ES=1;

	// �������ж�
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