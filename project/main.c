#include <reg52.h>
#include <stdio.h>

// 网络助手 <-> ESP8266 <-> 串口 <-> 单片机 <-> 超声波
//                                      ^
//                                      |
//                                      继电器
char CMD;
unsigned int time;
char buf[32] = {0};
float distance;
sbit TRIG = P1 ^ 0;  // TRIG
sbit ECHO = P1 ^ 1;  // ECHO
// 继电器
sbit p = P1 ^ 2;

void delay(int time_1)
{
    int i;
    int j;
    for (i = 0; i < 100; i++) {
        for (j = 0; j < time_1; j++)
            ;
    }
}

void inputstr(char character) { SBUF = character; }
void sendstring(char *p)
{
    while (*p != '\0') {
        inputstr(*p);
        p++;
        delay(10);
    }
}

void initUART_115200()
{
    SCON = 0x50;  // 0101 0000  方式一
    TH2 = 0xFF;
    TL2 = 0xFD;
    RCAP2H = 0xFF;
    RCAP2L = 0xFD;
    T2CON = 0x34;  //波特率115200	   算法巩固
}

void init_timer()
{
    TMOD = 0x01;  //定时器0的工作方式
    TH0 = 0xff;   //高八位，设置初始值 10us
    TL0 = 0xf6;   //底八位
}
void open_counttime()
{
    TH0 = 0;
    TL0 = 0;
    TR0 = 1;
}
void end_counttime() { TR0 = 0; }
int get_time()
{
    unsigned int a;
    a = TH0 * 256 + TL0;  // ***
    return a;
}

void CSB_Init()
{
    TMOD = 0x01;
    TH0 = 0;
    TL0 = 0;
    EA = 1;
    ET0 = 1;
    TRIG = 0;
}
void init_wifi()
{
    delay(2000);
    sendstring("AT+CWMODE_DEF=1\r\n");
    delay(5000);
    sendstring("AT+CWJAP_DEF=\"iphone1\",\"111222333\"\r\n");
    delay(5000);
    sendstring("AT+CIPSTART=\"TCP\",\"172.20.10.6\",8080\r\n");
    delay(3000);
    sendstring("AT+CIPMODE=1\r\n");
    delay(3000);
    sendstring("AT+CIPSEND\r\n");
    delay(300);
}

void main()
{
    initUART_115200();
    init_timer();
    init_wifi();
    while (1) {
        // 启动超声波
        CSB_Init();
        TRIG = 1;
        TR0 = 1;
        while (TF0 == 1)
            ;     //等待10us
        TR0 = 0;  //关闭计时器
        TRIG = 0;

        // 计算时间
        while (ECHO != 1)
            ;
        open_counttime();
        while (ECHO != 0)
            ;
        end_counttime();
        time = get_time();

        // 计算距离
        distance = (float)time * 0.017;
        sprintf(buf, "distance=%f\r\n", distance);

        sendstring(buf);
        delay(1);

        // 接收网络调试助手发送的数据
        CMD = SBUF;
        if (CMD == 1 || distance <= 10) {
            sendstring("open\n");
            p = 0;
            delay(500);
        }
        else if (CMD == 2) {
            sendstring("close\n");
            p = 1;
            delay(500);
        }
        RI = 0;
    }
}
