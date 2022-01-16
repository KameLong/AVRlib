#ifndef UNTITLED_USERT_H
#define UNTITLED_USERT_H

#include "typedef.h"
#include "string.h"
#include "avr/iom328p.h"
#include "avr/io.h"

#ifndef F_CPU
#warning "F_CPU not defined default is 1Mhz"
#define F_CPU 1000000
#endif

//RX TX ピンを用いてUSART通信をします
class UART{
public:
    static const uint8 SEND_BUF_SIZE=64;
    static const uint8 RECEIVE_BUF_SIZE=64;

    static char sendBuf[SEND_BUF_SIZE];
    static char receiveBuf[SEND_BUF_SIZE];
    static uint8 sendPos;
    static uint8 addPos;
    static uint8 receivePos;

    static void (* resCallBack)(char*);
    /**
       状態管理
       0bit 送信バッファ書き込み中
     */
    static uint8 state;

    //bitRateは通信速度
    static void init(uint32 bitRate){
        uint16 MYUBRR = 0;
        MYUBRR = uint16((float)F_CPU / float(8UL * bitRate)+0.5);
        UBRR0H = (unsigned char)(MYUBRR>>8);
        UBRR0L = (unsigned char) MYUBRR;

        UCSR0B = 0b11011000;     // 受信完了割り込み許可,送信完了割り込み許可・受信許可・送信許可
        UCSR0C = 0b00000110;                 // 非同期動作・パリティ：なし・ストップビット：1bit・データビット長：8bit
    }
    //=================================================================================================
// USART受信処理
//=================================================================================================
    static void receive(char res)
    {
        receiveBuf[receivePos]=res;
        if(receiveBuf[receivePos]=='\0'||receiveBuf[receivePos]=='\n'){
            receiveBuf[receivePos]='\0';
            char buf[RECEIVE_BUF_SIZE];
            strcpy(buf,receiveBuf);
            resCallBack(buf);
            receivePos=0;
            return;
        }
        receivePos++;
        if(receivePos==RECEIVE_BUF_SIZE-1){
            receiveBuf[receivePos]='\0';
            char buf[RECEIVE_BUF_SIZE];
            strcpy(buf,receiveBuf);
            resCallBack(buf);
            receivePos=0;
        }
    }
public:
    /**
     * writeBufferに文字列を追加する
     * @param data 書き込みたい文字列データ
     * @return  0:書き込みに成功
     *         -1:書き込み先buffer不足
     *         -2:別の書き込み実行中
     */

    static uint8 writeData(char* data){
        if((state&0b00000001)!=0){
            //すでに書き込み中
            return -2;
        }
        state=state|0b00000001;
        uint8 result=0;
        for(uint8 i=0;true;i++){
            if(data[i]==0){
                result=0;
                break;
            }
            if(sendBuf[addPos]!=0){
                result =-1;
                break;
            }
            sendBuf[addPos]=data[i];
            addPos++;
            if(addPos==SEND_BUF_SIZE){
                addPos=0;
            }
        }
        if((UCSR0A & _BV(UDRE0))){
            send();
        }
        state=state&0b11111110;
        return result;

    }
    static void send(){
        if(sendBuf[sendPos]==0){
            return;
        }
        transmit(sendBuf[sendPos]);
        sendBuf[sendPos]=0;
        sendPos++;
        if(sendPos==SEND_BUF_SIZE){
            sendPos=0;
        }
    }
private:
    static void transmit( unsigned char Data)
    {
        // 送信バッファが空きになるまで待つ
        while ( !(UCSR0A & _BV(UDRE0)) );
        UDR0 = Data;
    }



};
uint8 UART::addPos=0;
uint8 UART::sendPos=0;
char UART::sendBuf[UART::SEND_BUF_SIZE];
uint8 UART::state=0b00000000;
char UART::receiveBuf[UART::SEND_BUF_SIZE];
uint8 UART::receivePos=0;
void  (* UART::resCallBack)(char*);

//=================================================================================================
// USART割り込み
//=================================================================================================
ISR(USART_RX_vect){
    char res=UDR0;
    UART::receive(res);
}
ISR(USART_TX_vect){
    UART::send();
}


#endif //UNTITLED_USERT_H
