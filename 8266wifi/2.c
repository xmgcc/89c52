#include "3.h"
#include "reg52.h"
extern char command;
void delay(unsigned long time)
{
    int i;
    int j;
    for (i = 0; i < 100; i++)
        for (j = 0; j < time; j++)
            ;
}
void initstr()
{
    SCON = 0x50;
    T2CON = 0x34;
    TH2 = 0xFF;
    TL2 = 0xFD;

    RCAP2H = 0xFF;
    RCAP2L = 0xFD;
    TR2 = 1;
    EA = 1;
    ES = 1;
}
void printstr(char c)
{
    SBUF = c;
    while (TI != 1)
        ;
    TI = 0;
}

void sendString(char *str)
{
    while (*str != '\0') {
        printstr(*str);
        str++;
        // delay();
    }
}
void zhongduan() interrupt 4
{
    if (RI == 1) {
        RI = 0;
        command = SBUF;
    }
}

void initESP8266()
{
    // 1.入网 2.接入服务器
    sendString("AT+CWMODE_DEF=1\r\n");
    delay(1000);
    sendString("AT+CWJAP_DEF=\"Cc\",\"88888888\"\r\n");
    delay(5000);
    sendString("AT+CIPSTART=\"TCP\",\"172.20.10.2\",8080\r\n");
    delay(5000);
    sendString("AT+CIPMODE=1\r\n");
    delay(1000);
    sendString("AT+CIPSEND\r\n");
    delay(1000);
}

