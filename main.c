#include "reg52.h"

typedef unsigned char byte;

#if 0
// ֻ������
void init_uart_enable_send()
{
	//���ô��п��ƼĴ���SCON  0100 0000
	// ������ʽ1
	SCON = 0x40;

	// ���ò�����ѡ�����⹦�ܼĴ���PCON

	// ���㲨����
	// ʹ�ö�ʱ��T1��Ϊ�����ʷ���������Ҫ����T1���ж�
	
	// ���ö�ʱ��1Ϊ������ʽ2 0010 0000
	TMOD = 0X20;

	// ���ò����� 2^SMOD��ʾSMOD��־λ
	TH1=0xFD;
	TL1=0xFD;

	// ������ʱ��1
	TR1=1;
}


#endif

// �����ͺͽ���
void init_uart_enable_send_and_recv()
{
	
	//���ô��п��ƼĴ���SCON  0101 0000
	// ������ʽ1
	// �������,REN=1
	SCON = 0x50;

	// ���ò�����ѡ�����⹦�ܼĴ���PCON

	// ���㲨����
	// ʹ�ö�ʱ��T1��Ϊ�����ʷ���������Ҫ����T1���ж�
	
	// ���ö�ʱ��1Ϊ������ʽ2 0010 0000
	TMOD = 0X20;

	// ���ò����� 2^SMOD��ʾSMOD��־λ
	TH1=0xFD;
	TL1=0xFD;

	// ������ʱ��1
	TR1=1;
}


void delay()
{
	int i;
	int j;
	for (i = 0; i < 30; i++)
		for (j = 0; j < 30; j++)
			;
}


void send(byte ch)
{
	SBUF=ch;

	// ������ɺ�TI�ᱻӲ����λ
	while(!TI);

	// ��Ҫ�ֶ���λ
	TI=0;

	delay();
}

// �����жϴ�����
void uart_srv(void) interrupt 4
{
	if (RI == 1)
	{
		RI = 0;
		send('c');		
	}
}

// �����ж�
void enable_uart_interrupt()
{
	
	// ���������ж�	
	ES=1;

	// �������ж�
	EA=1;

}

void main()
{
	init_uart_enable_send_and_recv();
	
	enable_uart_interrupt();

	while(1)
	{
		
	}
}