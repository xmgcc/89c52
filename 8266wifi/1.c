#include "reg52.h"
#include "3.h"
#define SENDHELLO 1
#define SENDWORLD 0
#define DEFAULT -1
char command;

void main()
{
    command = DEFAULT;
    initstr();
    initESP8266();
    while (1) {
        if (command == SENDHELLO) {
            sendString("hello\r\n");
            delay(10);
        }
        else if (command == SENDWORLD) {
            sendString("world\r\n");
            delay(10);
        }
    }
}
