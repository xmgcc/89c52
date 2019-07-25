#include "reg52.h"

typedef unsigned char byte;

#if 0
// 只允许发送
void init_uart_enable_send()
{
	//配置串行控制寄存器SCON  0100 0000
	// 工作方式1
	SCON = 0x40;

	// 配置波特率选择特殊功能寄存器PCON

	// 计算波特率
	// 使用定时器T1作为波特率发生器，需要禁用T1的中断
	
	// 配置定时器1为工作方式2 0010 0000
	TMOD = 0X20;

	// 设置波特率 2^SMOD表示SMOD标志位
	TH1=0xFD;
	TL1=0xFD;

	// 启动定时器1
	TR1=1;
}


#endif

// 允许发送和接收
void init_uart_enable_send_and_recv()
{
	
	//配置串行控制寄存器SCON  0101 0000
	// 工作方式1
	// 允许接收,REN=1
	SCON = 0x50;

	// 配置波特率选择特殊功能寄存器PCON

	// 计算波特率
	// 使用定时器T1作为波特率发生器，需要禁用T1的中断
	
	// 配置定时器1为工作方式2 0010 0000
	TMOD = 0X20;

	// 设置波特率 2^SMOD表示SMOD标志位
	TH1=0xFD;
	TL1=0xFD;

	// 启动定时器1
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

	// 发送完成后，TI会被硬件置位
	while(!TI);

	// 需要手动复位
	TI=0;

	delay();
}

// 串口中断处理函数
void uart_srv(void) interrupt 4
{
	if (RI == 1)
	{
		RI = 0;
		send('c');		
	}
}

// 开启中断
void enable_uart_interrupt()
{
	
	// 开启串口中断	
	ES=1;

	// 开启总中断
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