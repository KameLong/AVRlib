#include "Timer.h"
#include "UART.h"

//1秒ごとにportBを反転させる
void timer(){
    PORTB=PORTB^0xFF;
    Timer::setTimeout(timer,1000);
    char str[10];
    sprintf(str,"%d\n",(int)Timer::time);

    UART::writeData(str);
}

//UARTで送られてきたデータをそのままUARTで返す
void UARTCallback(char *data){
    UART::writeData(data);
}


int main(){
    DDRB=0xFF;
    PORTB=0xFF;

    Timer::init();
    Timer::setTimeout(timer,1000);
    UART::init(9600);
    UART::resCallBack=UARTCallback;

    sei();
    while(1)
    {
    }
    return 0;
}
