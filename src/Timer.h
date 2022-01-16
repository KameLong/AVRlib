#ifndef UNTITLED_TIMER_H
#define UNTITLED_TIMER_H

#include "typedef.h"
#include "avr/iom328p.h"
#include "avr/io.h"
#include "avr/interrupt.h"

#ifndef F_CPU
#warning "F_CPU not defined default is 1Mhz"
#define F_CPU 1000000
#endif

typedef uint32 TIME;



//1ms単位の正確な割込処理
class Timer{
public:
    //時間のカウンタ
    static TIME time;
    static const uint8 CALLBACK_MAX=16;

    //コールバック関数
    static void(*callBack[CALLBACK_MAX])();
    static TIME timing[CALLBACK_MAX];

    static void init(){
        TCCR0A = 0b00000010; //標準動作
        TCCR0B = 0b00000011;//64刻み
        OCR0A =F_CPU/1000/64-1 ;//1ms間隔
        TIMSK0 = 0b0000010;
        for(uint8 i=0;i<CALLBACK_MAX;i++){
            callBack[i]= nullptr;
            timing[i]=0;
        }
        sei();
    }

    //1msごとに実行される処理
    static void clock(){
        time++;
        TIME mTime=time;
        for(uint8 i=0;i<CALLBACK_MAX;i++){
            if(callBack[i]== nullptr){
                continue;
            }
            if(mTime==timing[i]) {
                callBack[i]();
                callBack[i]= nullptr;
            }
        }
        return;
    }
    /**
     * 指定した時間後に処理を行う関数です。
     * jsのsetTimeoutと同様に使えますが、使用できる関数は引数なしです
     * @param func 実行したい関数
     * @param time 指定した時間後に実行します(ms)
     * @return 処理の予約に成功したらtrueが帰ります、失敗したらfalse
     */
    static bool setTimeout(void (*func)(),TIME after){
        for(uint8 i=0;i<CALLBACK_MAX;i++){
            if(callBack[i]== nullptr){
                timing[i]=time+after;
                callBack[i]=func;
                return true;
            }
        }
        return false;
    }
};
TIME Timer::time=0;
TIME Timer::timing[CALLBACK_MAX];
void(*Timer::callBack[CALLBACK_MAX])();


ISR(TIMER0_COMPA_vect)  //タイマ割り込み
{
     Timer::clock();
}



#endif //UNTITLED_TIMER_H
